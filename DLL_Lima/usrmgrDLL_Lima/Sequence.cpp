#include "StdAfx.h"
#include "Sequence.h"
#include "my_logger.h"

//Constructor
Sequence::Sequence(void) {
	//Var interne
	int tempEtape = 0;
	//1 sec for scrutation
	m_delay = 1000;
	m_countSec = 0;

	vNumEtape.push_back(new TypeAna("DLL.numEtapeL1", 10, 0));
	vNumEtape.push_back(new TypeAna("DLL.numEtapeL2", 11, 0));
	vNumEtape.at(0)->WriteVar(PROD_ATTENTE);
	vNumEtape.at(1)->WriteVar(PROD_ECHANGE);

}

//Start sequence
void Sequence::Start()
{
	m_tG7Main = boost::thread(&Sequence::G7Main, this);

}

//Stop sequence
void Sequence::Stop()
{
	m_tG7Main.interrupt();
	m_tG7Main.join();
}

//Check datas 
void Sequence::G7Main(){
	//Vars internes

	for (;;){
		try {
			const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

			for (unsigned short ligne = 1; ligne <= 2; ligne++) {

				//G7 PRODUCTION 
				switch ((int)vNumEtape.at(ligne - 1)->GetVarValue())
				{
				case PROD_ATTENTE:
					if (true) { //

						//Passage en étape ECHANGE
						vNumEtape.at(ligne - 1)->WriteVar(PROD_ECHANGE);
					}
					break;

				case PROD_ECHANGE:
					if (false) { //

						//Passage en étape ATTENTE
						vNumEtape.at(ligne - 1)->WriteVar(PROD_ATTENTE);
					}
					break;
				default:

					break;
				}//FIN G7



			}//FIN BOUCLE PAR LIGNE



			//Sleep entre scrutations
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));
			//incrément du compteur de secondes
			m_countSec = (m_countSec + 1) % 60;

		}
		catch (boost::thread_interrupted&)
		{
			//Thread interrupted
			return;
		}
	}	
}
