/* 
 * File:   XT7Main.h
 * Author: SET - nmset@yandex.com
 * Licence : LGPL  2.1
 * Copyright SET, M.D. - © 2014
 *
 * Created on 1 mars 2014, 15:00
 */

#ifndef XT7MAIN_H
#define	XT7MAIN_H

#include "UI/t7main.h"
#include "PedalManager.h"
#include "KeyboardSimulation.h"
#include <wx/config.h>
#include <wx/listctrl.h>
#include <wx/dir.h>

class PedalEVH;
class KeyboardSimulation;

class XT7Main : public T7Main {
public:
    friend class PedalEVH;
    XT7Main();
    XT7Main( wxWindow* parent, wxWindowID id = SYMBOL_T7MAIN_IDNAME, const wxString& caption = SYMBOL_T7MAIN_TITLE, const wxPoint& pos = SYMBOL_T7MAIN_POSITION, const wxSize& size = SYMBOL_T7MAIN_SIZE, long style = SYMBOL_T7MAIN_STYLE );
    virtual ~XT7Main();
private:
    wxConfig * config;
    IPedalMonitor * pedMonitor;
    PedalEVH * pedalEVH;
    PedalCodeIdentifier * pedCodeIdentifier;
    MediaProgress * mediaProgress;
    wxListView * lvMediaList;
    KeyboardSimulation * m_keyHandler;
    bool streamError;
    bool configError;
    wxArrayString hidPaths;
    wxString loadedMediaPath;
    
    void CollapsiblePaneChanged(wxCollapsiblePaneEvent& evt);
    void UpdateTitle();
    void MessageBox(const wxString& msg, const bool notify = false);
    bool ListenToPedal();
    void Init();
    void SavePedalDevice(wxCommandEvent& evt);
    void SavePedalAction(wxCommandEvent& evt);
    void SavePedalAutoRewind(wxCommandEvent& evt);
    bool ArePedalActionsConsistent();
    void StartPedalCodeIdentification(wxCommandEvent& evt);
    void UpdatePedalCode(const unsigned short code);
    bool AreAllPedalsIdentified();
    bool ArePedalsFullyConfigured();
    void SliderChanged(wxCommandEvent& evt); // OTHER EVENTS ARE wxScrollEvent
    void UpdateMediaProgressPosition();
    void UpdateMediaControlPosition(wxFileOffset position);
    void LoadMedia(wxListEvent& evt);
    void MediaRootChanged(wxFileDirPickerEvent& evt);
    void ListMedia(wxCommandEvent& evt);
    void SetStreamError();
    void ShowAbout(wxCommandEvent& evt);
    void OnIdle(wxIdleEvent& evt);
};

///////////////////////////////////////////////////////////////////////////////////////
/*
 * Event handler of PedalEvent class.
 */
class PedalEVH  : public PedalEvent {
public:
    friend class KeyboardSimulation;
    PedalEVH(XT7Main * parent, wxFileOffset newAutoRewindOnPause = 1000);
    virtual ~PedalEVH();
    void UpdateAutoRewind(wxFileOffset newAutoRewindOnPause);
    void ResetFastMoveWorkers();
    void PauseFastMoveWorkers();
private:
    XT7Main * m_owner;
    MediaFastMove * m_Forward;
    MediaFastMove * m_Rewind;
    wxFileOffset m_autoRewind;
    
    void Left(PedalEvent::PedalStatus status);
    void Middle(PedalEvent::PedalStatus status);
    void Right(PedalEvent::PedalStatus status);
    void OnPedalMonitorExit();
    void OnFastMoveExit(MediaFastMove * active);
    void Rewind(PedalEvent::PedalStatus status);
    void FastForward(PedalEvent::PedalStatus status);
    void Play(PedalEvent::PedalStatus status);
    void OnPedalCaught(const unsigned short code);
    void OnCodeIdentifierExit(PedalCodeIdentifier * active);
    void OnMediaProgressExit(MediaProgress * active);
    void OnMediaProgressPosition();
    void OnMediaControlPosition(wxFileOffset position);
    void OnStreamError();
};

#endif	/* XT7MAIN_H */

