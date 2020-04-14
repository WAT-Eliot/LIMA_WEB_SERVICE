using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace Web_Service_LIMA
{
    // REMARQUE : vous pouvez utiliser la commande Renommer du menu Refactoriser pour changer le nom d'interface "ICollectService" à la fois dans le code et le fichier de configuration.
    [ServiceContract]
    public interface ICollectService
    {
        [OperationContract]
        void DoWork();
    }
}
