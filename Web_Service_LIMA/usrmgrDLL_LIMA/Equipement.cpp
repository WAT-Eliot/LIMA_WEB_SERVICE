#include "StdAfx.h"
#include "Equipement.h"
#include "my_logger.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/filesystem.hpp>

/*#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif


#define LOCAL_FILE      "/tmp/uploadthis.txt"
#define UPLOAD_FILE_AS  "while-uploading.txt"
#define REMOTE_URL      "ftp://example.com/"  UPLOAD_FILE_AS
#define RENAME_FILE_TO  "renamed-and-fine.txt"
*/

//pointer 
//boost::ptr_vector< Variable > vVariables;
//std::vector<std::shared_ptr<Variable>> vVariables;
std::vector<TypeBool*> vVariablesBool;
std::vector<TypeAna*> vVariablesAna;
std::vector<TypeString*> vVariablesString;
std::vector<TypeBool*> vVariablesAl;
//std::vector<std::vector<Variable*> > vVariablesAlarmes;
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

//void Variable::SetVar() {}



//Destructors
Variable::~Variable() {}


//--------------------------------TypeBool
//Constructors

TypeBool::TypeBool() {}
TypeBool::TypeBool(CString name, int adviseN, bool value) : Variable(name, adviseN), m_value(value) {
//		LOG_DEBUG << "Variable " << m_name << " créée avec valeur." << m_value << " et adv n." << m_adviseN << ".";

	if (adviseN != 0) {
		this->Advise();
		vVariablesBool.push_back(this);
	}

}

void TypeBool::WriteVar(bool value)
{
	if (value != m_value) {
		svmgrAPI->LogVarWrite(m_name, value, 0);
		//m_value = value;
	}
}

void TypeBool::SetVar(bool value)
{
//	LOG_DEBUG << "value = " << value << "";
	try {
		m_value = value;
//		LOG_DEBUG << "Copie variable : valeur = " << value << "";
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
//		LOG_DEBUG << "Variable " << m_name << " créée avec valeur." << m_value << " et adv n." << m_adviseN << ".";

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
		//LOG_DEBUG << "Ecriture de " << m_name << " -> " << value << "";
		svmgrAPI->AnaVarWrite(m_name, value, 0);
		//m_value = value;
	}
}

void TypeAna::SetVar(double value)
{
//	LOG_DEBUG << "value = " << value << "";
	try { 
		m_value = value; 
//		LOG_DEBUG << "Copie variable : valeur = " << value << "";
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
//		LOG_DEBUG << "Variable " << m_name << " créée avec valeur." << m_value << " et adv n." << m_adviseN << ".";

	if (adviseN != 0) {
		try {
			this->Advise();
			vVariablesString.push_back(this);
		}catch (...) { LOG_ERROR << "TypeString - Ca bug a essai de s'abonner ou push vecteur."; }
	}

}

void TypeString::WriteVar(CString value)
{
	if (value != m_value) {
		//Enlever le /r présent dans le string
		value.Remove('\r');
		svmgrAPI->TxtVarWrite(m_name, value , 0);
		//m_value = value;
	}
}

void TypeString::SetVar(CString value)
{
//	LOG_DEBUG << "value = " << value << "";
	try { 
		m_value = value; 
//		LOG_DEBUG << "Copie variable : valeur = " << m_value << "";
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
	
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant =		new TypeBool(names.demandeDistant,		start + 2, 0);
	m_varCommun.modeDistant =			new TypeBool(names.modeDistant,			start + 3, 0);
	m_varCommun.alarmeNonBloquante =	new TypeBool(names.alarmeNonBloquante,	start + 4, 0);
	m_varCommun.alarmeBloquante =		new TypeBool(names.alarmeBloquante,		start + 5, 0);
	m_varCommun.numAlarme =				new TypeAna(names.numAlarme,			start + 6, 0);
	m_varCommun.etatAnimation =			new TypeAna(names.etatAnimation,		start + 7, 0);
	m_varCommun.mdvSPV =				new TypeAna(names.mdvSPV,				start + 8, 0);
	m_varCommun.mdvAPI =				new TypeAna(names.mdvAPI,				start + 9, 0);
	m_varCommun.timeoutComAPI =			new TypeBool(names.timeoutComAPI,		start + 10, 0);
	m_varCommun.timeoutMarcheAPI =		new TypeBool(names.timeoutMarcheAPI,	start + 11, 0);


	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	IDmachine = ++ID;
	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;
}

//Destructors
Equipement::~Equipement() {}

bool Equipement::GestionProprete(int ligne) {
	return 0;
}

bool Equipement::GestionConfig(int ligne)
{
	return false;
}

bool Equipement::EnvoiDemVidange(int ligne, bool ValidTare) { 
	/*
	Modif 12/12/19
	*/
	return 0; 
}

// Retourne true si l'équipement est distant
bool Equipement::GetModeDistant(){
	/*
	Modifier 10/12/19
	*/
	return this->m_varCommun.demandeDistant->GetVarValue() && this->m_varCommun.modeDistant->GetVarValue() ;
}

// Retourne true si l'équipement etait distant
bool Equipement::GetModeDistant_OLD() {
	/*
	Ajout 10/12/19
	*/
	return this->modeDistant_OLD;
}

// Affecte l'etat de l'équipement distant/local
bool Equipement::SetModeDistant_OLD(bool val) {
	/*
	Ajout 10/12/19
	*/
	this->modeDistant_OLD = val;

	return true;
}

int Equipement::GetCodeDef()
{
	int code = 0;
	code = IDmachine * 10000 + int(this->m_varCommun.alarmeBloquante->GetVarValue());
	return code;
}

bool Equipement::ResetDemandeProprete(int ligne)
{
	return 1;
}

bool Equipement::ResetDemandeVidange(int ligne)
{
	return 1;
}

bool Equipement::ResetDemandeConfiguration(int ligne)
{
	return 1;
}

bool Equipement::SetMotEtat() {
	//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);
	
	return true;
}

bool Equipement::MotDeVieAPI() {
	//Si le MDV API n'a pas bougé
	if (this->memMdVAPI == this->m_varCommun.mdvAPI->GetVarValue()) {
		cptTimeoutCom++;
	}
	else {
		//Recopie du mot de vie API vers SPV
		this->m_varCommun.mdvSPV->WriteVar(this->m_varCommun.mdvAPI->GetVarValue());
		//Recopie de la mémoire
		this->memMdVAPI = (int)this->m_varCommun.mdvAPI->GetVarValue();
		cptTimeoutCom = 0;
		this->m_varCommun.timeoutComAPI->WriteVar(false);
	}
	//Si compteur fail est trop grand, on envoie le défaut
	if (cptTimeoutCom > 3) {
		this->m_varCommun.timeoutComAPI->WriteVar(true);
	}
	return true;
}

bool Equipement::EnvoiDemMarche(int ligne) {
	return false;
}

bool Equipement::EnvoiDmeArret(int ligne) {
	return false;
}

int Equipement::ID;

//Affecte le bit machine sortie
bool Equipement::SetMachineSortie(int ligne, bool val) {
	/*
	Ajout 11/12/19
	*/
	bool retour = true;
	if (ligne == 1) {
		this->machineSortie_L1 = val;
		LOG_INFO << "Machine : " << this->IDmachine << " sortie de la campagne";
	}
	else {
		this->machineSortie_L2 = val;
		LOG_INFO << "Machine : " << this->IDmachine << " sortie de la campagne";
	}

	return retour;
}

double Equipement::GetEtatAPI(int ligne) {
	/*
	Ajout 26/12/19
	*/
	return 0;
}

bool Equipement::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 27/03/2020
	*/

	return true;
}


//--------------------------------RobotCaisses
//Constructors

RobotCaisses::RobotCaisses() {};

RobotCaisses::RobotCaisses(sNameRobotCaisses names, int start){

	m_varRobotCaisses.demandeMarche =		new TypeBool(names.demandeMarche,				start , 0);
	m_varCommun.etatAPI =					new TypeAna(names.etatAPI,				start + 1, 0);
	m_varCommun.demandeDistant =			new TypeBool(names.demandeDistant,			start + 2, 0);
	m_varCommun.modeDistant =				new TypeBool(names.modeDistant,				start + 3, 0);
	m_varCommun.alarmeNonBloquante =		new TypeBool(names.alarmeNonBloquante,		start + 4, 0);
	m_varCommun.alarmeBloquante =			new TypeBool(names.alarmeBloquante,			start + 5, 0);
	m_varCommun.numAlarme =					new TypeAna(names.numAlarme,				start + 6, 0);

	m_varRobotCaisses.circuitPropreteL1 =	new TypeAna(names.circuitPropreteL1,		start + 9, 0);
	m_varRobotCaisses.circuitPropreteL2 =	new TypeAna(names.circuitPropreteL2,		start + 10, 0);
	m_varRobotCaisses.paramPropreteL1 =		new TypeAna(names.paramPropreteL1,		start + 11, 0);
	m_varRobotCaisses.paramPropreteL2 =		new TypeAna(names.paramPropreteL2,		start + 12, 0);
	m_varRobotCaisses.codeArticleL1 =		new TypeAna(names.codeArticleL1,		start + 13, 0);
	m_varRobotCaisses.codeArticleL2 =		new TypeAna(names.codeArticleL2,		start + 14, 0);
	m_varRobotCaisses.RnumeroBacL1 =		new TypeAna(names.RnumeroBacL1,			start + 15, 0);
	m_varRobotCaisses.RnumeroBacL2 =		new TypeAna(names.RnumeroBacL2,			start + 16, 0);
	m_varRobotCaisses.RcodeArticleL1 =		new TypeAna(names.RcodeArticleL1,		start + 17, 0);
	m_varRobotCaisses.RcodeArticleL2 =		new TypeAna(names.RcodeArticleL2,		start + 18, 0);
	m_varRobotCaisses.RqtePiecesL1 =		new TypeAna(names.RqtePiecesL1,			start + 19, 0);
	m_varRobotCaisses.RqtePiecesL2 =		new TypeAna(names.RqtePiecesL2,			start + 20, 0);
	m_varRobotCaisses.demandePropreteL1 =	new TypeBool(names.demandePropreteL1,	start + 21, 0);
	m_varRobotCaisses.demandePropreteL2 =	new TypeBool(names.demandePropreteL2,	start + 22, 0);
	m_varRobotCaisses.demandeConfigL1 =		new TypeBool(names.demandeConfigL1,		start + 25, 0);
	m_varRobotCaisses.demandeConfigL2 =		new TypeBool(names.demandeConfigL2,		start + 26, 0);
	m_varRobotCaisses.timeoutConfigL1 =		new TypeBool(names.timeoutConfigL1,		start + 27, 0);
	m_varRobotCaisses.timeoutConfigL2 =		new TypeBool(names.timeoutConfigL2,		start + 28, 0);
	m_varRobotCaisses.demandeVidangeL1 =	new TypeBool(names.demandeVidangeL1,	start + 29, 0);

	m_varRobotCaisses.demandeVidangeL2 =	new TypeBool(names.demandeVidangeL2,	start + 31, 0);


	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 33, 0);
	m_varRobotCaisses.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 35, 0);
	m_varRobotCaisses.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 36, 0);
	m_varRobotCaisses.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 37, 0);
	m_varRobotCaisses.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 38, 0);
	m_varCommun.mdvSPV =			new TypeAna(names.mdvSPV, start + 39, 0);
	m_varCommun.mdvAPI =			new TypeAna(names.mdvAPI, start + 40, 0);
	m_varCommun.timeoutComAPI =		new TypeBool(names.timeoutComAPI, start + 41, 0);
	m_varCommun.timeoutMarcheAPI =	new TypeBool(names.timeoutMarcheAPI, start + 42, 0);

	m_varRobotCaisses.topBacVerseL1 = new TypeBool(names.topBacVerseL1, start + 43, 0);
	m_varRobotCaisses.topBacVerseL2 = new TypeBool(names.topBacVerseL2, start + 44, 0);

	m_varRobotCaisses.HUcontIDL1 = new TypeAna(names.HUcontIDL1, start + 45, 0);
	m_varRobotCaisses.HUbatchIDL1 = new TypeString(names.HUbatchIDL1, start + 46, "");
	m_varRobotCaisses.HUIDL1 = new TypeString(names.HUIDL1, start + 47, "");
	m_varRobotCaisses.HUqtyL1 = new TypeAna(names.HUqtyL1, start + 48, 0);
	m_varRobotCaisses.HUcontIDL2 = new TypeAna(names.HUcontIDL2, start + 49, 0);
	m_varRobotCaisses.HUbatchIDL2 = new TypeString(names.HUbatchIDL2, start + 50, "");
	m_varRobotCaisses.HUIDL2 = new TypeString(names.HUIDL2, start + 51, 0);
	m_varRobotCaisses.HUqtyL2 = new TypeAna(names.HUqtyL2, start + 52, 0);

	m_varRobotCaisses.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 53, 0);
	m_varRobotCaisses.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 54, 0);

	m_varRobotCaisses.ackBacVerseL1 = new TypeBool(names.ackBacVerseL1, start + 55, 0);
	m_varRobotCaisses.ackBacVerseL2 = new TypeBool(names.ackBacVerseL2, start + 56, 0);
	m_varRobotCaisses.CampagneL1 = new TypeBool(names.CampagneL1, start + 57, 0); // Ajout 19/03/2020
	m_varRobotCaisses.CampagneL2 = new TypeBool(names.CampagneL2, start + 58, 0); // Ajout 19/03/2020
	m_varRobotCaisses.DemandeArticleL1 = new TypeBool(names.DemandeArticleL1, start + 59, 0); // Ajout 24/03/2020
	m_varRobotCaisses.DemandeArticleL2 = new TypeBool(names.DemandeArticleL2, start + 60, 0); // Ajout 24/03/2020

	//IDmachine = ++ID;
	IDmachine = 0;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);
	this->cptTimeoutConfig = 0;
}

bool RobotCaisses::GestionProprete(int ligne) {
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varRobotCaisses.demandeMarche->WriteVar(true);

	//Pour ligne 1
	if (ligne == 1) {
		//Si pas de demande, faire une demande
		if (!this->m_varRobotCaisses.demandePropreteL1->GetVarValue()) {
			LOG_INFO << "Demande de propreté Robot Syleps Ligne 1";
			this->m_varRobotCaisses.demandePropreteL1->WriteVar(true);
		}
		if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_PROPRETE || this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION || this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		
	}
	if (ligne == 2) {
		//Si pas de demande, faire une demande
		if (!this->m_varRobotCaisses.demandePropreteL2->GetVarValue()) {
			LOG_INFO << "Demande de propreté Robot Syleps Ligne 2";
			this->m_varRobotCaisses.demandePropreteL2->WriteVar(true);
		}
		if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_PROPRETE || this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION || this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		
	}
	return retour;
}

bool RobotCaisses::GestionConfig(int ligne){
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varRobotCaisses.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varRobotCaisses.timeoutConfigL1->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varRobotCaisses.demandeConfigL1->GetVarValue()) {
				this->m_varRobotCaisses.codeArticleL1->WriteVar(vFlatSetup.at(ligne - 1).refPieces);
				
				//Demande effectuée
				this->m_varRobotCaisses.demandeConfigL1->WriteVar(true);
			}//Sinon, incrémenter le compteur 
			else { 
				cptTimeoutConfig++; 
			}
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varRobotCaisses.demandeConfigL1->WriteVar(false);
			this->m_varRobotCaisses.timeoutConfigL1->WriteVar(true);
		}
	}
	else {
		//Si déja configuré
		if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varRobotCaisses.timeoutConfigL2->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varRobotCaisses.demandeConfigL2->GetVarValue()) {
				this->m_varRobotCaisses.codeArticleL2->WriteVar(vFlatSetup.at(ligne - 1).refPieces);
				//Demande effectuée
				this->m_varRobotCaisses.demandeConfigL2->WriteVar(true);
			}//Sinon, incrémenter le compteur 
			else { 
				cptTimeoutConfig++; 
			}
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			this->m_varRobotCaisses.demandeConfigL2->WriteVar(false);
			this->m_varRobotCaisses.timeoutConfigL2->WriteVar(true);
		}
	}
	return retour;
}

bool RobotCaisses::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	if (ligne == 1) {
		if (ValidTare) {
			//Si déja en marche
			if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varRobotCaisses.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varRobotCaisses.demandeVidangeL1->GetVarValue()) {
					this->m_varRobotCaisses.demandeVidangeL1->WriteVar(true);
				}

				//test machine
				if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
					this->m_varRobotCaisses.demandeVidangeL1->WriteVar(false);
					retour = true;
				}
			}
		}
		else {
			retour = true;
		}
	}
	else {
		if (ValidTare) {
			//Si déja en marche
			if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varRobotCaisses.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varRobotCaisses.demandeVidangeL2->GetVarValue()) {
					this->m_varRobotCaisses.demandeVidangeL2->WriteVar(true);
				}

				//test machine
				if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
					this->m_varRobotCaisses.demandeVidangeL2->WriteVar(false);
					retour = true;
				}
			}
		}
		else {
			retour = true;
		}
	}
	return retour;
}

bool RobotCaisses::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varRobotCaisses.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool RobotCaisses::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varRobotCaisses.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool RobotCaisses::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.demandeConfigL1->WriteVar(false);
		this->m_varRobotCaisses.timeoutConfigL1->WriteVar(false);
	}
	else {
		this->m_varRobotCaisses.demandeConfigL2->WriteVar(false);
		this->m_varRobotCaisses.timeoutConfigL2->WriteVar(false);
	}
	
	return 1;
}

bool RobotCaisses::SetMotEtat() {

	/*//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		(this->m_varRobotCaisses.timeoutConfigL1->GetVarValue() || this->m_varRobotCaisses.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varRobotCaisses.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varRobotCaisses.demandePropreteL1->GetVarValue(),
		this->m_varRobotCaisses.demandeConfigL1->GetVarValue(),
		this->m_varRobotCaisses.demandeVidangeL1->GetVarValue(),
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varRobotCaisses.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varRobotCaisses.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varRobotCaisses.demandePropreteL2->GetVarValue(),
		this->m_varRobotCaisses.demandeConfigL2->GetVarValue(),
		this->m_varRobotCaisses.demandeVidangeL2->GetVarValue(),
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varRobotCaisses.etatAPIL2->GetVarValue())
	);

	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varRobotCaisses.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		(this->m_varRobotCaisses.timeoutConfigL1->GetVarValue() || this->m_varRobotCaisses.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varRobotCaisses.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varRobotCaisses.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		(this->m_varRobotCaisses.timeoutConfigL1->GetVarValue() || this->m_varRobotCaisses.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varRobotCaisses.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			this->m_varCommun.alarmeNonBloquante->GetVarValue(),
			0,
			0,
			0,
			0,
			(this->m_varRobotCaisses.timeoutConfigL1->GetVarValue() || this->m_varRobotCaisses.timeoutConfigL2->GetVarValue()),
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

int RobotCaisses::GetAdvNtopBac(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.topBacVerseL1->GetAdviseNumber();
	}
	else {
		return this->m_varRobotCaisses.topBacVerseL2->GetAdviseNumber();
	}
}

std::string RobotCaisses::GetLineHU(int ligne) {
	std::string str = "";
	
	if (ligne == 1) {
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUcontIDL1->GetVarValue()) ;
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUbatchIDL1->GetVarValue());
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUIDL1->GetVarValue());
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUqtyL1->GetVarValue());
		str += ";";
	}
	else {
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUcontIDL2->GetVarValue());
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUbatchIDL2->GetVarValue());
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUIDL2->GetVarValue());
		str += ";";
		str += boost::lexical_cast<std::string>(m_varRobotCaisses.HUqtyL2->GetVarValue());
		str += ";";
	}
	str += "\n";
	return str;
}

double RobotCaisses::GetHUcontID(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.HUcontIDL1->GetVarValue();
	}
	else {
		return this->m_varRobotCaisses.HUcontIDL2->GetVarValue();
	}
}

CString RobotCaisses::GetbatchID(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.HUbatchIDL1->GetVarValue();
	}
	else {
		return this->m_varRobotCaisses.HUbatchIDL2->GetVarValue();
	}
}

CString RobotCaisses::GetHUID(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.HUIDL1->GetVarValue();
	}
	else {
		return this->m_varRobotCaisses.HUIDL2->GetVarValue();
	}
}

double RobotCaisses::GetHUqty(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.HUqtyL1->GetVarValue();
	}
	else {
		return this->m_varRobotCaisses.HUqtyL2->GetVarValue();
	}
}

bool RobotCaisses::ResetTopBacVerse(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.topBacVerseL1->WriteVar(false);
	}
	else
	{
		this->m_varRobotCaisses.topBacVerseL2->WriteVar(false);
	}
	return true;
}

bool RobotCaisses::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1){
		//Si déja en marche
		if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varRobotCaisses.demandePropreteL1->WriteVar(false);
			this->m_varRobotCaisses.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varRobotCaisses.demandeMarcheL1->GetVarValue()) {
				this->m_varRobotCaisses.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varRobotCaisses.demandePropreteL1->WriteVar(false);
				this->m_varRobotCaisses.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varRobotCaisses.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varRobotCaisses.demandePropreteL2->WriteVar(false);
				this->m_varRobotCaisses.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varRobotCaisses.demandeMarcheL2->GetVarValue()) {
					this->m_varRobotCaisses.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varRobotCaisses.demandePropreteL2->WriteVar(false);
					this->m_varRobotCaisses.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varRobotCaisses.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool RobotCaisses::EnvoiDmeArret(int ligne) {
	/*
	Ajout 19/03/2020
	*/
	bool retour = false;
	if(ligne == 1){
		//Arret cellule
		if (!this->m_varRobotCaisses.CampagneL2->GetVarValue()) {
			this->m_varRobotCaisses.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varRobotCaisses.demandeMarcheL1->GetVarValue()) {
			this->m_varRobotCaisses.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varRobotCaisses.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
	}
	else {
		//Arret cellule
		if (!this->m_varRobotCaisses.CampagneL1->GetVarValue()) {
			this->m_varRobotCaisses.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varRobotCaisses.demandeMarcheL2->GetVarValue()) {
			this->m_varRobotCaisses.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varRobotCaisses.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}

	}
	return retour;
}

bool RobotCaisses::EnvoiMarcheCellule()
{
	this->m_varRobotCaisses.demandeMarche->WriteVar(true);
	return true;
}

bool RobotCaisses::SetAckBacVerse(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.ackBacVerseL1->WriteVar(true);
	}
	else
	{
		this->m_varRobotCaisses.ackBacVerseL2->WriteVar(true);
	}
	return true;
}

bool RobotCaisses::ResetAckBacVerse(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCaisses.ackBacVerseL1->WriteVar(false);
	}
	else
	{
		this->m_varRobotCaisses.ackBacVerseL2->WriteVar(false);
	}
	return true;
}

bool RobotCaisses::GetAckBacVerse(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.ackBacVerseL1->GetVarValue();
	}
	else
	{
		return this->m_varRobotCaisses.ackBacVerseL2->GetVarValue();
	}
}

bool RobotCaisses::GetTopBacVerse(int ligne)
{
	if (ligne == 1) {
		return this->m_varRobotCaisses.topBacVerseL1->GetVarValue();
	}
	else
	{
		return this->m_varRobotCaisses.topBacVerseL2->GetVarValue();
	}
}

bool RobotCaisses::GetFlagModeDistant(int ligne) {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;

	if (ligne == 1) {
		return this->FlagModeDistantRobotBacsBleus1;
	}
	else {
		return this->FlagModeDistantRobotBacsBleus2;
	}

}

bool RobotCaisses::SetFlagModeDistant(int ligne, bool val) {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;

	if (ligne == 1) {
		this->FlagModeDistantRobotBacsBleus1 = val;
	}
	else {
		this->FlagModeDistantRobotBacsBleus2 = val;
	}
	return retour;
}

double  RobotCaisses::GetEtatAPI(int ligne) {
	/*
	Ajout 26/12/19
	*/
	if (ligne == 1) {
		return this->m_varRobotCaisses.etatAPIL1->GetVarValue();
	}
	else {
		return this->m_varRobotCaisses.etatAPIL2->GetVarValue();
	}

}

bool RobotCaisses::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varRobotCaisses.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varRobotCaisses.CampagneL2->WriteVar(val);
	}
	return true;
}

bool RobotCaisses::GetDemandeArticle(int ligne){
	/*
	Ajout 24/03/2020
	*/
	if (ligne == 1) {
		return this->m_varRobotCaisses.DemandeArticleL1->GetVarValue();
	}
	else
	{
		return this->m_varRobotCaisses.DemandeArticleL2->GetVarValue();
	}
}

//--------------------------------RobotCouvercle
//Constructors

RobotCouvercle::RobotCouvercle() {};

RobotCouvercle::RobotCouvercle(sNameRobotCouvercle names, int start) {

	m_varRobotCouvercle.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varRobotCouvercle.circuitPropreteL1 = new TypeAna(names.circuitPropreteL1, start + 9, 0);
	m_varRobotCouvercle.circuitPropreteL2 = new TypeAna(names.circuitPropreteL2, start + 10, 0);
	m_varRobotCouvercle.paramPropreteL1 = new TypeAna(names.paramPropreteL1, start + 11, 0);
	m_varRobotCouvercle.paramPropreteL2 = new TypeAna(names.paramPropreteL2, start + 12, 0);
	m_varRobotCouvercle.magasinCouvercleNivBas = new TypeBool(names.magasinCouvercleNivBas, start + 13, 0);
	m_varRobotCouvercle.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 14, 0);
	m_varRobotCouvercle.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 15, 0);
	m_varRobotCouvercle.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 18, 0);
	m_varRobotCouvercle.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 20, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 22, 0);
	m_varRobotCouvercle.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 23, 0);
	m_varRobotCouvercle.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 24, 0);
	m_varRobotCouvercle.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 25, 0);
	m_varRobotCouvercle.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 26, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 27, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 28, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 29, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 30, 0);

	m_varRobotCouvercle.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 31, 0);
	m_varRobotCouvercle.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 32, 0);

	m_varRobotCouvercle.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 33, 0);
	m_varRobotCouvercle.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 34, 0);

	m_varRobotCouvercle.magEtiquetteNivBasL1 = new TypeBool(names.magEtiquetteNivBasL1, start + 35, 0);
	m_varRobotCouvercle.magEtiquetteNivBasL2 = new TypeBool(names.magEtiquetteNivBasL2, start + 36, 0);
	m_varRobotCouvercle.CampagneL1 = new TypeBool(names.CampagneL1, start + 37, 0); // Ajout 19/03/2020
	m_varRobotCouvercle.CampagneL2 = new TypeBool(names.CampagneL2, start + 38, 0); // Ajout 19/03/2020

	//IDmachine = ++ID;
	IDmachine = 1000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);
	this->cptTimeoutConfig = 0;
}

bool RobotCouvercle::GestionProprete(int ligne) {
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varRobotCouvercle.demandeMarche->WriteVar(true);

	//Pour ligne 1
	if (ligne == 1) {
		//Si pas de demande, faire une demande
		if (!this->m_varRobotCouvercle.demandePropreteL1->GetVarValue()) {
		LOG_INFO << "Demande de propreté robot couvercle ligne 1";
		this->m_varRobotCouvercle.demandePropreteL1->WriteVar(true);
	}
		if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_PROPRETE || this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION || this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
	
	}
	if (ligne == 2) {
		//Si pas de demande, faire une demande
		if (!this->m_varRobotCouvercle.demandePropreteL2->GetVarValue()) {
		LOG_INFO << "Demande de propreté robot couvercle ligne 2";
		this->m_varRobotCouvercle.demandePropreteL2->WriteVar(true);
	}
		if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_PROPRETE || this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION || this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
	}
	return retour;
}

bool RobotCouvercle::GestionConfig(int ligne){
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varRobotCouvercle.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varRobotCouvercle.demandeConfigL1->GetVarValue()) {
				//Demande effectuée
				this->m_varRobotCouvercle.demandeConfigL1->WriteVar(true);
			} 
			
		}
	}
	else {
		//Si déja configuré
		if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varRobotCouvercle.demandeConfigL2->GetVarValue()) {
				//Demande effectuée
				this->m_varRobotCouvercle.demandeConfigL2->WriteVar(true);
			}
		}
	}
	return retour;
}

bool RobotCouvercle::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varRobotCouvercle.demandeVidangeL1->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varRobotCouvercle.demandeVidangeL1->GetVarValue()) {
				this->m_varRobotCouvercle.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varRobotCouvercle.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varRobotCouvercle.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varRobotCouvercle.demandeVidangeL2->GetVarValue()) {
				this->m_varRobotCouvercle.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varRobotCouvercle.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}
	}
	return retour;
}

bool RobotCouvercle::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCouvercle.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varRobotCouvercle.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool RobotCouvercle::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCouvercle.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varRobotCouvercle.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool RobotCouvercle::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varRobotCouvercle.demandeConfigL1->WriteVar(false);
	}
	else {
		this->m_varRobotCouvercle.demandeConfigL2->WriteVar(false);
	}

	return 1;
}

bool RobotCouvercle::SetMotEtat() {

	/*//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varRobotCouvercle.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varRobotCouvercle.demandePropreteL1->GetVarValue(),
		this->m_varRobotCouvercle.demandeConfigL1->GetVarValue(),
		this->m_varRobotCouvercle.demandeVidangeL1->GetVarValue(),
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varRobotCouvercle.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varRobotCouvercle.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varRobotCouvercle.demandePropreteL2->GetVarValue(),
		this->m_varRobotCouvercle.demandeConfigL2->GetVarValue(), 
		this->m_varRobotCouvercle.demandeVidangeL2->GetVarValue(),
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varRobotCouvercle.etatAPIL2->GetVarValue())
	);

	//Si une ligne en config, mettre robot Etiquette en bleu avec prio sur la ligne 1
	if (this->m_varRobotCouvercle.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || 
			this->m_varRobotCouvercle.magasinCouvercleNivBas->GetVarValue() || 
			this->m_varRobotCouvercle.magEtiquetteNivBasL1->GetVarValue() || 
			this->m_varRobotCouvercle.magEtiquetteNivBasL2->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varRobotCouvercle.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varRobotCouvercle.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() ||
			this->m_varRobotCouvercle.magasinCouvercleNivBas->GetVarValue() ||
			this->m_varRobotCouvercle.magEtiquetteNivBasL1->GetVarValue() ||
			this->m_varRobotCouvercle.magEtiquetteNivBasL2->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varRobotCouvercle.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			(this->m_varCommun.alarmeNonBloquante->GetVarValue() ||
				this->m_varRobotCouvercle.magasinCouvercleNivBas->GetVarValue() ||
				this->m_varRobotCouvercle.magEtiquetteNivBasL1->GetVarValue() ||
				this->m_varRobotCouvercle.magEtiquetteNivBasL2->GetVarValue()),
			0,
			0,
			0,
			0,
			0,
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

bool RobotCouvercle::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varRobotCouvercle.demandePropreteL1->WriteVar(false);
			this->m_varRobotCouvercle.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varRobotCouvercle.demandeMarcheL1->GetVarValue()) {
				this->m_varRobotCouvercle.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varRobotCouvercle.demandePropreteL1->WriteVar(false);
				this->m_varRobotCouvercle.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varRobotCouvercle.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varRobotCouvercle.demandePropreteL2->WriteVar(false);
				this->m_varRobotCouvercle.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varRobotCouvercle.demandeMarcheL2->GetVarValue()) {
					this->m_varRobotCouvercle.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varRobotCouvercle.demandePropreteL2->WriteVar(false);
					this->m_varRobotCouvercle.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varRobotCouvercle.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool RobotCouvercle::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varRobotCouvercle.CampagneL2->GetVarValue()) {
			this->m_varRobotCouvercle.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varRobotCouvercle.demandeMarcheL1->GetVarValue()) {
			this->m_varRobotCouvercle.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varRobotCouvercle.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
	}
	else {
		//Arret cellule
		if (!this->m_varRobotCouvercle.CampagneL1->GetVarValue()) {
			this->m_varRobotCouvercle.demandeMarche->WriteVar(false);
		}
			
		//Si pas de demande, faire une demande
		if (this->m_varRobotCouvercle.demandeMarcheL2->GetVarValue()) {
			this->m_varRobotCouvercle.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varRobotCouvercle.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
			
		
	}
	return retour;
}

bool RobotCouvercle::EnvoiMarcheCellule()
{
	this->m_varRobotCouvercle.demandeMarche->WriteVar(true);
	return true;
}

bool RobotCouvercle::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varRobotCouvercle.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varRobotCouvercle.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------Ensacheuse
//Constructors

Ensacheuse::Ensacheuse() {};

Ensacheuse::Ensacheuse(sNameEnsacheuse names, int start) {


	m_varEnsacheuse.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varEnsacheuse.finRouleauFilm = new TypeBool(names.finRouleauFilm, start + 8, 0);
	m_varEnsacheuse.hauteurSachet = new TypeAna(names.hauteurSachet, start + 9, 0);
	m_varEnsacheuse.adjonctionEau = new TypeAna(names.adjonctionEau, start + 10, 0);
	m_varEnsacheuse.qteEauReelle = new TypeAna(names.qteEauReelle, start + 11, 0);
	m_varEnsacheuse.circuitProprete = new TypeAna(names.circuitProprete, start + 12, 0);
	m_varEnsacheuse.paramProprete = new TypeAna(names.paramProprete, start + 13, 0);
	m_varEnsacheuse.demandeProprete = new TypeBool(names.demandeProprete, start + 14, 0);
	m_varEnsacheuse.demandeConfig = new TypeBool(names.demandeConfig, start + 16, 0);
	m_varEnsacheuse.timeoutConfig = new TypeBool(names.timeoutConfig, start + 17, 0);
	m_varEnsacheuse.demandeVidange = new TypeBool(names.demandeVidange, start + 18, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 20, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 21, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 22, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 23, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 24, 0);
	m_varEnsacheuse.animationIcone = new TypeAna(names.animationIcone, start + 25, 0);
	m_varEnsacheuse.Campagne = new TypeBool(names.Campagne, start + 26, 0); // Ajout 19/03/2020

	//IDmachine = ++ID;
	IDmachine = 2000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	this->cptTimeoutConfig = 0;
}

bool Ensacheuse::GestionProprete(int ligne) {
	bool retour = false;

	//Si pas de demande, faire une demande
	if (!this->m_varEnsacheuse.demandeProprete->GetVarValue()) {
		this->m_varEnsacheuse.demandeProprete->WriteVar(true);
	}

	//test machine
	//if (this->m_varEnsacheuse.circuitProprete->GetVarValue() == this->m_varEnsacheuse.paramProprete->GetVarValue()) {
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PROPRETE || this->m_varCommun.etatAPI->GetVarValue() == ETAT_CONFIGURATION || this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		retour = true;
	}
	
	return retour;
}

bool Ensacheuse::GestionConfig(int ligne)
{
	bool retour = false;

	//Si déja configuré
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_CONFIGURATION) {
		retour = true;
		cptTimeoutConfig = 0;

		this->m_varEnsacheuse.timeoutConfig->WriteVar(false);
	}
	//Non configuré
	else {
		//Si pas de demande, faire une demande et envoyer config
		if (!this->m_varEnsacheuse.demandeConfig->GetVarValue()) {
			this->m_varEnsacheuse.adjonctionEau->WriteVar(vFlatSetup.at(ligne - 1).adjonctionEau);
			this->m_varEnsacheuse.hauteurSachet->WriteVar(vFlatSetup.at(ligne - 1).hauteurSachet);
			//Demande effectuée
			this->m_varEnsacheuse.demandeConfig->WriteVar(true);
		}//Sinon, incrémenter le compteur 
		else { cptTimeoutConfig++; }
	}
	if (cptTimeoutConfig >= TIMEOUT_CONFIG_MANUEL) {
		cptTimeoutConfig = 0;
		this->m_varEnsacheuse.demandeConfig->WriteVar(false);
		this->m_varEnsacheuse.timeoutConfig->WriteVar(true);
	}
	return retour;
}

bool Ensacheuse::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
		this->m_varEnsacheuse.demandeVidange->WriteVar(false);
		retour = true;
	}
	//Pas en vidange
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varEnsacheuse.demandeVidange->GetVarValue()) {
			this->m_varEnsacheuse.demandeVidange->WriteVar(true);
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
			this->m_varEnsacheuse.demandeVidange->WriteVar(false);
			retour = true;
		}
	}
	return retour;
}

bool Ensacheuse::ResetDemandeProprete(int ligne)
{
	
	this->m_varEnsacheuse.demandeProprete->WriteVar(false);
	
	return 1;
}

bool Ensacheuse::ResetDemandeVidange(int ligne)
{

	this->m_varEnsacheuse.demandeVidange->WriteVar(false);

	return 1;
}

bool Ensacheuse::ResetDemandeConfiguration(int ligne)
{
	
	this->m_varEnsacheuse.demandeConfig->WriteVar(false);
	this->m_varEnsacheuse.timeoutConfig->WriteVar(false);
	return 1;
}

bool Ensacheuse::SetMotEtat() {

	//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varEnsacheuse.finRouleauFilm->GetVarValue()),
		this->m_varEnsacheuse.demandeProprete->GetVarValue(),
		this->m_varEnsacheuse.demandeConfig->GetVarValue(),
		this->m_varEnsacheuse.demandeVidange->GetVarValue(),
		0,
		this->m_varEnsacheuse.timeoutConfig->GetVarValue(),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);

	//Icone
	this->m_varEnsacheuse.animationIcone->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varEnsacheuse.demandeProprete->GetVarValue(),
		this->m_varEnsacheuse.demandeConfig->GetVarValue(),
		this->m_varEnsacheuse.demandeVidange->GetVarValue(),
		this->machineSortie_L1 || this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varCommun.etatAPI->GetVarValue())
	);

	return true;
}

int Ensacheuse::GetQteEauRAdviseN(){
	return m_varEnsacheuse.qteEauReelle->GetAdviseNumber();
}

bool Ensacheuse::EnvoiDemMarche(int ligne) {
	bool retour = false;

	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		this->m_varEnsacheuse.demandeProprete->WriteVar(false);
		this->m_varEnsacheuse.demandeConfig->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	//Pas en marche
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varEnsacheuse.demandeMarche->GetVarValue()) {
			this->m_varEnsacheuse.demandeMarche->WriteVar(true);
		}
		else {
			cptTimeoutMarche++;
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varEnsacheuse.demandeProprete->WriteVar(false);
			this->m_varEnsacheuse.demandeConfig->WriteVar(false);
			retour = true;
		}
	}
	if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
		cptTimeoutMarche = 0;
		this->m_varEnsacheuse.demandeMarche->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
	}
	return retour;
}

bool Ensacheuse::EnvoiDmeArret(int ligne) {
	bool retour = false;

	
	//Si pas de demande, faire une demande
	if (this->m_varEnsacheuse.demandeMarche->GetVarValue()) {
		this->m_varEnsacheuse.demandeMarche->WriteVar(false);
	}

	//test machine
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_ARRET) {
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	
	return retour;
}

bool Ensacheuse::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	
	this->m_varEnsacheuse.Campagne->WriteVar(val);
	
	return true;
}


//--------------------------------Formeuse
//Constructors

Formeuse::Formeuse() {};

Formeuse::Formeuse(sNameFormeuse names, int start) {


	m_varFormeuse.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

	//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varFormeuse.typeContenant = new TypeString(names.typeContenant, start + 9, 0);
	m_varFormeuse.typeContenantPre = new TypeString(names.typeContenantPre, start + 10, 0);
	m_varFormeuse.circuitProprete = new TypeAna(names.circuitProprete, start + 11, 0);
	m_varFormeuse.paramProprete = new TypeAna(names.paramProprete, start + 12, 0);
	m_varFormeuse.demandeProprete = new TypeBool(names.demandeProprete, start + 13, 0);
	m_varFormeuse.demandeConfig = new TypeBool(names.demandeConfig, start + 14, 0);
	m_varFormeuse.timeoutConfig = new TypeBool(names.timeoutConfig, start + 15, 0);
	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 16, 0);
	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 17, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 18, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 19, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 20, 0);
	m_varFormeuse.demandeVidange = new TypeBool(names.demandeVidange, start + 21, 0);
	m_varFormeuse.magasinCartonNivBas = new TypeBool(names.magasinCartonNivBas, start + 22, 0);
	m_varFormeuse.magasinAdhesifNivBas = new TypeBool(names.magasinAdhesifNivBas, start + 23, 0);
	m_varFormeuse.animationIcone = new TypeAna(names.animationIcone, start + 24, 0);
	m_varFormeuse.CampagneL1 = new TypeBool(names.CampagneL1, start + 25, 0); // Ajout 27/03/2020
	m_varFormeuse.CampagneL2 = new TypeBool(names.CampagneL2, start + 25, 0); // Ajout 27/03/2020
	//IDmachine = ++ID;
	IDmachine = 9000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	this->cptTimeoutConfig = 0;
}

bool Formeuse::GestionProprete(int ligne) {
	/*
	Modif
	*/
	bool retour = false;

	//Si pas de demande, faire une demande
	if (!this->m_varFormeuse.demandeProprete->GetVarValue()) {
		this->m_varFormeuse.demandeProprete->WriteVar(true);
	}
	//test machine
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PROPRETE || this->m_varCommun.etatAPI->GetVarValue() == ETAT_CONFIGURATION || this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		retour = true;
	}

	return retour;
}

bool Formeuse::GestionConfig(int ligne) {
	/*
	Modif
	*/
	bool retour = false;

	//Si déja configuré
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_CONFIGURATION) {
		retour = true;
		cptTimeoutConfig = 0;
		this->m_varFormeuse.timeoutConfig->WriteVar(false);
	}
	//Non configuré
	else {
		//Si pas de demande, faire une demande et envoyer config
		if (!this->m_varFormeuse.demandeConfig->GetVarValue()) {
			this->m_varFormeuse.typeContenant->WriteVar(vFlatSetup.at(ligne - 1).typeConditionnement);
			//Demande effectuée
			this->m_varFormeuse.demandeConfig->WriteVar(true);
		}//Sinon, incrémenter le compteur 
		else { cptTimeoutConfig++; }
	}
	if (cptTimeoutConfig >= TIMEOUT_CONFIG_MANUEL) {
		cptTimeoutConfig = 0;
		this->m_varFormeuse.timeoutConfig->WriteVar(true);
	}
	return retour;
}

bool Formeuse::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
		this->m_varFormeuse.demandeVidange->WriteVar(false);
		retour = true;
	}
	//Pas en vidange
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varFormeuse.demandeVidange->GetVarValue()) {
			this->m_varFormeuse.demandeVidange->WriteVar(true);
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
			this->m_varFormeuse.demandeVidange->WriteVar(false);
			retour = true;
		}
	}

	return retour;
}

bool Formeuse::ResetDemandeProprete(int ligne)
{

	this->m_varFormeuse.demandeProprete->WriteVar(false);
	this->m_varFormeuse.circuitProprete->WriteVar(false);

	return 1;
}

bool Formeuse::ResetDemandeVidange(int ligne) {
	this->m_varFormeuse.demandeVidange->WriteVar(false);

	return 1;
}

bool Formeuse::ResetDemandeConfiguration(int ligne)
{

	this->m_varFormeuse.demandeConfig->WriteVar(false);
	this->m_varFormeuse.timeoutConfig->WriteVar(false);
	return 1;
}

bool Formeuse::IsSamePackaging(CString cont) {
	bool retour = false;
	if (cont == this->m_varFormeuse.typeContenantPre->GetVarValue()) {
		retour = true;
	}
	return retour;
}

bool Formeuse::IsDispo() {
	/*
	Modif 06/12/19
	*/
	bool retour = true;
	//Si alarme ou déja en prod = pas dispo
	if (this->m_varCommun.demandeDistant->GetVarValue() == false
		|| this->m_varFormeuse.demandeMarche->GetVarValue() == true) {
		retour = false;
		/*
		this->m_varCommun.alarmeBloquante->GetVarValue()
		|| this->m_varFormeuse.demandeConfig->GetVarValue() == true
		|| this->m_varFormeuse.demandeProprete->GetVarValue() == true
		|| this->m_varCommun.timeoutComAPI->GetVarValue() == true
		*/
	}

	return retour;
}

bool Formeuse::SetMotEtat() {

	//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varFormeuse.magasinAdhesifNivBas->GetVarValue() || this->m_varFormeuse.magasinCartonNivBas->GetVarValue()),
		this->m_varFormeuse.demandeProprete->GetVarValue(),
		this->m_varFormeuse.demandeConfig->GetVarValue(),
		this->m_varFormeuse.demandeVidange->GetVarValue(),
		0,
		this->m_varFormeuse.timeoutConfig->GetVarValue(),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);

	//Icone
	this->m_varFormeuse.animationIcone->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varFormeuse.demandeProprete->GetVarValue(),
		this->m_varFormeuse.demandeConfig->GetVarValue(),
		this->m_varFormeuse.demandeVidange->GetVarValue(),
		this->machineSortie_L1 || this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varCommun.etatAPI->GetVarValue())
	);

	return true;
}

bool Formeuse::EnvoiDemMarche(int ligne) {
	bool retour = false;

	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		this->m_varFormeuse.demandeProprete->WriteVar(false);
		this->m_varFormeuse.demandeConfig->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	//Pas en marche
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varFormeuse.demandeMarche->GetVarValue()) {
			this->m_varFormeuse.demandeMarche->WriteVar(true);
		}
		else {
			cptTimeoutMarche++;
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varFormeuse.demandeProprete->WriteVar(false);
			this->m_varFormeuse.demandeConfig->WriteVar(false);
			retour = true;
		}
	}
	if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
		cptTimeoutMarche = 0;
		this->m_varFormeuse.demandeMarche->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
	}
	return retour;
}

bool Formeuse::EnvoiDmeArret(int ligne) {
	bool retour = false;


	//Si pas de demande, faire une demande
	if (this->m_varFormeuse.demandeMarche->GetVarValue()) {
		this->m_varFormeuse.demandeMarche->WriteVar(false);
	}

	//test machine
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_ARRET) {
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}

	return retour;
}

bool Formeuse::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	Modif 27/03/2020
	*/
	if (ligne == 1) {
		this->m_varFormeuse.CampagneL1->WriteVar(val);
	}
	else {
		this->m_varFormeuse.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------Peseuse
//Constructors

Peseuse::Peseuse() {};

Peseuse::Peseuse(sNamePeseuse names, int start) {


	m_varPeseuse.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varPeseuse.circuitProprete = new TypeAna(names.circuitProprete, start + 8, 0);
	m_varPeseuse.paramProprete = new TypeAna(names.paramProprete, start + 9, 0);
	m_varPeseuse.demandeProprete = new TypeBool(names.demandeProprete, start + 10, 0);
	m_varPeseuse.demandeConfig = new TypeBool(names.demandeConfig, start + 12, 0);
	m_varPeseuse.cheminFichier = new TypeString(names.cheminFichier, start + 13, 0);
	m_varPeseuse.timeoutConfig = new TypeBool(names.timeoutConfig, start + 14, 0);
	m_varPeseuse.demandeVidange = new TypeBool(names.demandeVidange, start + 15, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 17, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 18, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 19, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 20, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 21, 0);

	m_varPeseuse.animationIcone = new TypeAna(names.animationIcone, start + 22, 0);
	m_varPeseuse.coherenceSetup = new TypeBool(names.coherenceSetup, start + 22, 0);
	m_varPeseuse.Campagne = new TypeBool(names.Campagne, start + 23, 0); // Ajout 19/03/2020

	IDmachine = ++ID;
	IDmachine = 3000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	this->cptTimeoutConfig = 0;
	this->NBRCreationFichier = 0; //Ajout 20/19/19
}

bool Peseuse::GestionProprete(int ligne) {
	/*
	Modif 20/12/19
	*/
	bool retour = false;

	//Si pas de demande, faire une demande
	if (!this->m_varPeseuse.demandeProprete->GetVarValue()) {
		LOG_INFO << "Demande de propreté peseuse ligne " << ligne << "";
		this->m_varPeseuse.demandeProprete->WriteVar(true);
		this->NBRCreationFichier = 0;
		//Alarme coherence setup
		this->m_varPeseuse.coherenceSetup->WriteVar(false);
	}
	//test machine
	//if (this->m_varPeseuse.circuitProprete->GetVarValue() == this->m_varPeseuse.paramProprete->GetVarValue()) {
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PROPRETE || this->m_varCommun.etatAPI->GetVarValue() == ETAT_CONFIGURATION || this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		retour = true;
	}
		
	return retour;
}

bool Peseuse::GestionConfig(int ligne)
{
	/*
	Modif 20/12/19
	*/
	bool retour = false;
	CString chemin = this->m_varPeseuse.cheminFichier->GetVarValue();

	//Effectuer la demande
	this->m_varPeseuse.demandeConfig->WriteVar(true);

	//Si tare ok, lancer la procédure de config
	if (sVarEqp.APIbalance->GetValidTarre(ligne)) {
		//Tant que pas état configuration, envoyer le fichier
		if (this->m_varCommun.etatAPI->GetVarValue() != ETAT_CONFIGURATION) {
			if (NBRCreationFichier <= 10) {
				//Supprimer le fichier existant
				remove(chemin);
				LOG_INFO << chemin << " deleted.";
				//Ecrire le fichier
				this->NBRCreationFichier++;
				this->EnvoiPoids(sVarEqp.APIbalance->GetPoidsTarre(ligne), ligne);
				
				//Tester si le fichier existe
				if (boost::filesystem::exists((LPCTSTR)chemin))
				{
					this->m_varCommun.etatAPI->WriteVar(ETAT_CONFIGURATION);
					retour = true;
					cptTimeoutConfig = 0;
					
					this->m_varPeseuse.timeoutConfig->WriteVar(false);
				}
				else {
					cptTimeoutConfig++;
				}
			}
			else {
				LOG_INFO << "Erreur coherence setup ligne " << ligne;
				//Alarme coherence setup
				this->m_varPeseuse.coherenceSetup->WriteVar(true);
			}
		}
		//Si équipement configuré
		else {
			retour = true;
			cptTimeoutConfig = 0;
		}
	}
	if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
		cptTimeoutConfig = 0;
		this->m_varPeseuse.demandeConfig->WriteVar(false);
		this->m_varPeseuse.timeoutConfig->WriteVar(true);
	}
	
	return retour;
}

bool Peseuse::EnvoiPoids(double poids, int ligne)
{
	LOG_INFO << "Création fichier";
	bool result = false;
	CString sRefPiece = ""; 
	sRefPiece.Format(_T("%u"), (int)(vFlatSetup.at(ligne - 1).refPieces));
	//Ajout de l'espace avant les 3 premiers caracteres
	std::string str = ((LPCTSTR)sRefPiece);
	str.insert((str.length() -3), 1,' ');
	sRefPiece = str.c_str();
	CString sPoidsPiece = ""; sPoidsPiece.Format(_T("%f"), poids);
	/*CString sPoidsNom = "";
	CString sPoidsMax = "";*/
	//En nb pieces
	CString sNbPiece = ""; 
	CString sNbNom = "";
	CString sNbMax = "";
	CString sSachetParCart = "";
	CString sBatchNum = ""; sBatchNum = vFlatSetup.at(ligne - 1).numLot;
	CString sNumOF = ""; sNumOF.Format(_T("%u"), (int)vFlatSetup.at(ligne - 1).numOF);
	CString sSachOuCart = "";
	CString sTypeCtn = "";
	//Si sachet
	if (vFlatSetup.at(ligne - 1).nbPiecesPSachet > 0) {
		sTypeCtn = "0";
		//On met quand meme le type de contenant car la doc Bilwinco semble fausse. 03/10/2019
		sTypeCtn = vFlatSetup.at(ligne - 1).typeConditionnement;
		sSachetParCart.Format(_T("%u"), (int)(vFlatSetup.at(ligne - 1).nbPiecesPContenant / vFlatSetup.at(ligne - 1).nbPiecesPSachet));
		LOG_DEBUG << "nbPiecesPContenant / nbPiecesPSachet = " << vFlatSetup.at(ligne - 1).nbPiecesPContenant << "/" << vFlatSetup.at(ligne - 1).nbPiecesPSachet << " = " << sSachetParCart;
		sSachOuCart = "1";
		/*sPoidsNom.Format(_T("%f"), poids * vFlatSetup.at(ligne - 1).nbPiecesPSachet);
		sPoidsMax.Format(_T("%f"), (poids * vFlatSetup.at(ligne - 1).nbPiecesPSachet) * 1.01);*/
		sNbPiece.Format(_T("%u"), (int)vFlatSetup.at(ligne - 1).nbPiecesPSachet);
		sNbNom = sNbPiece;
		sNbMax.Format(_T("%u"), ((int)(vFlatSetup.at(ligne - 1).nbPiecesPSachet * 1.01)));
		
	}
	//Si vrac
	else {
		sTypeCtn = vFlatSetup.at(ligne - 1).typeConditionnement;
		sSachetParCart = "0";
		sSachOuCart = "0";
		/*sPoidsNom.Format(_T("%f"), poids * vFlatSetup.at(ligne - 1).nbPiecesPContenant);
		sPoidsMax.Format(_T("%f"), (poids * vFlatSetup.at(ligne - 1).nbPiecesPContenant) * 1.01);*/
		sNbPiece.Format(_T("%u"), (int)vFlatSetup.at(ligne - 1).nbPiecesPContenant);
		sNbNom = sNbPiece;
		sNbMax.Format(_T("%u"), ((int)(vFlatSetup.at(ligne - 1).nbPiecesPContenant * 1.01)));
	}
	
	CString contenu =
		//1) I   = Item Number (10 symbols, numeric) = product name/number ( reverence on 6 char+ space + variant on 3 char)
		sRefPiece + ";" +
		//2) W   = Weight of item (8 symbols, numeric).
		sPoidsPiece + ";" +
		//3) T   = Type box / Bag (5 symbols, alphanumeric), 0 = bag, otherwise carton.
		sTypeCtn + ";" +
		// 4) P   = Pieces of item  (5 symbols, numeric) = nominal weight of pieces in box / Bag.
		//sPoidsNom + ";" +
		sNbPiece + ";" +
		//5) P   = Pieces of item in box / Bag (5 symbols, numeric) = minimal weight of pieces in box / Bag.
		//sPoidsNom + ";" +
		sNbNom + ";" +
		//6) P = Pieces of item in box / Bag(5 symbols, numeric) = maximal weight of pieces in box / Bag.
		//sPoidsMax + ";" +
		sNbMax + ";" +
		//7) B   = Batch Number (16 symbols, alphanumeric).
		sBatchNum + ";" +
		//8) O = Order Number(10 symbols, numeric).
		sNumOF + ";" +
		//9) Label  Y / N(1 / 0).value is always 0 on machine 202.
		//10) on machine 202 : Quantity of unload cycles
		//on machine 201 : partial unload Y / N(1 / 0)
		//11) Box vibration Y / N(1 / 0)
		"0;1;0;" +
		//12) N = Number of Bags in a box(2 symbols, numeric)
		sSachetParCart + ";" +
		//13) Bag / Carton(1 / 0)
		sSachOuCart;

	CString chemin = this->m_varPeseuse.cheminFichier->GetVarValue();
	LOG_INFO << "Chemin fichier : " << chemin << "";
	LOG_INFO << "Contenu : " << contenu << "\n";

	//Créer, remplir et enregistrer
	std::ofstream outfile(chemin);
	outfile << contenu << std::endl;
	outfile.close();

	return result;
}

bool Peseuse::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
		this->m_varPeseuse.demandeVidange->WriteVar(false);
		retour = true;
	}
	//Pas en vidange
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varPeseuse.demandeVidange->GetVarValue()) {
			this->m_varPeseuse.demandeVidange->WriteVar(true);
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() >= ETAT_VIDE) {
			this->m_varPeseuse.demandeVidange->WriteVar(false);
			retour = true;
		}
	}
	return retour;
}

bool Peseuse::ResetDemandeProprete(int ligne)
{
	
	this->m_varPeseuse.demandeProprete->WriteVar(false);

	return 1;
}

bool Peseuse::ResetDemandeVidange(int ligne)
{
	
	this->m_varPeseuse.demandeVidange->WriteVar(false);

	return 1;
}

bool Peseuse::ResetDemandeConfiguration(int ligne)
{

	this->m_varPeseuse.demandeConfig->WriteVar(false);
	this->m_varPeseuse.timeoutConfig->WriteVar(false);
	return 1;
}

bool Peseuse::SetMotEtat() {

	//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		this->m_varPeseuse.demandeProprete->GetVarValue(),
		this->m_varPeseuse.demandeConfig->GetVarValue(),
		this->m_varPeseuse.demandeVidange->GetVarValue(),
		0,
		this->m_varPeseuse.timeoutConfig->GetVarValue(),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);

	//Icone
	this->m_varPeseuse.animationIcone->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varPeseuse.demandeProprete->GetVarValue(),
		this->m_varPeseuse.demandeConfig->GetVarValue(),
		this->m_varPeseuse.demandeVidange->GetVarValue(),
		this->machineSortie_L1 || this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varCommun.etatAPI->GetVarValue())
	);

	return true;
}

bool Peseuse::EnvoiDemMarche(int ligne) {
	bool retour = false;

	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		this->m_varPeseuse.demandeProprete->WriteVar(false);
		this->m_varPeseuse.demandeConfig->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	//Pas en marche
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varPeseuse.demandeMarche->GetVarValue()) {
			this->m_varPeseuse.demandeMarche->WriteVar(true);
		}
		else {
			cptTimeoutMarche++;
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varPeseuse.demandeProprete->WriteVar(false);
			this->m_varPeseuse.demandeConfig->WriteVar(false);
			retour = true;
		}
	}
	if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
		cptTimeoutMarche = 0;
		this->m_varPeseuse.demandeMarche->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
	}
	return retour;
}

bool Peseuse::EnvoiDmeArret(int ligne) {
	bool retour = false;

	
	//Si pas de demande, faire une demande
	if (this->m_varPeseuse.demandeMarche->GetVarValue()) {
		this->m_varPeseuse.demandeMarche->WriteVar(false);
	}

	//test machine
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_ARRET) {
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	
	return retour;
}

bool Peseuse::ResetCoherenceSetup() {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;

	this->m_varPeseuse.coherenceSetup->WriteVar(false);

	return retour;
}

bool Peseuse::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/

	this->m_varPeseuse.Campagne->WriteVar(val);

	return true;
}

//--------------------------------Transitique
//Constructors

Transitique::Transitique() {};

Transitique::Transitique(sNameTrans names, int start) {

	m_varTrans.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varTrans.typeContenantL1 = new TypeString(names.typeContenantL1, start + 9, "");
	m_varTrans.typeContenantL2 = new TypeString(names.typeContenantL2, start + 10, "");
	m_varTrans.circuitPropreteL1 = new TypeAna(names.circuitPropreteL1, start + 11, 0);
	m_varTrans.circuitPropreteL2 = new TypeAna(names.circuitPropreteL2, start + 12, 0);
	m_varTrans.paramPropreteL1 = new TypeAna(names.paramPropreteL1, start + 13, 0);
	m_varTrans.paramPropreteL2 = new TypeAna(names.paramPropreteL2, start + 14, 0);
	m_varTrans.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 15, 0);
	m_varTrans.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 16, 0);
	m_varTrans.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 19, 0);
	m_varTrans.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 20, 0);
	m_varTrans.timeoutConfigL1 = new TypeBool(names.timeoutConfigL1, start + 21, 0);
	m_varTrans.timeoutConfigL2 = new TypeBool(names.timeoutConfigL2, start + 22, 0);
	m_varTrans.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 23, 0);
	m_varTrans.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 25, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 27, 0);
	m_varTrans.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 29, 0);
	m_varTrans.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 30, 0);
	m_varTrans.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 31, 0);
	m_varTrans.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 32, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 33, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 34, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 35, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 36, 0);

	m_varTrans.ligneF1 = new TypeAna(names.ligneF1, start + 37, 0);
	m_varTrans.ligneF2 = new TypeAna(names.ligneF2, start + 38, 0);
	m_varTrans.numOFL1 = new TypeAna(names.numOFL1, start + 39, 0);
	m_varTrans.numOFL2 = new TypeAna(names.numOFL2, start + 40, 0);

	m_varTrans.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 41, 0);
	m_varTrans.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 42, 0);

	m_varTrans.vracSacL1 = new TypeAna(names.vracSacL1, start + 43, 0);
	m_varTrans.vracSacL2 = new TypeAna(names.vracSacL2, start + 44, 0);
	m_varTrans.qteParColisL1 = new TypeAna(names.qteParColisL1, start + 45, 0);
	m_varTrans.qteParColisL2 = new TypeAna(names.qteParColisL2, start + 46, 0);
	m_varTrans.designationL1 = new TypeString(names.designationL1, start + 47, "");
	m_varTrans.designationL2 = new TypeString(names.designationL2, start + 48, "");

	m_varTrans.numLotL1 = new TypeString(names.numLotL1, start + 49, "");
	m_varTrans.numLotL2 = new TypeString(names.numLotL2, start + 50, "");
	m_varTrans.codeArticleL1 = new TypeAna(names.codeArticleL1, start + 51, 0);
	m_varTrans.codeArticleL2 = new TypeAna(names.codeArticleL2, start + 52, 0);

	//Ajout 03/12/2019
	m_varTrans.Provenance_Piece_L1 = new TypeAna(names.Provenance_Piece_L1, start + 53, 0);

	//Ajout 06/02/2020
	m_varTrans.derogationIntroductionManuelL1 = new TypeBool(names.derogationIntroductionManuelL1, start + 54, 0);
	m_varTrans.derogationIntroductionManuelL2 = new TypeBool(names.derogationIntroductionManuelL2, start + 55, 0);
	m_varTrans.CampagneL1 = new TypeBool(names.CampagneL1, start + 56, 0); // Ajout 19/03/2020
	m_varTrans.CampagneL2 = new TypeBool(names.CampagneL2, start + 57, 0); // Ajout 19/03/2020


	IDmachine = ++ID;
	IDmachine = 4000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	this->cptTimeoutConfig = 0;
}

bool Transitique::GestionProprete(int ligne) {
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varTrans.demandeMarche->WriteVar(true);

	//Pour ligne 1
	if (ligne == 1) {
		//Si pas de demande, faire une demande
		if (!this->m_varTrans.demandePropreteL1->GetVarValue()) {
			LOG_INFO << "Demande propreté Transitique ligne " << ligne << "";
			this->m_varTrans.demandePropreteL1->WriteVar(true);
		}
		if (this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_PROPRETE || this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION || this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}	
	}
	if (ligne == 2) {
		//Si pas de demande, faire une demande
		if (!this->m_varTrans.demandePropreteL2->GetVarValue()) {
			LOG_INFO << "Demande propreté Transitique ligne " << ligne << "";
			this->m_varTrans.demandePropreteL2->WriteVar(true);
		}
		if (this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_PROPRETE || this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION) {
			retour = true;
		}
			
	}
	return retour;
}

bool Transitique::GestionConfig(int ligne) {
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varTrans.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si pas de demande, faire une demande et envoyer config
		if (!this->m_varTrans.demandeConfigL1->GetVarValue()) {
			this->m_varTrans.typeContenantL1->WriteVar(vFlatSetup.at(ligne - 1).typeConditionnement);
			this->m_varTrans.numOFL1->WriteVar(vFlatSetup.at(ligne - 1).numOF);
			this->m_varTrans.qteParColisL1->WriteVar(vFlatSetup.at(ligne - 1).nbPiecesPContenant);
			this->m_varTrans.designationL1->WriteVar(vFlatSetup.at(ligne - 1).descProduit);
			if (vFlatSetup.at(ligne - 1).nbPiecesPSachet > 0) {
				this->m_varTrans.vracSacL1->WriteVar(2);
			}
			else{ this->m_varTrans.vracSacL1->WriteVar(1); }
			this->m_varTrans.codeArticleL1->WriteVar(vFlatSetup.at(ligne - 1).refPieces);
			this->m_varTrans.numLotL1->WriteVar(vFlatSetup.at(ligne - 1).numLot);
			//Si demande de propreté pour l'équipement alors attendre que celui ci soit propre
			//Demande effectuée
			this->m_varTrans.demandeConfigL1->WriteVar(true);
		}
		//Si déja configuré
		if (this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			//this->m_varTrans.demandeConfigL1->WriteVar(false);
			this->m_varTrans.timeoutConfigL1->WriteVar(false);
		}
		//Non configuré
		else {
			//Sinon, incrémenter le compteur 
			 cptTimeoutConfig++; 
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varTrans.demandeConfigL1->WriteVar(false);
			this->m_varTrans.timeoutConfigL1->WriteVar(true);
		}
	}
	else {
		//Si pas de demande, faire une demande et envoyer config
		if (!this->m_varTrans.demandeConfigL2->GetVarValue()) {
			this->m_varTrans.typeContenantL2->WriteVar(vFlatSetup.at(ligne - 1).typeConditionnement);
			this->m_varTrans.numOFL2->WriteVar(vFlatSetup.at(ligne - 1).numOF);
			this->m_varTrans.qteParColisL2->WriteVar(vFlatSetup.at(ligne - 1).nbPiecesPContenant);
			this->m_varTrans.designationL2->WriteVar(vFlatSetup.at(ligne - 1).descProduit);
			if (vFlatSetup.at(ligne - 1).nbPiecesPSachet > 0) {
				this->m_varTrans.vracSacL2->WriteVar(2);
			}
			else { this->m_varTrans.vracSacL2->WriteVar(1); }
			this->m_varTrans.codeArticleL2->WriteVar(vFlatSetup.at(ligne - 1).refPieces);
			this->m_varTrans.numLotL2->WriteVar(vFlatSetup.at(ligne - 1).numLot);
			//Si demande de propreté pour l'équipement alors attendre que celui ci soit propre
			//Demande effectuée
			this->m_varTrans.demandeConfigL2->WriteVar(true);
		}
		//Si déja configuré
		if (this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			//this->m_varTrans.demandeConfigL1->WriteVar(false);
			this->m_varTrans.timeoutConfigL2->WriteVar(false);
		}
		//Non configuré
		else {
			//Sinon, incrémenter le compteur 
			cptTimeoutConfig++;
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varTrans.demandeConfigL2->WriteVar(false);
			this->m_varTrans.timeoutConfigL2->WriteVar(true);
		}
	}
	return retour;
}

bool Transitique::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = false;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varTrans.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varTrans.demandeVidangeL1->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varTrans.demandeVidangeL1->GetVarValue()) {
				this->m_varTrans.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varTrans.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varTrans.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varTrans.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varTrans.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varTrans.demandeVidangeL2->GetVarValue()) {
				this->m_varTrans.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varTrans.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varTrans.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}
	}
	return retour;
}

bool Transitique::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varTrans.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varTrans.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool Transitique::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varTrans.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varTrans.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool Transitique::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varTrans.demandeConfigL1->WriteVar(false);
		this->m_varTrans.timeoutConfigL1->WriteVar(false);
	}
	else {
		this->m_varTrans.demandeConfigL2->WriteVar(false);
		this->m_varTrans.timeoutConfigL2->WriteVar(false);
	}
	return 1;
}

bool Transitique::SetLigneFormeuse(int formeuse, int ligne) {
	/*Convertir FORMEUSE POUR ... vers LIGNE POUR ...
	*/
	//	-	0 = Gefbox
	//	-   1 = Formeuse 1
	//	-   2 = Formeuse 2
	//	-   3 = Alimentation en manuel
	//Si ligne = 3 affectation des 2 campagnes en meme temps

	if (ligne == 1) {
		this->m_varTrans.ligneF1->WriteVar(formeuse);
	}
	if (ligne == 2) {
		this->m_varTrans.ligneF2->WriteVar(formeuse);
	}
	if (ligne == 3) {
		this->m_varTrans.ligneF1->WriteVar(formeuse);
		this->m_varTrans.ligneF2->WriteVar(formeuse);
	}
	return true;
}

bool Transitique::SetMotEtat() {

	//Général
	/*this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		(this->m_varTrans.timeoutConfigL1->GetVarValue() || this->m_varTrans.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varTrans.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varTrans.demandePropreteL1->GetVarValue(),
		this->m_varTrans.demandeConfigL1->GetVarValue(),
		this->m_varTrans.demandeVidangeL1->GetVarValue(),
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varTrans.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varTrans.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varTrans.demandePropreteL2->GetVarValue(),
		this->m_varTrans.demandeConfigL2->GetVarValue(),
		this->m_varTrans.demandeVidangeL2->GetVarValue(),
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varTrans.etatAPIL2->GetVarValue())
	);

	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varTrans.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		(this->m_varTrans.timeoutConfigL1->GetVarValue() || this->m_varTrans.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3 ) {
			this->m_varCommun.etatAnimation->WriteVar(this->m_varTrans.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varTrans.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		(this->m_varTrans.timeoutConfigL1->GetVarValue() || this->m_varTrans.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
			this->m_varCommun.etatAnimation->WriteVar(this->m_varTrans.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			this->m_varCommun.alarmeNonBloquante->GetVarValue(),
			0,
			0,
			0,
			0,
			(this->m_varTrans.timeoutConfigL1->GetVarValue() || this->m_varTrans.timeoutConfigL2->GetVarValue()),
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

int Transitique::GetAffFormAPI(int formeuse)
{
	int retour = 0;
	if (formeuse == 1) {
		retour = (int)this->m_varTrans.ligneF1->GetVarValue();
	}
	else {
		retour = (int)this->m_varTrans.ligneF2->GetVarValue();
	}

	return retour;
}

bool Transitique::UpdateAffFormAPI(int ligne, bool valF1, bool valF2, bool gefbox) {
	int result = 0;
	if (valF1 == true) { result = 1; }
	else if (valF2 == true) { result = 2; }
	else if (gefbox == false) { result = 3; }
	if (ligne == 1) {
		this->m_varTrans.ligneF1->WriteVar(result);
	}
	else {
		this->m_varTrans.ligneF2->WriteVar(result);
	}
	return false;
}

bool Transitique::EnvoiMarcheCellule()
{
	this->m_varTrans.demandeMarche->WriteVar(true);
	return true;
}

bool Transitique::EnvoiDemMarche(int ligne) {
	bool retour = false;

	//Ligne 1
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varTrans.demandePropreteL1->WriteVar(false);
			this->m_varTrans.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varTrans.demandeMarcheL1->GetVarValue()) {
				this->m_varTrans.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varTrans.demandePropreteL1->WriteVar(false);
				this->m_varTrans.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varTrans.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	//Ligne 2
	else {
		{
			//Si déja en marche
			if (this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varTrans.demandePropreteL2->WriteVar(false);
				this->m_varTrans.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varTrans.demandeMarcheL2->GetVarValue()) {
					this->m_varTrans.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varTrans.demandePropreteL2->WriteVar(false);
					this->m_varTrans.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varTrans.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool Transitique::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varTrans.CampagneL2->GetVarValue()) {
			this->m_varTrans.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varTrans.demandeMarcheL1->GetVarValue()) {
			this->m_varTrans.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varTrans.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
		//RAZ des configs setup
		this->m_varTrans.typeContenantL1->WriteVar("");
		this->m_varTrans.numOFL1->WriteVar(0);
		this->m_varTrans.qteParColisL1->WriteVar(0);
		this->m_varTrans.designationL1->WriteVar("");
		this->m_varTrans.vracSacL1->WriteVar(0); 
		this->m_varTrans.codeArticleL1->WriteVar(0);
		this->m_varTrans.numLotL1->WriteVar("");
	}
	else {
		//Arret cellule
		if (!this->m_varTrans.CampagneL1->GetVarValue()) {
			this->m_varTrans.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varTrans.demandeMarcheL2->GetVarValue()) {
			this->m_varTrans.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varTrans.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//RAZ des configs setup
		this->m_varTrans.typeContenantL2->WriteVar("");
		this->m_varTrans.numOFL2->WriteVar(0);
		this->m_varTrans.qteParColisL2->WriteVar(0);
		this->m_varTrans.designationL2->WriteVar("");
		this->m_varTrans.vracSacL2->WriteVar(0);
		this->m_varTrans.codeArticleL2->WriteVar(0);
		this->m_varTrans.numLotL2->WriteVar("");
	}
	return retour;
}

//Recuperation de la provenance de la ligne 1
void Transitique::SetProvenance_L1(bool val) {
	/*
	Ajout 03/12/2019
	*/
	this->m_varTrans.Provenance_Piece_L1->WriteVar(val+1);
}

bool Transitique::GetDerogationIntroManu(int ligne) {
	/*
	Ajout 06/02/2020
	*/
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varTrans.derogationIntroductionManuelL1->GetVarValue();
	}
	else {
		retour = this->m_varTrans.derogationIntroductionManuelL2->GetVarValue();
	}
	return retour;
}

void Transitique::SetDerogationIntroManu(int ligne, bool val) {
	/*
	Ajout 06/02/2020
	*/
	if (ligne == 1) {
		this->m_varTrans.derogationIntroductionManuelL1->WriteVar(val);
	}
	else {
		this->m_varTrans.derogationIntroductionManuelL2->WriteVar(val);
	}
}

int Transitique::GetLigneFormeuse(int ligne) {
	/*
	Ajout 06/02/2020
	*/
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varTrans.ligneF1->GetVarValue();
	}
	else {
		retour = this->m_varTrans.ligneF2->GetVarValue();
	}
	return retour;
}

bool Transitique::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varTrans.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varTrans.CampagneL2->WriteVar(val);
	}
	return true;
}


//-----------------Balance------------------------

Balance::Balance() {};

Balance::Balance(sNameBal names, int start) {

	m_varBal.validationTarreL1 = new TypeBool(names.validationTarreL1, start + 0, 0);
	m_varBal.validationTarreL2 = new TypeBool(names.validationTarreL2, start + 1, 0);
	m_varBal.poidsLuTarreL1 = new TypeAna(names.poidsLuTarreL1, start + 2, 0);
	m_varBal.poidsLuTarreL2 = new TypeAna(names.poidsLuTarreL2, start + 3, 0);
	m_varBal.poidsLuRompu = new TypeAna(names.poidsLuRompu, start + 4, 0);
	m_varBal.bacRompuValidL1 = new TypeBool(names.bacRompuValidL1, start + 5, 0);
	m_varBal.bacRompuValidL2 = new TypeBool(names.bacRompuValidL2, start + 6, 0);
	m_varBal.voyantBalanceTarreL1 = new TypeBool(names.voyantBalanceTarreL1, start + 7, 0);
	m_varBal.voyantBalanceTarreL2 = new TypeBool(names.voyantBalanceTarreL2, start + 8, 0);
	m_varBal.derogationTarreL1 = new TypeBool(names.derogationTarreL1, start + 9, 0); //Ajout 26/12/19
	m_varBal.derogationTarreL2 = new TypeBool(names.derogationTarreL2, start + 10, 0); //Ajout 26/12/19
	m_varBal.validationColisL1 = new TypeBool(names.validationColisL1, start + 11, 0); //Ajout 26/12/19
	m_varBal.validationColisL2 = new TypeBool(names.validationColisL2, start + 12, 0); //Ajout 26/12/19
	m_varBal.derogationColisL1 = new TypeBool(names.derogationColisL1, start + 13, 0); //Ajout 26/12/19
	m_varBal.derogationColisL2 = new TypeBool(names.derogationColisL2, start + 14, 0); //Ajout 26/12/19
	m_varBal.DLLVoyantTarreL1 = new TypeBool(names.DLLVoyantTarreL1, start + 15, 0); //Ajout 24/03/2020
	m_varBal.DLLVoyantTarreL2 = new TypeBool(names.DLLVoyantTarreL2, start + 16, 0); //Ajout 24/03/2020

}

double Balance::GetPoidsTarre(int ligne) {
	if (ligne == 1) {
		return this->m_varBal.poidsLuTarreL1->GetVarValue();
	}
	else{
		return this->m_varBal.poidsLuTarreL2->GetVarValue();
	}
}

bool Balance::ResetPoidsTarre(int ligne){
	if (ligne == 1) {
		this->m_varBal.poidsLuTarreL1->WriteVar(0);
	}
	else {
		this->m_varBal.poidsLuTarreL2->WriteVar(0);
	}
	return true;
}

bool Balance::GetFinRompus(int ligne){
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varBal.bacRompuValidL1->GetVarValue();
	}
	else {
		retour = this->m_varBal.bacRompuValidL2->GetVarValue();
	}
	return retour;
}

void Balance::ResetFinRompus(int ligne){
	if (ligne == 1) {
		this->m_varBal.bacRompuValidL1->WriteVar(false);
	}
	else {
		this->m_varBal.bacRompuValidL2->WriteVar(false);
	}
}

bool Balance::GetValidTarre(int ligne){
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varBal.validationTarreL1->GetVarValue();
	}
	else {
		retour = this->m_varBal.validationTarreL2->GetVarValue();
	}
	return retour;
}

void Balance::ResetValidTarre(int ligne){
	/* INFO :
	Modification 03/12/2019 Duplication validation tare
	*/
	if (ligne == 1) {
		this->m_varBal.validationTarreL1->WriteVar(false);
	}
	else {
		this->m_varBal.validationTarreL2->WriteVar(false);
	}
}

bool Balance::ResetDemandeProprete(int ligne){
	return true;
}

bool Balance::ResetDemandeVidange(int ligne){
	return true;
}

bool Balance::ResetDemandeConfiguration(int ligne){
	return true;
}

bool Balance::SetMotEtat() {

	return true;
}

bool Balance::EnvoiDemMarche(int ligne) {
	return true;
}

bool Balance::EnvoiDmeArret(int ligne) {
	return true;
}

bool Balance::SetVoyantBalanceTarre(int ligne, bool val) {
	if (ligne == 1) {
		this->m_varBal.voyantBalanceTarreL1->WriteVar(val);
	}
	else {
		this->m_varBal.voyantBalanceTarreL2->WriteVar(val);
	}

	return true;
}

bool Balance::ResetDerogationTarre(int ligne) {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;
	if (ligne == 1) {
		this->m_varBal.derogationTarreL1->WriteVar(false);
	}
	else {
		this->m_varBal.derogationTarreL2->WriteVar(false);
	}

	return retour;
}

bool Balance::ResetValidColis(int ligne) {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;
	if (ligne == 1) {
		this->m_varBal.validationColisL1->WriteVar(false);
	}
	else {
		this->m_varBal.validationColisL2->WriteVar(false);
	}
	return retour;
}

bool Balance::ResetDerogationColis(int ligne) {
	/*
	Ajout 26/12/19
	*/
	bool retour = true;
	if (ligne == 1) {
		this->m_varBal.derogationColisL1->WriteVar(false);
	}
	else {
		this->m_varBal.derogationColisL2->WriteVar(false);
	}
	return retour;
}

bool Balance::GetDLLVoyantTarre(int ligne) {
	/*
	Ajout 24/03/2020
	*/
	if (ligne == 1) {
		return this->m_varBal.DLLVoyantTarreL1->GetVarValue();
	}
	else {
		return this->m_varBal.DLLVoyantTarreL2->GetVarValue();
	}
}

bool Balance::SetDLLVoyantTarre(int ligne, bool val) {
	/*
	Ajout 24/03/2020
	*/
	if (ligne == 1) {
		this->m_varBal.DLLVoyantTarreL1->WriteVar(val);
	}
	else {
		this->m_varBal.DLLVoyantTarreL2->WriteVar(val);
	}

	return true;
}


//--------------------------------Palettiseur
//Constructors

RobotPalettisation::RobotPalettisation() {};

RobotPalettisation::RobotPalettisation(sNamePal names, int start) {

	m_varPal.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varPal.circuitPropreteL1 = new TypeAna(names.circuitPropreteL1, start + 9, 0);
	m_varPal.circuitPropreteL2 = new TypeAna(names.circuitPropreteL2, start + 10, 0);
	m_varPal.paramPropreteL1 = new TypeAna(names.paramPropreteL1, start + 11, 0);
	m_varPal.paramPropreteL2 = new TypeAna(names.paramPropreteL2, start + 12, 0);
	m_varPal.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 13, 0);
	m_varPal.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 14, 0);
	m_varPal.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 17, 0);
	m_varPal.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 19, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 21, 0);
	m_varPal.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 23, 0);
	m_varPal.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 24, 0);
	m_varPal.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 25, 0);
	m_varPal.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 26, 0);
	m_varPal.etatAnimationT1 = new TypeAna(names.etatAnimationT1, start + 27, 0);
	m_varPal.etatAnimationT2 = new TypeAna(names.etatAnimationT2, start + 28, 0);
	m_varPal.etatAPIT1 = new TypeAna(names.etatAPIT1, start + 29, 0);
	m_varPal.etatAPIT2 = new TypeAna(names.etatAPIT2, start + 30, 0);
	m_varPal.circuitPropreteT1 = new TypeAna(names.circuitPropreteT1, start + 31, 0);
	m_varPal.circuitPropreteT2 = new TypeAna(names.circuitPropreteT2, start + 32, 0);
	m_varPal.paramPropreteT1 = new TypeAna(names.paramPropreteT1, start + 33, 0);
	m_varPal.paramPropreteT2 = new TypeAna(names.paramPropreteT2, start + 34, 0);
	m_varPal.demandePropreteT1 = new TypeBool(names.demandePropreteT1, start + 35, 0);
	m_varPal.demandePropreteT2 = new TypeBool(names.demandePropreteT2, start + 36, 0);
	m_varPal.demandeVidangeT1 = new TypeBool(names.demandeVidangeT1, start + 37, 0);
	m_varPal.demandeVidangeT2 = new TypeBool(names.demandeVidangeT2, start + 38, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 39, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 40, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 41, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 42, 0);

	m_varPal.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 43, 0);
	m_varPal.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 44, 0);
	m_varPal.nbPPL1 = new TypeAna(names.nbPPL1, start + 45, 0);
	m_varPal.nbPPL2 = new TypeAna(names.nbPPL2, start + 46, 0);

	m_varPal.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 47, 0);
	m_varPal.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 48, 0);

	m_varPal.magPalettesVidesNivBas = new TypeBool(names.magPalettesVidesNivBas, start + 49, 0);
	m_varPal.CampagneL1 = new TypeBool(names.CampagneL1, start + 50, 0); // Ajout 19/03/2020
	m_varPal.CampagneL2 = new TypeBool(names.CampagneL2, start + 51, 0); // Ajout 19/03/2020

	//IDmachine = ++ID;
	IDmachine = 4000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);

	this->cptTimeoutConfig = 0;
}

bool RobotPalettisation::GestionProprete(int ligne) {
	bool retour = true;
	//Envoie demande de marche cellule
	this->m_varPal.demandeMarche->WriteVar(true);

	//Pour ligne 1
	if (ligne == 1) {
		//Si pas de demande, faire une demande
		if (!this->m_varPal.demandePropreteL1->GetVarValue()) {
			LOG_DEBUG << "Demande propreté robot palettisation ligne " << ligne << "";
			this->m_varPal.demandePropreteL1->WriteVar(true);
			retour = false;
		}
		
		if (this->m_varPal.etatAPIL1->GetVarValue() == ETAT_PROPRETE || this->m_varPal.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION || this->m_varPal.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
		}
		else { retour = false; }

		//Tapis 1/2 : si on fait des gefbox
		if (vFlatSetup.at(ligne - 1).typeConditionnement == "SC4 Orange" 
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC2 Vert"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC3 Ivoire"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "PC1 Jaune") {

			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandePropreteT1->GetVarValue()) {
				this->m_varPal.demandePropreteT1->WriteVar(true);
				retour = false;
			}
		
			//test machine
			if (this->m_varPal.circuitPropreteT1->GetVarValue() == this->m_varPal.paramPropreteT1->GetVarValue()) {
			}
			else { 
				retour = false; 
			}

			
			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandePropreteT2->GetVarValue()) {
				this->m_varPal.demandePropreteT2->WriteVar(true);
				retour = false;
			}
			//test machine
			if (this->m_varPal.circuitPropreteT2->GetVarValue() == this->m_varPal.paramPropreteT2->GetVarValue()) {
			}
			else { 
				retour = false; 
			}
		}
	}
	if (ligne == 2) {
		//Si pas de demande, faire une demande
		if (!this->m_varPal.demandePropreteL2->GetVarValue()) {
			LOG_DEBUG << "Demande propreté robot palettisation ligne " << ligne << "";
			this->m_varPal.demandePropreteL2->WriteVar(true);
			retour = false;
		}

		//test machine
		if (this->m_varPal.etatAPIL2->GetVarValue() == ETAT_PROPRETE || this->m_varPal.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION || this->m_varPal.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
		}
		else {
			retour = false; 
		}

		//Tapis 1/2 : si on fait des gefbox
		if (vFlatSetup.at(ligne - 1).typeConditionnement == "SC4 Orange"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC2 Vert"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC3 Ivoire"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "PC1 Jaune") {

			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandePropreteT1->GetVarValue()) {
				this->m_varPal.demandePropreteT1->WriteVar(true);
				retour = false;
			}
		//test machine
		if (this->m_varPal.circuitPropreteT1->GetVarValue() == this->m_varPal.paramPropreteT1->GetVarValue()) {
		}
		else { 
			retour = false; 
		}

			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandePropreteT2->GetVarValue()) {
				this->m_varPal.demandePropreteT2->WriteVar(true);
				retour = false;
			}
			//test machine
			if (this->m_varPal.circuitPropreteT2->GetVarValue() == this->m_varPal.paramPropreteT2->GetVarValue()) {
			}
			else { 
				retour = false; 
			}
		}
		
	}
	return retour;
}

bool RobotPalettisation::GestionConfig(int ligne){
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varPal.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varPal.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varPal.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varPal.demandeConfigL1->GetVarValue()) {
				//Demande effectuée
				this->m_varPal.demandeConfigL1->WriteVar(true);
			}
		}
	}
	else {
		//Si déja configuré
		if (this->m_varPal.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varPal.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varPal.demandeConfigL2->GetVarValue()) {
				//Demande effectuée
				this->m_varPal.demandeConfigL2->WriteVar(true);
			}
		}
	}
	return retour;
}

bool RobotPalettisation::EnvoiDemVidange(int ligne, bool ValidTare) {
	bool retour = true;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varPal.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varPal.demandeVidangeL1->WriteVar(false);
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandeVidangeL1->GetVarValue()) {
				this->m_varPal.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varPal.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeL1->WriteVar(false);
			}
			else { retour = false; }
		}
		//Tapis 1/2 : si on fait des gefbox
		if (vFlatSetup.at(ligne - 1).typeConditionnement == "SC4 Orange"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC2 Vert"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC3 Ivoire"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "PC1 Jaune") {
			//T1
			//Si déja en marche
			if (this->m_varPal.etatAPIT1->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeT1->WriteVar(false);
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varPal.demandeVidangeT1->GetVarValue()) {
					this->m_varPal.demandeVidangeT1->WriteVar(true);
				}

				//test machine
				if (this->m_varPal.etatAPIT1->GetVarValue() >= ETAT_VIDE) {
					this->m_varPal.demandeVidangeT1->WriteVar(false);
				}
				else { retour = false; }
			}
			//T2
			//Si déja en marche
			if (this->m_varPal.etatAPIT2->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeT2->WriteVar(false);
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varPal.demandeVidangeT2->GetVarValue()) {
					this->m_varPal.demandeVidangeT2->WriteVar(true);
				}

				//test machine
				if (this->m_varPal.etatAPIT2->GetVarValue() >= ETAT_VIDE) {
					this->m_varPal.demandeVidangeT2->WriteVar(false);
				}
				else { retour = false; }
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varPal.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varPal.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandeVidangeL2->GetVarValue()) {
				this->m_varPal.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varPal.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}

		//Tapis 1/2 : si on fait des gefbox
		if (vFlatSetup.at(ligne - 1).typeConditionnement == "SC4 Orange"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC2 Vert"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "RC3 Ivoire"
			|| vFlatSetup.at(ligne - 1).typeConditionnement == "PC1 Jaune") {
			//T1
			//Si déja en marche
			if (this->m_varPal.etatAPIT1->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeT1->WriteVar(false);
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varPal.demandeVidangeT1->GetVarValue()) {
					this->m_varPal.demandeVidangeT1->WriteVar(true);
				}

				//test machine
				if (this->m_varPal.etatAPIT1->GetVarValue() >= ETAT_VIDE) {
					this->m_varPal.demandeVidangeT1->WriteVar(false);
				}
				else { retour = false; }
			}
			//T2
			//Si déja en marche
			if (this->m_varPal.etatAPIT2->GetVarValue() >= ETAT_VIDE) {
				this->m_varPal.demandeVidangeT2->WriteVar(false);
			}
			//Pas en vidange
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varPal.demandeVidangeT2->GetVarValue()) {
					this->m_varPal.demandeVidangeT2->WriteVar(true);
				}

				//test machine
				if (this->m_varPal.etatAPIT2->GetVarValue() >= ETAT_VIDE) {
					this->m_varPal.demandeVidangeT2->WriteVar(false);
				}
				else { retour = false; }
			}
		}
	}
	return retour;
}

bool RobotPalettisation::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varPal.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varPal.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool RobotPalettisation::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varPal.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varPal.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool RobotPalettisation::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varPal.demandeConfigL1->WriteVar(false);
	}
	else {
		this->m_varPal.demandeConfigL2->WriteVar(false);
	}

	return 1;
}

bool RobotPalettisation::SetMotEtat() {

	/*//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varPal.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varPal.demandePropreteL1->GetVarValue(),
		this->m_varPal.demandeConfigL1->GetVarValue(),
		this->m_varPal.demandeVidangeL1->GetVarValue(),
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varPal.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varPal.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varPal.demandePropreteL2->GetVarValue(),
		this->m_varPal.demandeConfigL2->GetVarValue(),
		this->m_varPal.demandeVidangeL2->GetVarValue(),
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varPal.etatAPIL2->GetVarValue())
	);
	//Tapis 1
	this->m_varPal.etatAnimationT1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varPal.demandePropreteT1->GetVarValue(),
		0,
		this->m_varPal.demandeVidangeT1->GetVarValue(),
		0,
		0,
		0,
		0,
		this->m_varPal.etatAPIT1->GetVarValue())
	);
	//Tapis 2
	this->m_varPal.etatAnimationT2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varPal.demandePropreteT2->GetVarValue(),
		0,
		this->m_varPal.demandeVidangeT2->GetVarValue(),
		0,
		0,
		0,
		0,
		this->m_varPal.etatAPIT2->GetVarValue())
	);

	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varPal.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varPal.magPalettesVidesNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varPal.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varPal.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varPal.magPalettesVidesNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varPal.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varPal.magPalettesVidesNivBas->GetVarValue()),
			0,
			0,
			0,
			0,
			0,
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

int RobotPalettisation::GetAdvNPP(int ligne)
{
	if (ligne == 1) {
		return this->m_varPal.nbPPL1->GetAdviseNumber();
	}
	else {
		return this->m_varPal.nbPPL2->GetAdviseNumber();
	}
}

bool RobotPalettisation::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varPal.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varPal.demandePropreteL1->WriteVar(false);
			this->m_varPal.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varPal.demandeMarcheL1->GetVarValue()) {
				this->m_varPal.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varPal.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varPal.demandePropreteL1->WriteVar(false);
				this->m_varPal.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varPal.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varPal.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varPal.demandePropreteL2->WriteVar(false);
				this->m_varPal.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varPal.demandeMarcheL2->GetVarValue()) {
					this->m_varPal.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varPal.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varPal.demandePropreteL2->WriteVar(false);
					this->m_varPal.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varPal.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool RobotPalettisation::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varPal.CampagneL2->GetVarValue()) {
			this->m_varPal.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varPal.demandeMarcheL1->GetVarValue()) {
			this->m_varPal.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varPal.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
	}
	else {
		//Arret cellule
		if (!this->m_varPal.CampagneL1->GetVarValue()) {
			this->m_varPal.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varPal.demandeMarcheL2->GetVarValue()) {
			this->m_varPal.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varPal.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
			
		
	}
	return retour;
}

bool RobotPalettisation::EnvoiMarcheCellule()
{
	this->m_varPal.demandeMarche->WriteVar(true);
	return true;
}

double RobotPalettisation::GetNbrPiecePalettisee(int ligne) {
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		return this->m_varPal.nbPPL1->GetVarValue();
	}
	else {
		return this->m_varPal.nbPPL2->GetVarValue();
	}
}

bool RobotPalettisation::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varPal.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varPal.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------Etiqueteuse
//Constructors

Etiqueteuse::Etiqueteuse() {};

Etiqueteuse::Etiqueteuse(sNameEtq names, int start) {

	m_varEtq.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varEtq.rouleauEtiquetteNivBas = new TypeBool(names.rouleauEtiquetteNivBas, start + 9, 0);
	m_varEtq.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 10, 0);
	m_varEtq.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 11, 0);
	m_varEtq.timeoutConfigL1 = new TypeBool(names.timeoutConfigL1, start + 12, 0);
	m_varEtq.timeoutConfigL2 = new TypeBool(names.timeoutConfigL2, start + 13, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 14, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 15, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 16, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 17, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 18, 0);

	m_varEtq.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 19, 0);
	m_varEtq.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 20, 0);

	m_varEtq.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 21, 0);
	m_varEtq.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 22, 0);
	m_varEtq.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 23, 0);
	m_varEtq.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 24, 0);

	m_varEtq.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 25, 0);
	m_varEtq.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 26, 0);

	m_varEtq.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 27, 0);
	m_varEtq.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 28, 0);
	m_varEtq.CampagneL1 = new TypeBool(names.CampagneL1, start + 29, 0); // Ajout 19/03/2020
	m_varEtq.CampagneL2 = new TypeBool(names.CampagneL2, start + 30, 0); // Ajout 19/03/2020

	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);
	//IDmachine = ++ID;
	IDmachine = 4000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	this->cptTimeoutConfig = 0;
}

bool Etiqueteuse::GestionProprete(int ligne) {
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varEtq.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varEtq.demandePropreteL1->GetVarValue()) {
			//Demande effectuée
			this->m_varEtq.demandePropreteL1->WriteVar(true);
		}
		//test machine
		if (this->m_varEtq.etatAPIL1->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
	}
	else {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varEtq.demandePropreteL2->GetVarValue()) {
			//Demande effectuée
			this->m_varEtq.demandePropreteL2->WriteVar(true);
		}
		//test machine
		if (this->m_varEtq.etatAPIL2->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
	}
	//Je retourne toujours true car je vais pas bloquer la prod pour un équipement 
	//qui simule cette démarche pour affichage seulement
	return retour;
}

bool Etiqueteuse::GestionConfig(int ligne)
{
	bool retour = false;
	//Envoie demande de marche cellule
	this->m_varEtq.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varEtq.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varEtq.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varEtq.timeoutConfigL1->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varEtq.demandeConfigL1->GetVarValue()) {
				//Demande effectuée
				this->m_varEtq.demandeConfigL1->WriteVar(true);
			}//Sinon, incrémenter le compteur 
			else { cptTimeoutConfig++; }
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varEtq.demandeConfigL1->WriteVar(false);
			this->m_varEtq.timeoutConfigL1->WriteVar(true);
		}
	}
	else {
		//Si déja configuré
		if (this->m_varEtq.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varEtq.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varEtq.timeoutConfigL2->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varEtq.demandeConfigL2->GetVarValue()) {
				//Demande effectuée
				this->m_varEtq.demandeConfigL2->WriteVar(true);
			}//Sinon, incrémenter le compteur 
			else { cptTimeoutConfig++; }
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varEtq.demandeConfigL2->WriteVar(false);
			this->m_varEtq.timeoutConfigL2->WriteVar(true);
		}
	}
	return retour;
}

bool Etiqueteuse::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varEtq.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varEtq.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool Etiqueteuse::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varEtq.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varEtq.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool Etiqueteuse::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varEtq.demandeConfigL1->WriteVar(false);
		this->m_varEtq.timeoutConfigL1->WriteVar(false);
	}
	else {
		this->m_varEtq.demandeConfigL2->WriteVar(false);
		this->m_varEtq.timeoutConfigL2->WriteVar(false);
	}
	return 1;
}

bool Etiqueteuse::EnvoiDemVidange(int ligne, bool ValidTare)
{
	bool retour = false;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varEtq.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varEtq.demandeVidangeL1->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varEtq.demandeVidangeL1->GetVarValue()) {
				this->m_varEtq.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varEtq.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varEtq.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varEtq.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varEtq.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varEtq.demandeVidangeL2->GetVarValue()) {
				this->m_varEtq.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varEtq.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varEtq.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}
	}
	return retour;
}

bool Etiqueteuse::SetMotEtat() {

	//Général
	/*this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		(this->m_varEtq.timeoutConfigL1->GetVarValue() || this->m_varEtq.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varEtq.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		0,
		this->m_varEtq.demandeConfigL1->GetVarValue(),
		0,
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varEtq.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varEtq.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		0,
		this->m_varEtq.demandeConfigL2->GetVarValue(),
		0,
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varEtq.etatAPIL2->GetVarValue())
	);
	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varEtq.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varEtq.rouleauEtiquetteNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		(this->m_varEtq.timeoutConfigL1->GetVarValue() || this->m_varEtq.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varEtq.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varEtq.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varEtq.rouleauEtiquetteNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		(this->m_varEtq.timeoutConfigL1->GetVarValue() || this->m_varEtq.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varEtq.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varEtq.rouleauEtiquetteNivBas->GetVarValue()),
			0,
			0,
			0,
			0,
			(this->m_varEtq.timeoutConfigL1->GetVarValue() || this->m_varEtq.timeoutConfigL2->GetVarValue()),
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

bool Etiqueteuse::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varEtq.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varEtq.demandePropreteL1->WriteVar(false);
			this->m_varEtq.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varEtq.demandeMarcheL1->GetVarValue()) {
				this->m_varEtq.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varEtq.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varEtq.demandePropreteL1->WriteVar(false);
				this->m_varEtq.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varEtq.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varEtq.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varEtq.demandePropreteL2->WriteVar(false);
				this->m_varEtq.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varEtq.demandeMarcheL2->GetVarValue()) {
					this->m_varEtq.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varEtq.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varEtq.demandePropreteL2->WriteVar(false);
					this->m_varEtq.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varEtq.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool Etiqueteuse::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varEtq.CampagneL2->GetVarValue()) {
			this->m_varEtq.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varEtq.demandeMarcheL1->GetVarValue()) {
			this->m_varEtq.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varEtq.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
	}
	else {
		
		//Arret cellule
		if (!this->m_varEtq.CampagneL1->GetVarValue()) {
			this->m_varEtq.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varEtq.demandeMarcheL2->GetVarValue()) {
			this->m_varEtq.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varEtq.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
			
		
	}
	return retour;
}

bool Etiqueteuse::EnvoiMarcheCellule()
{
	this->m_varEtq.demandeMarche->WriteVar(true);
	return true;
}

bool Etiqueteuse::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varEtq.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varEtq.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------Imprimante
//Constructors

Imprimante::Imprimante() {};

Imprimante::Imprimante(sNameImp names, int start) {

	m_varImp.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varImp.encreNivBas = new TypeBool(names.encreNivBas, start + 9, 0);
	m_varImp.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 10, 0);
	m_varImp.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 11, 0);
	m_varImp.timeoutConfigL1 = new TypeBool(names.timeoutConfigL1, start + 12, 0);
	m_varImp.timeoutConfigL2 = new TypeBool(names.timeoutConfigL2, start + 13, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 14, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 15, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 16, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 17, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 18, 0);
	m_varImp.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 19, 0);
	m_varImp.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 20, 0);

	m_varImp.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 21, 0);
	m_varImp.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 22, 0);
	m_varImp.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 23, 0);
	m_varImp.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 24, 0);

	m_varImp.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 25, 0);
	m_varImp.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 26, 0);

	m_varImp.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 27, 0);
	m_varImp.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 28, 0);
	m_varImp.CampagneL1 = new TypeBool(names.CampagneL1, start + 29, 0); // Ajout 19/03/2020
	m_varImp.CampagneL2 = new TypeBool(names.CampagneL2, start + 30, 0); // Ajout 19/03/2020

	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);
	//IDmachine = ++ID;
	IDmachine = 4000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	this->cptTimeoutConfig = 0;
}

bool Imprimante::GestionProprete(int ligne) {
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varImp.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varImp.demandePropreteL1->GetVarValue()) {
			//Demande effectuée
			this->m_varImp.demandePropreteL1->WriteVar(true);
		}
		//test machine
		if (this->m_varImp.etatAPIL1->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
	}
	else {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varImp.demandePropreteL2->GetVarValue()) {
			//Demande effectuée
			this->m_varImp.demandePropreteL2->WriteVar(true);
		}
		//test machine
		if (this->m_varImp.etatAPIL2->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
	}
	return retour;
}

bool Imprimante::GestionConfig(int ligne)
{
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varImp.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varImp.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varImp.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varImp.timeoutConfigL1->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varImp.demandeConfigL1->GetVarValue()) {
				//Demande effectuée
				this->m_varImp.demandeConfigL1->WriteVar(true);
			}//Sinon, incrémenter le compteur
			else { cptTimeoutConfig++; }
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varImp.demandeConfigL1->WriteVar(false);
			this->m_varImp.timeoutConfigL1->WriteVar(true);
		}
	}
	else {
		//Si déja configuré
		if (this->m_varImp.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varImp.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
			cptTimeoutConfig = 0;
			this->m_varImp.timeoutConfigL2->WriteVar(false);
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varImp.demandeConfigL2->GetVarValue()) {
				//Demande effectuée
				this->m_varImp.demandeConfigL2->WriteVar(true);
			}//Sinon, incrémenter le compteur
			else { cptTimeoutConfig++; }
		}
		if (cptTimeoutConfig >= TIMEOUT_CONFIG) {
			cptTimeoutConfig = 0;
			this->m_varImp.demandeConfigL2->WriteVar(false);
			this->m_varImp.timeoutConfigL2->WriteVar(true);
		}
	}
	return retour;
}

bool Imprimante::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varImp.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varImp.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool Imprimante::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varImp.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varImp.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool Imprimante::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varImp.demandeConfigL1->WriteVar(false);
		this->m_varImp.timeoutConfigL1->WriteVar(false);
	}
	else {
		this->m_varImp.demandeConfigL2->WriteVar(false);
		this->m_varImp.timeoutConfigL2->WriteVar(false);
	}
	return 1;
}

bool Imprimante::EnvoiDemVidange(int ligne, bool ValidTare)
{
	bool retour = false;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varImp.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varImp.demandeVidangeL1->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varImp.demandeVidangeL1->GetVarValue()) {
				this->m_varImp.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varImp.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varImp.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varImp.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varImp.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varImp.demandeVidangeL2->GetVarValue()) {
				this->m_varImp.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varImp.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varImp.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}
	}
	return retour;
}

bool Imprimante::SetMotEtat() {

	/*//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		(this->m_varImp.timeoutConfigL1->GetVarValue() || this->m_varImp.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varImp.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		0,
		this->m_varImp.demandeConfigL1->GetVarValue(),
		0,
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varImp.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varImp.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		0,
		this->m_varImp.demandeConfigL2->GetVarValue(),
		0,
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varImp.etatAPIL2->GetVarValue())
	);

	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varImp.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varImp.encreNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		(this->m_varImp.timeoutConfigL1->GetVarValue() || this->m_varImp.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varImp.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varImp.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varImp.encreNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		(this->m_varImp.timeoutConfigL1->GetVarValue() || this->m_varImp.timeoutConfigL2->GetVarValue()),
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varImp.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varImp.encreNivBas->GetVarValue()),
			0,
			0,
			0,
			0,
			(this->m_varImp.timeoutConfigL1->GetVarValue() || this->m_varImp.timeoutConfigL2->GetVarValue()),
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	return true;
}

bool Imprimante::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varImp.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varImp.demandePropreteL1->WriteVar(false);
			this->m_varImp.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varImp.demandeMarcheL1->GetVarValue()) {
				this->m_varImp.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varImp.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varImp.demandePropreteL1->WriteVar(false);
				this->m_varImp.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varImp.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varImp.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varImp.demandePropreteL2->WriteVar(false);
				this->m_varImp.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varImp.demandeMarcheL2->GetVarValue()) {
					this->m_varImp.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varImp.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varImp.demandePropreteL2->WriteVar(false);
					this->m_varImp.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varImp.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool Imprimante::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varImp.CampagneL2->GetVarValue()) {
			this->m_varImp.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varImp.demandeMarcheL1->GetVarValue()) {
			this->m_varImp.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varImp.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
	}
	else {
		//Arret cellule
		if (!this->m_varImp.CampagneL1->GetVarValue()) {
			this->m_varImp.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varImp.demandeMarcheL2->GetVarValue()) {
			this->m_varImp.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varImp.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
	}
	return retour;
}

bool Imprimante::EnvoiMarcheCellule()
{
	this->m_varImp.demandeMarche->WriteVar(true);
	return true;
}

bool Imprimante::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varImp.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varImp.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------Fermeuse
//Constructors

Fermeuse::Fermeuse() {};

Fermeuse::Fermeuse(sNameFerm names, int start) {

	m_varFerm.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

//	vVariablesAlarmes.push_back(std::vector<Variable*>{this->m_varCommun.alarmeNonBloquante, this->m_varCommun.numAlarme});

	m_varFerm.rouleauAdhesifNivBas = new TypeBool(names.rouleauAdhesifNivBas, start + 9, 0);

	m_varCommun.etatAnimation = new TypeAna(names.etatAnimation, start + 10, 0);

	m_varCommun.mdvSPV = new TypeAna(names.mdvSPV, start + 11, 0);
	m_varCommun.mdvAPI = new TypeAna(names.mdvAPI, start + 12, 0);
	m_varCommun.timeoutComAPI = new TypeBool(names.timeoutComAPI, start + 13, 0);
	m_varCommun.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 14, 0);
	m_varFerm.demandeConfigL1 = new TypeBool(names.demandeConfigL1, start + 15, 0);
	m_varFerm.demandeConfigL2 = new TypeBool(names.demandeConfigL2, start + 16, 0);
	m_varFerm.demandePropreteL1 = new TypeBool(names.demandePropreteL1, start + 17, 0);
	m_varFerm.demandePropreteL2 = new TypeBool(names.demandePropreteL2, start + 18, 0);
	m_varFerm.etatAPIL1 = new TypeAna(names.etatAPIL1, start + 19, 0);
	m_varFerm.etatAPIL2 = new TypeAna(names.etatAPIL2, start + 20, 0);
	m_varFerm.etatAnimationL1 = new TypeAna(names.etatAnimationL1, start + 21, 0);
	m_varFerm.etatAnimationL2 = new TypeAna(names.etatAnimationL2, start + 22, 0);

	m_varFerm.demandeVidangeL1 = new TypeBool(names.demandeVidangeL1, start + 23, 0);
	m_varFerm.demandeVidangeL2 = new TypeBool(names.demandeVidangeL2, start + 24, 0);

	m_varFerm.demandeMarcheL1 = new TypeBool(names.demandeMarcheL1, start + 25, 0);
	m_varFerm.demandeMarcheL2 = new TypeBool(names.demandeMarcheL2, start + 26, 0);
	m_varFerm.CampagneL1 = new TypeBool(names.CampagneL1, start + 27, 0); // Ajout 19/03/2020
	m_varFerm.CampagneL2 = new TypeBool(names.CampagneL2, start + 28, 0); // Ajout 19/03/2020

	//IDmachine = ++ID;
	IDmachine = 8000;

	memMdVAPI = 0;
	cptTimeoutCom = 0;
	cptTimeoutMarche = 0;

	vVariablesAlarmes.push_back(std::tuple<int, TypeAna*, int>(this->m_varCommun.alarmeBloquante->GetAdviseNumber(), this->m_varCommun.numAlarme, IDmachine));
	vVariablesAl.push_back(m_varCommun.alarmeNonBloquante);
	vVariablesAl.push_back(m_varCommun.alarmeBloquante);
	this->cptTimeoutConfig = 0;
}

bool Fermeuse::GestionProprete(int ligne) {
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varFerm.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varFerm.demandePropreteL1->GetVarValue()) {
			//Demande effectuée
			this->m_varFerm.demandePropreteL1->WriteVar(true);
		}
		//test machine
		if (this->m_varFerm.etatAPIL1->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
	}
	else {
		//Si pas de demande, faire une demande et envoyer proprete
		if (!this->m_varFerm.demandePropreteL2->GetVarValue()) {
			//Demande effectuée
			this->m_varFerm.demandePropreteL2->WriteVar(true);
		}
		//test machine
		if (this->m_varFerm.etatAPIL2->GetVarValue() >= ETAT_PROPRETE) {
			retour = true;
		}
		
		
	}
	//Je retourne toujours true car je vais pas bloquer la prod pour un équipement 
	//qui simule cette démarche pour affichage seulement
	return retour;
}

bool Fermeuse::GestionConfig(int ligne) {
	bool retour = false;

	//Envoie demande de marche cellule
	this->m_varFerm.demandeMarche->WriteVar(true);

	if (ligne == 1) {
		//Si déja configuré
		if (this->m_varFerm.etatAPIL1->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varFerm.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varFerm.demandeConfigL1->GetVarValue()) {
				//Demande effectuée
				this->m_varFerm.demandeConfigL1->WriteVar(true);
			}
		}
	}
	else {
		//Si déja configuré
		if (this->m_varFerm.etatAPIL2->GetVarValue() == ETAT_CONFIGURATION
			|| this->m_varFerm.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
		//Non configuré
		else {
			//Si pas de demande, faire une demande et envoyer config
			if (!this->m_varFerm.demandeConfigL2->GetVarValue()) {
				//Demande effectuée
				this->m_varFerm.demandeConfigL2->WriteVar(true);
			}
		}
	}
	return retour;
}

bool Fermeuse::ResetDemandeProprete(int ligne)
{
	if (ligne == 1) {
		this->m_varFerm.demandePropreteL1->WriteVar(false);
	}
	else {
		this->m_varFerm.demandePropreteL2->WriteVar(false);
	}
	return 1;
}

bool Fermeuse::ResetDemandeVidange(int ligne)
{
	if (ligne == 1) {
		this->m_varFerm.demandeVidangeL1->WriteVar(false);
	}
	else {
		this->m_varFerm.demandeVidangeL2->WriteVar(false);
	}
	return 1;
}

bool Fermeuse::ResetDemandeConfiguration(int ligne)
{
	if (ligne == 1) {
		this->m_varFerm.demandeConfigL1->WriteVar(false);
	}
	else {
		this->m_varFerm.demandeConfigL2->WriteVar(false);
	}

	return 1;
}

bool Fermeuse::EnvoiDemVidange(int ligne, bool ValidTare)
{
	bool retour = false;
	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varFerm.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
			this->m_varFerm.demandeVidangeL1->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varFerm.demandeVidangeL1->GetVarValue()) {
				this->m_varFerm.demandeVidangeL1->WriteVar(true);
			}

			//test machine
			if (this->m_varFerm.etatAPIL1->GetVarValue() >= ETAT_VIDE) {
				this->m_varFerm.demandeVidangeL1->WriteVar(false);
				retour = true;
			}
		}
	}
	else {
		//Si déja en marche
		if (this->m_varFerm.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
			this->m_varFerm.demandeVidangeL2->WriteVar(false);
			retour = true;
		}
		//Pas en vidange
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varFerm.demandeVidangeL2->GetVarValue()) {
				this->m_varFerm.demandeVidangeL2->WriteVar(true);
			}

			//test machine
			if (this->m_varFerm.etatAPIL2->GetVarValue() >= ETAT_VIDE) {
				this->m_varFerm.demandeVidangeL2->WriteVar(false);
				retour = true;
			}
		}
	}
	return retour;
}

bool Fermeuse::SetMotEtat() {

	/*//Général
	this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		this->m_varCommun.alarmeNonBloquante->GetVarValue(),
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue())
	);*/
	//L1
	this->m_varFerm.etatAnimationL1->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varFerm.demandePropreteL1->GetVarValue(),
		this->m_varFerm.demandeConfigL1->GetVarValue(),
		this->m_varFerm.demandeVidangeL1->GetVarValue(),
		this->machineSortie_L1, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varFerm.etatAPIL1->GetVarValue())
	);
	//L2
	this->m_varFerm.etatAnimationL2->WriteVar(GetMotEtat(
		0,
		0,
		this->m_varFerm.demandePropreteL2->GetVarValue(),
		this->m_varFerm.demandeConfigL2->GetVarValue(),
		this->m_varFerm.demandeVidangeL2->GetVarValue(),
		this->machineSortie_L2, //Sortie de la campagne
		0,
		0,
		0,
		this->m_varFerm.etatAPIL2->GetVarValue())
	);

	//Si une ligne en config, mettre transitique en bleu avec prio sur la ligne 1
	if (this->m_varFerm.etatAnimationL1->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varFerm.rouleauAdhesifNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varFerm.etatAnimationL1->GetVarValue());
	}
	else if (this->m_varFerm.etatAnimationL2->GetVarValue() > 4 && GetMotEtat(
		this->m_varCommun.alarmeBloquante->GetVarValue(),
		(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varFerm.rouleauAdhesifNivBas->GetVarValue()),
		0,
		0,
		0,
		0,
		0,
		this->m_varCommun.timeoutComAPI->GetVarValue(),
		this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
		this->m_varCommun.etatAPI->GetVarValue()) < 3) {
		this->m_varCommun.etatAnimation->WriteVar(this->m_varFerm.etatAnimationL2->GetVarValue());
	}
	else
	{
		this->m_varCommun.etatAnimation->WriteVar(GetMotEtat(
			this->m_varCommun.alarmeBloquante->GetVarValue(),
			(this->m_varCommun.alarmeNonBloquante->GetVarValue() || this->m_varFerm.rouleauAdhesifNivBas->GetVarValue()),
			0,
			0,
			0,
			0,
			0,
			this->m_varCommun.timeoutComAPI->GetVarValue(),
			this->m_varCommun.timeoutMarcheAPI->GetVarValue(),
			this->m_varCommun.etatAPI->GetVarValue())
		);
	}
	
	return true;
}

bool Fermeuse::EnvoiDemMarche(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Si déja en marche
		if (this->m_varFerm.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
			this->m_varFerm.demandePropreteL1->WriteVar(false);
			this->m_varFerm.demandeConfigL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		//Pas en marche
		else {
			//Si pas de demande, faire une demande
			if (!this->m_varFerm.demandeMarcheL1->GetVarValue()) {	
				this->m_varFerm.demandeMarcheL1->WriteVar(true);
			}
			else {
				cptTimeoutMarche++;
			}

			//test machine
			if (this->m_varFerm.etatAPIL1->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varFerm.demandePropreteL1->WriteVar(false);
				this->m_varFerm.demandeConfigL1->WriteVar(false);
				retour = true;
			}
		}
		if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
			cptTimeoutMarche = 0;
			this->m_varFerm.demandeMarcheL1->WriteVar(false);
			this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
		}
	}
	else {
		{
			//Si déja en marche
			if (this->m_varFerm.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
				this->m_varFerm.demandePropreteL2->WriteVar(false);
				this->m_varFerm.demandeConfigL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
				retour = true;
			}
			//Pas en marche
			else {
				//Si pas de demande, faire une demande
				if (!this->m_varFerm.demandeMarcheL2->GetVarValue()) {
					this->m_varFerm.demandeMarcheL2->WriteVar(true);
				}
				else {
					cptTimeoutMarche++;
				}

				//test machine
				if (this->m_varFerm.etatAPIL2->GetVarValue() == ETAT_PRODUCTION) {
					this->m_varFerm.demandePropreteL2->WriteVar(false);
					this->m_varFerm.demandeConfigL2->WriteVar(false);
					retour = true;
				}
			}
			if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
				cptTimeoutMarche = 0;
				this->m_varFerm.demandeMarcheL2->WriteVar(false);
				this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
			}
		}
	}
	return retour;
}

bool Fermeuse::EnvoiDmeArret(int ligne) {
	bool retour = false;

	if (ligne == 1) {
		//Arret cellule
		if (!this->m_varFerm.CampagneL2->GetVarValue()) {
			this->m_varFerm.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varFerm.demandeMarcheL1->GetVarValue()) {
			this->m_varFerm.demandeMarcheL1->WriteVar(false);
		}

		//test machine
		if (this->m_varFerm.etatAPIL1->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
		
	}
	else {
		//Arret cellule
		if (!this->m_varFerm.CampagneL1->GetVarValue()) {
			this->m_varFerm.demandeMarche->WriteVar(false);
		}
		//Si pas de demande, faire une demande
		if (this->m_varFerm.demandeMarcheL2->GetVarValue()) {
			this->m_varFerm.demandeMarcheL2->WriteVar(false);
		}

		//test machine
		if (this->m_varFerm.etatAPIL2->GetVarValue() == ETAT_ARRET) {
			this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
			retour = true;
		}
	}
	return retour;
}

bool Fermeuse::EnvoiMarcheCellule()
{
	this->m_varFerm.demandeMarche->WriteVar(true);
	return true;
}

bool Fermeuse::SetCampagne(int ligne, bool val)
{
	/*
	Ajout 19/03/2020
	*/
	if (ligne == 1) {
		this->m_varFerm.CampagneL1->WriteVar(val);
	}
	else
	{
		this->m_varFerm.CampagneL2->WriteVar(val);
	}
	return true;
}

//--------------------------------MES
//Constructors

MES::MES(sNameMES names, int start) {

	
	m_varMES.rayproCounterL1 =				new TypeAna(names.rayproCounterL1, start, 0);
	m_varMES.rayproCounterL2 =				new TypeAna(names.rayproCounterL2, start + 1, 0);

	//Setup Ligne 1
	m_varMES.setupL1.refPieces = new TypeAna(names.setupL1.refPieces, start + 2, 0);
	m_varMES.setupL1.descProduit = new TypeString(names.setupL1.descProduit, start + 3, "");
	m_varMES.setupL1.idOperateur = new TypeAna(names.setupL1.idOperateur, start + 4, 0);
	m_varMES.setupL1.numLot = new TypeString(names.setupL1.numLot, start + 5, 0);
	m_varMES.setupL1.numOF = new TypeAna(names.setupL1.numOF, start + 6, 0);
	m_varMES.setupL1.nbPieces = new TypeAna(names.setupL1.nbPieces, start + 7, 0);
	m_varMES.setupL1.nbPiecesPPalette = new TypeAna(names.setupL1.nbPiecesPPalette, start + 8, 0);
	m_varMES.setupL1.nbPiecesPContenant = new TypeAna(names.setupL1.nbPiecesPContenant, start + 9, 0);
	m_varMES.setupL1.typeConditionnement = new TypeString(names.setupL1.typeConditionnement, start + 10, "");
	m_varMES.setupL1.nbPiecesPSachet = new TypeAna(names.setupL1.nbPiecesPSachet, start + 11, 0);
	m_varMES.setupL1.hauteurSachet = new TypeAna(names.setupL1.hauteurSachet, start + 12, 0);
	m_varMES.setupL1.poidsPiece = new TypeAna(names.setupL1.poidsPiece, start + 13, 0);
	m_varMES.setupL1.adjonctionEau = new TypeAna(names.setupL1.adjonctionEau, start + 14, 0);
	
	
	//Setup Ligne 2
	m_varMES.setupL2.refPieces = new TypeAna(names.setupL2.refPieces, start + 15, 0);
	m_varMES.setupL2.descProduit = new TypeString(names.setupL2.descProduit, start + 16, "");
	m_varMES.setupL2.idOperateur = new TypeAna(names.setupL2.idOperateur, start + 17, 0);
	m_varMES.setupL2.numLot = new TypeString(names.setupL2.numLot, start + 18, "");
	m_varMES.setupL2.numOF = new TypeAna(names.setupL2.numOF, start + 19, 0);
	m_varMES.setupL2.nbPieces = new TypeAna(names.setupL2.nbPieces, start + 20, 0);
	m_varMES.setupL2.nbPiecesPPalette = new TypeAna(names.setupL2.nbPiecesPPalette, start + 21, 0);
	m_varMES.setupL2.nbPiecesPContenant = new TypeAna(names.setupL2.nbPiecesPContenant, start + 22, 0);
	m_varMES.setupL2.typeConditionnement = new TypeString(names.setupL2.typeConditionnement, start + 23, "");
	m_varMES.setupL2.nbPiecesPSachet = new TypeAna(names.setupL2.nbPiecesPSachet, start + 24, 0);
	m_varMES.setupL2.hauteurSachet = new TypeAna(names.setupL2.hauteurSachet, start + 25, 0);
	m_varMES.setupL2.poidsPiece = new TypeAna(names.setupL2.poidsPiece, start + 26, 0);
	m_varMES.setupL2.adjonctionEau = new TypeAna(names.setupL2.adjonctionEau, start + 27, 0);
	
	//Sync L1
	m_varMES.syncL1.spvAttLot = new TypeBool(names.syncL1.spvAttLot, start + 28, 0);
	m_varMES.syncL1.spvAttConfirmLancProd = new TypeBool(names.syncL1.spvAttConfirmLancProd, start + 29, 0);
	m_varMES.syncL1.mesConfirmLancProd = new TypeBool(names.syncL1.mesConfirmLancProd, start + 30, 0);
	m_varMES.syncL1.mesDemFinProd = new TypeBool(names.syncL1.mesDemFinProd, start + 31, 0);
	m_varMES.syncL1.plcSyncSetup = new TypeAna(names.syncL1.plcSyncSetup, start + 32, 0);
	m_varMES.syncL1.mesSyncSetup = new TypeAna(names.syncL1.mesSyncSetup, start + 33, 0);
	m_varMES.syncL1.mesStartReq = new TypeBool(names.syncL1.mesStartReq, start + 34, 0);
	m_varMES.syncL1.spvStartReq = new TypeBool(names.syncL1.spvStartReq, start + 35, 0);
	m_varMES.syncL1.mestoppageSync = new TypeAna(names.syncL1.mestoppageSync, start + 36, 0);
	m_varMES.syncL1.spvtoppageSync = new TypeAna(names.syncL1.spvtoppageSync, start + 37, 0);
	m_varMES.syncL1.tsStoppageHH = new TypeAna(names.syncL1.tsStoppageHH, start + 38, 0);
	m_varMES.syncL1.tsStoppageMM = new TypeAna(names.syncL1.tsStoppageMM, start + 39, 0);
	m_varMES.syncL1.tsStoppageSS = new TypeAna(names.syncL1.tsStoppageSS, start + 40, 0);
	m_varMES.syncL1.idStoppageCause = new TypeAna(names.syncL1.idStoppageCause, start + 41, 0);
	m_varMES.syncL1.qteEauREnsach = new TypeAna(names.syncL1.qteEauREnsach, start + 42, 0);
	m_varMES.syncL1.infPalettePleine = new TypeBool(names.syncL1.infPalettePleine, start + 43, 0);
	m_varMES.syncL1.plcBatchReqStatus = new TypeAna(names.syncL1.plcBatchReqStatus, start + 44, 0);
	m_varMES.syncL1.mesBatchReq = new TypeBool(names.syncL1.mesBatchReq, start + 45, 0);
	m_varMES.syncL1.plcConfirmFinProd = new TypeBool(names.syncL1.plcConfirmFinProd, start + 46, 0);
	
	//Sync L2
	m_varMES.syncL2.spvAttLot = new TypeBool(names.syncL2.spvAttLot, start + 47, 0);
	m_varMES.syncL2.spvAttConfirmLancProd = new TypeBool(names.syncL2.spvAttConfirmLancProd, start + 48, 0);
	m_varMES.syncL2.mesConfirmLancProd = new TypeBool(names.syncL2.mesConfirmLancProd, start + 49, 0);
	m_varMES.syncL2.mesDemFinProd = new TypeBool(names.syncL2.mesDemFinProd, start + 50, 0);
	m_varMES.syncL2.plcSyncSetup = new TypeAna(names.syncL2.plcSyncSetup, start + 51, 0);
	m_varMES.syncL2.mesSyncSetup = new TypeAna(names.syncL2.mesSyncSetup, start + 52, 0);
	m_varMES.syncL2.mesStartReq = new TypeBool(names.syncL2.mesStartReq, start + 53, 0);
	m_varMES.syncL2.spvStartReq = new TypeBool(names.syncL2.spvStartReq, start + 54, 0);
	m_varMES.syncL2.mestoppageSync = new TypeAna(names.syncL2.mestoppageSync, start + 55, 0);
	m_varMES.syncL2.spvtoppageSync = new TypeAna(names.syncL2.spvtoppageSync, start + 56, 0);
	m_varMES.syncL2.tsStoppageHH = new TypeAna(names.syncL2.tsStoppageHH, start + 57, 0);
	m_varMES.syncL2.tsStoppageMM = new TypeAna(names.syncL2.tsStoppageMM, start + 58, 0);
	m_varMES.syncL2.tsStoppageSS = new TypeAna(names.syncL2.tsStoppageSS, start + 59, 0);
	m_varMES.syncL2.idStoppageCause = new TypeAna(names.syncL2.idStoppageCause, start + 60, 0);
	m_varMES.syncL2.qteEauREnsach = new TypeAna(names.syncL2.qteEauREnsach, start + 61, 0);
	m_varMES.syncL2.infPalettePleine = new TypeBool(names.syncL2.infPalettePleine, start + 62, 0);
	m_varMES.syncL2.plcBatchReqStatus = new TypeAna(names.syncL2.plcBatchReqStatus, start + 63, 0);
	m_varMES.syncL2.mesBatchReq = new TypeBool(names.syncL2.mesBatchReq, start + 64, 0);
	m_varMES.syncL2.plcConfirmFinProd = new TypeBool(names.syncL2.plcConfirmFinProd, start + 65, 0);

	m_varMES.syncL1.mesSyncTime = new TypeAna(names.syncL1.mesSyncTime, start + 66, 0);
	m_varMES.syncL1.plcSyncTime = new TypeAna(names.syncL1.plcSyncTime, start + 67, 0);
	m_varMES.syncL2.mesSyncTime = new TypeAna(names.syncL2.mesSyncTime, start + 68, 0);
	m_varMES.syncL2.plcSyncTime = new TypeAna(names.syncL2.plcSyncTime, start + 69, 0);
	m_varMES.syncL1.numStoppageCause = new TypeAna(names.syncL1.numStoppageCause, start + 70, 0);
	m_varMES.syncL2.numStoppageCause = new TypeAna(names.syncL2.numStoppageCause, start + 71, 0);
	m_varMES.syncL1.numStoppageStation = new TypeAna(names.syncL1.numStoppageStation, start + 72, 0);
	m_varMES.syncL2.numStoppageStation = new TypeAna(names.syncL2.numStoppageStation, start + 73, 0);
	

	m_varMES.syncL1.mesSyncHU = new TypeAna(names.syncL1.mesSyncHU, start + 74, 0);
	m_varMES.syncL1.spvSyncHU = new TypeAna(names.syncL1.spvSyncHU, start + 75, 0);
	m_varMES.syncL1.huHH = new TypeAna(names.syncL1.huHH, start + 76, 0);
	m_varMES.syncL1.huMM = new TypeAna(names.syncL1.huMM, start + 77, 0);
	m_varMES.syncL1.huSS = new TypeAna(names.syncL1.huSS, start + 78, 0);
	m_varMES.syncL1.huContID = new TypeAna(names.syncL1.huContID, start + 79, 0);
	m_varMES.syncL1.huBatchID = new TypeString(names.syncL1.huBatchID, start + 80,"");
	m_varMES.syncL1.huID = new TypeString(names.syncL1.huID, start + 81, "");
	m_varMES.syncL1.huQte = new TypeAna(names.syncL1.huQte, start + 82, 0);

	m_varMES.syncL2.mesSyncHU = new TypeAna(names.syncL2.mesSyncHU, start + 83, 0);
	m_varMES.syncL2.spvSyncHU = new TypeAna(names.syncL2.spvSyncHU, start + 84, 0);
	m_varMES.syncL2.huHH = new TypeAna(names.syncL2.huHH, start + 85, 0);
	m_varMES.syncL2.huMM = new TypeAna(names.syncL2.huMM, start + 86, 0);
	m_varMES.syncL2.huSS = new TypeAna(names.syncL2.huSS, start + 87, 0);
	m_varMES.syncL2.huContID = new TypeAna(names.syncL2.huContID, start + 88, 0);
	m_varMES.syncL2.huBatchID = new TypeString(names.syncL2.huBatchID, start + 89, "");
	m_varMES.syncL2.huID = new TypeString(names.syncL2.huID, start + 90, "");
	m_varMES.syncL2.huQte = new TypeAna(names.syncL2.huQte, start + 91, 0);
	m_varMES.setupL1.provenance = new TypeBool(names.setupL1.provenance, start + 92, 0);
	m_varMES.setupL2.provenance = new TypeBool(names.setupL2.provenance, start + 93, 0);
	m_varMES.syncL1.batchGoodPartQty = new TypeAna(names.syncL1.batchGoodPartQty, start + 94, 0);
	m_varMES.syncL2.batchGoodPartQty = new TypeAna(names.syncL2.batchGoodPartQty, start + 95, 0);

	m_varMES.plcCounterL1 = new TypeAna(names.plcCounterL1, start + 96, 0);
	m_varMES.plcCounterL2 = new TypeAna(names.plcCounterL2, start + 97, 0);

	m_varMES.commEnsL1 = new TypeBool(names.commEnsL1, start + 98, 0);
	m_varMES.commEnsL2 = new TypeBool(names.commEnsL2, start + 99, 0);
	m_varMES.commEtq = new TypeBool(names.commEtq, start + 100, 0);
	m_varMES.commFerm = new TypeBool(names.commFerm, start + 101, 0);
	m_varMES.commForm1 = new TypeBool(names.commForm1, start + 102, 0);
	m_varMES.commForm2 = new TypeBool(names.commForm2, start + 103, 0);
	m_varMES.commImp = new TypeBool(names.commImp, start + 104, 0);
	m_varMES.commPesL1 = new TypeBool(names.commPesL1, start + 105, 0);
	m_varMES.commPesL2 = new TypeBool(names.commPesL2, start + 106, 0);
	m_varMES.commRbb = new TypeBool(names.commRbb, start + 107, 0);
	m_varMES.commRe = new TypeBool(names.commRe, start + 108, 0);
	m_varMES.commRp = new TypeBool(names.commRp, start + 109, 0);
	m_varMES.commTr = new TypeBool(names.commTr, start + 110, 0);

	m_varMES.setupL1.handlingUnit0 = new TypeString(names.setupL1.handlingUnit0, start + 111, 0);
	m_varMES.setupL1.handlingUnit1 = new TypeString(names.setupL1.handlingUnit1, start + 112, 0);
	m_varMES.setupL1.handlingUnit2 = new TypeString(names.setupL1.handlingUnit2, start + 113, 0);
	m_varMES.setupL1.handlingUnitSize0 = new TypeAna(names.setupL1.handlingUnitSize0, start + 114, 0);
	m_varMES.setupL1.handlingUnitSize1 = new TypeAna(names.setupL1.handlingUnitSize1, start + 115, 0);
	m_varMES.setupL1.handlingUnitSize2 = new TypeAna(names.setupL1.handlingUnitSize2, start + 116, 0);
	m_varMES.setupL2.handlingUnit0 = new TypeString(names.setupL2.handlingUnit0, start + 117, 0);
	m_varMES.setupL2.handlingUnit1 = new TypeString(names.setupL2.handlingUnit1, start + 118, 0);
	m_varMES.setupL2.handlingUnit2 = new TypeString(names.setupL2.handlingUnit2, start + 119, 0);
	m_varMES.setupL2.handlingUnitSize0 = new TypeAna(names.setupL2.handlingUnitSize0, start + 120, 0);
	m_varMES.setupL2.handlingUnitSize1 = new TypeAna(names.setupL2.handlingUnitSize1, start + 121, 0);
	m_varMES.setupL2.handlingUnitSize2 = new TypeAna(names.setupL2.handlingUnitSize2, start + 122, 0);


}

double MES::GetRayProCounter(int ligne) {
	if (ligne == 1) {
		return this->m_varMES.rayproCounterL1->GetVarValue();
	} else {
		return this->m_varMES.rayproCounterL2->GetVarValue();
	}
	
}

void MES::IncrementBitDeViePLC(int ligne) {
	if (ligne == 1) {
		this->m_varMES.plcCounterL1->WriteVar(fmod((this->m_varMES.plcCounterL1->GetVarValue() + 1), 10000));
	}
	else {
		this->m_varMES.plcCounterL2->WriteVar(fmod((this->m_varMES.plcCounterL2->GetVarValue() + 1), 10000));
	}
}

bool MES::GetValidCampagne(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varMES.syncL1.mesStartReq->GetVarValue();
	}
	else
	{
		retour = this->m_varMES.syncL2.mesStartReq->GetVarValue();
	}
	return retour;
}

bool MES::SetValidFinCampagne(int ligne) {
	if (ligne == 1) {
		this->m_varMES.syncL1.plcBatchReqStatus->WriteVar(true);
	}
	else {
		this->m_varMES.syncL2.plcBatchReqStatus->WriteVar(true);
	}
	return 1;
}

bool MES::SetValidCampagne(int ligne) {
	if (ligne == 1) {
		LOG_DEBUG << "Ecriture a 1 de spvStartReq";
		this->m_varMES.syncL1.spvStartReq->WriteVar(true);
	}
	else {
		this->m_varMES.syncL2.spvStartReq->WriteVar(true);
	}
	return 1;
}

bool MES::ResetValidCampagne(int ligne) {
	if (ligne == 1) {

		this->m_varMES.syncL1.spvStartReq->WriteVar(false);
	}
	else {
		this->m_varMES.syncL2.spvStartReq->WriteVar(false);
	}
	return 1;
}

bool MES::SetPlcBatchReqStatus(int ligne, double val) {
	if (ligne == 1) {
		this->m_varMES.syncL1.plcBatchReqStatus->WriteVar(val);
	}
	else {
		this->m_varMES.syncL2.plcBatchReqStatus->WriteVar(val);
	}
	return 1;
}

bool MES::SetSyncTime(int ligne) {
	//Si mesSyncTime > 0 - plcSyncTime = MesSyncTime
	//Si mesSyncTime = 0 - plcSyncTime = 0
	if (ligne == 1) {
		if (this->m_varMES.syncL1.mesSyncTime->GetVarValue() > 0) {
			this->m_varMES.syncL1.plcSyncTime->WriteVar(this->m_varMES.syncL1.mesSyncTime->GetVarValue());
		}
		if (this->m_varMES.syncL1.mesSyncTime->GetVarValue() == 0) {
			this->m_varMES.syncL1.plcSyncTime->WriteVar(0);
		}
	}
	else {
		if (this->m_varMES.syncL2.mesSyncTime->GetVarValue() > 0) {
			this->m_varMES.syncL2.plcSyncTime->WriteVar(this->m_varMES.syncL2.mesSyncTime->GetVarValue());
		}
		if (this->m_varMES.syncL2.mesSyncTime->GetVarValue() == 0) {
			this->m_varMES.syncL2.plcSyncTime->WriteVar(0);
		}
	}
	return 1;
}

bool MES::ResetValidFinCampagne(int ligne) {
	if (ligne == 1) {
		this->m_varMES.syncL1.plcBatchReqStatus->WriteVar(false);
	}
	else {
		this->m_varMES.syncL2.plcBatchReqStatus->WriteVar(false);
	}
	return 1;
}

double MES::GetMesSyncSetup(int ligne) {
	double retour;
	if (ligne == 1) {
		retour = this->m_varMES.syncL1.mesSyncSetup->GetVarValue();
	}
	else
	{
		retour = this->m_varMES.syncL2.mesSyncSetup->GetVarValue();
	}
	return retour;
}

double MES::GetspvSyncSetup(int ligne) {
	double retour;
	if (ligne == 1) {
		retour = this->m_varMES.syncL1.plcSyncSetup->GetVarValue();
	}
	else
	{
		retour = this->m_varMES.syncL2.plcSyncSetup->GetVarValue();
	}
	return retour;
}

double MES::GetMesSyncStoppage(int ligne) {
	if (ligne == 1) {
		return this->m_varMES.syncL1.mestoppageSync->GetVarValue();
	}
	else {
		return this->m_varMES.syncL2.mestoppageSync->GetVarValue();
	}
	
}

double MES::GetspvSyncStoppage(int ligne) {
	if (ligne == 1) {
		return this->m_varMES.syncL1.spvtoppageSync->GetVarValue();
	}
	else
	{
		return this->m_varMES.syncL2.spvtoppageSync->GetVarValue();
	}
	
}

double MES::SetspvSyncSetup(int ligne, double val) {
	if (ligne == 1) {
		this->m_varMES.syncL1.plcSyncSetup->WriteVar(val);
		LOG_DEBUG << "(L" << ligne << ") Ecriture du plcSyncSetup a : " << val << "";
	}
	else {
		this->m_varMES.syncL2.plcSyncSetup->WriteVar(val);
	}
	return 1;
}

double MES::SetspvSyncStoppage(int ligne,double val) {
	if (ligne == 1) {
		this->m_varMES.syncL1.spvtoppageSync->WriteVar(val);
		LOG_DEBUG << "-->" << m_varMES.syncL1.spvtoppageSync->GetVarName() << "";
	}
	else {
		this->m_varMES.syncL2.spvtoppageSync->WriteVar(val);
	}
	
	return 1;
}

sFlatSetup MES::GetMesSetup(int ligne) {
	sFlatSetup temp;

	if (ligne == 1) {
		
		temp.refPieces = this->m_varMES.setupL1.refPieces->GetVarValue();
		
		temp.descProduit = this->m_varMES.setupL1.descProduit->GetVarValue();
		
		temp.idOperateur = this->m_varMES.setupL1.idOperateur->GetVarValue();
		
		temp.numLot = this->m_varMES.setupL1.numLot->GetVarValue();
		
		temp.numOF = this->m_varMES.setupL1.numOF->GetVarValue();
		
		temp.nbPieces = this->m_varMES.setupL1.nbPieces->GetVarValue();
		

		//Partie dépendant du sachet ou non
		//Si rien dans HandlingUnit2 = vrac
		//Attention : Les variables ne sont pas écrasées entre Setups, donc il peut y avoir un résidu
		//de la campagne précédente dans handlingunit2
		//Donc si HandlingUnit1=HandlingUnit2="Palette", on est toujours en vrac
		if (this->m_varMES.setupL1.handlingUnitSize2->GetVarValue() == 0 
			|| this->m_varMES.setupL1.handlingUnit1->GetVarValue() == this->m_varMES.setupL1.handlingUnit2->GetVarValue() )
		{
			temp.nbPiecesPSachet = 0;
			
			temp.nbPiecesPContenant = this->m_varMES.setupL1.handlingUnitSize0->GetVarValue();
			
			temp.nbPiecesPPalette = this->m_varMES.setupL1.handlingUnitSize1->GetVarValue();
			
			temp.typeConditionnement = this->m_varMES.setupL1.handlingUnit0->GetVarValue();
			
			
		}
		else {
			temp.nbPiecesPSachet = this->m_varMES.setupL1.handlingUnitSize0->GetVarValue();
			
			temp.nbPiecesPContenant = this->m_varMES.setupL1.handlingUnitSize1->GetVarValue();
			
			temp.nbPiecesPPalette = this->m_varMES.setupL1.handlingUnitSize2->GetVarValue();
			
			temp.typeConditionnement = this->m_varMES.setupL1.handlingUnit1->GetVarValue();
			
		}
		//Transfert dans les vars OPC
		this->m_varMES.setupL1.nbPiecesPSachet->WriteVar(temp.nbPiecesPSachet);
		this->m_varMES.setupL1.nbPiecesPContenant->WriteVar(temp.nbPiecesPContenant);
		this->m_varMES.setupL1.nbPiecesPPalette->WriteVar(temp.nbPiecesPPalette);
		this->m_varMES.setupL1.typeConditionnement->WriteVar(temp.typeConditionnement);


		temp.hauteurSachet = this->m_varMES.setupL1.hauteurSachet->GetVarValue();
		
		temp.poidsPiece = this->m_varMES.setupL1.poidsPiece->GetVarValue();
		
		temp.adjonctionEau = this->m_varMES.setupL1.adjonctionEau->GetVarValue();

		temp.provenance = this->m_varMES.setupL1.provenance->GetVarValue();
		
		
	}
	else {
		
		temp.refPieces = this->m_varMES.setupL2.refPieces->GetVarValue();
		
		temp.descProduit = this->m_varMES.setupL2.descProduit->GetVarValue();
		
		temp.idOperateur = this->m_varMES.setupL2.idOperateur->GetVarValue();
		
		temp.numLot = this->m_varMES.setupL2.numLot->GetVarValue();
		
		temp.numOF = this->m_varMES.setupL2.numOF->GetVarValue();
		
		temp.nbPieces = this->m_varMES.setupL2.nbPieces->GetVarValue();
		
		//Partie dépendant du sachet ou non
		//Si rien dans HandlingUnit2 = vrac
		if (this->m_varMES.setupL2.handlingUnitSize2->GetVarValue() == 0
			|| this->m_varMES.setupL2.handlingUnit1->GetVarValue() == this->m_varMES.setupL2.handlingUnit2->GetVarValue())
		{
			temp.nbPiecesPSachet = 0;
			
			temp.nbPiecesPContenant = this->m_varMES.setupL2.handlingUnitSize0->GetVarValue();
			
			temp.nbPiecesPPalette = this->m_varMES.setupL2.handlingUnitSize1->GetVarValue();
			
			temp.typeConditionnement = this->m_varMES.setupL2.handlingUnit0->GetVarValue();
			

		}
		else {
			temp.nbPiecesPSachet = this->m_varMES.setupL2.handlingUnitSize0->GetVarValue();
			
			temp.nbPiecesPContenant = this->m_varMES.setupL2.handlingUnitSize1->GetVarValue();
			
			temp.nbPiecesPPalette = this->m_varMES.setupL2.handlingUnitSize2->GetVarValue();
			
			temp.typeConditionnement = this->m_varMES.setupL2.handlingUnit1->GetVarValue();
			
		}

		//Transfert dans les vars OPC
		this->m_varMES.setupL2.nbPiecesPSachet->WriteVar(temp.nbPiecesPSachet);
		this->m_varMES.setupL2.nbPiecesPContenant->WriteVar(temp.nbPiecesPContenant);
		this->m_varMES.setupL2.nbPiecesPPalette->WriteVar(temp.nbPiecesPPalette);
		this->m_varMES.setupL2.typeConditionnement->WriteVar(temp.typeConditionnement);

		temp.hauteurSachet = this->m_varMES.setupL2.hauteurSachet->GetVarValue();
		
		temp.poidsPiece = this->m_varMES.setupL2.poidsPiece->GetVarValue();
	
		temp.adjonctionEau = this->m_varMES.setupL2.adjonctionEau->GetVarValue();

		temp.provenance = this->m_varMES.setupL2.provenance->GetVarValue();
		
	}
	


	return  temp;
}

bool MES::SetStoppageDatas(int id, double num, bool val) {
	const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();


	//Si entre 0 et 299 : défaut L1
	//if (num <= 299) {
	if (val == true) {
		this->m_varMES.syncL1.tsStoppageHH->WriteVar(double(now.time_of_day().hours()));
		this->m_varMES.syncL1.tsStoppageMM->WriteVar(double(now.time_of_day().minutes()));
		this->m_varMES.syncL1.tsStoppageSS->WriteVar(double(now.time_of_day().seconds()));
		//this->m_varMES.syncL1.numStoppageCause->WriteVar(double(id + num));
		this->m_varMES.syncL1.numStoppageCause->WriteVar(num);
		this->m_varMES.syncL1.numStoppageStation->WriteVar(double(id));
		this->m_varMES.syncL1.idStoppageCause->WriteVar(val);

	}
	else
	{ 
		this->m_varMES.syncL1.tsStoppageHH->WriteVar(0);
		this->m_varMES.syncL1.tsStoppageMM->WriteVar(0);
		this->m_varMES.syncL1.tsStoppageSS->WriteVar(0);
		//this->m_varMES.syncL1.numStoppageCause->WriteVar(double(id + num));
		this->m_varMES.syncL1.numStoppageCause->WriteVar(0);
		this->m_varMES.syncL1.numStoppageStation->WriteVar(0);
		this->m_varMES.syncL1.idStoppageCause->WriteVar(val);
	
	}
	//Si entre 334 et 599 : défaut L2
	/*else if (num <= 599){
		this->m_varMES.syncL2.tsStoppageHH->WriteVar(double(now.time_of_day().hours()));
		this->m_varMES.syncL2.tsStoppageMM->WriteVar(double(now.time_of_day().minutes()));
		this->m_varMES.syncL2.tsStoppageSS->WriteVar(double(now.time_of_day().seconds()));
		//this->m_varMES.syncL2.numStoppageCause->WriteVar(double(id + num));
		this->m_varMES.syncL2.numStoppageCause->WriteVar(num);
		this->m_varMES.syncL2.numStoppageStation->WriteVar(double(id));
		this->m_varMES.syncL2.idStoppageCause->WriteVar(val);
	}
	//Si entre 600 et 999 : défaut sur les 2 lignes
	else {
		this->m_varMES.syncL1.tsStoppageHH->WriteVar(double(now.time_of_day().hours()));
		this->m_varMES.syncL1.tsStoppageMM->WriteVar(double(now.time_of_day().minutes()));
		this->m_varMES.syncL1.tsStoppageSS->WriteVar(double(now.time_of_day().seconds()));
		//this->m_varMES.syncL1.numStoppageCause->WriteVar(double(id + num));
		this->m_varMES.syncL1.numStoppageCause->WriteVar(num);
		this->m_varMES.syncL1.numStoppageStation->WriteVar(double(id));
		this->m_varMES.syncL1.idStoppageCause->WriteVar(val);

		this->m_varMES.syncL2.tsStoppageHH->WriteVar(double(now.time_of_day().hours()));
		this->m_varMES.syncL2.tsStoppageMM->WriteVar(double(now.time_of_day().minutes()));
		this->m_varMES.syncL2.tsStoppageSS->WriteVar(double(now.time_of_day().seconds()));
		//this->m_varMES.syncL2.numStoppageCause->WriteVar(double(id + num));
		this->m_varMES.syncL2.numStoppageCause->WriteVar(num);
		this->m_varMES.syncL2.numStoppageStation->WriteVar(double(id));
		this->m_varMES.syncL2.idStoppageCause->WriteVar(val);
	}*/
	

	return 1;
}

double MES::GetMesSyncHU(int ligne)
{
	double retour;
	if (ligne == 1) {
		retour = this->m_varMES.syncL1.mesSyncHU->GetVarValue();
	}
	else
	{
		retour = this->m_varMES.syncL2.mesSyncHU->GetVarValue();
	}
	return retour;
}

double MES::GetspvSyncHU(int ligne)
{
	double retour;
	if (ligne == 1) {
		retour = this->m_varMES.syncL1.spvSyncHU->GetVarValue();
	}
	else
	{
		retour = this->m_varMES.syncL2.spvSyncHU->GetVarValue();
	}
	return retour;
}

double MES::SetspvSyncHU(int ligne, double val)
{
	LOG_DEBUG << "Ecriture de PLCSYNCHU a " << val << "";
	if (ligne == 1) {
		this->m_varMES.syncL1.spvSyncHU->WriteVar(val);
	}
	else {
		this->m_varMES.syncL2.spvSyncHU->WriteVar(val);
	}

	return 1;
}

bool MES::SetHUDatas(int ligne, huInput hu) {
	//Changement heure pour timezone vers Raypro
	if (hu.hour == 0) {
		hu.hour = 23;
	}
	else {
		hu.hour -= 1;
	}


	if (ligne == 1) {
		this->m_varMES.syncL1.huHH->WriteVar(double(hu.hour));
		this->m_varMES.syncL1.huMM->WriteVar(double(hu.minute));
		this->m_varMES.syncL1.huSS->WriteVar(double(hu.second));
		this->m_varMES.syncL1.huContID->WriteVar(hu.containerID);
		this->m_varMES.syncL1.huBatchID->WriteVar(hu.batchID);
		this->m_varMES.syncL1.huID->WriteVar(hu.huID);
		this->m_varMES.syncL1.huQte->WriteVar(hu.containerQuantity);

	}
	else
	{
		this->m_varMES.syncL2.huHH->WriteVar(double(hu.hour));
		this->m_varMES.syncL2.huMM->WriteVar(double(hu.minute));
		this->m_varMES.syncL2.huSS->WriteVar(double(hu.second));
		this->m_varMES.syncL2.huContID->WriteVar(hu.containerID);
		this->m_varMES.syncL2.huBatchID->WriteVar(hu.batchID);
		this->m_varMES.syncL2.huID->WriteVar(hu.huID);
		this->m_varMES.syncL2.huQte->WriteVar(hu.containerQuantity);

	}

	return 1;
}

bool MES::SetGoodPartQty(int ligne, double nbCont)
{
	//Nombre de pièces conditionnées, il faut bien le multiplier par le 
	// nombre de pièces par contenant
	if (ligne == 1) {
		this->m_varMES.syncL1.batchGoodPartQty->WriteVar(nbCont * vFlatSetup.at(ligne - 1).nbPiecesPContenant);
	}
	else {
		this->m_varMES.syncL2.batchGoodPartQty->WriteVar(nbCont * vFlatSetup.at(ligne - 1).nbPiecesPContenant);

	}
	return false;
}

bool MES::setProvenance(int ligne, bool val)
{
	if (ligne == 1) {
		this->m_varMES.setupL1.provenance->WriteVar(val);
	}
	else {
		this->m_varMES.setupL2.provenance->WriteVar(val);
	}

	return true;
}

bool MES::GetProvenance(int ligne)
{
	if (ligne == 1) {
		return this->m_varMES.setupL1.provenance->GetVarValue();
	}
	else {
		return this->m_varMES.setupL2.provenance->GetVarValue();
	}
}

bool MES::GetMesBatchReqStatus(int ligne) {
	/*
	Ajout 10/02/2020
	*/
	if (ligne == 1) {
		return this->m_varMES.syncL1.mesBatchReq->GetVarValue();
	}
	else {
		return this->m_varMES.syncL2.mesBatchReq->GetVarValue();
	}
}

//Destructors
MES::~MES() {}

//--------------------------------SPV
//Constructors

SPV::SPV() {};

SPV::SPV(sNameSPV names, int start) {


	m_varSPV.modeDegradeL1 = new TypeBool(names.modeDegradeL1, start, 0);
	m_varSPV.validSetupL1 = new TypeBool(names.validSetupL1, start + 1, 0);
	m_varSPV.validSetupL2 = new TypeBool(names.validSetupL2, start + 2, 0);
	m_varSPV.confirmLancProdL1 = new TypeBool(names.confirmLancProdL1, start + 3, 0);
	m_varSPV.confirmLancProdL2 = new TypeBool(names.confirmLancProdL2, start + 4, 0);
	m_varSPV.plcDemFinProdL1 = new TypeBool(names.plcDemFinProdL1, start + 5, 0);
	m_varSPV.plcDemFinProdL2 = new TypeBool(names.plcDemFinProdL2, start + 6, 0);

	
	//Setup Ligne 1
	m_varSPV.setupL1.refPieces = new TypeAna(names.setupL1.refPieces, start + 7, 0);
	m_varSPV.setupL1.descProduit = new TypeString(names.setupL1.descProduit, start + 8, "");
	m_varSPV.setupL1.idOperateur = new TypeAna(names.setupL1.idOperateur, start + 9, 0);
	m_varSPV.setupL1.numLot = new TypeString(names.setupL1.numLot, start + 10, "");
	m_varSPV.setupL1.numOF = new TypeAna(names.setupL1.numOF, start + 11, 0);
	m_varSPV.setupL1.nbPieces = new TypeAna(names.setupL1.nbPieces, start + 12, 0);
	m_varSPV.setupL1.nbPiecesPPalette = new TypeAna(names.setupL1.nbPiecesPPalette, start + 13, 0);
	m_varSPV.setupL1.nbPiecesPContenant = new TypeAna(names.setupL1.nbPiecesPContenant, start + 14, 0);
	m_varSPV.setupL1.typeConditionnement = new TypeString(names.setupL1.typeConditionnement, start + 15, "");
	m_varSPV.setupL1.nbPiecesPSachet = new TypeAna(names.setupL1.nbPiecesPSachet, start + 16, 0);
	m_varSPV.setupL1.hauteurSachet = new TypeAna(names.setupL1.hauteurSachet, start + 17, 0);
	m_varSPV.setupL1.poidsPiece = new TypeAna(names.setupL1.poidsPiece, start + 18, 0);
	m_varSPV.setupL1.adjonctionEau = new TypeAna(names.setupL1.adjonctionEau, start + 19, 0);
	

	//Setup Ligne 2
	m_varSPV.setupL2.refPieces = new TypeAna(names.setupL2.refPieces, start + 20, 0);
	m_varSPV.setupL2.descProduit = new TypeString(names.setupL2.descProduit, start + 21, "");
	m_varSPV.setupL2.idOperateur = new TypeAna(names.setupL2.idOperateur, start + 22, 0);
	m_varSPV.setupL2.numLot = new TypeString(names.setupL2.numLot, start + 23, "");
	m_varSPV.setupL2.numOF = new TypeAna(names.setupL2.numOF, start + 24, 0);
	m_varSPV.setupL2.nbPieces = new TypeAna(names.setupL2.nbPieces, start + 25, 0);
	m_varSPV.setupL2.nbPiecesPPalette = new TypeAna(names.setupL2.nbPiecesPPalette, start + 26, 0);
	m_varSPV.setupL2.nbPiecesPContenant = new TypeAna(names.setupL2.nbPiecesPContenant, start + 27, 0);
	m_varSPV.setupL2.typeConditionnement = new TypeString(names.setupL2.typeConditionnement, start + 28, "");
	m_varSPV.setupL2.nbPiecesPSachet = new TypeAna(names.setupL2.nbPiecesPSachet, start + 29, 0);
	m_varSPV.setupL2.hauteurSachet = new TypeAna(names.setupL2.hauteurSachet, start + 30, 0);
	m_varSPV.setupL2.poidsPiece = new TypeAna(names.setupL2.poidsPiece, start + 31, 0);
	m_varSPV.setupL2.adjonctionEau = new TypeAna(names.setupL2.adjonctionEau, start + 32, 0);

	//////////////////////////////////////

	m_varSPV.plcConfirmFinProdL1 = new TypeBool(names.plcConfirmFinProdL1, start + 33, 0);
	m_varSPV.plcConfirmFinProdL2 = new TypeBool(names.plcConfirmFinProdL2, start + 34, 0);
	m_varSPV.annulerProdL1 = new TypeBool(names.annulerProdL1, start + 35, 0);
	m_varSPV.annulerProdL2 = new TypeBool(names.annulerProdL2, start + 36, 0);
	m_varSPV.cptBacVersesL1 = new TypeAna(names.cptBacVersesL1, start + 37, 0);
	m_varSPV.cptBacVersesL2 = new TypeAna(names.cptBacVersesL2, start + 38, 0);

	m_varSPV.setupL1.provenance = new TypeBool(names.setupL1.provenance, start + 39, 0);
	m_varSPV.setupL2.provenance = new TypeBool(names.setupL2.provenance, start + 40, 0);
	m_varSPV.fatBtnManu = new TypeBool(names.fatBtnManu, start + 41, 0);
	m_varSPV.affF1 = new TypeAna(names.affF1, start + 42, 0);
	m_varSPV.affF2 = new TypeAna(names.affF2, start + 43, 0);
	m_varSPV.validAffForm1 = new TypeBool(names.validAffForm1, start + 44, 0);

	m_varSPV.modeDegradeL2 = new TypeBool(names.modeDegradeL2, start + 45, 0);

	m_varSPV.validAffForm2 = new TypeBool(names.validAffForm2, start + 46, 0);

	m_varSPV.passageDegradeL1 = new TypeBool(names.passageDegradeL1, start + 47, 0);
	m_varSPV.passageDegradeL2 = new TypeBool(names.passageDegradeL2, start + 48, 0);


	m_varSPV.annulerCampReglageL1 = new TypeBool(names.annulerCampReglageL1, start + 49, 0);
	m_varSPV.annulerCampReglageL2 = new TypeBool(names.annulerCampReglageL2, start + 50, 0);

	m_varSPV.campagneDegradeL1 = new TypeBool(names.campagneDegradeL1, start + 51, 0);
	m_varSPV.campagneDegradeL2 = new TypeBool(names.campagneDegradeL2, start + 52, 0);


	m_varSPV.bitPopupProvenanceL1 = new TypeBool(names.bitPopupProvenanceL1, start + 53, 0);
	m_varSPV.bitPopupProvenanceL2 = new TypeBool(names.bitPopupProvenanceL2, start + 54, 0);
	
	m_varSPV.choixProvenanceL1 = new TypeBool(names.choixProvenanceL1, start + 55, 0);
	m_varSPV.choixProvenanceL2 = new TypeBool(names.choixProvenanceL2, start + 56, 0);


	m_varSPV.HUcontIDL1 =	new TypeAna(names.HUcontIDL1, start + 57, 0);
	m_varSPV.HUbatchIDL1 =	new TypeString(names.HUbatchIDL1, start + 58, "");
	m_varSPV.HUIDL1 =		new TypeString(names.HUIDL1, start + 59, "");
	m_varSPV.HUqtyL1 =		new TypeAna(names.HUqtyL1, start + 60, 0);
	m_varSPV.validHUHumidification = new TypeBool(names.validHUHumidification, start + 61, 0);

	m_varSPV.passageDistantL1 = new TypeBool(names.passageDistantL1, start + 62, 0);
	m_varSPV.passageDistantL2 = new TypeBool(names.passageDistantL2, start + 63, 0);

	m_varSPV.bitPopupChoixFormeuseL1 = new TypeBool(names.bitPopupChoixFormeuseL1, start + 64, 0); //Ajout 03/02/20
	m_varSPV.bitPopupChoixFormeuseL2 = new TypeBool(names.bitPopupChoixFormeuseL2, start + 65, 0); //Ajout 03/02/20
	m_varSPV.ChoixFormeuseL1 = new TypeAna(names.choixFormeuseL1, start + 66, 0); //Ajout 07/02/20
	m_varSPV.ChoixFormeuseL2 = new TypeAna(names.choixFormeuseL2, start + 67, 0); //Ajout 07/02/20
	m_varSPV.provenancePieceL1 = new TypeBool(names.provenancePieceL1, start + 68, 0); // Ajout 11/02/2020
	m_varSPV.provenancePieceL2 = new TypeBool(names.provenancePieceL2, start + 69, 0); // Ajout 11/02/2020
	m_varSPV.rompuEnCours = new TypeBool(names.rompuEnCours, start + 70, 0); // Ajout 11/02/2020
	m_varSPV.spvNouveauSetupL1 = new TypeBool(names.spvNouveauSetupL1, start + 71, 0); // Ajout 17/03/2020
	m_varSPV.spvNouveauSetupL2 = new TypeBool(names.spvNouveauSetupL2, start + 72, 0); // Ajout 17/03/2020
	m_varSPV.etatCampagneL1 = new TypeString(names.etatCampagneL1, start + 73, ""); // Ajout 18/03/2020
	m_varSPV.etatCampagneL2 = new TypeString(names.etatCampagneL2, start + 74, ""); // Ajout 18/03/2020
	m_varSPV.bitPopupNbrPieceValideL1 = new TypeBool(names.bitPopupNbrPieceValideL1, start + 75, 0); // Ajout 18/03/2020
	m_varSPV.bitPopupNbrPieceValideL2 = new TypeBool(names.bitPopupNbrPieceValideL2, start + 76, 0); // Ajout 18/03/2020
	m_varSPV.nbrPieceValideL1 = new TypeBool(names.nbrPieceValideL1, start + 77, 0); // Ajout 18/03/2020
	m_varSPV.nbrPieceValideL2 = new TypeBool(names.nbrPieceValideL2, start + 78, 0); // Ajout 18/03/2020


	m_varSPV.actualisationRefConditionnement = new TypeBool(names.actualisationRefConditionnement, start + 79, 0); // Ajout 19/03/2020

}

bool SPV::GetEtatDegrade(int ligne) {
	if (ligne == 1) {
		return this->m_varSPV.modeDegradeL1->GetVarValue();
	}
	else {
		return this->m_varSPV.modeDegradeL2->GetVarValue();
	}
	
}

bool SPV::GetFatBtnManu() {
	return this->m_varSPV.fatBtnManu->GetVarValue();
}

bool SPV::SetFatBtnManu(bool val) {
	this->m_varSPV.fatBtnManu->WriteVar(val);
	return 1;
}

void SPV::ResetValidAffForm(int formeuse)
{
	if (formeuse == 1) {
		this->m_varSPV.validAffForm1->WriteVar(false);
	}
	else {
		this->m_varSPV.validAffForm2->WriteVar(false);
	}
}

/*bool SPV::GetDmeDegrade(int ligne)
{
	if (ligne == 1) {
		return m_varSPV.passageDegradeL1->GetVarValue();
	}
	else {
		return m_varSPV.passageDegradeL2->GetVarValue();
	}
}*/

bool SPV::ChangeAffFormeuse(int formeuse) {

	/*

	MESI : J'enleve tout ça, ça ne marche pas

	LOG_DEBUG << "ChangeAffFormeuse(" << formeuse << ")";
	//Comparer les changements
	if (formeuse == 1 && this->m_varSPV.affF1->GetVarValue() != sVarEqp.APItransitique->GetAffFormAPI(1)) {
		//Lancement d'une séquence de propreté/configuration
		m_g7_formeuse1 = true;
		LOG_DEBUG << "m_g7_formeuse1 = true;";
		//MAJ coté API
		//sVarEqp.APItransitique->UpdateAffFormAPI(1, f1);
	}

	if (formeuse == 2 && this->m_varSPV.affF2->GetVarValue() != sVarEqp.APItransitique->GetAffFormAPI(2)) {
		//Lancement d'une séquence de propreté/configuration
		m_g7_formeuse2 = true;
		LOG_DEBUG << "m_g7_formeuse2 = true;";

		//MAJ coté API
		//sVarEqp.APItransitique->UpdateAffFormAPI(2, f2);
	}
	*/

	// _OLD_FORMEUSE
	/*//MESI : je fais une fonction bête et méchante
	//Pour la formeuse 1
	if (formeuse == 1) {
		m_g7_formeuse1 = true;
		
	}
	else {
		m_g7_formeuse2 = true;
	}*/

	//On parle pas en formeuse mais en ligne
	if (formeuse == 1) {
		if (this->m_varSPV.affF1->GetVarValue() == 0) {
			sVarEqp.APItransitique->SetLigneFormeuse(0, formeuse);
			this->m_varSPV.validAffForm1->WriteVar(false);
			LOG_DEBUG << "affF1 = 0";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 1) {
			sVarEqp.APItransitique->SetLigneFormeuse(1, formeuse);
			m_g7_formeuse1 = true;
			LOG_DEBUG << "affF1 = 1";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 2) {
			sVarEqp.APItransitique->SetLigneFormeuse(2, formeuse);
			m_g7_formeuse2 = true;
			LOG_DEBUG << "affF1 = 2";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 3) {
			sVarEqp.APItransitique->SetLigneFormeuse(3, formeuse);
			this->m_varSPV.validAffForm1->WriteVar(false);
			LOG_DEBUG << "affF1 = 3";
		}
	}
	else {
		if (this->m_varSPV.affF1->GetVarValue() == 0) {
			sVarEqp.APItransitique->SetLigneFormeuse(0, formeuse);
			this->m_varSPV.validAffForm2->WriteVar(false);
			LOG_DEBUG << "affF1 = 0";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 1) {
			sVarEqp.APItransitique->SetLigneFormeuse(1, formeuse);
			m_g7_formeuse1 = true;
			LOG_DEBUG << "affF1 = 1";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 2) {
			sVarEqp.APItransitique->SetLigneFormeuse(2, formeuse);
			m_g7_formeuse2 = true;
			LOG_DEBUG << "affF1 = 2";
		}
		if (this->m_varSPV.affF1->GetVarValue() == 3) {
			sVarEqp.APItransitique->SetLigneFormeuse(3, formeuse);
			this->m_varSPV.validAffForm2->WriteVar(false);
			LOG_DEBUG << "affF1 = 3";
		}
	}
	return false;
}

bool SPV::Getm_g7_formeuse(int formeuse)
{
	bool retour = false;
	if (formeuse == 1) {
		retour = m_g7_formeuse1;
	}
	else {
		retour = m_g7_formeuse2;
	}
	//LOG_DEBUG << "Getm_g7_formeuse = (F" << formeuse << ") " << retour << "";
	return retour;
}

bool SPV::Setm_g7_formeuse(int formeuse)
{
	LOG_DEBUG << "Passage dans Setm_g7_formeuse (formeuse = " << formeuse << ")";
	// MESI : on vire tout ca
	//Si désactivation, on raz le bit SPV et on affecte bien les formeuses
	/*if (val == false) {
		this->m_varSPV.validAffForm1->WriteVar(false);
		sVarEqp.APItransitique->SetLigneFormeuse(formeuse, (int)this->m_varSPV.affF1->GetVarValue());
	}
	m_g7_formeuse1 = val;*/
	int newAffFormeuse = 0;

	if (formeuse == 1) {
		newAffFormeuse = (int)this->m_varSPV.affF1->GetVarValue();
	}
	else {
		newAffFormeuse = (int)this->m_varSPV.affF2->GetVarValue();
	}
	int oldAffL1 = sVarEqp.APItransitique->GetAffFormAPI(1);
	int oldAffL2 = sVarEqp.APItransitique->GetAffFormAPI(2);

	LOG_DEBUG << "newAffFormeuse = " << newAffFormeuse << "";
	LOG_DEBUG << "oldAffL1 = " << oldAffL1 << "";
	LOG_DEBUG << "oldAffL2 = " << oldAffL2 << "";

	//Si on l'affecte à la ligne 1
	if (newAffFormeuse == 1) {
		//Il faut mettre F1 dans Affectation L1.
		sVarEqp.APItransitique->SetLigneFormeuse(formeuse, 1);
		//Si la ligne 2 avait la formeuse 1, on la retire
		if (oldAffL2 == formeuse) {
			sVarEqp.APItransitique->SetLigneFormeuse(0, 2);
		}	
	}
	//Si on l'affecte à la ligne 2
	if (newAffFormeuse == 2) {
		//Il faut mettre F1 dans Affectation L2.
		sVarEqp.APItransitique->SetLigneFormeuse(formeuse, 2);
		//Si la ligne 1 avait la formeuse 2, on la retire
		if (oldAffL2 == formeuse) {
			sVarEqp.APItransitique->SetLigneFormeuse(0, 1);
		}
	}
	//Si on l'affecte à la ligne 1 et 2
	if (newAffFormeuse == 3) {
		//Il faut mettre F1 dans Affectation L2.
		sVarEqp.APItransitique->SetLigneFormeuse(formeuse, 2);
		//Il faut mettre F1 dans Affectation L1.
		sVarEqp.APItransitique->SetLigneFormeuse(formeuse, 1);
	}
	//Si on enleve l'affectation
	if (newAffFormeuse == 0) {
		//Il faut l'enlever dans Affectation L2.
		if (oldAffL2 >= formeuse) {
			sVarEqp.APItransitique->SetLigneFormeuse((oldAffL2 - formeuse), 2);
		}
		if (oldAffL1 >= formeuse) {
			//Il faut l'enlever dans Affectation L1.
			sVarEqp.APItransitique->SetLigneFormeuse((oldAffL1 - formeuse), 1);
		}
	}

	//RAZ var interne formeuse et bouton SCADA
	if (formeuse == 1) {
		m_g7_formeuse1 = false;
		this->m_varSPV.validAffForm1->WriteVar(false);
	}
	else {
		m_g7_formeuse2 = false;
		this->m_varSPV.validAffForm2->WriteVar(false);
	}

	/*LOG_DEBUG << "m_g7_formeuse1 = " << m_g7_formeuse1 << "";
	LOG_DEBUG << "m_g7_formeuse2 = " << m_g7_formeuse2 << "";*/

	return false;
}

int SPV::GetValidAttrFormAdvNum(int formeuse)
{
	if (formeuse == 1) {
		return this->m_varSPV.validAffForm1->GetAdviseNumber();
	}
	else
	{
		return this->m_varSPV.validAffForm2->GetAdviseNumber();
	}
}

sFlatSetup SPV::GetSetupLocal(int ligne) {
	sFlatSetup temp;
	
	if (ligne == 1) {
		temp.refPieces = this->m_varSPV.setupL1.refPieces->GetVarValue();
		temp.descProduit = this->m_varSPV.setupL1.descProduit->GetVarValue();
		temp.idOperateur = this->m_varSPV.setupL1.idOperateur->GetVarValue();
		temp.numLot = this->m_varSPV.setupL1.numLot->GetVarValue();
		temp.numOF = this->m_varSPV.setupL1.numOF->GetVarValue();
		temp.nbPieces = this->m_varSPV.setupL1.nbPieces->GetVarValue();
		temp.nbPiecesPPalette = this->m_varSPV.setupL1.nbPiecesPPalette->GetVarValue();
		temp.nbPiecesPContenant = this->m_varSPV.setupL1.nbPiecesPContenant->GetVarValue();
		temp.typeConditionnement = this->m_varSPV.setupL1.typeConditionnement->GetVarValue();
		temp.nbPiecesPSachet = this->m_varSPV.setupL1.nbPiecesPSachet->GetVarValue();
		temp.hauteurSachet = this->m_varSPV.setupL1.hauteurSachet->GetVarValue();
		temp.poidsPiece = this->m_varSPV.setupL1.poidsPiece->GetVarValue();
		temp.adjonctionEau = this->m_varSPV.setupL1.adjonctionEau->GetVarValue();
		temp.provenance = this->m_varSPV.setupL1.provenance->GetVarValue();


	}
	else {
		temp.refPieces = this->m_varSPV.setupL2.refPieces->GetVarValue();
		temp.descProduit = this->m_varSPV.setupL2.descProduit->GetVarValue();
		temp.idOperateur = this->m_varSPV.setupL2.idOperateur->GetVarValue();
		temp.numLot = this->m_varSPV.setupL2.numLot->GetVarValue();
		temp.numOF = this->m_varSPV.setupL2.numOF->GetVarValue();
		temp.nbPieces = this->m_varSPV.setupL2.nbPieces->GetVarValue();
		temp.nbPiecesPPalette = this->m_varSPV.setupL2.nbPiecesPPalette->GetVarValue();
		temp.nbPiecesPContenant = this->m_varSPV.setupL2.nbPiecesPContenant->GetVarValue();
		temp.typeConditionnement = this->m_varSPV.setupL2.typeConditionnement->GetVarValue();
		temp.nbPiecesPSachet = this->m_varSPV.setupL2.nbPiecesPSachet->GetVarValue();
		temp.hauteurSachet = this->m_varSPV.setupL2.hauteurSachet->GetVarValue();
		temp.poidsPiece = this->m_varSPV.setupL2.poidsPiece->GetVarValue();
		temp.adjonctionEau = this->m_varSPV.setupL2.adjonctionEau->GetVarValue();
		temp.provenance = this->m_varSPV.setupL2.provenance->GetVarValue();
	}
	

	LOG_DEBUG << "Réception du Setup L" << ligne << " en local :";
	LOG_DEBUG << "Ref pièce : " << temp.refPieces << "";
	LOG_DEBUG << "Desc produit : " << temp.descProduit << "";
	LOG_DEBUG << "ID opérateur : " << temp.idOperateur << "";
	LOG_DEBUG << "Num lot : " << temp.numLot << "";
	LOG_DEBUG << "Num OF : " << temp.numOF << "";
	LOG_DEBUG << "Nb pièces : " << temp.nbPieces << "";
	LOG_DEBUG << "Pièces par palette : " << temp.nbPiecesPPalette << "";
	LOG_DEBUG << "Pièces par contenant : " << temp.nbPiecesPContenant << "";
	LOG_DEBUG << "Type conditionnement : " << temp.typeConditionnement << "";
	LOG_DEBUG << "Nombre pièces par sachet : " << temp.nbPiecesPSachet << "";
	LOG_DEBUG << "Hauteur sachet : " << temp.hauteurSachet << "";
	LOG_DEBUG << "Poids pièce : " << temp.poidsPiece << "";
	LOG_DEBUG << "Adjonction eau : " << temp.adjonctionEau << "";

	return  temp ;
}

bool SPV::ResetSetupLocal(int ligne)
{
	if (ligne == 1) {
		this->m_varSPV.setupL1.refPieces->WriteVar(0);
		this->m_varSPV.setupL1.descProduit->WriteVar(0);
		this->m_varSPV.setupL1.idOperateur->WriteVar(0);
		this->m_varSPV.setupL1.numLot->WriteVar("");
		this->m_varSPV.setupL1.numOF->WriteVar(0);
		this->m_varSPV.setupL1.nbPieces->WriteVar(0);
		this->m_varSPV.setupL1.nbPiecesPPalette->WriteVar(0);
		this->m_varSPV.setupL1.nbPiecesPContenant->WriteVar(0);
		this->m_varSPV.setupL1.typeConditionnement->WriteVar("");
		this->m_varSPV.setupL1.nbPiecesPSachet->WriteVar(0);
		this->m_varSPV.setupL1.hauteurSachet->WriteVar(0);
		this->m_varSPV.setupL1.poidsPiece->WriteVar(0);
		this->m_varSPV.setupL1.adjonctionEau->WriteVar(0);
		this->m_varSPV.setupL1.provenance->WriteVar(0);


	}
	else {
		this->m_varSPV.setupL2.refPieces->WriteVar(0);
		this->m_varSPV.setupL2.descProduit->WriteVar(0);
		this->m_varSPV.setupL2.idOperateur->WriteVar(0);
		this->m_varSPV.setupL2.numLot->WriteVar("");
		this->m_varSPV.setupL2.numOF->WriteVar(0);
		this->m_varSPV.setupL2.nbPieces->WriteVar(0);
		this->m_varSPV.setupL2.nbPiecesPPalette->WriteVar(0);
		this->m_varSPV.setupL2.nbPiecesPContenant->WriteVar(0);
		this->m_varSPV.setupL2.typeConditionnement->WriteVar("");
		this->m_varSPV.setupL2.nbPiecesPSachet->WriteVar(0);
		this->m_varSPV.setupL2.hauteurSachet->WriteVar(0);
		this->m_varSPV.setupL2.poidsPiece->WriteVar(0);
		this->m_varSPV.setupL2.adjonctionEau->WriteVar(0);
		this->m_varSPV.setupL2.provenance->WriteVar(0);
	}

	return true;
}

bool SPV::GetValidSetupLocal(int ligne) {

	bool retour;
	if (ligne == 1) {
		retour = this->m_varSPV.validSetupL1->GetVarValue();
	}
	else {
		retour = this->m_varSPV.validSetupL2->GetVarValue();
	}

	return retour;
}

bool SPV::ResetValidSetupLocal(int ligne)
{
	if (ligne == 1) {
		this->m_varSPV.validSetupL1->WriteVar(false);
	}
	else {
		this->m_varSPV.validSetupL2->WriteVar(false);
	}
	return 1;
}

bool SPV::GetValidCampagne(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.confirmLancProdL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.confirmLancProdL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetValidCampagne(int ligne)
{
	if (ligne == 1) {
		this->m_varSPV.confirmLancProdL1->WriteVar(false);
	}
	else {
		this->m_varSPV.confirmLancProdL2->WriteVar(false);
	}
	return 1;
}

bool SPV::GetDmeFinCampagne(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.plcDemFinProdL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.plcDemFinProdL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetDmeFinCampagne(int ligne)
{
	if (ligne == 1) {
		this->m_varSPV.plcDemFinProdL1->WriteVar(false);
	}
	else {
		this->m_varSPV.plcDemFinProdL2->WriteVar(false);
	}
	return 1;
}

void SPV::SetEtatDegrade(int ligne, bool val)
{
	if (ligne == 1) {
		this->m_varSPV.modeDegradeL1->WriteVar(val);	
	}
	else {
		this->m_varSPV.modeDegradeL2->WriteVar(val);
	}
	
}

bool SPV::GetValidFinCampagne(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.plcConfirmFinProdL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.plcConfirmFinProdL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetValidFinCampagne(int ligne) {
	if (ligne == 1) {
		this->m_varSPV.plcConfirmFinProdL1->WriteVar(false);
	}
	else {
		this->m_varSPV.plcConfirmFinProdL2->WriteVar(false);
	}
	return 1;
}

bool SPV::GetAnnulerCampagne(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.annulerCampReglageL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.annulerCampReglageL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetAnnulerCampagne(int ligne) {
	if (ligne == 1) {
		this->m_varSPV.annulerCampReglageL1->WriteVar(false);
	}
	else {
		this->m_varSPV.annulerCampReglageL2->WriteVar(false);
	}
	return 1;
}

double SPV::CalculMoyenneBac(int ligne)
{
	/*
	Modif 04/03/2020
	*/
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::posix_time::ptime oneHourBefore = boost::posix_time::second_clock::local_time() - boost::posix_time::hours(1);
	boost::posix_time::time_duration duree;

	LOG_DEBUG << "now = " << now << "";
	LOG_DEBUG << "oneHourBefore = " << oneHourBefore << "";
	if (ligne == 1) {
		//Ajouter l'entrée
		vMoyenneBacsL1.push_back(now);
		//Enlever les entrées trop vieilles
		for (unsigned int i = vMoyenneBacsL1.size(); i > 0 ; i--) {
			unsigned short j = i - 1;
			if (vMoyenneBacsL1.at(j) < oneHourBefore) {
				vMoyenneBacsL1.erase(vMoyenneBacsL1.begin() + j);
			}
		}

		/*
		duree = vMoyenneBacsL1.back() - vMoyenneBacsL1.front();
		if (duree.total_seconds() > 0) {
			//LOG_DEBUG << "duree = " << duree << " total_sec = " << duree.total_seconds() << "";
			this->m_varSPV.cptBacVersesL1->WriteVar(double((vMoyenneBacsL1.size() * 3600) / duree.total_seconds()));
			

		}
		*/
		LOG_DEBUG << "Moyenne bac/h L1 = " << double(vMoyenneBacsL1.size());
		//Affichage SCADA
		this->m_varSPV.cptBacVersesL1->WriteVar(double(vMoyenneBacsL1.size()));
	}
	else {
		//Ajouter l'entrée
		vMoyenneBacsL2.push_back(now);
		LOG_DEBUG << "Push_Back L2";
		//Enlever les entrées trop vieilles
		for (unsigned short i = 0; i < vMoyenneBacsL2.size(); i++) {
			unsigned short j = i - 1;
			LOG_DEBUG << "Boucle for : " << j;
			if (vMoyenneBacsL2.at(j) < oneHourBefore) {
				vMoyenneBacsL2.erase(vMoyenneBacsL2.begin() + j);
				LOG_DEBUG << "Erase L2";
			}
		}
		/*
		duree = vMoyenneBacsL2.back() - vMoyenneBacsL2.front();
		if (duree.total_seconds() > 0) {
			this->m_varSPV.cptBacVersesL2->WriteVar(double((vMoyenneBacsL2.size() * duree.total_seconds()) / 3600));
		}
		*/

		LOG_DEBUG << "Moyenne bac/h L2 = " << double(vMoyenneBacsL2.size());
		//Affichage SCADA
		this->m_varSPV.cptBacVersesL2->WriteVar(double(vMoyenneBacsL2.size()));
	}

	return 0;
}

double SPV::UpdateCalculMoyenneBac()
{
	/*
	Modif 04/03/2020
	*/

	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	boost::posix_time::ptime oneHourBefore = boost::posix_time::second_clock::local_time() - boost::posix_time::hours(1);
	boost::posix_time::time_duration duree;

	if(vMoyenneBacsL1.size() > 0){
		//Enlever les entrées trop vieilles
		for (unsigned int i = vMoyenneBacsL1.size(); i > 0; i--) {
			unsigned short j = i - 1;
			if (vMoyenneBacsL1.at(j) < oneHourBefore) {
				vMoyenneBacsL1.erase(vMoyenneBacsL1.begin() + j);
			}
		}
		/*
		if (vMoyenneBacsL1.size() > 0) {
			//MAJ 
			duree = vMoyenneBacsL1.back() - vMoyenneBacsL1.front();
			if (duree.total_seconds() > 0) {
				this->m_varSPV.cptBacVersesL1->WriteVar(double((vMoyenneBacsL1.size() * 3600) / duree.total_seconds()));

			}
		}
		*/

		//Mise a jour
		this->m_varSPV.cptBacVersesL1->WriteVar(double(vMoyenneBacsL1.size()));

	}
	
	if (vMoyenneBacsL2.size() > 0) {
		//Enlever les entrées trop vieilles
		for (unsigned int i = vMoyenneBacsL2.size(); i > 0; i--) {
			unsigned short j = i - 1;
			if (vMoyenneBacsL2.at(j) < oneHourBefore) {
				vMoyenneBacsL2.erase(vMoyenneBacsL2.begin() + j);
			}
		}
		/*
		//MAJ 
		if (vMoyenneBacsL2.size() > 0) {
			duree = vMoyenneBacsL2.back() - vMoyenneBacsL2.front();
			if (duree.total_seconds() > 0) {
				this->m_varSPV.cptBacVersesL2->WriteVar(double((vMoyenneBacsL2.size() * 3600) / duree.total_seconds()));

			}
		}
		*/

		//Mise a jour
		this->m_varSPV.cptBacVersesL2->WriteVar(double(vMoyenneBacsL2.size()));
	}

	return 0;
}

bool SPV::GetPassageDegrade(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.passageDegradeL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.passageDegradeL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetPassageDegrade(int ligne) {
	if (ligne == 1) {
		this->m_varSPV.passageDegradeL1->WriteVar(false);
	}
	else {
		this->m_varSPV.passageDegradeL2->WriteVar(false);
	}
	return 1;
}

void SPV::SetCampagneDegrade(int ligne, bool val) {
	if (ligne == 1) {
		this->m_varSPV.campagneDegradeL1->WriteVar(val);
	}
	else {
		this->m_varSPV.campagneDegradeL2->WriteVar(val);
	}
}

bool SPV::GetBitPopupProvenance(int ligne)
{
	if (ligne == 1) {
		return this->m_varSPV.bitPopupProvenanceL1->GetVarValue();
	}
	else {
		return this->m_varSPV.bitPopupProvenanceL2->GetVarValue();
	}
}

bool SPV::SetBitPopupProvenance(int ligne, bool val)
{
	if (ligne == 1) {
		this->m_varSPV.bitPopupProvenanceL1->WriteVar(val);
	}
	else {
		this->m_varSPV.bitPopupProvenanceL2->WriteVar(val);
	}

	return true;
}

bool SPV::GetChoixProvenance(int ligne)
{
	if (ligne == 1) {
		return this->m_varSPV.choixProvenanceL1->GetVarValue();
	}
	else {
		return this->m_varSPV.choixProvenanceL2->GetVarValue();
	}
}

bool SPV::SetChoixProvenance(int ligne, bool val)
{
	if (ligne == 1) {
		this->m_varSPV.choixProvenanceL1->WriteVar(val);
	}
	else {
		this->m_varSPV.choixProvenanceL2->WriteVar(val);
	}

	return true;
}

void SPV::SetSetup(int ligne, sFlatSetup setupDistant)
{
	if (ligne == 1) {
		this->m_varSPV.setupL1.refPieces->WriteVar(setupDistant.refPieces);
		this->m_varSPV.setupL1.descProduit->WriteVar(setupDistant.descProduit);
		this->m_varSPV.setupL1.idOperateur->WriteVar(setupDistant.idOperateur);
		this->m_varSPV.setupL1.numLot->WriteVar(setupDistant.numLot);
		this->m_varSPV.setupL1.numOF->WriteVar(setupDistant.numOF);
		this->m_varSPV.setupL1.nbPieces->WriteVar(setupDistant.nbPieces);
		this->m_varSPV.setupL1.nbPiecesPPalette->WriteVar(setupDistant.nbPiecesPPalette);
		this->m_varSPV.setupL1.nbPiecesPContenant->WriteVar(setupDistant.nbPiecesPContenant);
		this->m_varSPV.setupL1.typeConditionnement->WriteVar(setupDistant.typeConditionnement);
		this->m_varSPV.setupL1.nbPiecesPSachet->WriteVar(setupDistant.nbPiecesPSachet);
		this->m_varSPV.setupL1.hauteurSachet->WriteVar(setupDistant.hauteurSachet);
		this->m_varSPV.setupL1.poidsPiece->WriteVar(setupDistant.poidsPiece);
		this->m_varSPV.setupL1.adjonctionEau->WriteVar(setupDistant.adjonctionEau);
		this->m_varSPV.setupL1.provenance->WriteVar(setupDistant.provenance);

	}
	else {
		this->m_varSPV.setupL2.refPieces->WriteVar(setupDistant.refPieces);
		this->m_varSPV.setupL2.descProduit->WriteVar(setupDistant.descProduit);
		this->m_varSPV.setupL2.idOperateur->WriteVar(setupDistant.idOperateur);
		this->m_varSPV.setupL2.numLot->WriteVar(setupDistant.numLot);
		this->m_varSPV.setupL2.numOF->WriteVar(setupDistant.numOF);
		this->m_varSPV.setupL2.nbPieces->WriteVar(setupDistant.nbPieces);
		this->m_varSPV.setupL2.nbPiecesPPalette->WriteVar(setupDistant.nbPiecesPPalette);
		this->m_varSPV.setupL2.nbPiecesPContenant->WriteVar(setupDistant.nbPiecesPContenant);
		this->m_varSPV.setupL2.typeConditionnement->WriteVar(setupDistant.typeConditionnement);
		this->m_varSPV.setupL2.nbPiecesPSachet->WriteVar(setupDistant.nbPiecesPSachet);
		this->m_varSPV.setupL2.hauteurSachet->WriteVar(setupDistant.hauteurSachet);
		this->m_varSPV.setupL2.poidsPiece->WriteVar(setupDistant.poidsPiece);
		this->m_varSPV.setupL2.adjonctionEau->WriteVar(setupDistant.adjonctionEau);
		this->m_varSPV.setupL2.provenance->WriteVar(setupDistant.provenance);

	}
	
}

double SPV::GetHUcontID()
{
	return this->m_varSPV.HUcontIDL1->GetVarValue();
}

CString SPV::GetbatchID()
{
	return this->m_varSPV.HUbatchIDL1->GetVarValue();
}

CString SPV::GetHUID()
{
	return this->m_varSPV.HUIDL1->GetVarValue();
}

double SPV::GetHUqty()
{
	return this->m_varSPV.HUqtyL1->GetVarValue();
}

int SPV::GetValidHUHumidAdvNum()
{
	return this->m_varSPV.validHUHumidification->GetAdviseNumber();
}

void SPV::ResetValidHUHumid()
{
	this->m_varSPV.validHUHumidification->WriteVar(false);
}

void SPV::setProvenance(int ligne, bool val)
{
	if (ligne == 1) {
		this->m_varSPV.setupL1.provenance->WriteVar(val);
		sVarEqp.APItransitique->SetProvenance_L1(val);
	}
	else {
		this->m_varSPV.setupL2.provenance->WriteVar(val);
	}

}

bool SPV::GetPassageDistant(int ligne) {
	bool retour = false;
	if (ligne == 1) {
		retour = this->m_varSPV.passageDistantL1->GetVarValue();
	}
	else
	{
		retour = this->m_varSPV.passageDistantL2->GetVarValue();
	}
	return retour;
}

bool SPV::ResetPassageDistant(int ligne) {
	if (ligne == 1) {
		this->m_varSPV.passageDistantL1->WriteVar(false);
	}
	else {
		this->m_varSPV.passageDistantL2->WriteVar(false);
	}
	return 1;
}

bool SPV::GetBitPopupChoixFormeuse(int ligne)
{
	/*
	Ajout 03/02/20
	*/
	if (ligne == 1) {
		return this->m_varSPV.bitPopupChoixFormeuseL1->GetVarValue();
	}
	else {
		return this->m_varSPV.bitPopupChoixFormeuseL2->GetVarValue();
	}
}

bool SPV::SetBitPopupChoixFormeuse(int ligne, bool val)
{
	/*
	Ajout 03/02/20
	*/
	if (ligne == 1) {
		this->m_varSPV.bitPopupChoixFormeuseL1->WriteVar(val);
	}
	else {
		this->m_varSPV.bitPopupChoixFormeuseL2->WriteVar(val);
	}

	return true;
}

double SPV::GetChoixFormeuse(int ligne)
{
	/*
	Ajout 07/02/20
	*/
	if (ligne == 1) {
		return this->m_varSPV.ChoixFormeuseL1->GetVarValue();
	}
	else {
		return this->m_varSPV.ChoixFormeuseL2->GetVarValue();
	}
}

bool SPV::SetChoixFormeuse(int ligne, int val)
{
	/*
	Ajout 07/02/20
	*/
	if (ligne == 1) {
		this->m_varSPV.ChoixFormeuseL1->WriteVar(val);
	}
	else {
		this->m_varSPV.ChoixFormeuseL2->WriteVar(val);
	}

	return true;
}

bool SPV::SetProvenancePiece(int ligne, int val) {
	/*
	Ajout 11/02/2020
	*/
	if (ligne == 1) {
		this->m_varSPV.provenancePieceL1->WriteVar(val);
	}
	else {
		this->m_varSPV.provenancePieceL2->WriteVar(val);
	}
	
	return true;
}

bool SPV::SetRompuEnCoursResumer(bool val) {
	/*
	Ajout 11/02/2020
	*/

	this->m_varSPV.rompuEnCours->WriteVar(val);

	return true;
}

bool SPV::SetRompuEnCours(int ligne, bool val)
{
	/*
	Ajout 11/02/20
	*/
	if (ligne == 1) {
		this->m_varSPV.rompuEnCoursL1 = val;
	}
	else {
		this->m_varSPV.rompuEnCoursL2 = val;
	}

	return true;
}

bool SPV::GetRompuEnCours(int ligne)
{
	/*
	Ajout 11/02/20
	*/
	if (ligne == 1) {
		return this->m_varSPV.rompuEnCoursL1;
	}
	else {
		return this->m_varSPV.rompuEnCoursL2;
	}
}

bool SPV::GetspvNouveauSetup(int ligne) {
	/*
	Ajout 17/03/2020
	*/
	if (ligne == 1) {
		return this->m_varSPV.spvNouveauSetupL1->GetVarValue();
	}
	else {
		return this->m_varSPV.spvNouveauSetupL2->GetVarValue();
	}
}

bool SPV::SetspvNouveauSetup(int ligne, bool val)
{
	/*
	Ajout 17/03/2020
	*/
	if (ligne == 1) {
		this->m_varSPV.spvNouveauSetupL1->WriteVar(val);
	}
	else {
		this->m_varSPV.spvNouveauSetupL2->WriteVar(val);
	}

	return true;
}

bool SPV::SetEtatCampagne(int ligne, CString val)
{
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		this->m_varSPV.etatCampagneL1->WriteVar(val);
	}
	else {
		this->m_varSPV.etatCampagneL2->WriteVar(val);
	}

	return true;
}

bool SPV::GetBitNbrPieceValide(int ligne) {
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		return this->m_varSPV.bitPopupNbrPieceValideL1->GetVarValue();
	}
	else {
		return this->m_varSPV.bitPopupNbrPieceValideL2->GetVarValue();
	}
}

bool SPV::SetBitNbrPieceValide(int ligne, bool val)
{
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		this->m_varSPV.bitPopupNbrPieceValideL1->WriteVar(val);
	}
	else {
		this->m_varSPV.bitPopupNbrPieceValideL2->WriteVar(val);
	}

	return true;
}

bool SPV::GetNbrPieceValide(int ligne) {
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		return this->m_varSPV.nbrPieceValideL1->GetVarValue();
	}
	else {
		return this->m_varSPV.nbrPieceValideL2->GetVarValue();
	}
}

bool SPV::SetNbrPieceValide(int ligne, bool val)
{
	/*
	Ajout 18/03/2020
	*/
	if (ligne == 1) {
		this->m_varSPV.nbrPieceValideL1->WriteVar(val);
	}
	else {
		this->m_varSPV.nbrPieceValideL2->WriteVar(val);
	}

	return true;
}

bool SPV::GetActualisationRefConditionnement() {
	/*
	Ajout 19/03/2020
	*/
	
	return this->m_varSPV.actualisationRefConditionnement->GetVarValue();
	
}

bool SPV::SetActualisationRefConditionnement(bool val) {
	/*
	Ajout 19/03/2020
	*/

	 this->m_varSPV.actualisationRefConditionnement->WriteVar(val);

	 return true;
}

//--------------------------------Trémie
//Constructors

Tremie::Tremie() {};

Tremie::Tremie(sNameTremie names, int start) {


	m_varTremie.demandeMarche = new TypeBool(names.demandeMarche, start, 0);
	m_varCommun.etatAPI = new TypeAna(names.etatAPI, start + 1, 0);
	m_varCommun.demandeDistant = new TypeBool(names.demandeDistant, start + 2, 0);
	m_varCommun.modeDistant = new TypeBool(names.modeDistant, start + 3, 0);
	m_varCommun.alarmeNonBloquante = new TypeBool(names.alarmeNonBloquante, start + 4, 0);
	m_varCommun.alarmeBloquante = new TypeBool(names.alarmeBloquante, start + 5, 0);
	m_varCommun.numAlarme = new TypeAna(names.numAlarme, start + 6, 0);

	m_varTremie.timeoutMarcheAPI = new TypeBool(names.timeoutMarcheAPI, start + 20, 0);

	//IDmachine = ++ID;
	IDmachine = 9500;

}

bool Tremie::EnvoiDemMarche(int ligne) {
	bool retour = false;

	//Si déja en marche
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
		//LOG_INFO << "Equipement déja en marche (" << this->m_varCommun.etatAPI->GetVarName() << ")";
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	//Pas en marche
	else {
		//Si pas de demande, faire une demande
		if (!this->m_varTremie.demandeMarche->GetVarValue()) {
			//LOG_INFO << "Demande de marche Equipement (" << this->m_varCommun.demandeMarche->GetVarName() << ")";
			this->m_varTremie.demandeMarche->WriteVar(true);
		}
		else {
			cptTimeoutMarche++;
		}

		//test machine
		if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_PRODUCTION) {
			retour = true;
		}
	}
	if (cptTimeoutMarche >= TIMEOUT_CONFIG) {
		cptTimeoutMarche = 0;
		this->m_varTremie.demandeMarche->WriteVar(false);
		this->m_varCommun.timeoutMarcheAPI->WriteVar(true);
	}
	return retour;
}

bool Tremie::EnvoiDmeArret(int ligne) {
	bool retour = false;

	
	//Si pas de demande, faire une demande
	if (this->m_varTremie.demandeMarche->GetVarValue()) {
		this->m_varTremie.demandeMarche->WriteVar(false);
	}

	//test machine
	if (this->m_varCommun.etatAPI->GetVarValue() == ETAT_ARRET) {
		this->m_varCommun.timeoutMarcheAPI->WriteVar(false);
		retour = true;
	}
	
	return retour;
}

double GetMotEtat(	bool alarmeBloquante,
					bool alarmeNonBloquante,
					bool demandeProprete,
					bool demandeConfig,
					bool demandeVidange,
					bool machineSortie,
					bool timeoutConfig,
					bool timeoutCom,
					bool timeoutMarche,
					double etat)
{
	/*
	Modif 11/12/19
	*/
	double result;
	//Si défaut bloquant
	if (alarmeBloquante) {
		result = ETAT_DEF_BLOQUANT;
	}
	else if (timeoutConfig) {
		result = ETAT_DEF_BLOQUANT;
	}
	else if (timeoutCom) {
		result = ETAT_DEF_BLOQUANT;
	}
	else if (timeoutMarche) {
		result = ETAT_DEF_BLOQUANT;
	}
	//Si défaut non bloquant
	else if (alarmeNonBloquante) {
		result = ETAT_DEF_NON_BLOQUANT;
	}
	//Si aucun défaut
	else {
		//Test des demandes
		//Machine distante mais sortie de la campagne
		if (machineSortie) {
			result = 12;
		}
		//Propreté
		else if (demandeProprete && etat < ETAT_PROPRETE && etat != ETAT_PRODUCTION ) {
			result = ETAT_DEM_PROPRE;
		}
		//Config
		else if (demandeConfig && etat < ETAT_CONFIGURATION && etat != ETAT_PRODUCTION ) {
			result = ETAT_DEM_CONFIG;
		}
		//Vidange
		else if (demandeVidange) {
			result = ETAT_DEM_VIDANGE;
		}
		//Test des états
		//Propreté
		else if (etat == ETAT_PROPRETE) {
			result = ETAT_PROPRETE;
		}
		//Config
		else if (etat == ETAT_CONFIGURATION) {
			result = ETAT_CONFIGURATION;
		}
		//Vidange
		else if (etat == ETAT_VIDE) {
			result = ETAT_VIDE;
		}
		//Production
		else if (etat == ETAT_PRODUCTION) {
			result = ETAT_PRODUCTION;
		}
		//Arreté
		else if (etat == ETAT_ARRET) {
			result = ETAT_ARRET;
		}
		//Indéterminé
		else {
			result = ETAT_INDETERMINE;
		}
	}
	return result;
}
