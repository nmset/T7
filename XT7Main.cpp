/* 
 * File:   XT7Main.cpp
 * Author: SET - nmset@yandex.com
 * Licence : LGPL 2.1
 * Copyright SET, M.D. - © 2014
 * 
 * Created on 1 mars 2014, 15:00
 */

#include "XT7Main.h"
#include <wx/msgdlg.h>
#include <wx/notifmsg.h>
#include <wx/stdpaths.h>
#include <iostream>
using namespace std;

int wxCALLBACK
CompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr WXUNUSED(sortData))
{
    // Borrowed from wxListCtrl sample app, mostly verbatim.
    if (item1 < item2)
        return -1;
    if (item1 > item2)
        return 1;

    return 0;
}

XT7Main::XT7Main() {
}

XT7Main::XT7Main( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
: T7Main(parent, id, caption, pos, size, style ){
    wxConfigBase::DontCreateOnDemand();
    config = static_cast<wxConfig*> (wxConfigBase::Get(false));
    if (!config) {
        config = new wxConfig(_APPNAME_T7_, _T("NMSET"), _APPNAME_T7_, wxEmptyString, wxCONFIG_USE_SUBDIR);
        wxConfigBase::Set(config);
    }
    Init();
    configError = !ArePedalsFullyConfigured();
    if (!configError) {
        ListenToPedal();
        Bind(wxEVT_IDLE, &XT7Main::OnIdle, this);
        m_keyHandler = new KeyboardSimulation(pedalEVH);
    } else {
        MessageBox(_(L"Configuration du pédalier incomplète."), true);
    }
    UpdateTitle();
}

XT7Main::~XT7Main() {
#ifdef T7_EMBEDDED
    // In case it's embedded in some other application
    if (!streamError && !configError) {
        if (pedMonitor && pedMonitor->GetThread()->IsRunning()) pedMonitor->GetThread()->Delete();
        if (mediaProgress && mediaProgress->GetThread()->IsRunning()) mediaProgress->GetThread()->Delete();
        if (pedCodeIdentifier  && pedCodeIdentifier->GetThread()->IsRunning()) pedCodeIdentifier->GetThread()->Delete();
    }
#endif
    delete m_keyHandler;
}

void XT7Main::UpdateTitle() {
    wxString title(_APPNAME_T7_ + _T(" - ")+ _APPVERSION_T7_);
    if (lvMediaList->IsEmpty() || lvMediaList->GetFocusedItem() == -1) {
        SetTitle(title);
        return;
    }
    // Get the file name
    wxListItem it;
    it.SetId(lvMediaList->GetFocusedItem());
    it.SetColumn(2);
    it.SetMask(wxLIST_MASK_TEXT);
    if (lvMediaList->GetItem(it)) {
        title += _T(" : ") + it.GetText();
        SetTitle(title);
    }
}

void XT7Main::MessageBox(const wxString& msg, const bool notify){
    if (!notify) {
        wxMessageBox(msg, _APPNAME_T7_, wxOK, this);
    } else {
        wxNotificationMessage notifMsg(_APPNAME_T7_, msg, this);
        notifMsg.Show();
    }
}
void XT7Main::CollapsiblePaneChanged(wxCollapsiblePaneEvent& evt) {
    if (evt.GetEventObject() == panePedalIDs || evt.GetEventObject() == panePedalHardware) {
        szPedalHardware->Layout();
        szPedalMain->Layout();
        szMediaInfo->Layout();
        panPedals->GetSizer()->Layout();
    }
}
void XT7Main::Init() {
    pedMonitor = NULL; pedalEVH = NULL; pedCodeIdentifier = NULL; mediaProgress = NULL;
    streamError = false;
    if (!wxDir::Exists(wxStandardPaths::Get().GetUserDataDir()))
        wxDir::Make(wxStandardPaths::Get().GetUserDataDir());
    // UI doesn't provide a direct way to insert a wxListView
    lvMediaList = new wxListView(panPedals);
    szMediaTop->Add(lvMediaList, 1, wxGROW|wxALL, 5);
    medMain->Show(false);
    // cmb = wxComboBox
    HIDTool::GetHIDDevices(cmbHIDDevices, hidPaths);
    wxString device = config->Read(_T("/PEDALES/Dispositif"));
    cmbHIDDevices->SetSelection(cmbHIDDevices->FindString(device, true));
    cmbPedals->Append(_(L"de gauche"));
    cmbPedals->Append(_(L"du milieu"));
    cmbPedals->Append(_(L"de droite"));
    cmbPedalActionLeft->Append(_(L"Retour rapide"));
    cmbPedalActionLeft->Append(_(L"Avance rapide"));
    cmbPedalActionLeft->Append(_(L"Lecture"));
    cmbPedalActionMiddle->Append(_(L"Retour rapide"));
    cmbPedalActionMiddle->Append(_(L"Avance rapide"));
    cmbPedalActionMiddle->Append(_(L"Lecture"));
    cmbPedalActionRight->Append(_(L"Retour rapide"));
    cmbPedalActionRight->Append(_(L"Avance rapide"));
    cmbPedalActionRight->Append(_(L"Lecture"));
    // Automatic rewind on pause
    // txt = wxTextCtrl
    txtMediaAutoRewind->SetValidator(wxTextValidator(wxFILTER_DIGITS));
    long autoRw = 1500;
    config->Read(_T("/PEDALES/DIVERS/RetourAuto"), &autoRw);
    txtMediaAutoRewind->SetValue(wxVariant(autoRw).GetString());
    /*
     * We provide default usual pedal actions :
     * Left pedal : rewind
     * Middle pedal : fast forward
     * Right pedal : play
     */
    long actionIndex = wxNOT_FOUND;
    if (!config->Read(_T("/PEDALES/ACTIONS/Gauche"), &actionIndex)
            && !config->Read(_T("/PEDALES/ACTIONS/Milieu"), &actionIndex)
            && !config->Read(_T("/PEDALES/ACTIONS/Droit"), &actionIndex)) {
        config->Write(_T("/PEDALES/ACTIONS/Gauche"), 0);
        config->Write(_T("/PEDALES/ACTIONS/Milieu"), 1);
        config->Write(_T("/PEDALES/ACTIONS/Droit"), 2);
        config->Flush();
    }
    actionIndex = wxNOT_FOUND;
    config->Read(_T("/PEDALES/ACTIONS/Gauche"), &actionIndex);
    cmbPedalActionLeft->SetSelection(actionIndex);
    actionIndex = wxNOT_FOUND;
    config->Read(_T("/PEDALES/ACTIONS/Milieu"), &actionIndex);
    cmbPedalActionMiddle->SetSelection(actionIndex);
    actionIndex = wxNOT_FOUND;
    config->Read(_T("/PEDALES/ACTIONS/Droit"), &actionIndex);
    cmbPedalActionRight->SetSelection(actionIndex);
    // lbl = wxStaticText
    lblMediaCurrent->SetLabel(wxEmptyString);
    lblMediaLength->SetLabel(wxEmptyString);
    // sld = wxSlider
    sldMediaPosition->Bind(wxEVT_SLIDER, &XT7Main::SliderChanged, this);
    lvMediaList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &XT7Main::LoadMedia, this);
    // dpk = wxDirPicker
    dpkMediaRoot->GetTextCtrl()->SetEditable(false);
    dpkMediaRoot->SetPath(config->Read(_T("/Medias/Racine")));
    dpkMediaRoot->Bind(wxEVT_DIRPICKER_CHANGED, &XT7Main::MediaRootChanged, this);
    // btn = wxButton
    btnMediaRootRefresh->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &XT7Main::ListMedia, this);
    btnAbout->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &XT7Main::ShowAbout, this);
    cmbHIDDevices->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XT7Main::SavePedalDevice, this);
    cmbPedalActionLeft->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XT7Main::SavePedalAction, this);
    cmbPedalActionMiddle->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XT7Main::SavePedalAction, this);
    cmbPedalActionRight->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XT7Main::SavePedalAction, this);
    cmbPedals->Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &XT7Main::StartPedalCodeIdentification, this);
    // pane = wxCollapsiblePane
    panePedalIDs->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &XT7Main::CollapsiblePaneChanged, this);
    panePedalHardware->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &XT7Main::CollapsiblePaneChanged, this);
    txtMediaAutoRewind->Bind(wxEVT_COMMAND_TEXT_ENTER, &XT7Main::SavePedalAutoRewind, this);
    // For display on Android handhelds where PPI may be set high for readable display
    wxDouble scaleFactor = 1.0;
    const wxDouble ppi = wxGetDisplayPPI().GetY();
    if (ppi > 96.0) scaleFactor = (ppi / 96.0);
    SetSize((int) (600.0 * scaleFactor), (int) (450.0 * scaleFactor));
}
bool XT7Main::ListenToPedal() {
    // Create a pedal monitor and an event handler.
    wxString device;
    if (!config->Read(_T("/PEDALES/Dispositif"), &device)) return false;
    if (!AreAllPedalsIdentified()) return false;
    wxString devicePath = wxEmptyString;
    if (cmbHIDDevices->GetSelection() != wxNOT_FOUND)
        devicePath = hidPaths.Item(cmbHIDDevices->GetSelection());
    long leftCode, middleCode, rightCode;
    config->Read(_T("/PEDALES/ID/Gauche"), &leftCode);
    config->Read(_T("/PEDALES/ID/Milieu"), &middleCode);
    config->Read(_T("/PEDALES/ID/Droit"), &rightCode);
    long autoRewind = 1500;
    config->Read(_T("/PEDALES/DIVERS/RetourAuto"), &autoRewind);
    pedalEVH = new PedalEVH(this, (wxFileOffset) autoRewind);
    pedMonitor = new PedalMonitor_OnOff(devicePath, pedalEVH, leftCode, middleCode, rightCode);
    //pedMonitor = new PedalMonitor_Override(devicePath, pedalEVH, leftCode, middleCode, rightCode);
    if (pedMonitor->GetThread()->Run() != wxTHREAD_NO_ERROR) {
        delete pedalEVH; pedalEVH = NULL;
        if (pedMonitor->GetThread()->IsRunning()) {
            pedMonitor->GetThread()->Delete();
            pedMonitor = NULL;
        }
        return false;
    }
    return true;
}
void XT7Main::SavePedalDevice(wxCommandEvent& evt) {
    if (config->Write(_T("/PEDALES/Dispositif"), cmbHIDDevices->GetValue())) {
        MessageBox(_(L"Enregistré. Vous ne devriez pas jouer avec cette valeur une fois bien paramétrée."), true);
        // Individual pedals must be identified again.
        config->DeleteGroup(_T("/PEDALES/ID"));
        config->Flush();
        //MessageBox(_T("Vous devez compléter l'identification de toutes les pédales."), true);
    } else {
        MessageBox(_(L"Echec de sauvegarde."), true);
    }
}
void XT7Main::SavePedalAction(wxCommandEvent& evt) {
    // What the user decides to do with each pedal
    bool inconsistent = false;
    wxComboBox * cmb = static_cast<wxComboBox*> (evt.GetEventObject());
    const int actionIndex = cmb->GetSelection();
    if (cmb == cmbPedalActionLeft) {
        inconsistent = (actionIndex == cmbPedalActionMiddle->GetSelection() || actionIndex == cmbPedalActionRight->GetSelection());
    }
    if (cmb == cmbPedalActionMiddle) {
        inconsistent = (actionIndex == cmbPedalActionLeft->GetSelection() || actionIndex == cmbPedalActionRight->GetSelection());
    }
    if (cmb == cmbPedalActionRight) {
        inconsistent = (actionIndex == cmbPedalActionLeft->GetSelection() || actionIndex == cmbPedalActionMiddle->GetSelection());
    }
    if (inconsistent) {
        MessageBox(_(L"Choix inconsistants."), true);
        return;
    }
    config->Write(_T("/PEDALES/ACTIONS/Gauche"), cmbPedalActionLeft->GetSelection());
    config->Write(_T("/PEDALES/ACTIONS/Milieu"), cmbPedalActionMiddle->GetSelection());
    config->Write(_T("/PEDALES/ACTIONS/Droit"), cmbPedalActionRight->GetSelection());
    config->Flush();
}
void XT7Main::SavePedalAutoRewind(wxCommandEvent& evt) {
    config->Write(_T("/PEDALES/DIVERS/RetourAuto"), wxVariant(txtMediaAutoRewind->GetValue()).GetLong());
    config->Flush();
    long newAutoRewind;
    config->Read(_T("/PEDALES/DIVERS/RetourAuto"), &newAutoRewind);
    if (pedalEVH) pedalEVH->UpdateAutoRewind((wxFileOffset) newAutoRewind);
}
bool XT7Main::ArePedalActionsConsistent() {
    // One pedal, one unique action.
    const int left = cmbPedalActionLeft->GetSelection();
    const int middle = cmbPedalActionMiddle->GetSelection();
    const int right = cmbPedalActionRight->GetSelection();
    if ((left == wxNOT_FOUND) || (middle == wxNOT_FOUND) || (right == wxNOT_FOUND)) return false;
    if ((left == middle) || (left == right)) return false;
    if ((middle == right)) return false;
    return true;
}
void XT7Main::StartPedalCodeIdentification(wxCommandEvent& evt) {
    // Starts the thread that waits for a pedal to be pressed to catch the byte sent
    if (streamError) return;
    lblPedalCode->SetLabel(wxEmptyString);
    if (!pedalEVH) pedalEVH = new PedalEVH(this);
    if (!pedCodeIdentifier) {
        wxString device = wxEmptyString;
        config->Read(_T("/PEDALES/Dispositif"), &device);
        if (device.IsEmpty()) {
            MessageBox(_T("Périphérique non déclaré."), true);
            return;
        }
        wxString devicePath = wxEmptyString;
        devicePath = hidPaths.Item(cmbHIDDevices->GetSelection());
        pedCodeIdentifier = new PedalCodeIdentifier(devicePath, pedalEVH);
        pedCodeIdentifier->GetThread()->Run();
    }
}
void XT7Main::UpdatePedalCode(const unsigned short code) {
    /*
     * Once started, process each pedal.
     * We delete all known pedal codes and start all over.
     * The user needs to do this only once
     * or if she changes hardware
     * Easier to code this way.
     */
    static bool started = false;
    if (!started) {
        config->DeleteGroup(_T("/PEDALES/ID"));
        config->Flush();
        started = true;
        MessageBox(_T("Vous devez compléter l'identification de toutes les pédales."), true);
    }
    switch (cmbPedals->GetSelection()) {
        case 0:
            config->Write(_T("/PEDALES/ID/Gauche"), (long) code);
            break;
        case 1:
            config->Write(_T("/PEDALES/ID/Milieu"), (long) code);
            break;
        case 2:
            config->Write(_T("/PEDALES/ID/Droit"), (long) code);
            break;
        default:
            return;
    }
    config->Flush();
    lblPedalCode->SetLabel(_(L"Enregistré. Vous pouvez identifier une autre pédale."));
    configError = !ArePedalsFullyConfigured();
    if (!configError) {
        MessageBox(_T("Cette fenêtre va se fermer. Veuillez recommencer."));
        Close();
    }
}
bool XT7Main::AreAllPedalsIdentified() {
    long left, middle, right;
    return (config->Read(_T("/PEDALES/ID/Gauche"), &left)
        && config->Read(_T("/PEDALES/ID/Milieu"), &middle)
        && config->Read(_T("/PEDALES/ID/Droit"), &right));
}
bool XT7Main::ArePedalsFullyConfigured() {
    wxString device = wxEmptyString;
    return (config->Read(_T("/PEDALES/Dispositif"), &device) && AreAllPedalsIdentified());
}
void XT7Main::SliderChanged(wxCommandEvent& evt) {
    // Manual media positioning
    if (medMain) {
        int pos = sldMediaPosition->GetValue();
        medMain->Seek((wxFileOffset) pos);
        wxTimeSpan past(0, 0, 0, pos);
        lblMediaCurrent->SetLabel(past.Format(_T("%H:%M:%S")));
        szMediaInfo->Layout();
    }
}
void XT7Main::UpdateMediaProgressPosition() {
    // Called from MediaProgress::Entry() via PedalEVH using CallAfter.
    if (medMain) {   
        sldMediaPosition->SetValue(medMain->Tell());
        wxTimeSpan past(0, 0, 0, medMain->Tell());
        lblMediaCurrent->SetLabel(past.Format(_T("%H:%M:%S")));
    } else {
        // Should not happen !!!
        sldMediaPosition->SetValue(0);
        lblMediaCurrent->SetLabel(wxEmptyString);
        lblMediaLength->SetLabel(wxEmptyString);
    }
    szMediaInfo->Layout();
}
void XT7Main::UpdateMediaControlPosition(wxFileOffset position) {
    // Called by PedalEVH from MediaFastMove using CallAfter.
    if (medMain) { 
        medMain->Seek(position, wxFromCurrent);
    }
}
void XT7Main::LoadMedia(wxListEvent& evt) {
    /*
     * N.B. : Length() is always 0 *here*, and in wxEVT_MEDIA_LOADED also.
     *  But may be real in OnIdle, media dependent.
     * Length() may be 0 in OnIdle for some valid media.
     * Media type does not seem to be a factor (mp3, mp4, avi ...).
     */
    UpdateTitle(); // In case we return early.
    if (lvMediaList->GetFocusedItem() == -1) return; // REALLY ? ON DOUBLE CLICK OR ENTER
    // Get the file name
    wxListItem it;
    it.SetId(lvMediaList->GetFocusedItem());
    it.SetColumn(2);
    it.SetMask(wxLIST_MASK_TEXT);
    if (lvMediaList->GetItem(it)) {
        wxString mediaRoot = config->Read(_T("/Medias/Racine"));
        if (mediaRoot.IsEmpty()) return;
        wxFileName media(mediaRoot + wxFileName::GetPathSeparator() + it.GetText());
        if (media.Exists()) {
            if (pedalEVH)
                pedalEVH->PauseFastMoveWorkers();
            // Manage media control
            medMain->Stop();
            if (medMain->Load(media.GetFullPath())) {
                UpdateTitle();
                loadedMediaPath = media.GetFullPath();
                medMain->Show();
                medMain->SetSize(medMain->GetBestSize());
                panPedals->GetSizer()->Layout();
            } else {
                medMain->Hide();
            }
            sldMediaPosition->SetValue(0);
            wxTimeSpan zero(0);
            lblMediaCurrent->SetLabel(zero.Format(_T("%H:%M:%S")));
            // Gives back positioning information
            if (!mediaProgress) {
                if (!configError) {
                    mediaProgress = new MediaProgress(pedalEVH);
                    mediaProgress->GetThread()->Run(); mediaProgress->GetThread()->Pause();
                }
            } else {
                // if LoadMedia called while an action is going on
                if (mediaProgress->GetThread()->IsRunning())
                        mediaProgress->GetThread()->Pause();
            }
        }
    }
}
void XT7Main::OnIdle(wxIdleEvent& evt) {
    if (lvMediaList->GetFocusedItem() == -1) return;

    wxListItem itFileName;
    itFileName.SetId(lvMediaList->GetFocusedItem());
    itFileName.SetColumn(2);
    itFileName.SetMask(wxLIST_MASK_TEXT);
    
    if (lvMediaList->GetItem(itFileName)) {
        wxString mediaRoot = config->Read(_T("/Medias/Racine"));
        if (mediaRoot.IsEmpty()) return;
        wxFileName media(mediaRoot + wxFileName::GetPathSeparator() + itFileName.GetText());
        if (media.GetFullPath() != loadedMediaPath)
            return;
    }
    wxListItem itDuration;
    itDuration.SetId(lvMediaList->GetFocusedItem());
    itDuration.SetColumn(1);
    itDuration.SetMask(wxLIST_MASK_TEXT);
    
    // Manage positioning controls. Update if changed only. May avoid flickering.
    if (sldMediaPosition->GetMax() != medMain->Length())
        sldMediaPosition->SetMax(medMain->Length());
    wxTimeSpan duration(0, 0, 0, medMain->Length());
    if (lblMediaLength->GetLabel() != duration.Format(_T("%H:%M:%S")))
        lblMediaLength->SetLabel(duration.Format(_T("%H:%M:%S")));
    if (lvMediaList->GetItem(itDuration)) {
        if (itDuration.GetText() != lblMediaLength->GetLabel()) {
            itDuration.SetText(lblMediaLength->GetLabel());
            lvMediaList->SetItem(itDuration);
        }
    }
}
void XT7Main::MediaRootChanged(wxFileDirPickerEvent& evt) {
    // A directory where we look for media files.
    config->Write(_T("/Medias/Racine"), dpkMediaRoot->GetPath());
    config->Flush();
    loadedMediaPath = wxString();
}
void XT7Main::ListMedia(wxCommandEvent& evt) {
    // We don't recurse in sub directories.
    wxString mediaRoot = config->Read(_T("/Medias/Racine"));
    if (mediaRoot.IsEmpty()) return;
    lvMediaList->ClearAll();
    wxArrayString files;
    wxDir::GetAllFiles(mediaRoot, &files, wxEmptyString, wxDIR_FILES);
    lvMediaList->AppendColumn(_T("Date"));
    lvMediaList->AppendColumn(_T("Durée"));
    lvMediaList->AppendColumn(_T("Dictée"));
    
    for (int i = 0; i < files.GetCount(); i++) {
        wxListItem it;
        wxFileName filename(files.Item(i));
        it.SetId(i);
        it.SetColumn(0);
        it.SetText(filename.GetModificationTime().Format(_T("%a %d %b %Y %H:%M:%S")));
        long idx = lvMediaList->InsertItem(it);
        lvMediaList->SetItemData(idx, (long) filename.GetModificationTime().GetTicks());
        
        it.SetColumn(2);
        it.SetText(filename.GetFullName());
        lvMediaList->SetItem(it);
    }
    lvMediaList->SetColumnWidth(0, wxLIST_AUTOSIZE);
    lvMediaList->SetColumnWidth(2, wxLIST_AUTOSIZE);
    lvMediaList->SortItems(CompareFunction, 0);
    panPedals->GetSizer()->Layout();
    loadedMediaPath = wxString();
    UpdateTitle();
}
void XT7Main::SetStreamError() {
    streamError = true;
    // We request application restart when stream is established again.
    // We don't try to detect if the problem is resolved. Restart is cheap.
    MessageBox(_(L"Erreur de communication avec le pédalier. Après avoir réglé le problème,"
            " veuillez redémarrer l'application."
            "\n\n Au clavier : F5, F6, F7, ESC."), true);
    medMain->Stop();
    UpdateMediaProgressPosition();
    if (mediaProgress && mediaProgress->GetThread()->IsRunning()) mediaProgress->GetThread()->Pause();
    if (pedalEVH) pedalEVH->ResetFastMoveWorkers();
}
void XT7Main::ShowAbout(wxCommandEvent& evt) {
    wxString msg = _APPNAME_T7_ + _T(" - ") + _APPVERSION_T7_ + _T("\n\n");
    msg += _(L"Auteur et copyright :") + _T("\n");
    msg += wxString(_T("SET, M.D.")) + _T("\n");
    msg += wxString(_T("nmset@yandex.com")) + _T("\n\n");
    msg += wxString(_(L"Sous license LGPL"));
    MessageBox(msg);
}
////////////////////////////////////////////////////////////////////////////////////////
/*
 * Throughout, we want one background non-blocking worker through each
 * pedal manager class. We do not want nor need concurrent multiple threads.
 * MediaFastMove will have two non-concurrent threads.
 * MediaFastMove threads are managed here. They never die.
 * MediaProgress thread is managed by XT7Main. It never dies.
 */
PedalEVH::PedalEVH(XT7Main * parent, wxFileOffset newAutoRewindOnPause) {
    m_owner = parent;
    m_Rewind = NULL;
    m_Forward = NULL;
    m_autoRewind = newAutoRewindOnPause;
}
PedalEVH::~PedalEVH() {
    ResetFastMoveWorkers();
}
void PedalEVH::UpdateAutoRewind(wxFileOffset newAutoRewindOnPause) {
    m_autoRewind = newAutoRewindOnPause;
}
void PedalEVH::ResetFastMoveWorkers() {
    // They should be assigned NULL from OnFastMoveExit()
    // If IsRunning() is false, Delete() crashes app.
    if (m_Rewind && m_Rewind->GetThread()->IsRunning()) {
        m_Rewind->GetThread()->Delete();
    }
    if (m_Forward && m_Forward->GetThread()->IsRunning()) {
        m_Forward->GetThread()->Delete();
    }
}
void PedalEVH::PauseFastMoveWorkers() {
    if (m_Rewind && m_Rewind->GetThread()->IsRunning()) {
        m_Rewind->GetThread()->Pause();
    }
    if (m_Forward && m_Forward->GetThread()->IsRunning()) {
        m_Forward->GetThread()->Pause();
    }
}
/*
 * A given pedal fires an action as declared by the user.
 */
void PedalEVH::Left(PedalEvent::PedalStatus status) {
    const int action = m_owner->cmbPedalActionLeft->GetSelection();
    switch (action) {
        case 0:
            Rewind(status);
            break;
        case 1:
            FastForward(status);
            break;
        case 2:
            Play(status);
            break;
        default :
            m_owner->MessageBox(_(L"Action mal déclarée."), true);
    }
    
}
void PedalEVH::Middle(PedalEvent::PedalStatus status) {
    const int action = m_owner->cmbPedalActionMiddle->GetSelection();
    switch (action) {
        case 0:
            Rewind(status);
            break;
        case 1:
            FastForward(status);
            break;
        case 2:
            Play(status);
            break;
        default :
            m_owner->MessageBox(_(L"Action mal déclarée."), true);
    }
}
void PedalEVH::Right(PedalEvent::PedalStatus status) {
    const int action = m_owner->cmbPedalActionRight->GetSelection();
    switch (action) {
        case 0:
            Rewind(status);
            break;
        case 1:
            FastForward(status);
            break;
        case 2:
            Play(status);
            break;
        default :
            m_owner->MessageBox(_(L"Action mal déclarée."), true);
    }
}
void PedalEVH::Rewind(PedalEvent::PedalStatus status) {
    if (status == PedalEvent::PRESSED) {
        if (!m_Rewind) {
            m_Rewind = new MediaFastMove(m_owner->medMain, -100, 10, this);
            m_Rewind->GetThread()->Run();
        }
        if (m_Rewind->GetThread()->IsPaused()) m_Rewind->GetThread()->Resume();
        // UI can update media position.
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsPaused()) m_owner->mediaProgress->GetThread()->Resume();
    }
    if (status == PedalEvent::RELEASED) {
        if (m_Rewind && m_Rewind->GetThread()->IsRunning()) m_Rewind->GetThread()->Pause();
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsRunning()) m_owner->mediaProgress->GetThread()->Pause();
    }
}
void PedalEVH::FastForward(PedalEvent::PedalStatus status) {
    if (status == PedalEvent::PRESSED) {
        if (!m_Forward) {
            m_Forward = new MediaFastMove(m_owner->medMain, 100, 10, this);
            m_Forward->GetThread()->Run();
        }
        if (m_Forward->GetThread()->IsPaused()) m_Forward->GetThread()->Resume();
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsPaused()) m_owner->mediaProgress->GetThread()->Resume();
    }
    if (status == PedalEvent::RELEASED) {
        if (m_Forward && m_Forward->GetThread()->IsRunning()) m_Forward->GetThread()->Pause();
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsRunning()) m_owner->mediaProgress->GetThread()->Pause();
    }
}
void PedalEVH::Play(PedalEvent::PedalStatus status) {
    // Not done through a thread.
    if (status == PedalEvent::PRESSED) {
        m_owner->medMain->Play();
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsPaused()) m_owner->mediaProgress->GetThread()->Resume();
    }
    if (status == PedalEvent::RELEASED) {
        m_owner->medMain->Pause();
        if (m_owner->mediaProgress && m_owner->mediaProgress->GetThread()->IsRunning()) m_owner->mediaProgress->GetThread()->Pause();
        wxFileOffset pos = m_owner->medMain->Tell();
        // Transcriptionists need that automatic rewind feature on pause.
        if (pos > m_autoRewind) {
            m_owner->medMain->Seek((m_autoRewind * -1), wxFromCurrent);
            OnMediaProgressPosition();
        }
    }
}
// From PedalCodeIdentifier
void PedalEVH::OnPedalCaught(const unsigned short code) {
    if (!code) return;
    XT7Main * xapp = static_cast<XT7Main*> (m_owner);
    xapp->CallAfter(&XT7Main::UpdatePedalCode, code);
}
void PedalEVH::OnCodeIdentifierExit(PedalCodeIdentifier* active) {
    m_owner->pedCodeIdentifier = NULL;
}
void PedalEVH::OnMediaProgressExit(MediaProgress* active) {
    m_owner->mediaProgress = NULL;
}
void PedalEVH::OnMediaProgressPosition() {
    XT7Main * xapp = static_cast<XT7Main*> (m_owner);
    xapp->CallAfter(&XT7Main::UpdateMediaProgressPosition);
}
void PedalEVH::OnMediaControlPosition(wxFileOffset position) {
    XT7Main * xapp = static_cast<XT7Main*> (m_owner);
    xapp->CallAfter(&XT7Main::UpdateMediaControlPosition, position);
}
void PedalEVH::OnStreamError() {
    m_owner->SetStreamError();
}
void PedalEVH::OnPedalMonitorExit() {
    m_owner->pedMonitor = NULL;
}
void PedalEVH::OnFastMoveExit(MediaFastMove * active) {
    if (active == m_Forward) {
        m_Forward = NULL;
    }
    if (active == m_Rewind) {
        m_Rewind = NULL;
    }
}
