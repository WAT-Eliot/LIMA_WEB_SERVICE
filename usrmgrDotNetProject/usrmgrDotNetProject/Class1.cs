using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
/*
using usrmgrDotNetProject.CollectShuttleToMx;
using usrmgrDotNetProject.EvacuationShuttleToMx;
*/

namespace usrmgrDotNetProject
{
    public class Main:IDisposable 
    {
        /*
        InterfacesPostTapingCollectShuttleToMx01Client collectClient = new InterfacesPostTapingCollectShuttleToMx01Client();
        InterfacesPostTapingEvacuationShuttleToMx01Client evacuationClient = new InterfacesPostTapingEvacuationShuttleToMx01Client();

        PostTapingCollectShuttleMissionStatusRequest resquestMissionStatusCollect = new PostTapingCollectShuttleMissionStatusRequest();
        PostTapingCollectShuttleReportRunningModeRequest resquestReportRunningModeCollect = new PostTapingCollectShuttleReportRunningModeRequest();
        PostTapingCollectShuttleTapingOutputConveyorRequest requestConveyorNumberCollect = new PostTapingCollectShuttleTapingOutputConveyorRequest();
        PostTapingEvacuationShuttleMissionStatusRequest resquestMissionStatusEvacuation = new PostTapingEvacuationShuttleMissionStatusRequest();
        PostTapingEvacuationShuttleReportRunningModeRequest resquestReportRunningModeEvacuation = new PostTapingEvacuationShuttleReportRunningModeRequest();
        */
        private static string tempPrefixeMx = "Mx";
        private string tempPrefixeCollecte = tempPrefixeMx + ".NavetteCollecte";
        private string tempPrefixeEvacuation = tempPrefixeMx + ".NavetteEvacuation";


        public Main()
        {
            SvMgrAPI.StartProject += SvMgrAPI_StartProject;
            //SvMgrAPI.StartProject += SvMgrAPI_Boucle;
            SvMgrAPI.OnDataChange2 += SvMgrAPI_OnDataChange2;
            //Init_Client_Lima();
        }



        private void SvMgrAPI_StartProject()
        {
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "DLL démarrée");
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Version : 09/04/2020 13H00");

            /*----------------------------- Déclaration des variables ----------------------------------------*/

            /*------------------------ Navette Collecte -----------------------------------------------------*/

            CollecteEchange1(0);
            CollecteEchange2(20);
            CollecteEchange3(40);
            CollecteEchange4(60);

            /*------------------------ Navette Evacuation -----------------------------------------------------*/

            EvacuationEchange1(100);
            EvacuationEchange2(120);
            EvacuationEchange3(1400);

            /*----------------------------- Fin Déclaration des variables ----------------------------------------*/

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Il y a : " + Variable.vVariableBool.Count() + " bools, " + Variable.vVariableAna.Count() + " valeurs ana, " + Variable.vVariableString.Count() + " chaines de caractere.");
        }

        private void CollecteEchange1(uint nbr)
        {
            string tempPrefixe = tempPrefixeCollecte + ".Echange1";

            TypeBool SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            TypeAna TapingOutputConveyorNumber = new TypeAna(tempPrefixe + ".TapingOutputConveyorNumber", nbr + 2, 0);
        }

        private void CollecteEchange2(uint nbr)
        {
            string tempPrefixe = tempPrefixeCollecte + ".Echange2";
            TypeBool ReceiveCom = new TypeBool(tempPrefixe + ".ReceiveCom", nbr + 1, false);
            TypeAna MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 2, 0);
            TypeAna SorterInputConveyorNumber = new TypeAna(tempPrefixe + ".SorterInputConveyorNumber", nbr + 3, 0);
            TypeAna TapingOutputConveyor = new TypeAna(tempPrefixe + ".TapingOutputConveyor", nbr + 4, 0);
        }

        private void CollecteEchange3(uint nbr)
        {
            string tempPrefixe = tempPrefixeCollecte + ".Echange3";
            TypeBool SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            TypeBool MissionStatus = new TypeBool(tempPrefixe + ".MissionStatus", nbr + 2, false);
            TypeAna MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 3, 0);
        }

        private void CollecteEchange4(uint nbr)
        {
            string tempPrefixe = tempPrefixeCollecte + ".Echange4";
            TypeBool SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            TypeAna DefaultCode = new TypeAna(tempPrefixe + ".DefaultCode", nbr + 2, 0);
            TypeAna EquipmentCode = new TypeAna(tempPrefixe + ".EquipmentCode", nbr + 3, 0);
            TypeAna RunningMode = new TypeAna(tempPrefixe + ".RunningMode", nbr + 4, 0);
            TypeString DefaultLabel = new TypeString(tempPrefixe + ".DefaultLabel", nbr + 5, "");
            TypeString RunningModeDate = new TypeString(tempPrefixe + ".RunningModeDate", nbr + 6, "");
            TypeString RunningModeTime = new TypeString(tempPrefixe + ".RunningModeTime", nbr + 7, "");
        }

        private void EvacuationEchange1(uint nbr)
        {
            string tempPrefixe = tempPrefixeEvacuation + ".Echange1";

            TypeBool ReceiveCom = new TypeBool(tempPrefixe + ".ReceiveCom", nbr + 1, false);
            TypeAna LayerDestination = new TypeAna(tempPrefixe + ".LayerDestination", nbr + 2, 0);
            TypeAna MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 2, 0);
            TypeAna SorterOutputConveyorNumber = new TypeAna(tempPrefixe + ".SorterOutputConveyorNumber", nbr + 2, 0);
        }

        private void EvacuationEchange2(uint nbr)
        {
            string tempPrefixe = tempPrefixeEvacuation + ".Echange2";
            TypeBool SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            TypeBool MissionStatus = new TypeBool(tempPrefixe + ".MissionStatus", nbr + 2, false);
            TypeAna MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 3, 0);
        }

        private void EvacuationEchange3(uint nbr)
        {
            string tempPrefixe = tempPrefixeEvacuation + ".Echange3";
            TypeBool SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            TypeAna DefaultCode = new TypeAna(tempPrefixe + ".DefaultCode", nbr + 2, 0);
            TypeAna EquipmentCode = new TypeAna(tempPrefixe + ".EquipmentCode", nbr + 3, 0);
            TypeAna RunningMode = new TypeAna(tempPrefixe + ".RunningMode", nbr + 4, 0);
            TypeString DefaultLabel = new TypeString(tempPrefixe + ".DefaultLabel", nbr + 5, "");
            TypeString RunningModeDate = new TypeString(tempPrefixe + ".RunningModeDate", nbr + 6, "");
            TypeString RunningModeTime = new TypeString(tempPrefixe + ".RunningModeTime", nbr + 7, "");
        }

        private void SvMgrAPI_OnDataChange2(SvMgrObjects.VariableResult[] ArrayVarResult)
        {
            
            for (int i = 0; i < ArrayVarResult.Length; i++) {
                switch (ArrayVarResult[i].varValue.vt)
                {
                    case SvMgrEnums.VarType.svmgr_vtLOG:
                        foreach (TypeBool typeBool in Variable.vVariableBool)
                        {
                            if ((typeBool.GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, typeBool.GetVarName() + " = " + ArrayVarResult[i].varValue.Log);
                                typeBool.SetVar(ArrayVarResult[i].varValue.Log);
                            }
                        }
                        break;
                    case SvMgrEnums.VarType.svmgr_vtANA:
                        foreach (TypeAna typeAna in Variable.vVariableAna)
                        {
                            if ((typeAna.GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, typeAna.GetVarName() + " = " + ArrayVarResult[i].varValue.Ana);
                                typeAna.SetVar(ArrayVarResult[i].varValue.Ana);
                            }
                        }
                        break;
                    case SvMgrEnums.VarType.svmgr_vtTXT:
                        foreach (TypeString typeString in Variable.vVariableString)
                        {
                            if ((typeString.GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, typeString.GetVarName() + " = " + ArrayVarResult[i].varValue.Txt);
                                typeString.SetVar(ArrayVarResult[i].varValue.Txt);
                            }
                        }
                        break;
                }
            }

            /*--------------------------------------------------- Action sur changement de valeur ----------------------------------------------------*/
            
            if (Variable.vVariableBool[0].GetVarValue())
            {

                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "MissionStatus OK");

            }
            
            /*--------------------------------------------------- Fin action sur changement de valeur ----------------------------------------------------*/
        }

        private void SvMgrAPI_Boucle()
        {
            for (;;)
            {
                try
                {
                    if (Variable.vVariableBool[0].GetVarValue())
                    {
                        
                    }


                    Thread.Sleep(1000);

                }
                catch (Exception e)
                {
                    return;
                }
            }
        }
        /*
        private void Init_Client_Lima()
        {
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Init Client Lima");
            //Collect
            resquestMissionStatusCollect.MissionNumber = 0;
            resquestMissionStatusCollect.MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok;

            resquestReportRunningModeCollect.DefaultCode = 0;
            resquestReportRunningModeCollect.DefaultLabel = "?";
            resquestReportRunningModeCollect.EquipmentCode = 0;
            resquestReportRunningModeCollect.RunningMode = PostTapingCollectShuttleRunningModeType.Manual;
            resquestReportRunningModeCollect.RunningModeDateTime = DateTime.Now;
            
            requestConveyorNumberCollect.TapingOutputConveyorNumber = 0;

            //Evacuation
            resquestMissionStatusEvacuation.MissionNumber = 0;
            resquestMissionStatusEvacuation.MissionStatus = PostTapingEvacuationShuttleMissionStatusType.Ok;

            resquestReportRunningModeEvacuation.DefaultCode = 0;
            resquestReportRunningModeEvacuation.DefaultLabel = "?";
            resquestReportRunningModeEvacuation.EquipmentCode = 0;
            resquestReportRunningModeEvacuation.RunningMode = PostTapingEvacuationShuttleRunningModeType.Manual;
            resquestReportRunningModeEvacuation.RunningModeDateTime = DateTime.Now;
        }

        private void Client_Lima()
        {
            

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collect");
            collectClient.MissionStatus(resquestMissionStatusCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "MissionStatus OK");

            collectClient.ReportRunningMode(resquestReportRunningModeCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "ReportRunningMode OK");

            collectClient.TapingOutputConveyorNumber(requestConveyorNumberCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "ConveyorNumber OK");

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Evacuation");
            evacuationClient.MissionStatus(resquestMissionStatusEvacuation);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "MissionStatus OK");

            evacuationClient.ReportRunningMode(resquestReportRunningModeEvacuation);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "ReportRunningMode OK");
        }
        */
        public void Dispose()
        {
            
        }

    }
}
