#pragma once

struct Process {
	int Id; // Process ID
	int Size; // Process Size
	int Pos; // Process Store Position
	Process* next;
};

class Memory {
public:
	Memory(int size);
	Memory(const Memory& mem);
	~Memory(); 

	void Alloc(int id, int size, bool flag);
	void Coalescing(bool flag);
	void Compaction(bool flag);
	void ProcessMove(int pos[2]);
	int GetHoleSize();
	void Print();
	int GetNum();

	int maxNum;
	int maxSize;
	Process* init;
};