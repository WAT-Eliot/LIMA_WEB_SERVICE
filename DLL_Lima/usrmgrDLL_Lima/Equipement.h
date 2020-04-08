#pragma once
#include "my_structs.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream> 


//Timeout de config = 120 x cycle (de base 1 seconde)
#define TIMEOUT_CONFIG 120
//Timeout plus long pour les équipements à réglage manu (Ensacheuse et Formeuse) 30min ->
#define TIMEOUT_CONFIG_MANUEL 1800

//Import PCVue methods
extern IAPIMgr * svmgrAPI;

class Variable
{
protected:
	CString m_name;       // Variable that will be read to after update
	int m_adviseN;		//Token for advise

public:

	Variable();
	Variable(CString name, int adviseN);

	CString GetVarName();
	void Advise();
	void Unadvise();
	int GetAdviseNumber();
	virtual void WriteVar();

	~Variable();
};

class TypeBool : public Variable {

	bool m_value;
public:
	TypeBool();
	TypeBool(CString name, int adviseN, bool value);
	void SetVar(bool value);
	void WriteVar(bool value);
	bool GetVarValue();
};

class TypeAna : public Variable {

	double m_value;
public:
	TypeAna();
	TypeAna(CString name, int adviseN, double value);

	void WriteVar(double value);
	void SetVar(double value);
	double GetVarValue();
};

class TypeString : public Variable {

	CString m_value;
public:
	TypeString();
	TypeString(CString name, int adviseN, CString value);

	void WriteVar(CString value);
	void SetVar(CString value);
	CString GetVarValue();
};

class Equipement
{
protected:
	
	//Liste des varaibles communes
	struct sMemberCommun {
		TypeAna		* MissionNumber;
		TypeString	* MissionStatus;
		TypeAna		* DefautCode;
		TypeString	* DefautLabel;
		TypeAna		* EquipementCode;
		TypeString	* RunningMode;
		TypeString	* RunningModeDateTime;
	} m_varCommun;
	

public:

	Equipement();
	Equipement(sNameCommun names, int start);
	
	~Equipement();
};

class NavetteCollecte : public Equipement {

	struct sMemberNavetteCollecte {
		TypeAna		* SorterInputConveyorNumber;
		TypeAna		* TapingOutputConveyorNumber;
		
	} m_varNavetteCollecte;


public:
	NavetteCollecte();
	NavetteCollecte(sNameNavetteCollecte names, int start);

};

class NavetteEvacuation : public Equipement {

	struct sMemberNavetteEvacuation {
		TypeAna		* LayerDestination;
		TypeAna		* SorterOutputConveyorNumber;

	} m_varNavetteEvacuation;

public:
	NavetteEvacuation();
	NavetteEvacuation(sNameNavetteEvacuation names, int start);

};