// Test of priority queue for objects
// http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects

#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

class Time {  // changed from struct to class
	public:
		Time ()  { } // Dummy Constructor -- no initializations
		Time (int a, int b, int c) 
		{
			h = a;
			m = b;
			s = c;
		}
		
		int getH() { return h;}
		int getM() { return m;}
		int getS() { return s;}
		
	private:
		int h = 0; // >= 0
		int m = 0; // 0-59
		int s = 0; // 0-59
};

class CompareTime {
public:
    bool operator()(Time& t1, Time& t2)
    {
       if (t1.getH() < t2.getH()) return true;
       if (t1.getH() == t2.getH() && t1.getM() < t2.getM()) return true;
       if (t1.getH() == t2.getH() && t1.getM() == t2.getM() && t1.getS() < t2.getS()) return true;
       return false;
    }
};

int main()
{
    priority_queue<Time, vector<Time>, CompareTime> pq;
	
	Time t;   // In order to use this declaration, there must be a no-parameter constructor for Time

	t = Time(3, 2, 40);  // This does NOT work as two-step declaration and first definition
	pq.push(t);
	
	t = Time(3, 2, 26);
	pq.push(t);
	
	t = Time(5, 16, 13);
	pq.push(t);
	
	t = Time(5, 14, 20);
	pq.push(t);
	

    while (! pq.empty()) 
	{
		// An attempt here to do an operation such as pq.top().getH() gives permissive error
		// Use two steps:  t2 = pq.top(); . . . t2.getH();
		
		Time t2 = pq.top();
	   
		cout << "Hours " << t2.getH() << endl;
		cout << "Min " << t2.getM() << endl;
		cout << "Secs " << t2.getS() << endl << "-------" << endl;
	   
       pq.pop();
    }
	
    return 0;
}