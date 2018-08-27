#include<iostream>
#include<fstream>
#include<iomanip>
#include<random>
#include<queue>
#include<ctime>

#define BEARPLOT  // When this line is NOT commented out, create output file of graphics commands

using std::cout;
using std::endl;

std::ofstream fout;

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
		timeRun = r;
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
	void setNext(int a);		//sets nextEventTime
	void setRun(int a);			//counts down runLength to 0

private:
	int spawnTime, runLength, priority, blockingModel;
	int readyLength, blockTime, exitTime, nextEventTime, timeRun;

	friend std::ostream& operator <<(std::ostream& ostreem, const Process& p);
	friend class ComparePrior;
	friend class CompareRun;
};

int Process::getSpawn() const	{ return spawnTime; }
int Process::getRunLen() const	{ return runLength; }
int Process::getPrior() const	{ return priority; }
int Process::getBlockM() const	{ return blockTime; }
int Process::getReady() const	{ return readyLength; }
int Process::getBlockT() const	{ return blockTime; }
int Process::getExit() const	{ return exitTime; }
int Process::getNext() const	{ return nextEventTime; }
int Process::getRun() const		{ return timeRun; }


void Process::setReady(int a)	{ readyLength += a; }
void Process::setBlocked(int a)	{ blockTime += a; }
void Process::setExit(int a)	{ exitTime = a; }
void Process::setNext(int a)	{ nextEventTime = a; }
void Process::setRun(int a)		{ timeRun -= a; }


std::ostream& operator<<(std::ostream& ost, const Process& p) {
	cout << "S: " << std::setw(5) << p.spawnTime << "R: " << std::setw(5) << p.runLength
		 << "P: " << std::setw(5) << p.priority << "B: " << std::setw(5) << p.blockingModel;
	return ost;
}

class Compare{
public:
	bool operator()(Process &p1, Process &p2)
	{
		if (p1.getRun() < p2.getRun() && p1.getPrior() < p2.getPrior())
			return true;
		return false;
	}
};

class CompareP{
public:
	bool operator()(Process &p1, Process &p2)
	{
		if (p1.getPrior() < p2.getPrior() && p1.getSpawn() < p2.getSpawn())
			return true;
		return false;
	}
};

class CompareB{
public:
	bool operator()(Process &p1, Process &p2)
	{
		if (p1.getNext() < p2.getNext() && p1.getRun() < p2.getRun() && p1.getPrior() < p2.getPrior())
			return true;
		return false;
	}
};

int main()
{
	std::queue<Process> newQ;
	std::priority_queue<Process, std::vector<Process>, Compare> ready;
	std::priority_queue<Process, std::vector<Process>, Compare> run;
	std::priority_queue<Process, std::vector<Process>, CompareB> blocked;
	std::priority_queue<Process, std::vector<Process>, CompareP> deadQ;

	int sim_Length = 0, numProcesses = 0, aveTime = 0;
	int p, s, r, b, i = 0, time=0, start, end;
	Process temp, process;
	std::srand(std::time(0));
	std::ifstream fin;

	fin.open("c:\\temp\\Assn4_TestData10.txt");
	if (!fin.is_open())
	{
		cout << "File failed to open for reading - Please try again." << endl;
		exit(0);
	}


	fin >> sim_Length;									//time to run simulation
	while (fin >> p >> s >> r >> b)						//priority spawn run blockMod
	{
		temp = Process(p, s, r, b);
		newQ.push(temp);
		i++;
		//cout << std::setw(3) << i << temp << endl;
		//cout << std::setw(5) << newQ.front() << endl;
		//cout << std::setw(5) << newQ.back() << endl;
	}													//all Processes in file to a queue of "new"


//The hard work
	do {
		time++;											//cout << time << endl;
		// New Queue -> Ready
		if (!newQ.empty())
		{
			process = newQ.front();						//get first process from new
			if (time == process.getSpawn())				//if the time is the spawn time, move to ready
			{
				ready.push(process);	start = time;
				newQ.pop();
				//cout << "New process " << process << endl; cout << "Ready       " << ready.top() << endl;
			}
		}
		//Ready -> Run
		if (!ready.empty())
		{
			if (run.empty())
			{
				run.push(ready.top());					//cout << "run" << run.top() << endl;
				ready.pop();			end = time;
				run.top().setReady(end-start);			//set time in ready as current time
				run.top().setNext(time + 5);
			}
			else
			{
				run.top().setRun(1);
				//Run -> Term
				if (run.top().getRun() == 0)			//if run time is complete - move to dead
				{
					run.top().setExit(time);			//cout << "run top " << run.top() << endl;
					deadQ.push(run.top());				//cout << "dead1" << deadQ.top() << endl;
					run.pop();
				}
				else if (run.top().getNext() == time)
				{
					//Run -> Ready
					if (run.top().getBlockM() == 1 || run.top().getBlockM() == 4 || run.top().getBlockM() == 5)		//if blockModel does not block
					{
						ready.push(run.top());			//send to ready
						run.pop();						//delete from run
					}
					//Run -> Block
					else
					{
						int chance = rand() % 20;
						if (run.top().getBlockM() == 2)
						{
							if (chance == 1)
							{
								run.top().setNext(time + 5);
							}
						}
						else if ( run.top().getBlockM() == 3)
						{
							if (chance == 3 || chance == 5 || chance == 7 || chance == 9)
							{
								run.top().setNext(time + 10);
							}
						}
						blocked.push(run.top());			//push to blocked
						run.pop();							//delete from run
						//cout << "block\t" << blocked.top() << endl;
					}
				}
			}
		}
		//blocked -> Ready
		if (!blocked.empty())
		{
			if (time == blocked.top().getNext())
			{
				blocked.top().getBlockM() == 2 ? blocked.top().setBlocked(5) : blocked.top().setBlocked(10);
				ready.push(blocked.top());
				blocked.pop();
			}
		}
	} while (time<sim_Length);

	cout << endl << "SIMULATION SUMMARY" << endl;
	cout << i << " total processes in file" << endl;
	int timeNready = 0, timeNblock = 0, count = deadQ.size();
	for (int i = 0; i < count; i++)
	{
		timeNready += deadQ.top().getReady();
		timeNblock += deadQ.top().getBlockT();
		deadQ.pop();
	}
	float avgReady = timeNready / count;
	float avgBlock = timeNblock / count;
	
	cout << "The average time in Ready (waiting) is  : " << avgReady << " clicks" << endl;
	cout << "The average time in Blocked (waiting) is: " << avgBlock << " clicks" << endl;
	cout << "\t" << count << " processes completed in " << sim_Length << " clicks." << endl;

		EXIT_SUCCESS;
}