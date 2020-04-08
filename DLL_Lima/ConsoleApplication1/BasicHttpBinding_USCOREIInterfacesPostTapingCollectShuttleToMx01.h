
#include "stdsoap2.h"
/* This defines the global XML namespaces[] table to #include and compile */
SOAP_NMAC struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL },
        { "SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "ns2", "http://schemas.microsoft.com/2003/10/Serialization/", NULL, NULL },
        { "ns3", "http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapingCollectShuttle01", NULL, NULL },
        { "ns1", "http://tempuri.org/", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
