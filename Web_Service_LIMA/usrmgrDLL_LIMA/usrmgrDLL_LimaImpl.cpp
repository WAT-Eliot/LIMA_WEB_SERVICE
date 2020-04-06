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
#include "Setup.h"

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
    // Following implementations are given as an example:

    void __stdcall StartProject ()   {
		LOG_INFO << "Project is starting.";
		LOG_INFO << "Version du 27/03/2020 12H00";

        // The StartProject method is helpful to allocate, initialise project
        // specific purpose.

        char szProjectDirectory [_MAX_PATH];
        svmgrAPI->GetProjectDirectory (svmgrProjectDirectory_Root, szProjectDirectory, sizeof(szProjectDirectory)-1);
        svmgrAPI->LogMessage (SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, "StartProject,%s", szProjectDirectory);

		

		//Création des équipements
		
		sVarEqp.APIrobotCaisses =		new RobotCaisses(sVarEqpRcb, 100);
		sVarEqp.APIrobotCouvercle =		new RobotCouvercle(sVarEqpRc, 160);
		sVarEqp.APIensacheuseL1 =		new Ensacheuse(sVarEnsL1, 200);
		sVarEqp.APIensacheuseL2 =		new Ensacheuse(sVarEnsL2, 250);
		sVarEqp.APIpeseuseL1 =			new Peseuse(sVarPesL1, 300);
		sVarEqp.APIpeseuseL2 =			new Peseuse(sVarPesL2, 350);
		sVarEqp.APItransitique =		new Transitique(sVarTrans, 400);
		sVarEqp.APIbalance =			new Balance(sVarBal, 470);
		sVarEqp.APIpalettiseur =		new RobotPalettisation(sVarPal, 500);
		sVarEqp.APIetiqueteuse =		new Etiqueteuse(sVarEtq, 560);
		sVarEqp.APIimprimante =			new Imprimante(sVarImp, 600);
		sVarEqp.APIfermeuse =			new Fermeuse(sVarFerm, 650);
		sVarEqp.APIformeuse1 =			new Formeuse(sVarForm1, 700);
		sVarEqp.APIformeuse2 =			new Formeuse(sVarForm2, 750);
		sVarEqp.mesRayPro =				new MES(sVarMES, 800);
		sVarEqp.spvWAT =				new SPV(sVarSPV, 1000);
		sVarEqp.Tremie =				new Tremie(sVarEqpTremie, 1250);
		

		LOG_INFO << "Il y a : " << vVariablesBool.size() << " bools, " 
			<< vVariablesAna.size() << " valeurs ana, "
			<< vVariablesString.size() << " chaines de caractere,  "
			<< vVariablesAl.size() << " alarmes, "
			<< vVariablesAlarmes.size() << " vVariablesAlarmes. ";

		// Lancer le G7 général
		sVarEqp.g7 = new Sequence();
		sVarEqp.g7->Start();
		

		//Import des HU
		sVarEqp.g7->ImportFichierHuInput(1);
		sVarEqp.g7->ImportFichierHuInput(2);
    }

    void __stdcall StopProject ()
    {
        // The StopProject method is helpful to stop some tasks (like advising
        // variables, etc.) and all tasks depending of the project.

		sVarEqp.g7->Stop();

        svmgrAPI->LogMessage (SVMGR_LVL_INFO, SVMGR_DEST_VIEWER, "StopProject");

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

							// _OLD_FORMEUSE
							/*//Test de validation de changement de formeuse
							if (pValue->bLog() && pVarHeader->ulClientHandle == sVarEqp.spvWAT->GetValidAttrFormAdvNum(1)) {
								sVarEqp.spvWAT->ChangeAffFormeuse(1);
								LOG_DEBUG << "sVarEqp.spvWAT->ChangeAffFormeuse(1)";
							}
							if (pValue->bLog() && pVarHeader->ulClientHandle == sVarEqp.spvWAT->GetValidAttrFormAdvNum(2)) {
								sVarEqp.spvWAT->ChangeAffFormeuse(2);
								LOG_DEBUG << "sVarEqp.spvWAT->ChangeAffFormeuse(2)";
							}*/
							if (pValue->bLog() && pVarHeader->ulClientHandle == sVarEqp.spvWAT->GetValidAttrFormAdvNum(1)) {
								sVarEqp.spvWAT->ChangeAffFormeuse(1);
								LOG_INFO << "sVarEqp.spvWAT->ChangeAffFormeuse(L1)";
							}
							if (pValue->bLog() && pVarHeader->ulClientHandle == sVarEqp.spvWAT->GetValidAttrFormAdvNum(2)) {
								sVarEqp.spvWAT->ChangeAffFormeuse(2);
								LOG_INFO << "sVarEqp.spvWAT->ChangeAffFormeuse(L2)";
							}

							//Si top bac, lancer fonction stockage HU Input
							//Test d'un nouveau bac pour HU Input
							for (unsigned int j = 1; j <=2; j++) {
								if (pValue->bLog() == 1 && pVarHeader->ulClientHandle == sVarEqp.APIrobotCaisses->GetAdvNtopBac(j)) {
									LOG_INFO << "Nouveau bac détecté";
									//Ajout dans vecteur
									sVarEqp.g7->AddHUTable(
										j,
										sVarEqp.APIrobotCaisses->GetHUcontID(j),
										sVarEqp.APIrobotCaisses->GetbatchID(j),
										sVarEqp.APIrobotCaisses->GetHUID(j),
										sVarEqp.APIrobotCaisses->GetHUqty(j)
									);

									//Ajouter le bac dans vecteur de calcule de moyenne
									sVarEqp.spvWAT->CalculMoyenneBac(j);
									
									/*//RAZ top bac versé
									sVarEqp.APIrobotCaisses->ResetTopBacVerse(j);*/

									//ACK top bac versé
									sVarEqp.APIrobotCaisses->SetAckBacVerse(j);
								}
							}

							//Si HU en humidification, ajouter un HU à la liste
							if (pValue->bLog() == 1 && pVarHeader->ulClientHandle == sVarEqp.spvWAT->GetValidHUHumidAdvNum()) {
								LOG_INFO << "HU Humidification détecté";
								//Ajout dans vecteur
								sVarEqp.g7->AddHUTable(
									1,
									sVarEqp.spvWAT->GetHUcontID(),
									sVarEqp.spvWAT->GetbatchID(),
									sVarEqp.spvWAT->GetHUID(),
									sVarEqp.spvWAT->GetHUqty()
								);


								//ACK top bac versé
								sVarEqp.spvWAT->ResetValidHUHumid();
							}

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

					//Lancement remontée alarme
					for (i = 0; i < vVariablesAlarmes.size(); i++) {
						if (std::get<0>(vVariablesAlarmes.at(i)) == pVarHeader->ulClientHandle) {
							sVarEqp.g7->StartAlarme(std::get<2>(vVariablesAlarmes.at(i)), std::get<1>(vVariablesAlarmes.at(i))->GetVarValue(), pValue->bAlarmLog());
							break;
						}
					}
					break;
				case svmgr_vtANA:
					for (i = 0; i < vVariablesAna.size(); i++) {
						// found nth element..print and break.
						if (vVariablesAna.at(i)->GetAdviseNumber() == pVarHeader->ulClientHandle) {
							//On affiche pas les mots de vie dans le log
							if (pVarHeader->ulClientHandle != 800 && pVarHeader->ulClientHandle != 801 
								&& pVarHeader->ulClientHandle != 139 && pVarHeader->ulClientHandle != 140
								&& pVarHeader->ulClientHandle != 187 && pVarHeader->ulClientHandle != 188
								&& pVarHeader->ulClientHandle != 221 && pVarHeader->ulClientHandle != 222
								&& pVarHeader->ulClientHandle != 271 && pVarHeader->ulClientHandle != 272
								&& pVarHeader->ulClientHandle != 318 && pVarHeader->ulClientHandle != 319
								&& pVarHeader->ulClientHandle != 368 && pVarHeader->ulClientHandle != 369
								&& pVarHeader->ulClientHandle != 433 && pVarHeader->ulClientHandle != 434
								&& pVarHeader->ulClientHandle != 539 && pVarHeader->ulClientHandle != 540
								&& pVarHeader->ulClientHandle != 565 && pVarHeader->ulClientHandle != 566
								&& pVarHeader->ulClientHandle != 615 && pVarHeader->ulClientHandle != 616
								&& pVarHeader->ulClientHandle != 661 && pVarHeader->ulClientHandle != 662
								&& pVarHeader->ulClientHandle != 717 && pVarHeader->ulClientHandle != 718
								&& pVarHeader->ulClientHandle != 866 && pVarHeader->ulClientHandle != 867
								&& pVarHeader->ulClientHandle != 868 && pVarHeader->ulClientHandle != 869
								&& pVarHeader->ulClientHandle != 896 && pVarHeader->ulClientHandle != 897
								&& pVarHeader->ulClientHandle != 767 && pVarHeader->ulClientHandle != 768){
								LOG_INFO << vVariablesAna.at(i)->GetVarName() << " = " << pValue->dAna() << "";
							}							
							vVariablesAna.at(i)->SetVar(pValue->dAna());

							//Envoi de données de suivi à Raypro :
							//NB pieces sorties
							if (pVarHeader->ulClientHandle == sVarEqp.APIpalettiseur->GetAdvNPP(1)) {
								LOG_INFO << "Changement goodPartQty L1 : " << pValue->dAna() << "";
								sVarEqp.mesRayPro->SetGoodPartQty(1, pValue->dAna());
							}
							if (pVarHeader->ulClientHandle == sVarEqp.APIpalettiseur->GetAdvNPP(2)) {
								LOG_INFO << "Changement goodPartQty L2 : " << pValue->dAna() << "";
								sVarEqp.mesRayPro->SetGoodPartQty(2, pValue->dAna());
							}
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

IAPIMgr * svmgrAPI = NULL;

////////////////////////////////////////////////////////////////////////////////
// Exchanges the User DLL and manager toolkit interface pointers

HRESULT WINAPI svmgrExchangeInterface (LPVOID * ppvInterface, IAPIMgr * pSvAPI)
{
    *ppvInterface = &theIUsrMgr;
    svmgrAPI = pSvAPI;
    return S_OK;
}
