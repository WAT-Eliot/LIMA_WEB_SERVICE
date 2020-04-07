#include "stdafx.h"
#include "usrmgrusrmgrDLL_LimaImpl.h"
#include "svmgrDefaultImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// Implementation of the ISVMgr interface

struct IUsrMgr : public ISVMgr
{
    // TODO: Add methods you have to implement here.
    //
    //       Following implementations are given as an example:

    void __stdcall StartProject()
    {
        // The StartProject method is helpful to allocate, initialise project
        // specific purpose.

        TCHAR szProjectDirectory[MAX_PATH];
        svmgrAPI->GetProjectDirectory(svmgrProjectDirectory_Root, szProjectDirectory, _countof(szProjectDirectory));
        svmgrAPI->LogMessage(SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, _T("StartProject,%s"), szProjectDirectory);
    }

    void __stdcall StopProject()
    {
        // The StopProject method is helpful to stop some tasks (like advising
        // variables, etc.) and all tasks depending of the project.

        svmgrAPI->LogMessage(SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, _T("StopProject"));
    }
};

////////////////////////////////////////////////////////////////////////////////
// The one and only IUsrMgr object instance

IUsrMgr theIUsrMgr;

////////////////////////////////////////////////////////////////////////////////
// Interface pointer to the manager toolkit API

IAPIMgr* svmgrAPI = NULL;

////////////////////////////////////////////////////////////////////////////////
// Exchanges the User DLL and manager toolkit interface pointers

HRESULT WINAPI svmgrExchangeInterface (LPVOID* ppvInterface, IAPIMgr* pSvAPI)
{
    *ppvInterface = &theIUsrMgr;
    svmgrAPI = pSvAPI;
    return S_OK;
}
