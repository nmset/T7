/* 
 * File:   PedalMonitor.h
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * Licence : LGPL  2.1
 * Copyright Saleem Edah-Tally, M.D. - © 2014
 *
 * Created on 18 février 2014, 20:52
 */

#ifndef PEDALMONITOR_H
#define	PEDALMONITOR_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/thread.h>
#include <wx/mediactrl.h>
#include <hidapi/hidapi.h>

class PedalEvent;
class PedalCodeIdentifier;
class PedalBytesPerClickFinder;
class MediaFastMove;
class MediaProgress;
class HIDTool;

class IPedalMonitor: public wxThreadHelper {
public:
    IPedalMonitor();
    virtual ~IPedalMonitor();
protected:        
    virtual wxThread::ExitCode Entry() = 0;

};
/*
 * In both modes below, pressing three pedals blocks everything.
 * No pedal pressed means no action.
 */
/*
 * In this mode, pressing a second pedal blocks everything.
 * Releasing one pedal resumes the action of the pressed pedal.
 */
class PedalMonitor_OnOff : public IPedalMonitor {
public:
    PedalMonitor_OnOff(const wxString& newDevice, PedalEvent * newPedalEVH, const unsigned short newLeftCode = 1, const unsigned short newMiddleCode = 2, const unsigned short newRightCode = 4);
    virtual ~PedalMonitor_OnOff();
protected:        
    virtual wxThread::ExitCode Entry();
private:
    PedalEvent * m_pedalEVH;
    unsigned short m_leftCode, m_middleCode, m_rightCode;
    unsigned short m_current;
    wxString m_device;
};
/*
 *In this mode, pressing a second pedal overrides the first pedal.
 * The action of the last pedal pressed takes precedence.
 * Releasing one pedal resumes the action of the pressed pedal.
 */
class PedalMonitor_Override : public IPedalMonitor {
public:
    PedalMonitor_Override(const wxString& newDevice, PedalEvent * newPedalEVH, const unsigned short newLeftCode = 1, const unsigned short newMiddleCode = 2, const unsigned short newRightCode = 4);
    virtual ~PedalMonitor_Override();
protected:        
    virtual wxThread::ExitCode Entry();
private:
    PedalEvent * m_pedalEVH;
    unsigned short m_leftCode, m_middleCode, m_rightCode;
    unsigned short m_current, m_previous;
    wxString m_device;
};
/*
 * A single class to manage events form many classes.
 * Function names are explicit.
 * Not using custom wxWidgets events facility because it's abstract to me, sorry.
 */
class PedalEvent {
public:
    enum PedalStatus{RELEASED = 0, PRESSED};
    enum PedalPressed{NONE = 0, LEFT, MIDDLE, RIGHT};
    PedalEvent();
    virtual ~PedalEvent();

    virtual void Left(PedalEvent::PedalStatus status);
    virtual void Middle(PedalEvent::PedalStatus PedalStatus);
    virtual void Right(PedalEvent::PedalStatus PedalStatus);
    virtual void OnPedalMonitorExit();
    virtual void OnFastMoveExit(MediaFastMove * active);
    virtual void OnCodeIdentifierExit(PedalCodeIdentifier * active);
    virtual void OnPedalCaught(const unsigned short code);
    virtual void OnMediaProgressExit(MediaProgress * active);
    virtual void OnMediaProgressPosition();
    virtual void OnMediaControlPosition(wxFileOffset position);
    virtual void OnStreamError();
private:
    
};
/*
 * For fast forward and rewind
 */
class MediaFastMove : public wxThreadHelper {
public:
    MediaFastMove (wxMediaCtrl * newMedia, wxFileOffset newStep, unsigned long newSleepMs, PedalEvent * newCaller); 
    virtual ~MediaFastMove();
    
protected:        
    virtual wxThread::ExitCode Entry();
    
private:
    wxMediaCtrl * m_media;
    wxFileOffset m_step;
    unsigned long m_sleepMs;
    PedalEvent * m_pedalEVH;
};
/*
 * Identify the bytes sent by each pedal.
 */
class PedalCodeIdentifier : public wxThreadHelper {
public:
    PedalCodeIdentifier(const wxString& newDevice, PedalEvent * newPedalEvent);
    ~PedalCodeIdentifier();
protected:
    virtual wxThread::ExitCode Entry();
private:
    PedalEvent * m_pedalEVH;
    wxString m_device;
};
/*
 * Update media position in UI.
 */
class MediaProgress : public wxThreadHelper {
public:
    MediaProgress(PedalEvent * newPedalEvent);
    ~MediaProgress();
protected:
    virtual wxThread::ExitCode Entry();
private:
    PedalEvent * m_pedalEVH;
};

class HIDTool {
public:
    HIDTool();
    virtual ~HIDTool();
    
    static void GetHIDDevices(wxComboBox * cmb, wxArrayString& paths);
private:
    static void AppendHIDDevice(hid_device_info * newDeviceInfo, wxComboBox * cmb, wxArrayString& paths);
};

#endif	/* PEDALMONITOR_H */

