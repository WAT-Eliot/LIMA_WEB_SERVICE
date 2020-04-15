using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading;
using usrmgrDotNetProject.CollectShuttleToMx;
using usrmgrDotNetProject.EvacuationShuttleToMx;
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
        public static string PrefixeCollecteEchange1 = tempPrefixeCollecte + ".Echange1";
        public static string PrefixeCollecteEchange2 = tempPrefixeCollecte + ".Echange2";
        public static string PrefixeCollecteEchange3 = tempPrefixeCollecte + ".Echange3";
        public static string PrefixeCollecteEchange4 = tempPrefixeCollecte + ".Echange4";
        public static string PrefixeEvacuationEchange1 = tempPrefixeEvacuation + ".Echange1";
        public static string PrefixeEvacuationEchange2 = tempPrefixeEvacuation + ".Echange2";
        public static string PrefixeEvacuationEchange3 = tempPrefixeEvacuation + ".Echange3";

        public const uint CollectEchange1 = 0;
        public const uint CollectEchange2 = 20;
        public const uint CollectEchange3 = 40;
        public const uint CollectEchange4 = 60;
        public const uint CollectVarCommun = 80;

        public const uint EvacEchange1 = 100;
        public const uint EvacEchange2 = 120;
        public const uint EvacEchange3 = 140;
        public const uint EvacVarCommun = 160;

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
            MissionNumber = 0,
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

        //Evacuation
        internal static InterfacesPostTapingEvacuationShuttleToMx01Client evacuationClient;

        PostTapingEvacuationShuttleMissionStatusRequest resquestMissionStatusEvacuation = new PostTapingEvacuationShuttleMissionStatusRequest
        {
            MissionNumber = 0,
            MissionStatus = PostTapingEvacuationShuttleMissionStatusType.Ok,
        };

        PostTapingEvacuationShuttleReportRunningModeRequest resquestReportRunningModeEvacuation = new PostTapingEvacuationShuttleReportRunningModeRequest
        {
            DefaultCode = 0,
            DefaultLabel = "?",
            EquipmentCode = 0,
            RunningMode = PostTapingEvacuationShuttleRunningModeType.Manual,
            RunningModeDateTime = DateTime.Now,
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
            VarCollecteCommun(CollectVarCommun);

            /*------------------------ Navette Evacuation -----------------------------------------------------*/

            EvacuationEchange1(EvacEchange1);
            EvacuationEchange2(EvacEchange2);
            EvacuationEchange3(EvacEchange3);
            VarEvacuationCommun(EvacVarCommun);

            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Il y a : " + (Variable.vVariableBool.Count() + Variable.vVariableAna.Count() + Variable.vVariableString.Count()) + " variables dont " + Variable.vVariableBool.Count() + " booleans, " + Variable.vVariableAna.Count() + " valeurs ana, " + Variable.vVariableString.Count() + " chaines de caractere.");

            /*----------------------------- Fin Déclaration des variables ----------------------------------------*/


            Service_Collect.Start(hostCollect);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Service Web Collect démarrée !");


            Service_Evacuation.Start(hostEvacuation);
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Service Web Evacuation démarrée !");

            /*----------------------------------- Web Service Création Client ----------------------------------------*/
            // Class constructeur
            System.ServiceModel.Channels.Binding binding = new System.ServiceModel.BasicHttpBinding();
            // Collecte
            try
            { 
                EndpointAddress address = new EndpointAddress("http://wstest.groupeliebot.fr/Mx/Lima1/Mx.Broker.Lima.Endpoint/Interface/InterfacesPostTapingCollectShuttleToMx0101.svc");
                collectClient = new InterfacesPostTapingCollectShuttleToMx01Client(binding, address);
            }

            catch (Exception e)
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte : " + e.Message);
            }

            try
            {
                EndpointAddress address = new EndpointAddress("http://wstest.groupeliebot.fr/Mx/Lima1/Mx.Broker.Lima.Endpoint/Interface/InterfacesPostTapingEvacuationShuttleToMx0101.svc");
                evacuationClient = new InterfacesPostTapingEvacuationShuttleToMx01Client(binding, address);
            }

            catch (Exception e)
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Evacuation : " + e.Message);
            }


            /*---------------------------------- Fin Web Service Création Client ---------------------------------------*/

        }

        //Class diferente
        public void CollecteEchange1(uint nbr)
        {
            // TapingOutputConveyorNumber
            NavetteCollecte.TapingOutputConveyorNumber.SendCom = new TypeBool(PrefixeCollecteEchange1 + ".SendCom", nbr + 1, false);
            NavetteCollecte.TapingOutputConveyorNumber.TapingOutputConveyorNumber = new TypeAna(PrefixeCollecteEchange1 + ".TapingOutputConveyorNumber", nbr + 2, 0);

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
            NavetteCollecte.MissionStatus.SendCom = new TypeBool(PrefixeCollecteEchange3 + ".SendCom", nbr + 1, false);
            NavetteCollecte.MissionStatus.MissionStatus = new TypeBool(PrefixeCollecteEchange3 + ".MissionStatus", nbr + 2, false);
            NavetteCollecte.MissionStatus.MissionNumber = new TypeAna(PrefixeCollecteEchange3 + ".MissionNumber", nbr + 3, 0);
        }

        private void CollecteEchange4(uint nbr)
        {
            // ReportRunningMode
            NavetteCollecte.ReportRunningMode.SendCom = new TypeBool(PrefixeCollecteEchange4 + ".SendCom", nbr + 1, false);
            NavetteCollecte.ReportRunningMode.DefaultCode = new TypeAna(PrefixeCollecteEchange4 + ".DefaultCode", nbr + 2, 0);
            NavetteCollecte.ReportRunningMode.EquipmentCode = new TypeAna(PrefixeCollecteEchange4 + ".EquipmentCode", nbr + 3, 0);
            NavetteCollecte.ReportRunningMode.RunningMode = new TypeAna(PrefixeCollecteEchange4 + ".RunningMode", nbr + 4, 0);
            NavetteCollecte.ReportRunningMode.DefaultLabel = new TypeString(PrefixeCollecteEchange4 + ".DefaultLabel", nbr + 5, "");
            NavetteCollecte.ReportRunningMode.RunningModeDate = new TypeString(PrefixeCollecteEchange4 + ".RunningModeDate", nbr + 6, "");
            NavetteCollecte.ReportRunningMode.RunningModeTime = new TypeString(PrefixeCollecteEchange4 + ".RunningModeTime", nbr + 7, "");
        }

        private void VarCollecteCommun(uint nbr)
        {
            NavetteCollecte.ErreurCom = new TypeBool(tempPrefixeCollecte + ".ErreurCom", nbr + 1, false);
            NavetteCollecte.Test = new TypeAna("Test", nbr + 2, 0);
        }

        private void EvacuationEchange1(uint nbr)
        {
            // LayerDestionation
            NavetteEvacuation.LayerEvacuation.ReceiveCom = new TypeBool(PrefixeEvacuationEchange1 + ".ReceiveCom", nbr + 1, false);
            NavetteEvacuation.LayerEvacuation.LayerDestination = new TypeAna(PrefixeEvacuationEchange1 + ".LayerDestination", nbr + 2, 0);
            NavetteEvacuation.LayerEvacuation.MissionNumber = new TypeAna(PrefixeEvacuationEchange1 + ".MissionNumber", nbr + 3, 0);
            NavetteEvacuation.LayerEvacuation.SorterOutputConveyorNumber = new TypeAna(PrefixeEvacuationEchange1 + ".SorterOutputConveyorNumber", nbr + 4, 0);
        }

        private void EvacuationEchange2(uint nbr)
        {
            // MissionStatus
            NavetteEvacuation.MissionStatus.SendCom = new TypeBool(PrefixeEvacuationEchange2 + ".SendCom", nbr + 1, false);
            NavetteEvacuation.MissionStatus.MissionStatus = new TypeBool(PrefixeEvacuationEchange2 + ".MissionStatus", nbr + 2, false);
            NavetteEvacuation.MissionStatus.MissionNumber = new TypeAna(PrefixeEvacuationEchange2 + ".MissionNumber", nbr + 3, 0);
        }

        private void EvacuationEchange3(uint nbr)
        {
            // ReportRunningMode
            NavetteEvacuation.ReportRunningMode.SendCom = new TypeBool(PrefixeEvacuationEchange3 + ".SendCom", nbr + 1, false);
            NavetteEvacuation.ReportRunningMode.DefaultCode = new TypeAna(PrefixeEvacuationEchange3 + ".DefaultCode", nbr + 2, 0);
            NavetteEvacuation.ReportRunningMode.EquipmentCode = new TypeAna(PrefixeEvacuationEchange3 + ".EquipmentCode", nbr + 3, 0);
            NavetteEvacuation.ReportRunningMode.RunningMode = new TypeAna(PrefixeEvacuationEchange3 + ".RunningMode", nbr + 4, 0);
            NavetteEvacuation.ReportRunningMode.DefaultLabel = new TypeString(PrefixeEvacuationEchange3 + ".DefaultLabel", nbr + 5, "");
            NavetteEvacuation.ReportRunningMode.RunningModeDate = new TypeString(PrefixeEvacuationEchange3 + ".RunningModeDate", nbr + 6, "");
            NavetteEvacuation.ReportRunningMode.RunningModeTime = new TypeString(PrefixeEvacuationEchange3 + ".RunningModeTime", nbr + 7, "");
        }

        private void VarEvacuationCommun(uint nbr)
        {
            NavetteEvacuation.ErreurCom = new TypeBool(tempPrefixeEvacuation + ".ErreurCom", nbr + 1, false);
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

                    indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.MissionStatus);
                    if (Variable.vVariableBool[indice].GetVarValue())
                    {
                        resquestMissionStatusCollect.MissionStatus = PostTapingCollectShuttleMissionStatusType.Nok;
                    }
                    else
                    {
                        resquestMissionStatusCollect.MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok;
                    }

                    try
                    {
                        collectClient.MissionStatus(resquestMissionStatusCollect);
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.MissionStatus.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- MissionStatus : OK");
                    }catch(Exception e)
                    {
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ErreurCom);
                        Variable.vVariableBool[indice].WriteVar(true);
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
                    
                    resquestReportRunningModeCollect.RunningModeDateTime = DateTime.Now;

                    try
                    {
                        collectClient.ReportRunningMode(resquestReportRunningModeCollect);
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ReportRunningMode.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- Report Running Mode : OK");
                    }catch (Exception e)
                    {
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ErreurCom);
                        Variable.vVariableBool[indice].WriteVar(true);
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
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.TapingOutputConveyorNumber.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Collecte -- Taping Output Conveyor Number : OK");
                    }
                    catch (Exception e)
                    {
                        indice = Variable.vVariableBool.IndexOf(NavetteCollecte.ErreurCom);
                        Variable.vVariableBool[indice].WriteVar(true);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Collecte -- Taping Output Conveyor Number : " + e.Message);
                    }

                }

                /*------------------------- Navette Evacuation --------------------------------------*/
                
                // MissionStatus
                indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.MissionStatus.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {
                    indice = Variable.vVariableAna.IndexOf(NavetteEvacuation.MissionStatus.MissionNumber);
                    resquestMissionStatusEvacuation.MissionNumber = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.MissionStatus.MissionStatus);
                    if (Variable.vVariableBool[indice].GetVarValue())
                    {
                        resquestMissionStatusEvacuation.MissionStatus = PostTapingEvacuationShuttleMissionStatusType.Nok;
                    }
                    else
                    {
                        resquestMissionStatusEvacuation.MissionStatus = PostTapingEvacuationShuttleMissionStatusType.Ok;
                    }

                    try
                    {
                        evacuationClient.MissionStatus(resquestMissionStatusEvacuation);
                        indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.MissionStatus.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Evacuation -- Mission Status : OK");
                    }
                    catch (Exception e)
                    {
                        indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.ErreurCom);
                        Variable.vVariableBool[indice].WriteVar(true);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Evacuation -- Mission Status : " + e.Message);
                    }
                }

                // ReportRunningMode
                indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.ReportRunningMode.SendCom);
                if (Variable.vVariableBool[indice].GetVarValue())
                {
                    indice = Variable.vVariableAna.IndexOf(NavetteEvacuation.ReportRunningMode.DefaultCode);
                    resquestReportRunningModeEvacuation.DefaultCode = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableString.IndexOf(NavetteEvacuation.ReportRunningMode.DefaultLabel);
                    resquestReportRunningModeEvacuation.DefaultLabel = Variable.vVariableString[indice].GetVarValue();

                    indice = Variable.vVariableAna.IndexOf(NavetteEvacuation.ReportRunningMode.EquipmentCode);
                    resquestReportRunningModeEvacuation.EquipmentCode = Convert.ToInt32(Variable.vVariableAna[indice].GetVarValue());

                    indice = Variable.vVariableAna.IndexOf(NavetteEvacuation.ReportRunningMode.RunningMode);
                    if (Variable.vVariableAna[indice].GetVarValue() == 0)
                    {
                        resquestReportRunningModeEvacuation.RunningMode = PostTapingEvacuationShuttleRunningModeType.Default;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 1)
                    {
                        resquestReportRunningModeEvacuation.RunningMode = PostTapingEvacuationShuttleRunningModeType.Manual;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 2)
                    {
                        resquestReportRunningModeEvacuation.RunningMode = PostTapingEvacuationShuttleRunningModeType.MissionInProgress;
                    }
                    else if (Variable.vVariableAna[indice].GetVarValue() == 3)
                    {
                        resquestReportRunningModeEvacuation.RunningMode = PostTapingEvacuationShuttleRunningModeType.Rest;
                    }

                    resquestReportRunningModeEvacuation.RunningModeDateTime = DateTime.Now;

                    try
                    {
                        evacuationClient.ReportRunningMode(resquestReportRunningModeEvacuation);
                        indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.ReportRunningMode.SendCom);
                        Variable.vVariableBool[indice].WriteVar(false);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Evacuation -- Report Running Mode : OK");
                    }
                    catch (Exception e)
                    {
                        indice = Variable.vVariableBool.IndexOf(NavetteEvacuation.ErreurCom);
                        Variable.vVariableBool[indice].WriteVar(true);
                        SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Evacuation -- Report Running Mode : " + e.Message);
                    }
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
