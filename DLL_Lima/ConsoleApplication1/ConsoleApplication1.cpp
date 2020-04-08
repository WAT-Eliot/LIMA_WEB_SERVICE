// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "soapBasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01Proxy.h"
#include "BasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01.h"


int main()
{
    std::cout << "Hello World!\n";

	

	ns3__PostTapingCollectShuttleMissionStatusRequest MissionStatusResquest = ns3__PostTapingCollectShuttleMissionStatusRequest();
	MissionStatusResquest.MissionNumber = new int(123);
	MissionStatusResquest.MissionStatus = new ns3__PostTapingCollectShuttleMissionStatusType(ns3__PostTapingCollectShuttleMissionStatusType__Ok);
	
	_ns1__MissionStatus MissionStatus = _ns1__MissionStatus();
	MissionStatus.request = new ns3__PostTapingCollectShuttleMissionStatusRequest(MissionStatusResquest);

	BasicHttpBinding_USCOREIInterfacesPostTapingCollectShuttleToMx01Proxy client(SOAP_XML_INDENT);

	client.MissionStatus = MissionStatus;
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
