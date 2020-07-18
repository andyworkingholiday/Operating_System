#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include "DoublyLinkedList.h"
#include "DoublyIterator.h"
#include "LinkedQueue.h"
#include "LinkedQueueIterator.h"
#include <conio.h>
#include <Windows.h>
using namespace std;

struct Block {
	int num; // block 번호
	bool free = false; // block 상태
	bool delay = false; // block이 delay 상태인지 여부

	Block() {
		num = 0;
	}

	Block(int n) {
		num = n;
	}
	string Display() {
		if (delay) {
			return to_string(num) + "_delayed_write";
		}
		else {
			return to_string(num);
		}
	}
	bool operator==(const Block& data) {
		if (data.num == num) {
			return true;
		}
		return false;
	}
};

int HeaderSize;
LinkedQueue<Block>* Hashqueue;
DoublyLinkedList<Block> FreeList;

void Init() {
	while (1) {
		cout << "Hash Queue Header 사이즈를 입력해주세요 : ";
		cin >> HeaderSize;
		if (HeaderSize >= 3 && HeaderSize <= 9) {
			Hashqueue = new LinkedQueue<Block>[HeaderSize];
			break;
		}
		else cout << "Hash Queue Header 사이즈는 3이상 9 이하입니다. 다시 입력해주세요." << endl;
	}
	
	cout << endl;
	for (int i = 0; i < HeaderSize; i++) {
		int blknum = 0;
		while (1) {
			cout << "No " << i << " Queue의 block의 갯수를 입력해주세요 : ";			
			cin >> blknum;
			if (blknum >= 3 && blknum <= 9) break;
			else cout << "Queue에 할당된 block의 사이즈는 3이상 9 이하입니다. 다시 입력해주세요." << endl << endl;
		}

		cout << "No " << i << " Queue에 할당될 block 번호를 입력해주세요." << endl;
		for (int j = 1; j <= blknum; j++) {
			while (1) {
				cout << j << "번 째 block 번호 : ";
				int blknum;
				cin >> blknum;
				if (blknum % HeaderSize == i) {
					Block block(blknum);
					Hashqueue[i].EnQueue(block);
					break;
				}
				else cout << "Block 번호를 headersize로 나눈 나머지 값이 " << i << "이 아닙니다. 다시 입력해주세요. " << endl << endl;
			}
			
		}
		cout << endl;
		
	}

	int freenum = 0;
	while (1) {
		cout << "FreeList에 할당할 block의 갯수를 입력해주세요 : ";
		cin >> freenum;
		if (freenum >= 3 && freenum <= 9) break;
		else cout << "FreeList에 할당될 block의 사이즈는 3이상 9 이하입니다. 다시 입력해주세요." << endl << endl;
	}

	cout << "FreeList에 할당할 block의 번호 "<<freenum<<"개를 입력해주세요 : ";
	while (freenum--) {		
		int a;
		cin >> a;
		Block block(a); block.free = true;
		FreeList.Add(block);
		int divnd = a % HeaderSize;
		Hashqueue[divnd].Replace(block);
	}
	

}

void Display() {
	for (int i = 0; i < HeaderSize; i++) {
		cout << "\tblk no " << i << " mod " << HeaderSize << " [ ";
		LinkedQueueIterator<Block> iter(Hashqueue[i]);
		Block temp;
		temp = iter.Next();
		while (iter.NextNotNull()) {
			cout << temp.Display() << " ";
			temp = iter.Next();
		}
		cout << "]" << endl;
	}

	cout << "\tFree List [";
	Block temp;
	DoublyIterator<Block> iter(FreeList);
	temp = iter.Next();
	while (iter.NextNotNull()) {
		cout << temp.Display() << " ";
		temp = iter.Next();
	}
	cout << "]" << endl;
}

void ChangetoDelayedWrite(int num) {
	int index = num % HeaderSize;
	bool found = false;
	Block block;

	LinkedQueueIterator<Block> iter(Hashqueue[index]);

	block = iter.Next();

	while (iter.NextNotNull()) {
		if (block.num == num) {
			if (block.delay == true) {
				block.delay = false;
			}
			else {
				block.delay = true;
			}
			FreeList.Replace(block);
			Hashqueue[index].Replace(block);
			found = true;
			break;
		}
		else
			block = iter.Next();
	}

	if (found == false) 
		cout << "\tBlock 번호가 잘못 되었습니다."<<endl;	
}

void ChangeState(int num) {								
	int index = num % HeaderSize;

	bool found = false;
	Block block;

	LinkedQueueIterator<Block> iter(Hashqueue[index]);

	block = iter.Next();

	while (iter.NextNotNull()) {
		if (block.num == num) {
			if (block.delay == true) {
				block.delay = false;
			}
			else {
				block.delay = true;
			}
			FreeList.Replace(block);
			Hashqueue[index].Replace(block);
			found = true;
			break;
		}
		block = iter.Next();
	}

	if (found == false) {
		cout << "\tBlock 번호가 잘못 되었습니다." << endl;
	}
}

int getblk(int num) {
	bool found = false;
	bool ex = false;

	int index = num % HeaderSize;

	Block block;
	int count = 0, count_free = 0;
	while (!found) {
		LinkedQueueIterator<Block> iter(Hashqueue[index]);
		block = iter.Next();

		while (iter.NextNotNull()) {
			if (block.num == num) {
				ex = true;
				break;
			}
			block = iter.Next();
		}

		if (ex) {
			if (block.free == false) {
				if (count == 3) {
					cout << endl;
					ChangeState(num);
				}

				if (count < 3) {
					cout << "\tSleep..." << count << " " << endl;
					Sleep(1000);
					count++;
					continue;
				}				
			}
			block.free = false;								
			FreeList.Delete(block);							
			Hashqueue[index].Replace(block);						
			return block.num;
		}

		else {
			if (FreeList.IsEmpty()) {				
				
				if (count_free == 3) {	
					cout << endl;
					Hashqueue[(num % HeaderSize) / 2].Dequeue(block);
					block.free = true;
					Hashqueue[(num % HeaderSize) / 2].Replace(block);
					FreeList.Add(block);
				}

				if (count_free < 3) {
					cout << "\tSleep..." << count_free << " " << endl;
					Sleep(1000);
					count_free++;
					continue;
				}			
			}

			block = FreeList.pop_back();

			if (block.delay) {								
				FreeList.Delete(block);								
				continue;
			}

			Hashqueue[block.num % HeaderSize].Delete(block);				
			block.num = num;
			block.delay = false;
			block.free = false;

			Hashqueue[num % HeaderSize].EnQueue(block);					

			return block.num;
		}
	}

}

int main() {
	Init();
	cout << endl << endl;

	int command;
	while (1) {
		Display();
		cout << endl;
		cout << "\t1. change block status to delayed write " << endl;
		cout << "\t2. getblk " << endl;
		cout << "\t3. exit" << endl;
		cout << endl;
		cout << "\tCommand를 입력해주세요 : ";
		cin >> command;

		if (command == 1) {
			cout << "\tDelayed Write로 변경시킬 block 번호를 입력해주세요 : ";
			int num;
			cin >> num;
			ChangetoDelayedWrite(num);
		}

		else if (command == 2) {
			cout << "\t할당 받을 block 번호를 입력해주세요 : ";
			int num;
			cin >> num;
			cout << "\tBlock " << getblk(num) << "을 할당 받았습니다." << endl;
		}

		else if (command == 3) break;

		else {
			cout << "\t잘못 입력하셨습니다. 커맨드를 다시 입력해주세요" << endl;
		}
	}

	return 0;
}