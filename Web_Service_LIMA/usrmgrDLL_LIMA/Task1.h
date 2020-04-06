#ifndef Task1_H
#define Task1_H
#include <boost/thread.hpp>


extern IAPIMgr * svmgrAPI;

class Task1
{
	int m_delay;
	boost::thread m_thread;

public:
	Task1();
	Task1(int delay);
	int getDelay();
	void setDelay(int delay);
	void DoSomething();
	void Start();
	void Stop();

	~Task1(void);
};

#endif