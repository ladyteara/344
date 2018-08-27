#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<queue>
#include<list>

using std::ostream;
using std::cout;

class Process{
public:
	Process(){
	}

	Process(int p, int s, int r, int b){
		//read in as priority spawn run blockMod
		spawnTime = s;		runLength = r;
		priority = p;		blockingModel = b;
		readyLength = 0;	blockTime = 0;
		exitTime = 0;		nextEventTime = 0;
		timeRun = 0;
	}

	//getters
	int getSpawn() const;
	int getRunLen() const;
	int getPrior() const;
	int getBlockM() const;
	int getReady() const;
	int getBlockT() const;
	int getExit() const;
	int getNext() const;
	int getRun() const;

	//setters
	void setReady(int a);		//sets readyLength
	void setBlocked(int a);		//sets blockedTime
	void setExit(int a);		//sets exitTime
	void setRun(int a);		//sets nextEventTime

private:
	int spawnTime = 0, runLength = 0, priority = 0, blockingModel = 0;
	int readyLength = 0, blockTime = 0, exitTime = 0, timeRun = 0;

	friend ostream& operator <<(ostream& ostreem, const Process& p);
	friend class ComparePrior;
	friend class CompareRun;
};

int Process::getSpawn() const	{ return spawnTime; }
int Process::getRunLen() const	{ return runLength; }
int Process::getPriority() const { return priority; }
int Process::getBlock() const	{ return blockTime; }
int Process::getRun() const		{ return timeRun; }

void Process::setReady(int a)	{ readyLength += a; }
void Process::setBlocked(int a)	{ blockTime += a; }
void Process::setExit(int a)	{ exitTime = a; }
void Process::setNext(int a)	{ nextEventTime = a; }
void Process::setRun(int a)		{ timeRun -= a; }

ostream& operator<<(ostream& ost, const Process& p) {
	cout << std::setw(10) << p.spawnTime << std::setw(5) << p.runLength << std::setw(5) << p.priority << std::setw(5) << p.blockingModel;
	return ost;
}

class Compare{
public:
	bool operator()(Process &p1, Process &p2)
	{
		if (p1.getRun() < p2.getRun() && p1.getPriority() < p2.getPriority())
			return true;
		return false;
	}
};

class ComparePrior{
public:
	bool operator()(Process &p1, Process &p2)
	{
		if (p1.getPriority() < p2.getPriority())
			return true;
		return false;
	}
};

int main()
{
	std::queue<Process> newQ;
	std::priority_queue<Process, std::vector<Process>, CompareRun> pqReady;
	std::list<Process> run;
	std::priority_queue<Process, std::vector<Process>, CompareRun> pqBlocked;
	std::queue<Process> deadQ;

	int sim_Length = 0, numComplete = 0, aveTime = 0;
	int p, s, r, b, i = 1, time=0;
	Process temp;
	Process running;

	std::ifstream fin;
	fin.open("c:\\temp\\Assn4_TestData50.txt");
	if (!fin.is_open())
	{
		cout << "File failed to open for reading - Please try again." << std::endl;
		exit(0);
	}


	fin >> sim_Length;					//time to run simulation
	while (fin >> p >> s >> r >> b)		//priority spawn run blockMod
	{
		temp = Process(p, s, r, b);
		//i++;
		//cout << std::setw(3) << i << temp << std::endl;
		newQ.push(temp);
	}									//all Processes in file to a queue of "new"

	do {
		// New Queue -> Ready
		Process process = newQ.front();				//get first process from new
		newQ.pop();	
		if (time == process.getSpawn())				//if the time is the spawn time, move to ready
		{
			pqReady.push(process);
		}

		/* BRENT'S CODE START */
		//Ready -> Run
		for (int i = 0; i < 5; i++)
		{
			Process current = pqReady.top();
			pqReady.pop();
			run.push_back(current);
			time++;
			current.setRun(1);
			if (current.getRun()==0)
			{
				deadQ.push(current);
				break; // ???Should break FOR loop
			}
		}
		cout << "Out of for loop" << std::endl;

		//Run -> pqBlock
		if (run.front().getSpawn() + run.front().getRun() >= pqReady.top().getSpawn())
		{
			Process toBlock = run.front();
			run.pop_front();
			pqBlocked.push(toBlock);
		}

		//Run -> Ready
		else
		{
			Process goBackToReady = run.front();
			run.pop_front();
			pqReady.push(goBackToReady);
		}

		//Run -> DeadQ
		//if (process.getRun()==0)
		//{
		//	deadQ.push(process);
		//	run.erase(run.begin());
		//}

		//pqBlock -> Ready
		if (toBlock.getBlock() == 2) //p.block == 2
		{
			//5% stay 5 ticks ->nextEvent=10
		}
		else if () //p.block==3
		{
			//20% for 10 ticks
		}
		else {
			Process goBackToReady = pqBlocked.top();
			pqBlocked.pop();
			pqReady.push(goBackToReady);
			time++;
			/* NOT SURE IF WE NEED TO SETRUN AS IN READY -> RUN EXAMPLE ABOVE */
		}
		

		time++;
	} while (time<sim_Length);






	//	if () //next process event is a Running process
	//		if runtime == 0;
	//	terminatedpq.push_back(process);
	//		else if //that process is Blocked
	//			//determine the amount of blocked
	//			pqBlock.push_back(process); //push process into BlockedPQ
	//		else
	//			readypq.push_back(process); //push process into ReadyPQ
	//		time++; //update Time
	//	else if () //next process event is a New process
	//		newpq.pop_back(process);  //pop process off NewPQ
	//	readypq.push_back(process); //push process into ReadyPQ
	//	time++; //update Time
	//	else if () //next process event is a Blocked process
	//		etc.
	//	else // next process event must be a Run process
	//	etc.
	//} while (time < sim_Length);
	//		//Report the simulation outcome

		EXIT_SUCCESS;
}