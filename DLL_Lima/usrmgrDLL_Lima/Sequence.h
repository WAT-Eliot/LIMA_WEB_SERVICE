#pragma once
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "Equipement.h"

//Import PCVue methods
extern IAPIMgr * svmgrAPI;
//Import variables: 
//extern MES * mesRayPro;


class Sequence
{
	//Consts
	const enum {
		//Production
		PROD_ATTENTE = 0,
		PROD_ECHANGE = 10,
		
	};

	//Members
	std::vector<TypeAna *>	vNumEtape;

	int m_mode;
	int m_delay;
	int m_countSec;
	
	boost::thread m_tG7Main;

public:
	//Constructor
	Sequence(void);

	//Start sequence
	void Start();

	//Start sequence alarme
	void StartAlarme(int id, double num, bool val);

	//Stop sequence
	void Stop();

	//G7 de production 
	void G7Main();

};


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}