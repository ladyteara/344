#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <math.h>	// pow() function 
#include <queue>	// std::priority_queue
#include <algorithm>    // std::min

#define TIME_SLICE_LENGTH   5   // Length of the "time slice" in the Run state 

#define BEARPLOT  // When this line is NOT commented out, create output file of graphics commands


// NOTE: Use C++ library priority queue, http://www.cplusplus.com/reference/queue/priority_queue/push/
//   modified for objects // http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects


// This line allows commonly-used functions to be used without specifying the 
// library in which that function is defined. For instance, this line allows
// the use of "cout" rather than the full specification "cout"
using namespace std;


// GLOBAL VARIABLES 
ofstream fout;  // output file for graphics
	
	
class Process
{
	
	public:
	
		Process ()  { } // Dummy Constructor -- no initializations
		Process(int p, int s, int r, int b)  // Constructor
		{
			static int IDcounter = 1;
			ProcessID = IDcounter++;  // TBD
			Priority = p;
			SpawnTime = s;
			RunTimeNeeded = r;
			BlockingModel = b;
		}

		int TimePutInThisState = 0; 
		int TimeToLeaveThisState = 0; // Time of next event, updated as needed.
		int ReadyTimeSoFar = 0;  // Extent of time in Ready state, updated as needed.
		int RunTimeSoFar = 0;   // Extent of time in Run state, updated as needed.
		int BlockedTimeSoFar = 0;   // Extent of time in Blocked state, updated as needed.
		int ExitTime = 0;  // Time the process is completed, updated as needed.
		
		void showMe()
		{
			cout << "Priority is " << Priority << endl;
			cout << "SpawnTime is " << SpawnTime << endl;
			cout << "RunTimeNeeded is " << RunTimeNeeded << endl;
			cout << "BlockingModel is " << BlockingModel << endl;
			cout << "---------" << endl;
			cout << "TimeToLeaveThisState is " << TimeToLeaveThisState << endl;
			cout << "ReadyTimeSoFar is " << ReadyTimeSoFar << endl;
			cout << "BlockedTimeSoFar is " << BlockedTimeSoFar << endl;
			cout << "ExitTime is " << ExitTime << endl;
			cout << "RunTimeSoFar is " << RunTimeSoFar << endl;
			cout << "------------------------------" << endl;
		}
		
		int getPriority() { return Priority; }  // getter method
		int getSpawnTime() { return SpawnTime; }  // getter method
		int getProcessID() { return ProcessID; }  // getter method
		int getRunTimeNeeded() { return RunTimeNeeded; }  // getter method
	
	private:
	
		// This set of data is static -- once set, values never change
		int ProcessID = 0;  // Identifying number
		int Priority = 0;  // Priority of the process, NEVER UPDATED BY POLICY.
		int SpawnTime = 0;  // Time the process is spawned, never updated.
		int RunTimeNeeded = 0;  // CPU length of process, never updated.
		int BlockingModel = 0; // Blocking model for the process, never updated.
		
		// model for likelihood and length of block on process
		// Use http://www.cplusplus.com/reference/random/normal_distribution/ In the example,
		// the distribution parameters are mean (mu) = 5.0 and stddev (sigma) = 2.0.
		
	
};


// Support priority queue ordering mechanism  --- order elements ONLY BY TIME
// http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
class orderByTime {
public:
    bool operator()(Process& p1, Process& p2)
    {
		// In this logical test the GREATER THAN operation is used to determine
		// the LESSER item --- that's because the C++ Priority Queue ordering
		// is "higher value is higher priority" which is the opposite of the
		// necessary ordering for TIME, which is "lower value is higher priority"
		if (p1.TimeToLeaveThisState > p2.TimeToLeaveThisState) return true;  // Note the GREATER THAN operation

		return false;
    }
};	

// Support priority queue ordering mechanism -- order elements by time within priority
// http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
class orderByPriAndTime {
public:
    bool operator()(Process& p1, Process& p2)
    {
		if (p1.getPriority() < p2.getPriority()) return true;
	   
		// In this logical test the GREATER THAN operation is used to determine
		// the LESSER item --- that's because the C++ Priority Queue ordering
		// is "higher value is higher priority" which is the opposite of the
		// necessary ordering for TIME, which is "lower value is higher priority"
		if (p1.getPriority() == p2.getPriority() and 
			p1.getSpawnTime() > p2.getSpawnTime()) return true;  // Note the GREATER THAN operation

		return false;
    }
};	

// -----------------------------------------------------------------------------
// Display the contents of a queue without losing or reordering elements, by recursively 
// printing elements and re-inserting
// http://stackoverflow.com/questions/4523178/how-to-print-out-all-elements-in-a-stdstack-or-stdqueue-conveniently
void displayNewQueue(std::priority_queue<Process, std::vector<Process>, orderByTime> &pq,
	int locationX, int locationY)
{
	Process newProcess;

    if(pq.empty())  // base case
    {
        cout << endl;
        return;
    }
    newProcess = pq.top();  // remove an element from the front of the priority queue
	newProcess.showMe();  // Display the current element
	
#ifdef BEARPLOT
	fout << "text " << locationX << " " << locationY << " 12 " <<
		" ID " << newProcess.getProcessID() << 
		" SpawnTime " << newProcess.getSpawnTime() << endl;
	//fout << "sleep 0.1" << endl;
#endif // BEARPLOT

	
    pq.pop();
	displayNewQueue(pq, locationX, locationY + 16);  // Recursive call to print the remainder of the Priority Queue
    pq.push(newProcess);  // Replace the element in the priority queue
	
} // end displayNewQueue



// -----------------------------------------------------------------------------
// Display the contents of a queue without losing or reordering elements, by recursively 
// printing elements and re-inserting
// http://stackoverflow.com/questions/4523178/how-to-print-out-all-elements-in-a-stdstack-or-stdqueue-conveniently
void displayTermQueue(std::priority_queue<Process, std::vector<Process>, orderByTime> &pq,
	int locationX, int locationY)
{
	Process newProcess;

    if(pq.empty())  // base case
    {
        cout << endl;
        return;
    }
    newProcess = pq.top();  // remove an element from the front of the priority queue
	newProcess.showMe();  // Display the current element
	
#ifdef BEARPLOT
	fout << "text " << locationX << " " << locationY << " 12 " <<
		" ID " << newProcess.getProcessID() << 
		" RdTSF " << newProcess.ReadyTimeSoFar << 
		" RnTSF " << newProcess.RunTimeSoFar << endl;
	//fout << "sleep 0.1" << endl;
#endif // BEARPLOT

	
    pq.pop();
	displayTermQueue(pq, locationX, locationY + 16);  // Recursive call to print the remainder of the Priority Queue
    pq.push(newProcess);  // Replace the element in the priority queue
	
} // end displayTermQueue



// -----------------------------------------------------------------------------
// Find the total Ready time of processes in the Terminated queue 
int readyTimeInTermQueue(std::priority_queue<Process, std::vector<Process>, orderByTime> &pq)
{
	Process newProcess;
	int timeBelowHere = 0;

    if(pq.empty())  // base case
    {
        cout << endl;
        return 0;
    }
    newProcess = pq.top();  // remove an element from the front of the priority queue
	//newProcess.showMe();  // Display the current element
	
    pq.pop();
	timeBelowHere = readyTimeInTermQueue(pq);  // Recursive call to print the remainder of the Priority Queue
    pq.push(newProcess);  // Replace the element in the priority queue
	
	return timeBelowHere + newProcess.ReadyTimeSoFar;
	
} // end readyTimeInTermQueue

// -----------------------------------------------------------------------------
// Display the contents of a queue without losing or reordering elements, by recursively 
// printing elements and re-inserting
// http://stackoverflow.com/questions/4523178/how-to-print-out-all-elements-in-a-stdstack-or-stdqueue-conveniently
void displayReadyQueue(std::priority_queue<Process, std::vector<Process>, orderByTime> &pq,
	int locationX, int locationY)
{
	Process newProcess;

    if(pq.empty())  // base case
    {
        cout << endl;
        return;
    }
    newProcess = pq.top();  // remove an element from the front of the priority queue
	newProcess.showMe();  // Display the current element
	
#ifdef BEARPLOT
	fout << "text " << locationX << " " << locationY << " 12 " <<
		" ID " << newProcess.getProcessID() << 
		" leave " << newProcess.TimeToLeaveThisState << endl;
#endif // BEARPLOT

	
    pq.pop();
	displayReadyQueue(pq, locationX, locationY + 16);  // Recursive call to print the remainder of the Priority Queue
    pq.push(newProcess);  // Replace the element in the priority queue
	
} // end displayReadyQueue




// -----------------------------------------------------------------------------
// Pass arguments or parameters from command-line execution. argc is the count of
// those parameters, including the executable filename. argv[] is an array of the 
// parameters.
int main (int argc, char *argv[])
{
    string token;
    int xValue, yValue;
    ifstream fin;

	
	int currentTime = 0;
	int numberOfProcesses = 0;
	int Priority = 0;
	int SpawnTime = 0;
	int RunTimeNeeded = 0;
	int BlockingModel = 0;
	int timeToSimulate = 0;  // Time for which to run the simulation of several processes
	
	int loopCounter = 0;  // DEBUG
	bool processMoved = false; // Flag: process moved state in main loop
	int RTITQ = 0; // Ready Time of processes in Terminal Queue
	
	Process newProcess;
	Process tempProcess;
	
	std::priority_queue<Process, std::vector<Process>, orderByTime> NewQueue;
	std::priority_queue<Process, std::vector<Process>, orderByTime> ReadyQueue;
	std::priority_queue<Process, std::vector<Process>, orderByTime> TerminatedQueue;

	// For convenience I use a queue for Run, but it contains at most one process
	std::priority_queue<Process, std::vector<Process>, orderByTime> RunningProcess;

	
    // Check the number of arguments. Expected: filename of a file
    if (argc != 2)  // This check is often hardcoded
    {   // If failure in parameters, offer advice for correction
        cout << "\nThis program uses command-line argument.\n";
        cout << "Usage: a.exe <filename>\n";
        exit(0);
    }



    try  // All lines within this block are part of the same exception handler
    {
        fin.open(argv[1]);
    } 
    catch (exception& ex) 
    {
        cout << ex.what();  // display standard explanation of the exception
        exit(0);  // exit the program
    }
	
#ifdef BEARPLOT
	// Set up BearPlot graphics
	fout.open("simulation.dat");
	fout << "1000 500 lightblue" << endl;
	
	// Print the states of process simulation
	fout << "circle 20 20 120 oldlace" << endl;
	fout << "text 80 80 16 New" << endl;
	fout << "line_solid 130 60 240 90" << endl;
	fout << "line_solid 220 70 240 90" << endl;  // one side of an arrowhead
	
	fout << "circle 220 80 120 oldlace" << endl;
	fout << "text 280 140 16 Ready" << endl;
	fout << "line_solid 340 120 460 80" << endl;  // line from Ready to Run
	fout << "line_solid 460 80 590 120" << endl;
	fout << "line_solid 560 90 590 120" << endl;  // one side of an arrowhead
	
	fout << "circle 600 80 120 oldlace" << endl;
	fout << "text 660 140 16 Run" << endl;
	fout << "line_solid 340 160 460 200" << endl;  // line from Run to Ready
	fout << "line_solid 460 200 590 160" << endl;
	fout << "line_solid 340 160 360 180" << endl;  // one side of an arrowhead
	
	fout << "circle 860 20 120 oldlace" << endl;
	fout << "text 920 80 16 Terminated" << endl;
	fout << "line_solid 720 120 860 100" << endl;  // line from Run to Terminated
	fout << "line_solid 860 100 830 90" << endl;  // one side of an arrowhead
	
	fout << "circle 500 320 120 oldlace" << endl;
	fout << "text 560 380 16 Blocked" << endl;
#endif // BEARPLOT

            
   
    // Read from the file, one token at a time. If the type of token is known, it
    // can be read into a corresponding variable type, such as 
    //          in >> x;    // Read the first item into an integer variable x.
    //          in >> str;  // Read the next item into a string variable str.
	// Read from a file all setup information for each simulated process
    fin >> timeToSimulate;  // Time for which to run the simulation of several processes 
    while (fin >> Priority)
    {
		fin >> SpawnTime;   
		fin >> RunTimeNeeded;
		fin >> BlockingModel;
		
		// Store each process as it is initialized.
		newProcess = Process(Priority, SpawnTime, RunTimeNeeded, BlockingModel);
		newProcess.TimeToLeaveThisState = SpawnTime;
		//newProcess.showMe();
		
		NewQueue.push(newProcess);
		
    } // end while
	
    fin.close();  // Close the input data file                

	
#ifdef BEARPLOT
	cout << "RECURSIVE Elements of New Queue..." << endl;
	displayNewQueue(NewQueue, 80, 160);
	// fout << "presskey" << endl;  // pause for viewing
#endif // BEARPLOT


	do 
	{
		
		// Default value is that no process moved from state to state. 
		// Only when no process moved state do we increment time.
		processMoved = false;

#ifdef BEARPLOT
		// Update time display.
		// Blank out previous time display with a background-colored rectangle
		fout << "rectangle 350 15 520 70 lightblue" << endl;
		fout << "text 440 40 20 Time: " << currentTime << endl; // re-display current time
		fout << "sleep 0.2" << endl;

		if ( ! RunningProcess.empty()) // A Run process exists, so display it
		{
			// Blank out previous Run display with a background-colored rectangle
			fout << "rectangle 540 200 800 230 lightblue" << endl;
			tempProcess = RunningProcess.top();  // Observe top element on the New queue
			fout << "text 660 220 12 " <<
				" ID " << tempProcess.getProcessID() << 
				" leave " << tempProcess.TimeToLeaveThisState << 
				" RnTN " << tempProcess.getRunTimeNeeded() << 
				" RnTSF " << tempProcess.RunTimeSoFar << 
				endl; // re-display Run process
			fout << "sleep 0.5" << endl;
		}
		
#endif // BEARPLOT


		
		// Move any and all New processes with appropriate time to Ready queue
		while (! NewQueue.empty() )  // this loop will be break-ed out of if appropriate
		{
			tempProcess = NewQueue.top();  // Observe top element on the New queue
			if (currentTime >= tempProcess.TimeToLeaveThisState)
			{
				NewQueue.pop();  // Take process off the New queue
				tempProcess.TimePutInThisState = currentTime;
				ReadyQueue.push(tempProcess);

#ifdef BEARPLOT			
			// Blank out previous queue display with a background-colored rectangle
			fout << "rectangle 0 140 180 500 lightblue" << endl;
			displayNewQueue(NewQueue, 80, 160);

			// Blank out previous queue display with a background-colored rectangle
			fout << "rectangle 190 200 420 500 lightblue" << endl;
			displayReadyQueue(ReadyQueue, 310, 220);
			fout << "sleep 0.5" << endl;
#endif // BEARPLOT

			}
			else // Either no element in New queue, or the top element is 
					// not yet to be moved to Ready queue
			{
				break; // get out of the infinite do loop
				
			}
			
			// DON'T update current time here.
			// There may be other New processes to move to Ready,
			// and there may be other processes that should change state
			// without the passage of time.
			
			// Indicate some process moved state. 
			// Only when no process moved state do we increment time.
			processMoved = true;

		}; // end while; moving appropriate New processes to Ready queue
		

		
		
		// Determine whether a Ready process moves to Run.
		// There must be a Ready process and no Run process.
		if ( (not ReadyQueue.empty()) and RunningProcess.empty())
		{
			tempProcess = ReadyQueue.top();  // Observe top element on the Ready queue
			ReadyQueue.pop();  // Take process off the Ready queue
			
			// update the process's ReadyTimeSoFar
			tempProcess.ReadyTimeSoFar = 
				tempProcess.ReadyTimeSoFar + 
					(currentTime - tempProcess.TimePutInThisState);
			tempProcess.TimePutInThisState = currentTime;
			
			// Set the time this process will leave the Run state.
			// That time is either a standard time slice or the time
			// that the process is complete, whichever comes first.
			tempProcess.TimeToLeaveThisState = currentTime + 
				std::min(TIME_SLICE_LENGTH, 
					tempProcess.getRunTimeNeeded() - tempProcess.RunTimeSoFar);
			RunningProcess.push(tempProcess);
			cout << "Just set this process to RUN:\n";
			tempProcess.showMe();

#ifdef BEARPLOT			
			// Blank out previous Run display with a background-colored rectangle
			fout << "rectangle 540 200 800 230 lightblue" << endl;
			tempProcess = RunningProcess.top();  // Observe top element on the Run queue
			fout << "text 660 220 12 " <<
				" ID " << tempProcess.getProcessID() << 
				" leave " << tempProcess.TimeToLeaveThisState << 
				" RnTN " << tempProcess.getRunTimeNeeded() << 
				" RnTSF " << tempProcess.RunTimeSoFar << 
				endl; // re-display Run process
			//fout << "sleep 0.2" << endl;


			// Blank out previous queue display with a background-colored rectangle
			fout << "rectangle 190 200 420 500 lightblue" << endl;
			displayReadyQueue(ReadyQueue, 310, 220);
			fout << "sleep 0.5" << endl;
#endif // BEARPLOT

			// Indicate some process moved state. 
			// Only when no process moved state do we increment time.
			processMoved = true;

		} // end if a Ready process moves to Run

		
		

		// if a Run process exists, proceed to determine its next state
		if ( not RunningProcess.empty() )  
		{
			tempProcess = RunningProcess.top(); 
			
			// if the current Run process leaves the Run state...
			if (tempProcess.TimeToLeaveThisState <= currentTime)
			{
				// Find the next state for the current Run process.
				// This could be either Ready, Terminated, or Blocked.
				
				RunningProcess.pop();  // Take process off the Run queue (Run queue is at most one element)
				
				// Update the time that the Run process has been running
				tempProcess.RunTimeSoFar = 
					tempProcess.RunTimeSoFar + 
						(tempProcess.TimeToLeaveThisState -
						tempProcess.TimePutInThisState);
				
				cout << "process is leaving RUN, its RnTSF " <<  tempProcess.RunTimeSoFar << endl;
				
				// if the process moves to Terminated . . .
				if (tempProcess.RunTimeSoFar >= tempProcess.getRunTimeNeeded())
				{
					// This process has received all the runtime it needs.
					// Move this running process to the Terminated queue
					tempProcess.ExitTime = currentTime;
					TerminatedQueue.push(tempProcess);
					
#ifdef BEARPLOT			
					// Update time display.
					// Blank out previous time display with a background-colored rectangle
					fout << "rectangle 350 15 520 70 lightblue" << endl;
					fout << "text 440 40 20 Time: " << currentTime << endl; // re-display current time
					fout << "sleep 0.2" << endl;

					// Blank out previous Run display with a background-colored rectangle
					fout << "rectangle 540 200 800 230 lightblue" << endl;

					// Blank out previous queue display with a background-colored rectangle
					fout << "rectangle 800 200 1000 500 lightblue" << endl;
					displayTermQueue(TerminatedQueue, 900, 220);
					fout << "sleep 0.5" << endl;
#endif // BEARPLOT					
					
					
				} // end if the current Run process is Terminated
				
				// else if  // current Run process moves to Blocked
			
				else // Default: current Run process moves to Ready
				{
				
					// This process has NOT received all the runtime it needs.
					// Move this running process to the Ready queue
					
					/*
					RnTSF already incremented
					tempProcess.RunTimeSoFar = // increment Run time
						tempProcess.RunTimeSoFar + 
							(currentTime - tempProcess.TimePutInThisState);
					*/
					tempProcess.TimePutInThisState = currentTime;
					ReadyQueue.push(tempProcess);
					//cout << "process is leaving RUN, its RnTSF " <<  tempProcess.RunTimeSoFar << endl;

					
#ifdef BEARPLOT			
					// Update time display.
					// Blank out previous time display with a background-colored rectangle
					fout << "rectangle 350 15 520 70 lightblue" << endl;
					fout << "text 440 40 20 Time: " << currentTime << endl; // re-display current time
					fout << "sleep 0.2" << endl;

					// Blank out previous Run display with a background-colored rectangle
					fout << "rectangle 540 200 800 230 lightblue" << endl;

					// Blank out previous queue display with a background-colored rectangle
					fout << "rectangle 190 200 420 500 lightblue" << endl;
					displayReadyQueue(ReadyQueue, 310, 220);
					fout << "sleep 0.5" << endl;
#endif // BEARPLOT	

				} // end if the current Run process moves to the Ready state

				// Indicate some process moved state. 
				// Only when no process moved state do we increment time.
				processMoved = true;

				
			} // end if a Run process leaves the Run state
			
			
		} // end if a Run process exists
		

		
		
		// If it is NOT true that any process moved state, then 
		// increment time to advance the simulation.
		if ( ! processMoved)
		{
			currentTime++;  // Move the simulator master clock forward
		}
		

		
		// FOR DEBUG
		if (loopCounter++ > 1000)
		{
			break;
		}
		
		
		
	} while (currentTime < timeToSimulate);
	
	// Final display of process in Terminated queue.
	cout << "\n\n-------------------------------------------------------\n";
	cout << "Total time in Ready Queue of processes in the Terminated Queue:\n";
	RTITQ = readyTimeInTermQueue(TerminatedQueue);
	cout << RTITQ << "\n\n";

#ifdef BEARPLOT			
	// Show the time in Ready queue by Terminated processes
	fout << "text 600 480 16 Time in Ready queue by Terminated processes: " 
		<< RTITQ << endl;
#endif // BEARPLOT	
	

}