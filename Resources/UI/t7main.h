/////////////////////////////////////////////////////////////////////////////
// Name:        t7main.h
// Purpose:     
// Author:      Saleem Edah-Tally
// Modified by: 
// Created:     sam. 01 mars 2014 14:23:16 CET
// RCS-ID:      
// Copyright:   Copyright Saleem Edah-Tally (nmset@yandex.com) - © 2014.
// Licence:     LGPL 2.1
/////////////////////////////////////////////////////////////////////////////

#ifndef _T7MAIN_H_
#define _T7MAIN_H_

#define _APPNAME_T7_ wxString(_T("T7"))
#define _APPVERSION_T7_ wxString(_T("7"))

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/mediactrl.h"
#include "wx/filepicker.h"
#include "wx/collpane.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxMediaCtrl;
class wxDirPickerCtrl;
class wxCollapsiblePane;
////@end forward declarations
class wxPanel;
class wxStaticText;
class wxComboBox;
class wxSlider;
/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_T7MAIN 10000
#define ID_SCROLLEDWINDOW 10001
#define ID_PANEL1 10049
#define ID_MEDIACTRL 10068
#define ID_SLIDER 10069
#define ID_BUTTON1 10079
#define ID_DIRPICKERCTRL1 10078
#define ID_COLLAPSIBLEPANE6 10070
#define ID_COLLAPSIBLEPANE7 10077
#define ID_COMBOBOX_HW 10003
#define ID_XCOMBOBOX6 10072
#define ID_BUTTON 10002
#define ID_XCOMBOBOX9 10075
#define ID_XCOMBOBOX7 10073
#define ID_XCOMBOBOX8 10074
#define ID_XTEXTCTRL7 10071
#define SYMBOL_T7MAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_T7MAIN_TITLE _("T7")
#define SYMBOL_T7MAIN_IDNAME ID_T7MAIN
#define SYMBOL_T7MAIN_SIZE wxSize(500, 400)
#define SYMBOL_T7MAIN_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * T7Main class declaration
 */

class T7Main: public wxFrame
{    
    DECLARE_CLASS( T7Main )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    T7Main();
    T7Main( wxWindow* parent, wxWindowID id = SYMBOL_T7MAIN_IDNAME, const wxString& caption = SYMBOL_T7MAIN_TITLE, const wxPoint& pos = SYMBOL_T7MAIN_POSITION, const wxSize& size = SYMBOL_T7MAIN_SIZE, long style = SYMBOL_T7MAIN_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_T7MAIN_IDNAME, const wxString& caption = SYMBOL_T7MAIN_TITLE, const wxPoint& pos = SYMBOL_T7MAIN_POSITION, const wxSize& size = SYMBOL_T7MAIN_SIZE, long style = SYMBOL_T7MAIN_STYLE );

    /// Destructor
    ~T7Main();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin T7Main event handler declarations

////@end T7Main event handler declarations

////@begin T7Main member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end T7Main member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin T7Main member variables
    wxScrolledWindow* scrlMain;
    wxBoxSizer* szMain;
    wxPanel* panPedals;
    wxBoxSizer* szMediaMain;
    wxBoxSizer* szMediaTop;
    wxMediaCtrl* medMain;
    wxSlider* sldMediaPosition;
    wxBoxSizer* szMediaInfo;
    wxStaticText* lblMediaCurrent;
    wxStaticText* lblMediaLength;
    wxButton* btnMediaRootRefresh;
    wxDirPickerCtrl* dpkMediaRoot;
    wxCollapsiblePane* panePedalIDs;
    wxBoxSizer* szPedalMain;
    wxCollapsiblePane* panePedalHardware;
    wxBoxSizer* szPedalHardware;
    wxComboBox* cmbHIDDevices;
    wxComboBox* cmbPedals;
    wxStaticText* lblPedalCode;
    wxButton* btnAbout;
    wxComboBox* cmbPedalActionLeft;
    wxComboBox* cmbPedalActionMiddle;
    wxComboBox* cmbPedalActionRight;
    wxTextCtrl* txtMediaAutoRewind;
////@end T7Main member variables
};

#endif
    // _T7MAIN_H_
