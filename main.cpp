#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class process
{
public:
	void queue(int exec,int pri);
	void qu(int exec, int pri);
	void qu_end(int exec, int pri);
	void execute();
	void asign_quantum(int quan);
	void sch_que();
	void readFile();
	int process_count = 0;
	bool quan = true;
private:
	process *next;
	process *head;
	int quantum;
	int exec_time;
	int priority;
	int pid;
	int hold_pid;
	int hold_exec;
	int hold_pri;
	bool inQueue = false;
	bool pEnds;
	bool firstNode = true;
}prs;

int main()
{
	//Read txt file
	ifstream ifs;
	ifs.open("input.txt");
	string str;

	while (getline(ifs,str))
	{
		//cout << str << endl;
		if (prs.quan)
		{
			int quan = stoi(str);
			//cout << quan << endl;
			prs.asign_quantum(quan);
			prs.quan = false;
		}
		else {
			istringstream iss(str);
			string exec, pri;
			iss >> exec >> pri;
			int i_exec = stoi(exec); int i_pri = stoi(pri);
			//cout << "exec: " << exec << "  pri: " << pri << endl;
			prs.queue(i_exec, i_pri);
			prs.process_count++;
		}
	}

	//prs.readFile();  //IORedirection

	prs.sch_que();
	prs.execute();

	system("pause");
	return 0;
}

void process::readFile() //IORedirection
{
	int exec, pri;
	cin >> quantum;
	while (cin >> exec >> pri)
	{
		queue(exec, pri);
		process_count++;
	}
}

void process::asign_quantum(int quan) //asign the quantum
{
	quantum = quan;
	//cout << quantum << endl;
}

void process::queue(int exec, int pri)
{
	//cout << exec << " " << pri << endl;
	if (inQueue)
	{
		if (pri > hold_pri)
		{
			process *temp = new process;
			temp->pid = hold_pid;
			temp->exec_time = hold_exec;
			temp->priority = hold_pri;
			temp->pEnds = true;
			//cout << "++" << endl;
			process *current = head;
			while (current->next != NULL)
				current = current->next;
			current->next = temp;
			inQueue = false;

			if (exec > quantum) //if required execution time greater than quantum
			{
				hold_exec = exec - quantum;
				hold_pid = process_count;
				hold_pri = pri;
				qu(quantum, pri);
				inQueue = true;
			}
			else
			{
				qu_end(exec, pri);
			}
		}
		else
		{
			if (exec > quantum)
			{
				process *temp = new process;
				temp->pid = hold_pid;
				temp->exec_time = hold_exec;
				temp->priority = hold_pri;
				temp->pEnds = true;

				hold_exec = exec - quantum;
				hold_pid = process_count;
				hold_pri = pri;
				qu(quantum, pri);

				process *current = head;
				while (current->next != NULL)
					current = current->next;
				current->next = temp;
			}
			else
			{
				qu_end(exec, pri);
			}
		}
	}
	else //no process is in queue
	{
		if (exec > quantum) //if required execution time greater than quantum
		{
			hold_exec = exec - quantum;
			hold_pid = process_count;
			hold_pri = pri;
			qu(quantum, pri);
			inQueue = true;
		}
		else
		{
			qu_end(exec, pri);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

void process::qu(int exec, int pri)
{
	if (firstNode)
	{
		process *firstN = new process;
		firstN->pid = process_count;
		firstN->exec_time = exec;
		firstN->priority = pri;
		head = firstN;
		firstNode = false;
	}
	else
	{
		process *temp = new process;
		process *current = head;
		while (current->next != NULL)
			current = current->next;
		temp->pid = process_count;
		temp->exec_time = exec;
		temp->priority = pri;
		current->next = temp;
	}
}

void process::qu_end(int exec, int pri)
{
	if (firstNode)
	{
		process *firstN = new process;
		firstN->pid = process_count;
		firstN->exec_time = exec;
		firstN->priority = pri;
		firstN->pEnds = true;
		head = firstN;
		firstNode = false;
	}
	else
	{
		process *temp = new process;
		process *current = head;
		while (current->next != NULL)
			current = current->next;
		temp->pid = process_count;
		temp->exec_time = exec;
		temp->priority = pri;
		temp->pEnds = true;
		current->next = temp;
	}
}

void process::execute()
{
	process *current = head;
	while (current->next != NULL)
	{
		cout << "Process " << current->pid << ": exec time =  " << current->exec_time << ", priority =  " << current->priority << endl;
		if (current->pEnds)
			cout << "Process " << current->pid << " ends." << endl;
		current = current->next;
	}
	cout << "Process " << current->pid << ": exec time =  " << current->exec_time << ", priority =  " << current->priority << endl;
	if (current->pEnds)
		cout << "Process " << current->pid << " ends." << endl;
}

void process::sch_que()
{
	process *current = head;
	cout << "Scheduling queue: ";

	while (current->next != NULL)
	{
		cout << "(" << current->pid << "," << current->exec_time << "," << current->priority << "),";
		current = current->next;
	}
	cout << "(" << current->pid << "," << current->exec_time << "," << current->priority << ")" << endl;;

}