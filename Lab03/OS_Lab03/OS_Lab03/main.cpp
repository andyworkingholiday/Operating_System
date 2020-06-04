#include <iostream>
#include "Memory.h"
using namespace std;


Memory* memory = new Memory(0);

int main() {
	int TotalSize;
	cout << "\t======== Contiguous Memory Allocation ======== " << endl << endl;
	
	cout << "\tEnter the Memory Size" << endl;

	while (1) {
		cout << "\tMemory Size : ";
		cin >> TotalSize;
		if (TotalSize > 1000) cout << "\n\tMemory Size should be less than 1000KB" << endl;
		else {
			cout << "\n\tMemory Initialize\t" << TotalSize << "KB\n";
			memory = new Memory(TotalSize);
			break;
		}
	}
	

	while (1) {
		int command;
		cout << endl << endl;
		cout << "\t---ID -- Command ----- " << endl;
		cout << "\t   1 : Request Process" << endl;
		cout << "\t   2 : Free Process" << endl;
		cout << "\t   0 : Quit " << endl;

		cout << endl << "\t Choose a Command--> ";
		cin >> command;
		cout << endl;

		if (command == 1) {
			int num, size;
			cout << "\tEnter the Process Number & Size" << endl << endl;
			cout << "\tProcess Number : ";
			cin >> num;
			cout << "\tProcess Size : ";
			cin >> size;
			cout << endl;
			memory->Alloc(num, size, true);
		}

		else if (command == 2) {
			int num;
			cout << "\tEnter the Process Number" << endl << endl;
			cout << "\tProcess Number : ";
			cin >> num;
			cout << endl;
			memory->Alloc(num, 0, true);
		}

		else if (command == 0) break;
		else cout << "\n\n\tInput Error!\n\n";


	}

	return 0;
}