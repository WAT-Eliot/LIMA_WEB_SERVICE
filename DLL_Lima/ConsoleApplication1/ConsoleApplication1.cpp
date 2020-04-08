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

	if (iVal == SOAP_OK) {
		std::cout << "Mission Status OK\n";
	}
	else {
		std::cout << "Mission Status NOK\n";
		client.soap_stream_fault(std::cerr);
	}


	//////////////////// Report Running Mode ////////////////

	ns3__PostTapingCollectShuttleReportRunningModeRequest ReportRunningModeResquest = ns3__PostTapingCollectShuttleReportRunningModeRequest();
	ReportRunningModeResquest.DefaultCode = new int(404);
	ReportRunningModeResquest.DefaultLabel = new std::string("Test");
	ReportRunningModeResquest.EquipmentCode = new int(5555);
	ReportRunningModeResquest.RunningMode = new ns3__PostTapingCollectShuttleRunningModeType(ns3__PostTapingCollectShuttleRunningModeType__Manual);
	ReportRunningModeResquest.RunningModeDateTime = new time_t(time(NULL));

	_ns1__ReportRunningMode *ReportRunningMode = new _ns1__ReportRunningMode();
	ReportRunningMode->request = new ns3__PostTapingCollectShuttleReportRunningModeRequest(ReportRunningModeResquest);
	_ns1__ReportRunningModeResponse ReportRunningModeResponse = _ns1__ReportRunningModeResponse();

	iVal = client.ReportRunningMode(ReportRunningMode, ReportRunningModeResponse);


	if (iVal == SOAP_OK) {
		std::cout << "Report Running Mode OK\n";
	}
	else {
		std::cout << "Report Running Mode NOK\n";
		client.soap_stream_fault(std::cerr);
	}

}