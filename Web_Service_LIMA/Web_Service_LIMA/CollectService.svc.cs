﻿using collectns;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace Web_Service_LIMA
{
    // REMARQUE : vous pouvez utiliser la commande Renommer du menu Refactoriser pour changer le nom de classe "CollectService" à la fois dans le code, le fichier svc et le fichier de configuration.
    // REMARQUE : pour lancer le client test WCF afin de tester ce service, sélectionnez CollectService.svc ou CollectService.svc.cs dans l'Explorateur de solutions et démarrez le débogage.
    public class CollectService : collectns.IInterfacesMxToPostTapingCollectShuttle01
    {
        public void LayerDestination(PostTapingCollectShuttleLayerDestinationRequest request)
        {
            
        }
    }
}
