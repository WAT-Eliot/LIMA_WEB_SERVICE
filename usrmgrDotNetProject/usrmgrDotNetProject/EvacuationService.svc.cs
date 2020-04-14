using evacuationns;
//using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using usrmgrDotNetProject;

namespace Web_Service_LIMA
{
    // REMARQUE : vous pouvez utiliser la commande Renommer du menu Refactoriser pour changer le nom de classe "EvacuationService" à la fois dans le code, le fichier svc et le fichier de configuration.
    // REMARQUE : pour lancer le client test WCF afin de tester ce service, sélectionnez EvacuationService.svc ou EvacuationService.svc.cs dans l'Explorateur de solutions et démarrez le débogage.
    public class EvacuationService : evacuationns.IInterfacesMxToPostTapingEvacuationShuttle01
    {
        //private static ILog log = LogManager.GetLogger("EvacuationService"); //Start logger

        public void LayerEvacuation(PostTapingEvacuationShuttleLayerEvacuationRequest request)
        {
            //log.Info(request.MissionNumber + " " + request.LayerDestination + " " + request.SorterOutputConveyorNumber);
            
        }
    }
}
