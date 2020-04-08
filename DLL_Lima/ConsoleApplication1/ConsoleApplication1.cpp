// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "soapH.h"
#include "soapBasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01Proxy.h"
#include "BasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01.h"


int main()
{
    std::cout << "Web_Service\n";

	int iVal = 0;
	
	ns3__PostTapingCollectShuttleMissionStatusRequest MissionStatusResquest = ns3__PostTapingCollectShuttleMissionStatusRequest();
	MissionStatusResquest.MissionNumber = new int(123);
	MissionStatusResquest.MissionStatus = new ns3__PostTapingCollectShuttleMissionStatusType(ns3__PostTapingCollectShuttleMissionStatusType__Ok);
	
	_ns1__MissionStatus * MissionStatus = new _ns1__MissionStatus();
	MissionStatus->request = new ns3__PostTapingCollectShuttleMissionStatusRequest(MissionStatusResquest);

	_ns1__MissionStatusResponse MissionStatusResponse = _ns1__MissionStatusResponse();

	BasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01Proxy client(SOAP_XML_INDENT);

	iVal = client.MissionStatus(MissionStatus, MissionStatusResponse);
	//struct soap * soap = soap_new();
	//iVal = soap. = "soap/XML";

	if (iVal == SOAP_OK) {
		std::cout << "Mission Status OK\n";
	}
	else {
		std::cout << "Mission Status NOK\n";
		client.soap_stream_fault(std::cerr);
	}

}