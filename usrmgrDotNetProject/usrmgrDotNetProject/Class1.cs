using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading;
using usrmgrDotNetProject.CollectShuttleToMx;
using Web_Service_LIMA;
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


        Variable.sNavetteCollecte NavetteCollecte = new Variable.sNavetteCollecte();
        Variable.sNavetteEvacuation NavetteEvacuation = new Variable.sNavetteEvacuation();

        /*---------------------------- Web service ----------------------------------------------*/
        // Evacuation
        static Uri baseAddress = new Uri("http://localhost:8080/InterfacesPostTapingCollectShuttleToMx0101");
        // Create the ServiceHost.
        ServiceHost hostEvacuation = new ServiceHost(typeof(EvacuationService), baseAddress);

         Web_Service_Evacuation Service_Evacuation = new Web_Service_Evacuation();

        //Client
        private InterfacesPostTapingCollectShuttleToMx01Client collectClient = new InterfacesPostTapingCollectShuttleToMx01Client();

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
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Version : 14/04/2020 11H00");

            /*----------------------------- Déclaration des variables ----------------------------------------*/

            /*------------------------ Navette Collecte -----------------------------------------------------*/
            
            CollecteEchange1(0);
            CollecteEchange2(20);
            CollecteEchange3(40);
            CollecteEchange4(60);

            /*------------------------ Navette Evacuation -----------------------------------------------------*/
            
            EvacuationEchange1(100);
            EvacuationEchange2(120);
            EvacuationEchange3(140);

            /*----------------------------- Fin Déclaration des variables ----------------------------------------*/

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Il y a : " + (Variable.vVariableBool.Count() + Variable.vVariableAna.Count() + Variable.vVariableString.Count()) + " variables dont " + Variable.vVariableBool.Count() + " booleans, " + Variable.vVariableAna.Count() + " valeurs ana, " + Variable.vVariableString.Count() + " chaines de caractere.");

            Service_Evacuation.Start(hostEvacuation);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Service Web Evacuation démarrée !");

            //////////////////
            
            //Collect
            PostTapingCollectShuttleMissionStatusRequest resquestMissionStatusCollect = new PostTapingCollectShuttleMissionStatusRequest
            {
                MissionNumber = 123,
                MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok,
            };

            //collectClient.MissionStatus(resquestMissionStatusCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Mission Status !");
        }

        public void CollecteEchange1(uint nbr)
        {
            // TapingOutputConveyorNumber
            string tempPrefixe = tempPrefixeCollecte + ".Echange1";
            NavetteCollecte.TapingOutputConveyorNumber.SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            NavetteCollecte.TapingOutputConveyorNumber.TapingOutputConveyorNumber = new TypeAna(tempPrefixe + ".TapingOutputConveyorNumber", nbr + 2, 0);

        }

        private void CollecteEchange2(uint nbr)
        {
            // LayerDestionation
            string tempPrefixe = tempPrefixeCollecte + ".Echange2";
            NavetteCollecte.LayerDestination.ReceiveCom = new TypeBool(tempPrefixe + ".ReceiveCom", nbr + 1, false);
            NavetteCollecte.LayerDestination.MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 2, 0);
            NavetteCollecte.LayerDestination.SorterInputConveyorNumber = new TypeAna(tempPrefixe + ".SorterInputConveyorNumber", nbr + 3, 0);
            NavetteCollecte.LayerDestination.TapingOutputConveyorNumber = new TypeAna(tempPrefixe + ".TapingOutputConveyor", nbr + 4, 0);
        }

        private void CollecteEchange3(uint nbr)
        {
            // MissionStatus
            string tempPrefixe = tempPrefixeCollecte + ".Echange3";
            NavetteCollecte.MissionStatus.SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            NavetteCollecte.MissionStatus.MissionStatus = new TypeBool(tempPrefixe + ".MissionStatus", nbr + 2, false);
            NavetteCollecte.MissionStatus.MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 3, 0);
        }

        private void CollecteEchange4(uint nbr)
        {
            // ReportRunningMode
            string tempPrefixe = tempPrefixeCollecte + ".Echange4";
            NavetteCollecte.ReportRunningMode.SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            NavetteCollecte.ReportRunningMode.DefaultCode = new TypeAna(tempPrefixe + ".DefaultCode", nbr + 2, 0);
            NavetteCollecte.ReportRunningMode.EquipmentCode = new TypeAna(tempPrefixe + ".EquipmentCode", nbr + 3, 0);
            NavetteCollecte.ReportRunningMode.RunningMode = new TypeAna(tempPrefixe + ".RunningMode", nbr + 4, 0);
            NavetteCollecte.ReportRunningMode.DefaultLabel = new TypeString(tempPrefixe + ".DefaultLabel", nbr + 5, "");
            NavetteCollecte.ReportRunningMode.RunningModeDate = new TypeString(tempPrefixe + ".RunningModeDate", nbr + 6, "");
            NavetteCollecte.ReportRunningMode.RunningModeTime = new TypeString(tempPrefixe + ".RunningModeTime", nbr + 7, "");
        }

        private void EvacuationEchange1(uint nbr)
        {
            // LayerDestionation
            string tempPrefixe = tempPrefixeEvacuation + ".Echange1";
            NavetteEvacuation.LayerEvacuation.ReceiveCom = new TypeBool(tempPrefixe + ".ReceiveCom", nbr + 1, false);
            NavetteEvacuation.LayerEvacuation.LayerDestination = new TypeAna(tempPrefixe + ".LayerDestination", nbr + 2, 0);
            NavetteEvacuation.LayerEvacuation.MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 2, 0);
            NavetteEvacuation.LayerEvacuation.SorterOutputConveyorNumber = new TypeAna(tempPrefixe + ".SorterOutputConveyorNumber", nbr + 2, 0);
        }

        private void EvacuationEchange2(uint nbr)
        {
            // MissionStatus
            string tempPrefixe = tempPrefixeEvacuation + ".Echange2";
            NavetteEvacuation.MissionStatus.SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            NavetteEvacuation.MissionStatus.MissionStatus = new TypeBool(tempPrefixe + ".MissionStatus", nbr + 2, false);
            NavetteEvacuation.MissionStatus.MissionNumber = new TypeAna(tempPrefixe + ".MissionNumber", nbr + 3, 0);
        }

        private void EvacuationEchange3(uint nbr)
        {
            // ReportRunningMode
            string tempPrefixe = tempPrefixeEvacuation + ".Echange3";
            NavetteEvacuation.ReportRunningMode.SendCom = new TypeBool(tempPrefixe + ".SendCom", nbr + 1, false);
            NavetteEvacuation.ReportRunningMode.DefaultCode = new TypeAna(tempPrefixe + ".DefaultCode", nbr + 2, 0);
            NavetteEvacuation.ReportRunningMode.EquipmentCode = new TypeAna(tempPrefixe + ".EquipmentCode", nbr + 3, 0);
            NavetteEvacuation.ReportRunningMode.RunningMode = new TypeAna(tempPrefixe + ".RunningMode", nbr + 4, 0);
            NavetteEvacuation.ReportRunningMode.DefaultLabel = new TypeString(tempPrefixe + ".DefaultLabel", nbr + 5, "");
            NavetteEvacuation.ReportRunningMode.RunningModeDate = new TypeString(tempPrefixe + ".RunningModeDate", nbr + 6, "");
            NavetteEvacuation.ReportRunningMode.RunningModeTime = new TypeString(tempPrefixe + ".RunningModeTime", nbr + 7, "");
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

                /*--------------------------------------------------- Action sur changement de valeur ----------------------------------------------------*/
                int indice;
                /*------------------------- Navette Collecte --------------------------------------*/
                // MissionStatus
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.SendCom);
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Indice : " + indice);
                if(Variable.vVariableBool[indice].GetVarValue())
                {

                    Variable.vVariableBool[indice].WriteVar(false);
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Navette Collecte -- MissionStatus OK");

                }
                
                // ReportRunningMode
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ReportRunningMode.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {

                    Variable.vVariableBool[indice].WriteVar(false);
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Navette Collecte -- ReportRunningMode OK");

                }

                // TapingOutputConveyorNumber
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.TapingOutputConveyorNumber.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {

                    Variable.vVariableBool[indice].WriteVar(false);
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Navette Collecte -- TapingOutputConveyorNumber OK");

                }

                /*------------------------- Navette Evacuation --------------------------------------*/
                
                // MissionStatus
                indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.MissionStatus.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {

                    Variable.vVariableBool[indice].WriteVar(false);
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Navette Evacuation -- MissionStatus OK");

                }

                // ReportRunningMode
                indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.ReportRunningMode.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {

                    Variable.vVariableBool[indice].WriteVar(false);
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Navette Evacuation -- ReportRunningMode OK");

                }

                /*--------------------------------------------------- Fin action sur changement de valeur ----------------------------------------------------*/
            }
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
            // Close the ServiceHost.
            hostEvacuation.Close();
        }

    }
}
