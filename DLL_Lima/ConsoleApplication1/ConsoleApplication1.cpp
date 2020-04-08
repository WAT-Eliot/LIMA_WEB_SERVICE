// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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
	if (iVal == SOAP_OK) {
		std::cout << "Mission Status OK\n";
	}
	else {
		std::cout << "Mission Status NOK\n";
		client.soap_stream_fault(std::cerr);
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
