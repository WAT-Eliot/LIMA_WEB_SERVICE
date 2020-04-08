#include "stdafx.h"
#include "usrmgrDLL_LimaImpl.h"
#include "svmgrDefaultImpl.h"
#include <map>
#include <vector>
#include <memory>
#include <boost\thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sink.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <fstream>
#include <iostream>

#include "my_structs.h"
#include "my_logger.h"
#include "Equipement.h"
#include "Sequence.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//-----------------Déclarations variables globales
extern std::vector<TypeBool*> vVariablesBool;
extern std::vector<TypeBool*> vVariablesAl;
extern std::vector<TypeAna*> vVariablesAna;
extern std::vector<TypeString*> vVariablesString;
extern std::vector < std::tuple<int, TypeAna*, int> > vVariablesAlarmes;

////////////////////////////////////////////////////////////////////////////////
// Implementation of the ISVMgr interface


struct IUsrMgr : public ISVMgr
{
    // TODO: Add methods you have to implement here.
    //
    //       Following implementations are given as an example:

    void __stdcall StartProject()
    {
		LOG_INFO << "Project is starting.";
		LOG_INFO << "Version du 07/04/2020 14H00";

        // The StartProject method is helpful to allocate, initialise project
        // specific purpose.

        char szProjectDirectory[_MAX_PATH];
        svmgrAPI->GetProjectDirectory(svmgrProjectDirectory_Root, szProjectDirectory, sizeof(szProjectDirectory)-1);
        svmgrAPI->LogMessage(SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, "StartProject,%s", szProjectDirectory);

		//Création des équipements
		sVarEqp.APInavetteCollecte =	new NavetteCollecte(sVarEqpCollecte, 100);
		sVarEqp.APInavetteEvacuation =	new NavetteEvacuation(sVarEqpEvac, 200);

		LOG_INFO << "Il y a : " << vVariablesBool.size() << " bools, "
			<< vVariablesAna.size() << " valeurs ana, "
			<< vVariablesString.size() << " chaines de caractere,  "
			<< vVariablesAl.size() << " alarmes, "
			<< vVariablesAlarmes.size() << " vVariablesAlarmes. ";

		// Lancer le G7 général
		sVarEqp.g7 = new Sequence();
		sVarEqp.g7->Start();

    }

    void __stdcall StopProject()
    {
        // The StopProject method is helpful to stop some tasks (like advising
        // variables, etc.) and all tasks depending of the project.
		sVarEqp.g7->Stop();

        svmgrAPI->LogMessage(SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, _T("StopProject"));
    }

	void __stdcall OnDataChange(char *Buffer)
	{
		_svmgrStreamHeader *pHeader = (_svmgrStreamHeader *)Buffer;
		_svmgrStreamVarHeader *pVarHeader = (_svmgrStreamVarHeader *)(Buffer + sizeof(_svmgrStreamHeader));
		_svmgrVarValue *pValue = (_svmgrVarValue *)(Buffer + sizeof(_svmgrStreamHeader) + sizeof(_svmgrStreamVarHeader));

		if (pHeader->dwVarCount > 1) {
			LOG_INFO << "Lancement OnDataChange (" << pHeader->dwVarCount << " vars)";
		}


		for (DWORD dwCmpt = 0; dwCmpt < pHeader->dwVarCount; dwCmpt++)
		{
			unsigned int i = 0;
			try {
				switch (pValue->vt)
				{

				case svmgr_vtLOG:
					for (i = 0; i < vVariablesBool.size(); i++) {
						// found nth element..print and break.
						if (vVariablesBool.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							LOG_INFO << vVariablesBool.at(i)->GetVarName() << " = " << pValue->bLog() << "";
							vVariablesBool.at(i)->SetVar(pValue->bLog());

							break;
						}
					}
					break;
				case svmgr_vtALARM:

					//Copie des alarmes
					for (i = 0; i < vVariablesAl.size(); i++) {
						// found nth element..print and break.
						if (vVariablesAl.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							LOG_INFO << vVariablesAl.at(i)->GetVarName() << " = " << pValue->bAlarmLog() << "";
							vVariablesAl.at(i)->SetVar(pValue->bAlarmLog());
							break;
						}
					}

					//Copie des alarmes déclarées dan le vecteur bool
					for (i = 0; i < vVariablesBool.size(); i++) {
						// found nth element..print and break.
						if (vVariablesBool.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							LOG_INFO << vVariablesBool.at(i)->GetVarName() << " = " << pValue->bAlarmLog() << "";
							vVariablesBool.at(i)->SetVar(pValue->bAlarmLog());
							break;
						}
					}

					break;
				case svmgr_vtANA:
					for (i = 0; i < vVariablesAna.size(); i++) {
						// found nth element..print and break.
						if (vVariablesAna.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							LOG_INFO << vVariablesAna.at(i)->GetVarName() << " = " << pValue->dAna() << "";
							vVariablesAna.at(i)->SetVar(pValue->dAna());

							break;
						}
					}
					break;
				case svmgr_vtTXT:
					//LOG_DEBUG << "N° " << pVarHeader->ulClientHandle << " - " << pValue->szTxt() << "";
					for (i = 0; i < vVariablesString.size(); i++) {
						// found nth element..print and break.
						if (vVariablesString.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							LOG_INFO << vVariablesString.at(i)->GetVarName() << " = " << pValue->szTxt() << "";
							vVariablesString.at(i)->SetVar(pValue->szTxt());
							break;
						}
					}
					break;
				}
			}
			catch (const std::out_of_range&) {
				LOG_ERROR << "Vector out of range pour var n°" << pValue->vt << "";
			}
			break;
		}
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
