/////////////////////////////////////////////////////////////////////////////
// Name:        t7app.cpp
// Purpose:     
// Author:      SET
// Modified by: 
// Created:     sam. 01 mars 2014 14:17:48 CET
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

#include "t7app.h"
#include "../XT7Main.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( T7App )
////@end implement app


/*
 * T7App type definition
 */

IMPLEMENT_CLASS( T7App, wxApp )


/*
 * T7App event table definition
 */

BEGIN_EVENT_TABLE( T7App, wxApp )

////@begin T7App event table entries
////@end T7App event table entries

END_EVENT_TABLE()


/*
 * Constructor for T7App
 */

T7App::T7App()
{
    Init();
}


/*
 * Member initialisation
 */

void T7App::Init()
{
////@begin T7App member initialisation
////@end T7App member initialisation
}

/*
 * Initialisation for T7App
 */

bool T7App::OnInit()
{    
////@begin T7App initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
////@end T7App initialisation

    wxSetlocale(LC_ALL, wxLocale::GetLanguageCanonicalName(wxLocale::GetSystemLanguage()) + _T(".") + wxLocale::GetSystemEncodingName());
    SetAppName(_APPNAME_T7_);
    SetExitOnFrameDelete(true);
    XT7Main * main = new XT7Main( NULL);
    SetTopWindow(main);
    main->Show();
    
    return true;
}


/*
 * Cleanup for T7App
 */

int T7App::OnExit()
{    
////@begin T7App cleanup
	return wxApp::OnExit();
////@end T7App cleanup
}
