/* 
 * File:   PedalMonitor.cpp
 * Author: SET - nmset@yandex.com
 * Licence : LGPL 2.1
 * Copyright SET, M.D. - © 2014
 *
 * Created on 18 février 2014, 20:52
 */

#include "PedalManager.h"
#include <iostream>
using namespace std;

IPedalMonitor::IPedalMonitor() {}
IPedalMonitor::~IPedalMonitor() {}
wxThread::ExitCode IPedalMonitor::Entry() {return (wxThread::ExitCode) 0;}

PedalMonitor_OnOff::PedalMonitor_OnOff(const wxString& newDevice, PedalEvent * newPedalEVH, const unsigned short newLeftCode, const unsigned short newMiddleCode, const unsigned short newRightCode) {
    CreateThread(wxTHREAD_DETACHED);
    m_pedalEVH = newPedalEVH;
    m_leftCode = newLeftCode;
    m_middleCode = newMiddleCode;
    m_rightCode = newRightCode;
    m_device = newDevice;
    m_current = PedalEvent::NONE;
}

PedalMonitor_OnOff::~PedalMonitor_OnOff() {
    hid_exit();
}

wxThread::ExitCode PedalMonitor_OnOff::Entry() {
    /* Two pedal devices have been tested :
     * The Infinity VEC IN-USB-2
     * The Olympus RS-28
     * Their individual pedals send different bytes :
     * VEC 
     *     LEFT : 1
     *     MIDDLE : 2
     *     RIGHT : 4
     * 
     * OLYMPUS RS-28
     *     LEFT : 1
     *     MIDDLE : 4
     *     RIGHT : 2   
     * 
     * This is obtained by mere exploration.
     * The hardware documentation does not provide this information.
     */
/*
 IF MORE THAN ONE PEDAL PRESSED, RELEASING ONE PEDAL FIRES
 * THE EVENT OF THE PRESSED PEDALS
 * 
 */
    if (!m_pedalEVH) {
        return (wxThread::ExitCode) 1;
    }
    if (hid_init() != 0) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_device * dev = hid_open_path(m_device.c_str());
    if (!dev) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_set_nonblocking(dev, 0);
    const unsigned short bsz = 256;
    unsigned char b[bsz];
    unsigned short code = 0;
    int nbRead = 0;
    while(true) {
        if (GetThread()->TestDestroy()) break;
        nbRead = hid_read(dev, b, bsz);
        if (GetThread()->TestDestroy()) break;
        if (nbRead == -1) break;
        /*
         * There's only one significant byte of all the bytes read.
         * All other bytes are zero. Ignore the order of the significant byte
         * byte adding them all.
         * This will break with a nasty device.
         * A hypothetical device may work as follows :
         *      LEFT :          0 4 1 0 0
         *      MIDDLE :        1 3 0 1 0
         *      RIGHT :         0 1 1 1 2
         * Adding the bytes won't identify which pedal is pressed.
         * But let's suppose hardware manufacturers are friendly.
         * A (nailed) HID mouse can be used for testing.
         */
        for (unsigned short i = 0; i < nbRead; i++) {
                code += (unsigned short) b[i];
        }
        // We stop everything when we catch anything.
        if (m_current == PedalEvent::LEFT) m_pedalEVH->Left(PedalEvent::RELEASED);
        if (m_current == PedalEvent::MIDDLE) m_pedalEVH->Middle(PedalEvent::RELEASED);
        if (m_current == PedalEvent::RIGHT) m_pedalEVH->Right(PedalEvent::RELEASED);
        // If more than one pedal is pressed, we start listening again.
        if (code != 0 && code != m_leftCode && code != m_middleCode && code != m_rightCode) {
            code = 0;
            continue;
        }
        // Only one pedal is pressed
        if (code == m_leftCode) {
            m_current = PedalEvent::LEFT;
            m_pedalEVH->Left(PedalEvent::PRESSED);
        }
        if (code == m_middleCode) {
            m_current = PedalEvent::MIDDLE;
            m_pedalEVH->Middle(PedalEvent::PRESSED);
        }
        if (code == m_rightCode) {
            m_current = PedalEvent::RIGHT;
            m_pedalEVH->Right(PedalEvent::PRESSED);
        }
        if (code == 0) m_current = PedalEvent::NONE;
        code = 0;
    }
    
    hid_close(dev);
    hid_exit();
    m_pedalEVH->OnPedalMonitorExit();
    return (wxThread::ExitCode) 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

PedalMonitor_Override::PedalMonitor_Override(const wxString& newDevice, PedalEvent * newPedalEVH, const unsigned short newLeftCode, const unsigned short newMiddleCode, const unsigned short newRightCode) {
    CreateThread(wxTHREAD_DETACHED);
    m_pedalEVH = newPedalEVH;
    m_leftCode = newLeftCode;
    m_middleCode = newMiddleCode;
    m_rightCode = newRightCode;
    m_device = newDevice;
    m_current = PedalEvent::NONE;
    m_previous = PedalEvent::NONE;
}

PedalMonitor_Override::~PedalMonitor_Override() {
    hid_exit();
}

wxThread::ExitCode PedalMonitor_Override::Entry() {
    if (!m_pedalEVH) {
        return (wxThread::ExitCode) 1;
    }
    if (hid_init() != 0) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_device * dev = hid_open_path(m_device.c_str());
    if (!dev) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_set_nonblocking(dev, 0);
    const unsigned short bsz = 256;
    unsigned char b[bsz];
    unsigned short code = 0;
    int nbRead = 0;
    while(true) {
        if (GetThread()->TestDestroy()) break;
        nbRead = hid_read(dev, b, bsz);
        if (GetThread()->TestDestroy()) break;
        if (nbRead == -1) break;
        for (unsigned short i = 0; i < nbRead; i++) {
                code += (unsigned short) b[i];
        }
        m_previous = m_current;
        /*
         * Two pedals are pressed.
         * We proceed in a deterministic way, we ony have 3 pedals afterall.
         */
        if (code == m_leftCode + m_middleCode) {
            if (m_current == PedalEvent::LEFT) {
                // Stop the current action
                m_pedalEVH->Left(PedalEvent::RELEASED);
                // Update the current action, the newly pressed pedal.
                m_current = PedalEvent::MIDDLE;
                code = code - m_leftCode; // OR code = middleCode;
            } else {
                m_pedalEVH->Middle(PedalEvent::RELEASED);
                m_current = PedalEvent::LEFT;
                code = code - m_middleCode;
            }
        }
        if (code == m_leftCode + m_rightCode) {
            if (m_current == PedalEvent::LEFT) {
                m_pedalEVH->Left(PedalEvent::RELEASED);
                m_current = PedalEvent::RIGHT;
                code = code - m_leftCode;
            } else {
                m_pedalEVH->Right(PedalEvent::RELEASED);
                m_current = PedalEvent::LEFT;
                code = code - m_rightCode;
            }
        }
        if (code == m_middleCode + m_rightCode) {
            if (m_current == PedalEvent::MIDDLE) {
                m_pedalEVH->Middle(PedalEvent::RELEASED);
                m_current = PedalEvent::RIGHT;
                code = code - m_middleCode;
            } else {
                m_pedalEVH->Right(PedalEvent::RELEASED);
                m_current = PedalEvent::MIDDLE;
                code = code - m_rightCode;
            }
        }
        // If one or three pedals are pressed, we still need to stop everything.
        if (m_previous == PedalEvent::LEFT) m_pedalEVH->Left(PedalEvent::RELEASED);
        if (m_previous == PedalEvent::MIDDLE) m_pedalEVH->Middle(PedalEvent::RELEASED);
        if (m_previous == PedalEvent::RIGHT) m_pedalEVH->Right(PedalEvent::RELEASED);
        // If three pedals are pressed, we start listening again.
        if (code == m_leftCode + m_middleCode + m_rightCode) {
            code = 0;
            continue;
        }
        // Proceed with the last pedal pressed.
        if (code == m_leftCode) {
            m_current = PedalEvent::LEFT;
            m_pedalEVH->Left(PedalEvent::PRESSED);
        }
        if (code == m_middleCode) {
            m_current = PedalEvent::MIDDLE;
            m_pedalEVH->Middle(PedalEvent::PRESSED);
        }
        if (code == m_rightCode) {
            m_current = PedalEvent::RIGHT;
            m_pedalEVH->Right(PedalEvent::PRESSED);
        }
        if (code == 0) m_current = PedalEvent::NONE;
        code = 0;
    }
    
    hid_close(dev);
    hid_exit();
    m_pedalEVH->OnPedalMonitorExit();
    return (wxThread::ExitCode) 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

PedalEvent::PedalEvent() {}
PedalEvent::~PedalEvent() {}

void PedalEvent::Left(PedalEvent::PedalStatus status) {}
void PedalEvent::Middle(PedalEvent::PedalStatus PedalStatus) {}
void PedalEvent::Right(PedalEvent::PedalStatus PedalStatus) {}
void PedalEvent::OnPedalMonitorExit() {}
void PedalEvent::OnFastMoveExit(MediaFastMove * active) {}
void PedalEvent::OnCodeIdentifierExit(PedalCodeIdentifier* active) {}
void PedalEvent::OnPedalCaught(const unsigned short code) {}
void PedalEvent::OnMediaProgressExit(MediaProgress * active) {}
void PedalEvent::OnMediaProgressPosition() {}
void PedalEvent::OnMediaControlPosition(wxFileOffset position) {}
void PedalEvent::OnStreamError() {}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * We move 'm_step' interval, and wait 'm_sleepMs' milliseconds.
 */
MediaFastMove::MediaFastMove(wxMediaCtrl * newMedia, wxFileOffset newStep, unsigned long newSleepMs, PedalEvent * newCaller) {
    CreateThread(wxTHREAD_DETACHED);
    m_media = newMedia;
    m_step = newStep;
    m_sleepMs = newSleepMs;
    m_pedalEVH = newCaller;
}
MediaFastMove::~MediaFastMove() {
    
}
wxThread::ExitCode MediaFastMove::Entry() {
    if (!m_pedalEVH) {
        return (wxThread::ExitCode) 1;
    }
    // Fast forward
    if (m_step > 0) {
        wxFileOffset pos = m_media->Tell();
        wxFileOffset ln = pos;
        wxFileOffset realStep = m_step;
        m_media->Pause();
        while (pos <= ln) {
            if (GetThread()->TestDestroy()) break;
            ln = m_media->Length();
            realStep = ((ln - pos) < m_step) ? ln - pos : m_step;
            //m_media->Seek(realStep, wxFromCurrent);
            m_pedalEVH->OnMediaControlPosition(realStep);
            wxMilliSleep(m_sleepMs);
            pos = m_media->Tell();
        }
    }
    // Rewind, with a negative step
    if (m_step < 0) {
        //wxFileOffset ln = m_media->Length();
        wxFileOffset pos = m_media->Tell();
        wxFileOffset realStep = m_step;
        m_media->Pause();
        while (pos > 0) {
            if (GetThread()->TestDestroy()) break;
            realStep = (pos < abs(m_step)) ? -pos : m_step;
            //m_media->Seek(realStep, wxFromCurrent);
            m_pedalEVH->OnMediaControlPosition(realStep);
            wxMilliSleep(m_sleepMs);
            pos = m_media->Tell();
        }
    }
    m_pedalEVH->OnFastMoveExit(this);
    return (wxThread::ExitCode) 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Forward a pedal event to PedalEvent class.
 * We want to identify the byte sent by pressing a pedal.
 */
PedalCodeIdentifier::PedalCodeIdentifier(const wxString& newDevice, PedalEvent* newPedalEvent) {
    CreateThread(wxTHREAD_DETACHED);
    m_pedalEVH = newPedalEvent;
    m_device = newDevice;
}
PedalCodeIdentifier::~PedalCodeIdentifier() {
    hid_exit();
}
wxThread::ExitCode PedalCodeIdentifier::Entry() {
    if (!m_pedalEVH) {
        return (wxThread::ExitCode) 1;
    }
    if (hid_init() != 0) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_device * dev = hid_open_path(m_device.c_str());
    if (!dev) {
        m_pedalEVH->OnStreamError();
        return (wxThread::ExitCode) 1;
    }
    hid_set_nonblocking(dev, 0);
    const unsigned short bsz = 256;
    unsigned char b[bsz];
    unsigned short code = 0;
    int nbRead = 0;
    while (true) {
        if (GetThread()->TestDestroy()) break;
        nbRead = hid_read(dev, b, bsz);
        if (GetThread()->TestDestroy()) break;
        if (nbRead == -1) break;
        for (unsigned short i = 0; i < nbRead; i++) {
                code += (unsigned short) b[i];
        }
        m_pedalEVH->OnPedalCaught(code);
        code = 0;
    }
    hid_close(dev);
    hid_exit();
    m_pedalEVH->OnCodeIdentifierExit(this);
    return (wxThread::ExitCode) 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Forward media position to PedalEvent class at a hard coded 250 ms interval.
 */
MediaProgress::MediaProgress(PedalEvent * newPedalEvent) {
    CreateThread(wxTHREAD_DETACHED);
    m_pedalEVH = newPedalEvent;
}
MediaProgress::~MediaProgress() {
    
}
wxThread::ExitCode MediaProgress::Entry() {
    if (!m_pedalEVH) {
        return (wxThread::ExitCode) 1;
    }
    while (true) {
        if (GetThread()->TestDestroy()) break;
        m_pedalEVH->OnMediaProgressPosition();
        wxMilliSleep(250);
    }
    
    m_pedalEVH->OnMediaProgressExit(this);
    return (wxThread::ExitCode) 0;
}

////////////////////////////////////////////////////////////////////////////////////

HIDTool::HIDTool() {
}

HIDTool::~HIDTool() {
}

void HIDTool::GetHIDDevices(wxComboBox * cmb, wxArrayString& paths) {
    if (hid_init() == 0) {
        hid_device_info * devices;
        devices = hid_enumerate(0, 0);
        AppendHIDDevice(devices, cmb, paths);
        hid_free_enumeration(devices);
        hid_exit();
    }
}
void HIDTool::AppendHIDDevice(hid_device_info* newDeviceInfo, wxComboBox * cmb, wxArrayString& paths) {
    if (newDeviceInfo) {
        wxString item(newDeviceInfo->manufacturer_string);
        item += _T(" - ") + wxString(newDeviceInfo->product_string);
        cmb->Append(item);
        paths.Add(wxString(newDeviceInfo->path));
        if (newDeviceInfo->next) AppendHIDDevice(newDeviceInfo->next, cmb, paths);
    }
}
