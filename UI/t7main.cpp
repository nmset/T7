/////////////////////////////////////////////////////////////////////////////
// Name:        t7main.cpp
// Purpose:     
// Author:      SET
// Modified by: 
// Created:     sam. 01 mars 2014 14:23:16 CET
// RCS-ID:      
// Copyright:   Copyright SET (nmset@yandex.com) - © 2014.
// Licence:     LGPL 2.1
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "t7main.h"

////@begin XPM images
////@end XPM images


/*
 * T7Main type definition
 */

IMPLEMENT_CLASS( T7Main, wxFrame )


/*
 * T7Main event table definition
 */

BEGIN_EVENT_TABLE( T7Main, wxFrame )

////@begin T7Main event table entries
////@end T7Main event table entries

END_EVENT_TABLE()


/*
 * T7Main constructors
 */

T7Main::T7Main()
{
    Init();
}

T7Main::T7Main( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * T7Main creator
 */

bool T7Main::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin T7Main creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end T7Main creation
    return true;
}


/*
 * T7Main destructor
 */

T7Main::~T7Main()
{
////@begin T7Main destruction
////@end T7Main destruction
}


/*
 * Member initialisation
 */

void T7Main::Init()
{
////@begin T7Main member initialisation
    scrlMain = NULL;
    szMain = NULL;
    panPedals = NULL;
    szMediaMain = NULL;
    szMediaTop = NULL;
    medMain = NULL;
    sldMediaPosition = NULL;
    szMediaInfo = NULL;
    lblMediaCurrent = NULL;
    lblMediaLength = NULL;
    btnMediaRootRefresh = NULL;
    dpkMediaRoot = NULL;
    panePedalIDs = NULL;
    szPedalMain = NULL;
    panePedalHardware = NULL;
    szPedalHardware = NULL;
    cmbHIDDevices = NULL;
    cmbPedals = NULL;
    lblPedalCode = NULL;
    btnAbout = NULL;
    cmbPedalActionLeft = NULL;
    cmbPedalActionMiddle = NULL;
    cmbPedalActionRight = NULL;
    txtMediaAutoRewind = NULL;
////@end T7Main member initialisation
}


/*
 * Control creation for T7Main
 */

void T7Main::CreateControls()
{    
////@begin T7Main content construction
    T7Main* itemFrame1 = this;

    scrlMain = new wxScrolledWindow( itemFrame1, ID_SCROLLEDWINDOW, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    scrlMain->SetScrollbars(1, 1, 0, 0);
    szMain = new wxBoxSizer(wxVERTICAL);
    scrlMain->SetSizer(szMain);

    panPedals = new wxPanel( scrlMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    szMain->Add(panPedals, 1, wxGROW|wxALL, 5);
    szMediaMain = new wxBoxSizer(wxVERTICAL);
    panPedals->SetSizer(szMediaMain);

    szMediaTop = new wxBoxSizer(wxVERTICAL);
    szMediaMain->Add(szMediaTop, 1, wxGROW|wxALL, 5);
    medMain = new wxMediaCtrl( panPedals, ID_MEDIACTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    szMediaTop->Add(medMain, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    sldMediaPosition = new wxSlider( panPedals, ID_SLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    szMediaTop->Add(sldMediaPosition, 0, wxGROW|wxALL, 5);

    szMediaInfo = new wxBoxSizer(wxHORIZONTAL);
    szMediaTop->Add(szMediaInfo, 0, wxALIGN_RIGHT|wxALL, 5);
    lblMediaCurrent = new wxStaticText( panPedals, wxID_STATIC, _("Position"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    szMediaInfo->Add(lblMediaCurrent, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblMediaLength = new wxStaticText( panPedals, wxID_STATIC, _("Total"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    szMediaInfo->Add(lblMediaLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    szMediaInfo->Add(10, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    szMediaTop->Add(itemBoxSizer13, 0, wxGROW|wxALL, 5);
    btnMediaRootRefresh = new wxButton( panPedals, ID_BUTTON1, wxGetTranslation(wxString(wxT("Rafra")) + (wxChar) 0x00EE + wxT("chir")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(btnMediaRootRefresh, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    dpkMediaRoot = new wxDirPickerCtrl( panPedals, ID_DIRPICKERCTRL1, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_USE_TEXTCTRL|wxDIRP_DIR_MUST_EXIST|wxDIRP_CHANGE_DIR );
    if (T7Main::ShowToolTips())
        dpkMediaRoot->SetToolTip(wxGetTranslation(wxString(wxT("Dossier racine des m")) + (wxChar) 0x00E9 + wxT("dias.")));
    itemBoxSizer13->Add(dpkMediaRoot, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    panePedalIDs = new wxCollapsiblePane( panPedals, ID_COLLAPSIBLEPANE6, wxGetTranslation(wxString(wxT("Configuration du p")) + (wxChar) 0x00E9 + wxT("dalier")), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
    szMediaMain->Add(panePedalIDs, 0, wxGROW|wxALL, 5);
    szPedalMain = new wxBoxSizer(wxVERTICAL);
    panePedalIDs->GetPane()->SetSizer(szPedalMain);

    panePedalHardware = new wxCollapsiblePane( panePedalIDs->GetPane(), ID_COLLAPSIBLEPANE7, wxGetTranslation(wxString(wxT("Mat")) + (wxChar) 0x00E9 + wxT("riel")), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
    if (T7Main::ShowToolTips())
        panePedalHardware->SetToolTip(wxGetTranslation(wxString(wxT("Vous ne devez pas jouer avec ces valeurs une fois param")) + (wxChar) 0x00E9 + wxT("tr") + (wxChar) 0x00E9 + wxT("es.")));
    szPedalMain->Add(panePedalHardware, 0, wxGROW|wxALL, 5);
    szPedalHardware = new wxBoxSizer(wxVERTICAL);
    panePedalHardware->GetPane()->SetSizer(szPedalHardware);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    szPedalHardware->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText21 = new wxStaticText( panePedalHardware->GetPane(), wxID_STATIC, wxGetTranslation(wxString(wxT("1. P")) + (wxChar) 0x00E9 + wxT("riph") + (wxChar) 0x00E9 + wxT("rique")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbHIDDevicesStrings;
    cmbHIDDevices = new wxComboBox( panePedalHardware->GetPane(), ID_COMBOBOX_HW, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbHIDDevicesStrings, wxCB_READONLY );
    itemBoxSizer20->Add(cmbHIDDevices, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    szPedalHardware->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText24 = new wxStaticText( panePedalHardware->GetPane(), wxID_STATIC, wxGetTranslation(wxString(wxT("2. Appuyez sur la p")) + (wxChar) 0x00E9 + wxT("dale")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbPedalsStrings;
    cmbPedals = new wxComboBox( panePedalHardware->GetPane(), ID_XCOMBOBOX6, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbPedalsStrings, wxCB_READONLY );
    if (T7Main::ShowToolTips())
        cmbPedals->SetToolTip(wxGetTranslation(wxString(wxT("Vous ne devriez pas jouer avec cette valeur une fois bien param")) + (wxChar) 0x00E9 + wxT("tr") + (wxChar) 0x00E9 + wxT("e.")));
    itemBoxSizer23->Add(cmbPedals, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    lblPedalCode = new wxStaticText( panePedalHardware->GetPane(), wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(lblPedalCode, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnAbout = new wxButton( panePedalHardware->GetPane(), ID_BUTTON, _("A propos"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(btnAbout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
    szPedalMain->Add(itemBoxSizer28, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( panePedalIDs->GetPane(), wxID_STATIC, _("Actions"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer28->Add(itemStaticText29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbPedalActionLeftStrings;
    cmbPedalActionLeft = new wxComboBox( panePedalIDs->GetPane(), ID_XCOMBOBOX9, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbPedalActionLeftStrings, wxCB_READONLY );
    if (T7Main::ShowToolTips())
        cmbPedalActionLeft->SetToolTip(wxGetTranslation(wxString(wxT("P")) + (wxChar) 0x00E9 + wxT("dale de gauche")));
    itemBoxSizer28->Add(cmbPedalActionLeft, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbPedalActionMiddleStrings;
    cmbPedalActionMiddle = new wxComboBox( panePedalIDs->GetPane(), ID_XCOMBOBOX7, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbPedalActionMiddleStrings, wxCB_READONLY );
    if (T7Main::ShowToolTips())
        cmbPedalActionMiddle->SetToolTip(wxGetTranslation(wxString(wxT("P")) + (wxChar) 0x00E9 + wxT("dale du milieu.")));
    itemBoxSizer28->Add(cmbPedalActionMiddle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString cmbPedalActionRightStrings;
    cmbPedalActionRight = new wxComboBox( panePedalIDs->GetPane(), ID_XCOMBOBOX8, wxEmptyString, wxDefaultPosition, wxDefaultSize, cmbPedalActionRightStrings, wxCB_READONLY );
    if (T7Main::ShowToolTips())
        cmbPedalActionRight->SetToolTip(wxGetTranslation(wxString(wxT("P")) + (wxChar) 0x00E9 + wxT("dale de droite.")));
    itemBoxSizer28->Add(cmbPedalActionRight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
    szPedalMain->Add(itemBoxSizer33, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText34 = new wxStaticText( panePedalIDs->GetPane(), wxID_STATIC, wxGetTranslation(wxString(wxT("Revenir en arri")) + (wxChar) 0x00E8 + wxT("re ") + (wxChar) 0x00E0 + wxT(" la fin de la lecture")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtMediaAutoRewind = new wxTextCtrl( panePedalIDs->GetPane(), ID_XTEXTCTRL7, _("1000"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
    txtMediaAutoRewind->SetMaxLength(6);
    if (T7Main::ShowToolTips())
        txtMediaAutoRewind->SetToolTip(wxGetTranslation(wxString(wxT("En millisecondes. Appuyez sur ENTR")) + (wxChar) 0x00C9 + wxT("E pour enregistrer apr") + (wxChar) 0x00E8 + wxT("s modification.")));
    itemBoxSizer33->Add(txtMediaAutoRewind, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    scrlMain->FitInside();

////@end T7Main content construction
}


/*
 * Should we show tooltips?
 */

bool T7Main::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap T7Main::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin T7Main bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end T7Main bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon T7Main::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin T7Main icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end T7Main icon retrieval
}
