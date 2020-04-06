#include "stdafx.h"
#include "Task1.h"


Task1::Task1()
{
	m_delay = 987;
}


Task1::Task1(int delay)
{
	m_delay = delay;
}

int Task1::getDelay()
{
    return m_delay;
}

void Task1::setDelay(int delay)
{
	m_delay = delay;
//	m_thread.interrupt();
//	m_thread.join();
//	this->Start();
}

void Task1::DoSomething()
{
	int counter = 0;
	for (;;)
	{
		counter = (counter + 1) % 5000;
//		svmgrAPI->AnaVarWrite ("VARS.V3", counter, 1);
//		svmgrAPI->AnaVarWrite ("VARS.V5", m_delay, 2);
		try
		{
			// Sleep and check for interrupt.
			// To check for interrupt without sleep,
			// use boost::this_thread::interruption_point()
			// which also throws boost::thread_interrupted
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_delay));
		}
		catch (boost::thread_interrupted&)
		{
			//Thread interrupted
			return;
		}
	}
}

void Task1::Start(){
	
	m_thread = boost::thread(&Task1::DoSomething, this);
}

void Task1::Stop(){
	
	m_thread.interrupt();
	m_thread.join();
}

Task1::~Task1(void) {

}

