#include "StdAfx.h"
#include "Equipement.h"
#include "my_logger.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/filesystem.hpp>

//pointer
std::vector<TypeBool*> vVariablesBool;
std::vector<TypeAna*> vVariablesAna;
std::vector<TypeString*> vVariablesString;
std::vector<TypeBool*> vVariablesAl;
std::vector < std::tuple<int, TypeAna*, int> > vVariablesAlarmes;
std::vector <boost::posix_time::ptime> vMoyenneBacsL1;
std::vector <boost::posix_time::ptime> vMoyenneBacsL2;


//--------------------------------Variable
//Constructors

Variable::Variable() {}
Variable::Variable(CString name, int adviseN) : m_name(name), m_adviseN(adviseN) {}

//
CString Variable::GetVarName() {
	return m_name;
}

void Variable::Advise() {
	if (svmgrAPI->VarAdvise(m_name, m_adviseN)) {
		//LOG_DEBUG << "Adv " << m_adviseN << ".";
	}
	else {
		LOG_ERROR << "Variable " << m_name << " (" << m_adviseN << ")  non abonnée.";
	}
}

void Variable::Unadvise() {
	svmgrAPI->VarUnadvise(m_name, m_adviseN);
}

int Variable::GetAdviseNumber() {
	return m_adviseN;
}

void Variable::WriteVar()
{
}


//Destructors
Variable::~Variable() {}


//--------------------------------TypeBool
//Constructors

TypeBool::TypeBool() {}
TypeBool::TypeBool(CString name, int adviseN, bool value) : Variable(name, adviseN), m_value(value) {
	if (adviseN != 0) {
		this->Advise();
		vVariablesBool.push_back(this);
	}

}

void TypeBool::WriteVar(bool value)
{
	if (value != m_value) {
		svmgrAPI->LogVarWrite(m_name, value, 0);
	}
}

void TypeBool::SetVar(bool value)
{
	try {
		m_value = value;
	}
	catch (...)
	{
		LOG_ERROR << "erreur machin !";
	}

}

bool TypeBool::GetVarValue()
{
	return this->m_value;
}

//--------------------------------TypeInt
//Constructors

TypeAna::TypeAna() {}
TypeAna::TypeAna(CString name, int adviseN, double value) : Variable(name, adviseN), m_value(value) {
	if (adviseN != 0) {
		try {
			this->Advise();
			vVariablesAna.push_back(this);
		}catch(...){ LOG_ERROR << "Ca bug a essai de s'abonner ou push vecteur."; }
	}

}

void TypeAna::WriteVar(double value)
{
	if (value != m_value) {
		svmgrAPI->AnaVarWrite(m_name, value, 0);
	}
}

void TypeAna::SetVar(double value)
{
	try { 
		m_value = value; 
	}
	catch (...)
	{
		LOG_ERROR << "erreur machin !";
	}

}

double TypeAna::GetVarValue()
{
	return this->m_value;
}


//--------------------------------TypeString
//Constructors

TypeString::TypeString() {}
TypeString::TypeString(CString name, int adviseN, CString value) : Variable(name, adviseN), m_value(value) {
	if (adviseN != 0) {
		try {
			this->Advise();
			vVariablesString.push_back(this);
		}
		catch (...) { 
			LOG_ERROR << "TypeString - Ca bug a essai de s'abonner ou push vecteur."; 
		}
	}

}

void TypeString::WriteVar(CString value)
{
	if (value != m_value) {
		//Enlever le /r présent dans le string
		value.Remove('\r');
		svmgrAPI->TxtVarWrite(m_name, value , 0);
	}
}

void TypeString::SetVar(CString value)
{
	try { 
		m_value = value; 
	}
	catch (...)
	{
		LOG_ERROR << "erreur machin !";
	}

}

CString TypeString::GetVarValue()
{
	return m_value;
}


//--------------------------------Equipement
//Constructors

Equipement::Equipement() {

}

Equipement::Equipement(sNameCommun names, int start) {
	
	m_varCommun.MissionNumber = new TypeAna(names.MissionNumber, start + 1, 0);
	m_varCommun.MissionStatus = new TypeString(names.MissionStatus, start + 2, 0);
	m_varCommun.DefautCode = new TypeAna(names.DefautCode, start + 3, 0);
	m_varCommun.DefautLabel = new TypeString(names.DefautLabel, start + 4, 0);
	m_varCommun.EquipementCode = new TypeAna(names.EquipementCode, start + 5, 0);
	m_varCommun.RunningMode = new TypeString(names.RunningMode, start + 6, 0);
	m_varCommun.RunningModeDateTime = new TypeString(names.RunningModeDateTime, start + 7, 0);

}

//Destructors
Equipement::~Equipement() {}


//--------------------------------Navette Collecte
//Constructors

NavetteCollecte::NavetteCollecte() {};

NavetteCollecte::NavetteCollecte(sNameNavetteCollecte names, int start){

	m_varCommun.MissionNumber = new TypeAna(names.MissionNumber, start + 1, 0);
	m_varCommun.MissionStatus = new TypeString(names.MissionStatus, start + 2, 0);
	m_varCommun.DefautCode = new TypeAna(names.DefautCode, start + 3, 0);
	m_varCommun.DefautLabel = new TypeString(names.DefautLabel, start + 4, 0);
	m_varCommun.EquipementCode = new TypeAna(names.EquipementCode, start + 5, 0);
	m_varCommun.RunningMode = new TypeString(names.RunningMode, start + 6, 0);
	m_varCommun.RunningModeDateTime = new TypeString(names.RunningModeDateTime, start + 7, 0);
	m_varNavetteCollecte.SorterInputConveyorNumber = new TypeAna(names.SorterInputConveyorNumber, start + 8, 0);
	m_varNavetteCollecte.TapingOutputConveyorNumber = new TypeAna(names.TapingOutputConveyorNumber, start + 9, 0);
	
}

//--------------------------------Navette Evacuation
//Constructors

NavetteEvacuation::NavetteEvacuation() {};

NavetteEvacuation::NavetteEvacuation(sNameNavetteEvacuation names, int start) {

	m_varCommun.MissionStatus = new TypeString(names.MissionStatus, start + 2, 0);
	m_varCommun.DefautCode = new TypeAna(names.DefautCode, start + 3, 0);
	m_varCommun.DefautLabel = new TypeString(names.DefautLabel, start + 4, 0);
	m_varCommun.EquipementCode = new TypeAna(names.EquipementCode, start + 5, 0);
	m_varCommun.RunningMode = new TypeString(names.RunningMode, start + 6, 0);
	m_varCommun.RunningModeDateTime = new TypeString(names.RunningModeDateTime, start + 7, 0);
	m_varNavetteEvacuation.LayerDestination = new TypeAna(names.LayerDestination, start + 8, 0);
	m_varNavetteEvacuation.SorterOutputConveyorNumber = new TypeAna(names.SorterOutputConveyorNumber, start + 9, 0);

};