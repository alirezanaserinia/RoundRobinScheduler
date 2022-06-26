#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Thread {
public:
	int process_id;
	int thread_id;
	int number_of_remaining_time_slots;
};

class Core {
public:
	Core(int id) { set_core_id(id); }
	void set_core_id(int id) { CID = id; }
	int get_core_id() { return CID; }
	vector<Thread> get_Queue() { return Queue; }
	void add_thread(Thread t);
	void remove_thread();
	void show_core_stat();
	void run_core();
	int sum_of_remaining_times();
private:
	int CID;
	vector<Thread> Queue;
};

void Core::add_thread(Thread t) {
	Queue.push_back(t);
}

void Core::remove_thread() {
		Queue.erase(Queue.begin());
}

void Core::show_core_stat() {
	cout << "Core number : " << get_core_id() << endl;
	for (int i = 0; i < get_Queue().size(); i++){
		cout << "Process ID : " << get_Queue()[i].process_id << " - Thread ID : " << get_Queue()[i].thread_id << endl;
		cout << "Number of time slots : " << get_Queue()[i].number_of_remaining_time_slots << endl;
	}	
}

void Core::run_core() {
	if (!get_Queue().empty()) {
		Thread temp;
		temp.process_id = get_Queue()[0].process_id;
		temp.thread_id = get_Queue()[0].thread_id;
		temp.number_of_remaining_time_slots = get_Queue()[0].number_of_remaining_time_slots - 1;
		if (temp.number_of_remaining_time_slots != 0)
			add_thread(temp);
		remove_thread();
		cout << "Core number : " << get_core_id() << endl;
		cout << "Process ID : " << temp.process_id << " - Thread ID : " << temp.thread_id << endl;
	}
}

int Core::sum_of_remaining_times() {
	int sum = 0;
	for (int i = 0; i < Queue.size(); i++)
		sum += Queue[i].number_of_remaining_time_slots;
	return sum;
}

class Process {
public:
	Process(int id, vector<int> threads);
	void set_process_id(int id) { PID = id; }
	void set_process_threads(vector<int> threads);
	int get_process_id() { return PID; }
	vector<int> get_threads() { return Threads; }
private:
	int PID;
	vector<int> Threads;
};

void Process::set_process_threads(vector<int> threads) {
	for (int i = 0; i < threads.size(); i++)
		Threads.push_back(threads[i]);
}

Process::Process(int id, vector<int> threads) {
	set_process_id(id);
	set_process_threads(threads);
}

vector<Core> add_core(vector<Core> cores) {
	Core ob(cores.size()+1);
	cores.push_back(ob);
	cout << "Core with core ID = " << cores.size() << " successfully added!" << endl;
	return cores;
}

int find_shortest_queue(vector<Core> cores) {
	int shortest_queue_index = 0;
	if (!cores.empty()) {
		for (int i = 1; i < cores.size(); i++)
			if (cores[shortest_queue_index].get_Queue().size() > cores[i].get_Queue().size())
				shortest_queue_index = i;
		return shortest_queue_index;
	}
	return 0;
}

vector<Core> insert_threads(vector<Core> cores, Process ob) {
	if (!ob.get_threads().empty()) {
		for (int i = 0; i < ob.get_threads().size(); i++) {
			Thread t;
			t.process_id = ob.get_process_id();
			t.thread_id = i + 1;
			t.number_of_remaining_time_slots = ob.get_threads()[i];
			cores[find_shortest_queue(cores)].add_thread(t);
		}
	}
	return cores;
}

vector<Process> add_process(vector<Core> & cores, vector<Process> processes, string str) {
	vector<int> t;
	for (int i = 14; i < str.size(); i += 2)
		t.push_back(int(str[i]) - 48);
	Process ob(processes.size() + 1, t);
	cores = insert_threads(cores, ob);
	processes.push_back(ob);
	cout << "Process with pid = " << processes.size() << " added!" << endl;
	return processes;
}

int find_max_remaining_times(vector<Core> cores) {
	int max = cores[0].sum_of_remaining_times();
	for (int i = 1; i < cores.size(); i++)
		if (max < cores[i].sum_of_remaining_times())
			max = cores[i].sum_of_remaining_times();
	return max;
}

void finish_tasks(vector<Core> cores) {
	int max = find_max_remaining_times(cores);
	for (int i = 0; i < max; i++) {
		cout << "Time Slice : " << i + 1 << endl;
		for (int j = 0; j < cores.size(); j++) 
			if (cores[j].get_Queue().size() != 0)
				cores[j].run_core();
	}
}

int main() {
	vector<Core> cores;
	vector<Process> processes;
	string str;
	while (getline(cin, str)) {
		if (str == "add_core") 
			cores = add_core(cores);	

		else if (str.substr(0,11) == "add_process") 
			processes = add_process(cores, processes, str);

		else if (str == "show_cores_stat") {
			if (!cores.empty())
				for (int i = 0; i < cores.size(); i++)
					cores[i].show_core_stat();
		}
		else if (str == "run_cores") {
			if (!cores.empty())
				for (int i = 0; i < cores.size(); i++)
					cores[i].run_core();
		}
		else if ( str == "finish_tasks")
			if (!cores.empty()) 
				finish_tasks(cores);
	}
	return 0;
}
