using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.Text;
using System.Threading.Tasks;

namespace usrmgrDotNetProject
{
    class Web_Service
    {
        public void Start(ServiceHost host)
        {
            // Enable metadata publishing.
            ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
            smb.HttpGetEnabled = true;
            smb.MetadataExporter.PolicyVersion = PolicyVersion.Policy15;
            host.Description.Behaviors.Add(smb);

            // Open the ServiceHost to start listening for messages. Since
            // no endpoints are explicitly configured, the runtime will create
            // one endpoint per base address for each service contract implemented
            // by the service.
            // Type as admin : netsh http add urlacl url=http://+:8080/InterfacesMxToPostTapingCollectShuttle01 user=WAT
            host.Open();
        }
    }
}
