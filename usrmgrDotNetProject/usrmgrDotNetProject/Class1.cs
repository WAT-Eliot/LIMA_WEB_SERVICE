using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading;
//using CollectShuttleToMx;
using usrmgrDotNetProject.CollectShuttleToMx;
using Web_Service_LIMA;
/*
using usrmgrDotNetProject.CollectShuttleToMx;
using usrmgrDotNetProject.EvacuationShuttleToMx;
*/

namespace usrmgrDotNetProject
{
    public class Main : IDisposable
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
        public static string tempPrefixeMx = "Mx";
        public static string tempPrefixeCollecte = tempPrefixeMx + ".NavetteCollecte";
        public static string tempPrefixeEvacuation = tempPrefixeMx + ".NavetteEvacuation";
        public static string PrefixeCollecteEchange2 = tempPrefixeCollecte + ".Echange2";

        public const uint CollectEchange1 = 0;
        public const uint CollectEchange2 = 20;
        public const uint CollectEchange3 = 40;
        public const uint CollectEchange4 = 60;

        public static Variable.sNavetteCollecte NavetteCollecte = new Variable.sNavetteCollecte();
        public static Variable.sNavetteEvacuation NavetteEvacuation = new Variable.sNavetteEvacuation();

        /*---------------------------- Web service ----------------------------------------------*/
        //Collect
        //static Uri baseAddressCollect = new Uri("http://localhost:8080/InterfacesMxToPostTapingCollectShuttle01");
        static Uri baseAddressCollect = new Uri("http://2.7.106.131:8080/InterfacesMxToPostTapingCollectShuttle01");
        // Create the ServiceHost.
        ServiceHost hostCollect = new ServiceHost(typeof(CollectService), baseAddressCollect);

        Web_Service Service_Collect = new Web_Service();
        
        // Evacuation
        //static Uri baseAddressEvacuation = new Uri("http://localhost:8080/InterfacesMxToPostTapingEvacuationShuttle01");
        static Uri baseAddressEvacuation = new Uri("http://2.7.106.131:8080/InterfacesMxToPostTapingEvacuationShuttle01");
        // Create the ServiceHost.
        ServiceHost hostEvacuation = new ServiceHost(typeof(EvacuationService), baseAddressEvacuation);

        Web_Service Service_Evacuation = new Web_Service();


        /*------------------------------------------------ Client -----------------------------------*/
        //Collecte
        internal static InterfacesPostTapingCollectShuttleToMx01Client collectClient;

        
        PostTapingCollectShuttleMissionStatusRequest resquestMissionStatusCollect = new PostTapingCollectShuttleMissionStatusRequest
        {
            MissionNumber = 123,
            MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok,
        };

        PostTapingCollectShuttleReportRunningModeRequest resquestReportRunningModeCollect = new PostTapingCollectShuttleReportRunningModeRequest
        {
            DefaultCode = 0,
            DefaultLabel = "?",
            EquipmentCode = 0,
            RunningMode = PostTapingCollectShuttleRunningModeType.Manual,
            RunningModeDateTime = DateTime.Now,
        };

        PostTapingCollectShuttleTapingOutputConveyorRequest requestConveyorNumberCollect = new PostTapingCollectShuttleTapingOutputConveyorRequest
        {
            TapingOutputConveyorNumber = 0,
        };

        /*----------------------------------------------- Fin Client ------------------------------------*/

        public Main()
        {
            SvMgrAPI.StartProject += SvMgrAPI_StartProject;
            SvMgrAPI.OnDataChange2 += SvMgrAPI_OnDataChange2;
        }

        private void SvMgrAPI_StartProject()
        {
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "DLL démarrée");
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Version : 14/04/2020 18H00");

            /*----------------------------- Déclaration des variables ----------------------------------------*/

            /*------------------------ Navette Collecte -----------------------------------------------------*/
            
            CollecteEchange1(CollectEchange1);
            CollecteEchange2(CollectEchange2);
            CollecteEchange3(CollectEchange3);
            CollecteEchange4(CollectEchange4);

            /*------------------------ Navette Evacuation -----------------------------------------------------*/
            
            EvacuationEchange1(100);
            EvacuationEchange2(120);
            EvacuationEchange3(140);

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Il y a : " + (Variable.vVariableBool.Count() + Variable.vVariableAna.Count() + Variable.vVariableString.Count()) + " variables dont " + Variable.vVariableBool.Count() + " booleans, " + Variable.vVariableAna.Count() + " valeurs ana, " + Variable.vVariableString.Count() + " chaines de caractere.");

            /*----------------------------- Fin Déclaration des variables ----------------------------------------*/


            Service_Collect.Start(hostCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Service Web Collect démarrée !");


            Service_Evacuation.Start(hostEvacuation);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Service Web Evacuation démarrée !");

            //////////////////
            try
            { // Class constructeur
                System.ServiceModel.Channels.Binding Test = new System.ServiceModel.BasicHttpBinding();
                EndpointAddress address = new EndpointAddress("http://wstest.groupeliebot.fr/Mx/Lima1/Mx.Broker.Lima.Endpoint/Interface/InterfacesPostTapingCollectShuttleToMx0101.svc");
                collectClient = new InterfacesPostTapingCollectShuttleToMx01Client(Test,address);
            }

            catch (Exception e)
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, e.Message);
            }

            collectClient.MissionStatus(resquestMissionStatusCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Mission Status !");
        }

        //Class diferente
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

            NavetteCollecte.LayerDestination.ReceiveCom = new TypeBool(PrefixeCollecteEchange2 + ".ReceiveCom", nbr + 1, false);
            NavetteCollecte.LayerDestination.MissionNumber = new TypeAna(PrefixeCollecteEchange2 + ".MissionNumber", nbr + 2, 0);
            NavetteCollecte.LayerDestination.SorterInputConveyorNumber = new TypeAna(PrefixeCollecteEchange2 + ".SorterInputConveyorNumber", nbr + 3, 0);
            NavetteCollecte.LayerDestination.TapingOutputConveyorNumber = new TypeAna(PrefixeCollecteEchange2 + ".TapingOutputConveyor", nbr + 4, 0);
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
                //Class
                // MissionStatus
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.SendCom);
                if(Variable.vVariableBool[indice].GetVarValue())
                {
                    indice = Variable.vVariableAna.IndexOf(NavetteCollecte.MissionStatus.MissionNumber);
                    resquestMissionStatusCollect.MissionNumber = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.SendCom);
                    if (Variable.vVariableBool[indice].GetVarValue())
                    {
                        resquestMissionStatusCollect.MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok;
                    }
                    else
                    {
                        resquestMissionStatusCollect.MissionStatus = PostTapingCollectShuttleMissionStatusType.Nok;
                    }

                    try
                    {
                        collectClient.MissionStatus(resquestMissionStatusCollect);
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- MissionStatus : OK");
                    }catch(Exception e)
                    {
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Collecte -- Mission Status : " + e.Message);
                    }

                }
                
                // ReportRunningMode
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ReportRunningMode.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {
                    indice = Variable.vVariableAna.IndexOf(NavetteCollecte.ReportRunningMode.DefaultCode);
                    resquestReportRunningModeCollect.DefaultCode = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableString.IndexOf(NavetteCollecte.ReportRunningMode.DefaultLabel);
                    resquestReportRunningModeCollect.DefaultLabel = Variable.vVariableString[indice].GetVarValue();

                    indice = Variable.vVariableAna.IndexOf(NavetteCollecte.ReportRunningMode.EquipmentCode);
                    resquestReportRunningModeCollect.EquipmentCode = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableAna.IndexOf(NavetteCollecte.ReportRunningMode.RunningMode);
                    if (Variable.vVariableAna[indice].GetVarValue() == 0)
                    {
                        resquestReportRunningModeCollect.RunningMode = PostTapingCollectShuttleRunningModeType.Default;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 1)
                    {
                        resquestReportRunningModeCollect.RunningMode = PostTapingCollectShuttleRunningModeType.Manual;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 2)
                    {
                        resquestReportRunningModeCollect.RunningMode = PostTapingCollectShuttleRunningModeType.MissionInProgress;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 3)
                    {
                        resquestReportRunningModeCollect.RunningMode = PostTapingCollectShuttleRunningModeType.Rest;
                    }

                    indice = Variable.vVariableString.IndexOf(NavetteCollecte.ReportRunningMode.RunningModeDate);
                    string sDate = Variable.vVariableString[indice].GetVarValue(); 
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Date : " + sDate);
                    indice = Variable.vVariableString.IndexOf(NavetteCollecte.ReportRunningMode.RunningModeTime);
                    string sTime = Variable.vVariableString[indice].GetVarValue();
                    SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Time : " + sTime);
                    DateTime Date = new DateTime();
                    
                    resquestReportRunningModeCollect.RunningModeDateTime = Date;

                    try
                    {
                        collectClient.ReportRunningMode(resquestReportRunningModeCollect);
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ReportRunningMode.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- Report Running Mode : OK");
                    }catch (Exception e)
                    {
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Collecte -- Report Running Mode : " + e.Message);
                    }


                }

                // TapingOutputConveyorNumber
                indice = Variable.vVariableBool.IndexOf(NavetteCollecte.TapingOutputConveyorNumber.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {
                    indice = Variable.vVariableAna.IndexOf(NavetteCollecte.TapingOutputConveyorNumber.TapingOutputConveyorNumber);
                    requestConveyorNumberCollect.TapingOutputConveyorNumber = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    try
                    {
                        collectClient.TapingOutputConveyorNumber(requestConveyorNumberCollect);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- Taping Output Conveyor Number : OK");
                    }
                    catch (Exception e)
                    {
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Collecte -- Taping Output Conveyor Number : " + e.Message);
                    }

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
       
        public void Dispose()
        {
            // Close the ServiceHost.
            hostEvacuation.Close();
            hostCollect.Close();
        }

    }
}
