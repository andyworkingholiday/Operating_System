#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;

//Task의 정보를 담는 구조체: 태스크이름, 실행시간, 주기가 저장된다.
struct Task{
	string name;
	int computation_time, period;
};

//차트 데이터 구조체: 태스크의 이름, 해당 시간 방문 여부가 저장된다.
struct Chartdata {
	string name;
	bool flag;
};

//vector안에서 주기순으로 정렬하기 위한 bool 함수
bool cmp(Task a, Task b) {
	return(a.period < b.period);
}

vector<Task> Ptasks; //주기 태스크
vector<Task> APtasks; // 비주기 태스크 APtasks에서의 period는 도착시간이라고 가정
Chartdata* BackgroundChart; //Background Aps에서 담기는 정보
Chartdata* PollingChart; //Polling Server에서 담기는 정보

int NumOfPTask, NumOfApTask; // 각 태스트 들의 갯수
int HyperPeriod = 1; // 최소공배수 개산할 때 필요
int Polling_Capacity, Poliing_Period; //폴링서버의 Capacity와 주기

//최대 태스크 갯수 10개, 최대 Hyperbolic Period = 100
int Gant[11][100];


//태스크를 입력받는 함수
void Input() {
	cout << "\tEnter the Number of Periodic Tasks : ";
	cin >> NumOfPTask;

	cout << "\tEnter the Number of Aperiodic Tasks : ";
	cin >> NumOfApTask;

	cout << endl;
	cout << "\t주기 Task의 정보를 입력해주세요" << endl;

	for (int i = 1; i <= NumOfPTask; i++) {
		cout << endl;
		int ctime, p;
		cout << "\tPeriodic Task " <<i<<" Computation Time : ";
		cin >> ctime;
		cout << "\tPeriodic Task " <<i<<" Period : ";
		cin >> p;

		string Tname = 'P' + to_string(i);
		Ptasks.push_back({ Tname,ctime,p });
	}

	cout << endl;
	cout << "\t비주기 Task의 정보를 입력해주세요" << endl;

	for (int i = 1; i <= NumOfApTask; i++) {
		cout << endl;
		int ctime, p;
		cout << "\tAperiodic Task " <<i<<" Computation Time : ";
		cin >> ctime;
		cout << "\tAperiodic Task " <<i<<" Arrival Time : ";
		cin >> p;

		string Tname = "AP" + to_string(i);
		APtasks.push_back({ Tname,ctime,p });
	}
}

void CalcLCM() {
	//최소공배수 계산

	for (int i = 0; i < NumOfPTask; i++) {
		HyperPeriod *= Ptasks[i].period;
	}

	for (int i = 1; i <= HyperPeriod; i++) {
		bool lcm = true;
		for (int j = 0; j < NumOfPTask; j++) {
			if (i % Ptasks[j].period != 0) {
				lcm = false;
				break;
			}
		}

		if (lcm) {
			HyperPeriod = i;
			break;
		}
	}
}

//태스크 정보를 표로 출력하는 함수
void print_table(vector<Task> v, int n) {
	puts("\t+-----+------------------+--------+");
	puts("\t| Num | Computation Time | Period |");
	puts("\t+-----+------------------+--------+");

	for (int i = 0; i < n; i++) {
		printf("\t| %2d  |        %2d        |   %2d   |\n"
			, i + 1, v[i].computation_time, v[i].period);
		puts("\t+-----+------------------+--------+");
	}
}


//간트 차트 출력 함수 (HyperPeriod가 70을 넘어갈 시 13인치 노트북에서는 잘릴 수 있다.)
void print_gantt_chart() {
	cout << "\t";
	for (int i = 0; i < HyperPeriod; i++) {
		cout << "+--";
	}
	cout <<"+"<< endl;

	cout << "\t";
	for (int i = 0; i < HyperPeriod; i++) {
		if (i < 10) {
			cout << "|0" << i;
		}
		else {
			cout << "|" << i;
		}
	}
	cout << "|" << endl;

	cout << "\t";
	for (int i = 0; i < HyperPeriod; i++) {
		cout << "+--";
	}
	cout << "+" << endl;

	for (int i = 1; i <= NumOfPTask + NumOfApTask; i++) {
		if (i <= NumOfPTask) {
			cout << "    P" << i << "  ";
			for (int j = 0; j < HyperPeriod; j++) {
				if (Gant[i][j]) cout << "===";
				else cout << "   ";
			}
		}

		else {
			cout << "   AP" << i - NumOfPTask << "  ";
			for (int j = 0; j < HyperPeriod; j++) {
				if (Gant[i][j]) cout << "===";
				else cout << "   ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

// Background APS에서 비주기 태스크의 평균지연시간을 계산하고 
// 차트에 해당 시간에 태스크를 넣는다.
void CalcBackground() {
	float AvgAperiodicWaitingTime = 0;
	// Chart에 각 시간에 수행되는 Task를 넣는다.
	for (int pNum = 0; pNum < NumOfPTask; pNum++) {
		Task now = Ptasks[pNum];
		int TaskAmount = 0, idx = 0, cnt = 0;
		while (1) {
			if (idx >= HyperPeriod) break;

			if (!BackgroundChart[idx].flag) {
				BackgroundChart[idx].name = now.name;
				BackgroundChart[idx].flag = true;
				TaskAmount++;

				if (TaskAmount == now.computation_time) {
					cnt++;
					idx = now.period * cnt;
					TaskAmount = 0;
				}

				else {
					idx++;
				}
			}

			else {
				idx++;
			}
		}
	}

	//Aperiodic Task를 비워진 시간에 넣는다. 

	for (int Apnum = 0; Apnum < NumOfApTask; Apnum++) {
		Task now = APtasks[Apnum];
		int TaskAmount = 0;
		int Arrive = now.period;
		for (int i = Arrive; i < HyperPeriod; i++) {
			if (!BackgroundChart[i].flag) {
				TaskAmount++;
				BackgroundChart[i].name = now.name;
				BackgroundChart[i].flag = true;
				if (TaskAmount == now.computation_time) {
					AvgAperiodicWaitingTime += (i - Arrive);
					break;
				}
			}
		}
	}

	//평균 비주기 태스크 지연시간 계산
	AvgAperiodicWaitingTime /= NumOfApTask;

	//간트 차트 출력을 위해 간트 Array에 정보 넣기
	for (int i = 0; i < HyperPeriod; i++) {
		if (BackgroundChart[i].flag) {
			string s = BackgroundChart[i].name;

			if (s[0] == 'P') {
				char num = s[1];
				int row = num - '0';
				Gant[row][i] = 1;
			}

			else {
				char num = s[2];
				int row = num - '0' + NumOfPTask;
				Gant[row][i] = 1;
			}
		}
	}

	//간트 차트 출력
	cout << endl << endl;
	cout << "\tBackground APS" << endl;
	cout << "\tGantt Chart" << endl;
	print_gantt_chart();
	memset(Gant, 0, sizeof(Gant));

	cout << endl;

	cout << "\t평균 대기 시간 : " << AvgAperiodicWaitingTime << "s" << endl;
}

// Polling Server에서 비주기 태스크의 평균지연시간을 계산하고 
// 차트에 해당 시간에 태스크를 넣는다.
void CalcPolling() {

	float  AvgAperiodicWaitingTime = 0;

	//Polling server의 주기에 맞추어 Aperiodic Task를 넣는다.

	for (int ApNum = 0; ApNum < NumOfApTask; ApNum++) {
		Task Now = APtasks[ApNum];
		int Taskamount = 0;
		int Arrive = Now.period;

		for (int i = Poliing_Period; i < HyperPeriod; i += Poliing_Period) {
			bool done = false;
			for (int j = 0; j < Polling_Capacity; j++) {
				int index = i + j;
				if (index >= HyperPeriod) continue;
				if (index < Arrive) continue;

				if (!PollingChart[index].flag) {
					PollingChart[index].name = Now.name;
					PollingChart[index].flag = true;
					Taskamount++;
					if (Taskamount == Now.computation_time) {
						AvgAperiodicWaitingTime += (i - Arrive);
						done = true;
						break;
					}
				}
			}

			if (done) break;
		}
	}

	// Chart에 각 시간에 수행되는 Task를 넣는다.
	for (int pNum = 0; pNum < NumOfPTask; pNum++) {
		Task now = Ptasks[pNum];
		int TaskAmount = 0, idx = 0, cnt = 0;
		while (1) {
			if (idx >= HyperPeriod) break;

			if (!PollingChart[idx].flag) {
				PollingChart[idx].name = now.name;
				PollingChart[idx].flag = true;
				TaskAmount++;

				if (TaskAmount == now.computation_time) {
					cnt++;
					idx = now.period * cnt;
					TaskAmount = 0;
				}

				else {
					idx++;
				}
			}

			else {
				idx++;
			}
		}
	}

	//평균 비주기 태스크 지연시간 계산
	AvgAperiodicWaitingTime /= NumOfApTask;

	//간트 차트 출력을 위해 간트 Array에 정보 넣기
	for (int i = 0; i < HyperPeriod; i++) {
		if (PollingChart[i].flag) {
			string s = PollingChart[i].name;

			if (s[0] == 'P') {
				char num = s[1];
				int row = num - '0';
				Gant[row][i] = 1;
			}

			else {
				char num = s[2];
				int row = num - '0' + NumOfPTask;
				Gant[row][i] = 1;
			}
		}
	}

	//간트 차트 출력
	cout << endl << endl;
	cout << "\tPolling Server APS" << endl;
	cout << "\tGantt Chart" << endl;
	print_gantt_chart();
	memset(Gant, 0, sizeof(Gant));

	cout << endl;
	cout << "\t평균 대기 시간 : " << AvgAperiodicWaitingTime << "s" << endl;
}

int main() {

	while (1) {
		cout << endl << endl;
		// 입력받기
		Input();

		//HyperPeriod 계산하기
		CalcLCM();

		// Periodic Task, Aperiodic Task 정보 출력
		cout << endl;
		cout << endl;
		cout << "\tPeriodic Tasks" << endl;
		print_table(Ptasks, NumOfPTask);

		cout << endl;
		cout << endl;
		cout << "\tAperiodic Tasks" << endl;
		print_table(APtasks, NumOfApTask);

		cout << endl;

		// 우선순위가 높은 즉, 주기가 짧거나 도착시간이 짧은 순으로 정렬
		sort(Ptasks.begin(), Ptasks.end(), cmp);
		sort(APtasks.begin(), APtasks.end(), cmp);

		// 차트 초기화
		BackgroundChart = new Chartdata[HyperPeriod];
		for (int i = 0; i < HyperPeriod; i++) {
			BackgroundChart[i].flag = false;
		}

		PollingChart = new Chartdata[HyperPeriod];
		for (int i = 0; i < HyperPeriod; i++) {
			PollingChart[i].flag = false;
		}

		//폴링 서버의 Capacity 및 주기 입력받기
		cout << "\tPolling Server Capacity : ";
		cin >> Polling_Capacity;
		cout << "\tPolling Server Period : ";
		cin >> Poliing_Period;

		// 프로그램은 계속 실행되므로 모든 정보를 초기화시킨다.
		CalcBackground();
		delete[] BackgroundChart;

		cout << endl;
		cout << endl;

		CalcPolling();
		delete[] PollingChart;

		Ptasks.clear();
		APtasks.clear();
		NumOfPTask = 0, NumOfApTask = 0, HyperPeriod = 1, Polling_Capacity = 0, Poliing_Period = 0;
		cout << endl;
		cout << "\t프로그램을 종료하려면 숫자 0을 입력해주세요(계속하려면 0제외 아무숫자) : ";
		int command;
		cin >> command;
		if (command == 0) break;
	}
	return 0;
}