#include "Memory.h"
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

Memory::Memory(int  size) {
	maxNum = 50;
	maxSize = size;

	init = new Process[maxNum];
	Process* empty = new Process();

	empty->Id = -1;
	empty->Size = size;
	empty->Pos = 0;
	
	init = empty;
}

Memory::Memory(const Memory& mem) {
	maxNum = mem.maxNum;
	maxSize = maxSize;
	Process* o_Location = mem.init;
	Process* Location;
	Process* newProc;

	newProc = new Process();
	newProc->Id = o_Location->Id;
	newProc->Size = o_Location->Size;
	newProc->Pos = o_Location->Pos;


	init = newProc;
	Location = init;
	o_Location = o_Location->next;
	while (o_Location != NULL) {
		newProc = new Process();
		newProc->Id = o_Location->Id;
		newProc->Size = o_Location->Size;
		newProc->Pos = o_Location->Pos;

		Location->next = newProc;
		Location = Location->next;
		o_Location = o_Location->next;
	}
}

Memory::~Memory() {
	while (init != NULL) {
		Process* temp = init;
		init = init->next;
		delete temp;
	}
}

bool Fragment(Process* proc);
void Compaction_BFS(const Memory* mem, int& min_size, queue<vector<int>>& Path);

void Memory::Alloc(int id, int size, bool flag) {
	//Free
	if (size == 0) {
		Process* location = init;
		while (location != NULL) {
			if (location->Id == id) {
				location->Id = -1;
				if (flag) cout << "\n\tFree Process " << id << ": " << location->Size << "KB\n";
				Coalescing(flag);
				break;
			}
			location = location->next;
		}
	}

	//BestFit

	else if(size > 0) {
		if(flag) cout << "\n\tREQUEST " << id << ": " << size << "KB\n";
		Process* min = NULL;
		int min_size = maxSize + 1;
		Process* location = init;
		int cnt = -1;

		//Find Bestfit
		while (location != NULL) {
			if (location->Id == -1 && location->Size >= size && location->Size < min_size) {
				min = location;
				min_size = location->Size;
			}

			cnt++;
			location = location->next;
		}

		//Process Allocation
		if (min != NULL) {
			Process* hole = new Process();
			hole->Id = -1;
			hole->Size = min->Size - size;
			hole->Pos = min->Pos + size;
			hole->next = min->next;

			min->Id = id;
			min->Size = size;
			min->next = hole;

			if (hole->Size == 0) {
				min->next = hole->next;
				delete hole;
			}

			cout << "\tBest Fit : Allocated at  " << min->Pos << "KB\n\n";
			if (flag) Print();
		}

		else if (GetHoleSize() >= size) {
			if (Fragment(init)) {
				Compaction(flag);
				Alloc(id, size, false);
				if (flag) Print();
			}
		}

		else {
			cout << "  Error: Process is to Big!\n";
			cout << "  Process " << id << " " << size << "KB\n";
			cout << GetHoleSize() << "KB Free Complete.\n";
		}
	}
}

void Memory::ProcessMove(int pos[2])
{
	if (pos[0] == pos[1]) return;

	Process* location = init;
	int cnt = 0;
	while (location != NULL) {
		if (cnt == pos[0])
			break;
		location = location->next;
		cnt++;
	}

	Process* obj = init;
	cnt = 0;
	while (obj != NULL) {
		if (cnt == pos[1])
			break;
		obj = obj->next;
		cnt++;
	}

	Process* hole = new Process();
	hole->Id = -1;
	hole->Size = obj->Size - location->Size;
	hole->Pos = obj->Pos + location->Size;
	hole->next = obj->next;

	obj->Id = location->Id;
	obj->Size = location->Size;
	obj->next = hole;
	location->Id = -1;

	
	if (hole->Size == 0) {
		obj->next = hole->next;
		delete hole;
	}

	Coalescing(false);
}

void Memory::Coalescing(bool flag) {
	
	Process* location = init;
	while (location != NULL) {
		if (location->Id == -1 && location->next != NULL && location->next->Id == -1) {
			Process* obj = location->next;
			if (flag) cout <<"\tCoalescing : "<<location->Pos << "KB & " << location->next->Pos << "KB\n";
			location->Size += location->next->Size;
			location->next = location->next->next;
			delete obj;
		}

		else if (location->Id == -1 && location->Size == 0) {
			Process* obj;
			if (location->next != NULL) {
				obj = location->next;
				location->Id = location->next->Id;
				location->Size = location->next->Size;
				location->next = location->next->next;
			}
			else {
				obj = init;
				while (obj->next != location)
					obj = obj->next;
				location = obj;
				obj = location->next;
				location->next = NULL;
			}
			
			delete obj;
		}
		else
			location = location->next;
	}
	if (flag)
		Print();
}

void Memory::Compaction(bool flag) {
	int min_size = maxSize + 1;
	queue<vector<int>> Path;
	Compaction_BFS(this, min_size, Path);

	cout << "\tCompaction ... \n";
	int total = Path.back()[2];
	Path.pop();
	while (!Path.empty()){
		int alloc_Info[2];
		vector<int> path = Path.front();
		Path.pop();

		Process* location = init;
		int cnt = 0;
		while (location != NULL) {
			if (cnt == path[0])
				break;
			location = location->next;
			cnt++;
		}
		cnt = location->Id;

		alloc_Info[0] = path[0];
		alloc_Info[1] = path[1];
		ProcessMove(alloc_Info);

		location = init;
		while (location != NULL) {
			if (location->Id == cnt)
				break;
			location = location->next;
		}

		if (flag)
			cout << "\t\tProcess" << location->Id << " " << location->Size << "KB" << " Moves to " << location->Pos << "KB\n";
	}
	
	cout << "\n\tTotal Move! :\t" << total << "KB\n\n";
	if (flag)
		Print();
}

bool Fragment(Process* proc) {
	Process* location = proc;
	int cnt = 0;
	while (location != NULL) {
		if (location->Id == -1) {
			cnt++;
			if (cnt >= 2) return true;
		}
		location = location->next;
	}
	return false;
}

void Compaction_BFS(const Memory* mem, int& min_size, queue<vector<int>>& Path) {
	int Num;
	queue<Memory> q;
	queue<queue<vector<int>>> v_Path;

	Memory initMemory = Memory(*mem);
	Num = initMemory.GetNum();
	q.push(initMemory);
	queue<vector<int>> in;
	in.push(vector<int>({ 0, 0, 0 }));
	v_Path.push(in);

	while (!q.empty()) {
		Memory r = q.front();
		q.pop();
		queue<vector<int>> pathNode = v_Path.front();
		v_Path.pop();
		int adjustPath[2] = { pathNode.back()[0], pathNode.back()[1] };
		r.ProcessMove(adjustPath);

		if (!Fragment(r.init)) {
			if (min_size > pathNode.back()[2]) {
				min_size = pathNode.back()[2];
				Path = pathNode;
			}
		}

		else if (min_size > pathNode.back()[2]) {
			int nodesizes = pathNode.size();
			if (nodesizes < Num) {
				int Pos1 = 0;
				Process* location1 = r.init;
				while (location1 != NULL) {
					if (location1->Id != -1) {
						int Pos2 = 0;
						Process* location2 = r.init;
						while (location2 != NULL) {
							if (location2->Id == -1 && location2->Size >= location1->Size) {
								if (location1->Size + pathNode.back()[2] < min_size) {
									Memory _memory = Memory(r);
									q.push(_memory);

									queue<vector<int>> _path = queue<vector<int>>(pathNode);
									_path.push({ Pos1, Pos2, location1->Size + pathNode.back()[2] });
									v_Path.push(_path);
								}
							}

							location2 = location2->next;
							Pos2++;
						}
					}
					location1 = location1->next;
					Pos1++;
				}
			}
		}
	}
}

int Memory::GetHoleSize() {
	Process* location = init;
	int result = 0;
	while (location != NULL) {
		if (location->Id == -1) 
			result += location->Size;
		
		location = location->next;
	}
	return result;
}

int Memory::GetNum() {
	Process* location = init;
	int cnt = 0;
	while (location != NULL) {
		if (location->Id != -1) {
			cnt++;
		}
		location = location->next;
	}
	return cnt;
}

void Memory::Print() {
	Process* location = init;
	int hole_cnt = 0, hole_size = 0;
	cout << "\tMemory\t" << maxSize << "KB\n\n";
	while (location != NULL) {
		cout << "\t\t" << location->Pos << "KB ~ "<<location->Pos+location->Size<<"KB\t\t";
		if (location->Id == -1) {
			cout << "Hole";
			hole_size += location->Size;
			hole_cnt++;
		}
			
		else
			cout << "Process" << location->Id;

		cout << "(" << location->Size << "KB)\n";
		location = location->next;
	}
	cout << endl << endl;


	cout << "\t" << hole_size << "KB Free";
	cout << "\t" << hole_cnt << " Block(s)";
	cout << "\tAverage Size = " << float(float(hole_size) / float(hole_cnt)) << "KB" << endl;
}