////////////////////////////////////////////////////////////////////////////////
// Defines the main application class for the DLL.

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"   // Main symbols

////////////////////////////////////////////////////////////////////////////////
// CUsrMgrApp

class CUsrMgrApp : public CWinApp
{
public:
    CUsrMgrApp();

// Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
