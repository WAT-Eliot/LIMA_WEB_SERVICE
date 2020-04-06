using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client_LIMA.CollectShuttleToMx;
using Client_LIMA;

namespace Console_LIMA
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Console_LIMA");
            InterfacesPostTapingCollectShuttleToMx01Client collectClient = new InterfacesPostTapingCollectShuttleToMx01Client();
            PostTapingCollectShuttleMissionStatusRequest resquestMissionStatus = new PostTapingCollectShuttleMissionStatusRequest
            {
                MissionNumber = 123,
                MissionStatus = PostTapingCollectShuttleMissionStatusType.Ok,
            };

            PostTapingCollectShuttleReportRunningModeRequest resquestReportRunningMode = new PostTapingCollectShuttleReportRunningModeRequest
            {
                DefaultCode = 0,
                DefaultLabel = "?",
                EquipmentCode = 0,
                RunningMode = PostTapingCollectShuttleRunningModeType.Manual,
                RunningModeDateTime = DateTime.Now,
            };

            PostTapingCollectShuttleTapingOutputConveyorRequest requestConveyorNumber = new PostTapingCollectShuttleTapingOutputConveyorRequest
            {
                TapingOutputConveyorNumber = 0,
            };

            collectClient.MissionStatus(resquestMissionStatus);
            Console.WriteLine("MissionStatus OK");

            collectClient.ReportRunningMode(resquestReportRunningMode);
            Console.WriteLine("ReportRunningMode OK");

            collectClient.TapingOutputConveyorNumber(requestConveyorNumber);
            Console.WriteLine("ConveyorNumber OK");

            Console.ReadKey();
        }
    }
}
