using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client_LIMA.CollectShuttleToMx;
using Client_LIMA.EvacuationShuttleToMx;
using Client_LIMA;

//Logger
using log4net;
using log4net.Config;
using System.IO;
using log4net.Repository.Hierarchy;


namespace Console_LIMA
{
    class Program
    {
        private static ILog log = LogManager.GetLogger("Console"); //Start logger
        static void Main(string[] args)
        {
            Console.WriteLine("Console_LIMA");
            log.Info("start"); //Log
            InterfacesPostTapingCollectShuttleToMx01Client collectClient = new InterfacesPostTapingCollectShuttleToMx01Client();
            InterfacesPostTapingEvacuationShuttleToMx01Client evacuationClient = new InterfacesPostTapingEvacuationShuttleToMx01Client();

            //Collect
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

            //Evacuation
            PostTapingEvacuationShuttleMissionStatusRequest resquestMissionStatusEvacuation = new PostTapingEvacuationShuttleMissionStatusRequest
            {
                MissionNumber = 123,
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

            Console.WriteLine("Collect");
            collectClient.MissionStatus(resquestMissionStatusCollect);
            Console.WriteLine("MissionStatus OK");

            collectClient.ReportRunningMode(resquestReportRunningModeCollect);
            Console.WriteLine("ReportRunningMode OK");

            collectClient.TapingOutputConveyorNumber(requestConveyorNumberCollect);
            Console.WriteLine("ConveyorNumber OK");

            Console.WriteLine("Evacuation");
            evacuationClient.MissionStatus(resquestMissionStatusEvacuation);
            Console.WriteLine("MissionStatus OK");

            evacuationClient.ReportRunningMode(resquestReportRunningModeEvacuation);
            Console.WriteLine("ReportRunningMode OK");

            Console.ReadKey();
        }
    }
}
