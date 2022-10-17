/////////////////////////////////////////////////////////////////////////////
// Name:        t7app.h
// Purpose:     
// Author:      SET
// Modified by: 
// Created:     sam. 01 mars 2014 14:17:48 CET
// RCS-ID:      
// Copyright:   Copyright SET (nmset@yandex.com) - © 2014.
// Licence:     LGPL 2.1
/////////////////////////////////////////////////////////////////////////////

#ifndef _T7APP_H_
#define _T7APP_H_

/*!
 * Includes
 */


////@begin includes
#include "wx/image.h"
////@end includes
#include <wx/app.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * T7App class declaration
 */

class T7App: public wxApp
{    
    DECLARE_CLASS( T7App )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    T7App();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin T7App event handler declarations

////@end T7App event handler declarations

////@begin T7App member function declarations

////@end T7App member function declarations

////@begin T7App member variables
////@end T7App member variables
private:
    wxLocale m_locale;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(T7App)
////@end declare app

#endif
    // _T7APP_H_
