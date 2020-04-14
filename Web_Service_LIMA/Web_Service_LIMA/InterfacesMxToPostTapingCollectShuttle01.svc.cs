using collectns;
using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Threading.Tasks;

namespace Web_Service_LIMA
{
    // REMARQUE : vous pouvez utiliser la commande Renommer du menu Refactoriser pour changer le nom de classe "CollectService" à la fois dans le code, le fichier svc et le fichier de configuration.
    // REMARQUE : pour lancer le client test WCF afin de tester ce service, sélectionnez CollectService.svc ou CollectService.svc.cs dans l'Explorateur de solutions et démarrez le débogage.

    public class InterfacesMxToPostTapingCollectShuttle01 : collectns.IInterfacesMxToPostTapingCollectShuttle01
    {
        private static ILog log = LogManager.GetLogger("CollectService"); //Start logger
        public void LayerDestination(PostTapingCollectShuttleLayerDestinationRequest request)
        {
            log.Info(request.MissionNumber + " " + request.SorterInputConveyorNumber + " " + request.TapingOutputConveyorNumber);
        }
    }
}
