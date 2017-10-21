// ProcessDispatch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<string>
#include<cstdlib>
#include<Windows.h>
#include<iostream>
using namespace std;

#define NUM_OF_PROCESS 5
#define TIME_SPLIT 2

class PCB {
public:
	PCB( int id, int startTime, int excuteTime,int priority) {
		this->processID = id;
		this->startTime = startTime;
		this->excuteTime = excuteTime;
		this->timeSplit = TIME_SPLIT;
		this->hasDone = false;
		this->priority = priority;
	}
	~PCB( ) {}

	bool hasDone;
	long startTime;
	int excuteTime;
	int processID;
	int timeSplit;
	int priority;
};
//sort by start time
void sort( PCB *pro[] ) {
	for ( size_t i = 0; i < NUM_OF_PROCESS; i++ ) {
		int min = i;
		for ( size_t j = i + 1; j< NUM_OF_PROCESS; j++ )
		{
			if ( pro[ j ]->startTime < pro[ min ]->startTime ) min = j;
		}
		PCB* temp = pro[ i ];
		pro[ i ] = pro[ min ];
		pro[ min ] = temp;
	}
}
//sort by priority
void sort( PCB *pro[],int head, int range ) {
	for ( size_t i = head; i < range; i++ ) {
		int max = i;
		for ( size_t j = i + 1; j< range; j++ )
		{
			if ( pro[ j ]->priority > pro[ max ]->priority ) max = j;
		}
		PCB* temp = pro[ i ];
		pro[ i ] = pro[ max ];
		pro[ max ] = temp;
	}
}

bool available( PCB* pro[] ) {
	for ( size_t i = 0; i < NUM_OF_PROCESS; i++ )
		if ( !pro[ i ]->hasDone ) return true;
	return false;
}

void nextProcess(  PCB* pro[],int &curr ,int sys_time ) {
	pro[ curr ]->timeSplit = TIME_SPLIT;
	for ( int i = 1; i <= NUM_OF_PROCESS; i++ ) {
		if ( !pro[ ( curr + i ) % NUM_OF_PROCESS ]->hasDone && pro[ ( curr + i ) % NUM_OF_PROCESS ]->startTime <= sys_time) {
			curr = ( curr + i ) % NUM_OF_PROCESS;
			return;
		}
	}
}

void printQueue( PCB* pro[] ,int sys_time,int curr) {
	cout << sys_time << "\t";
	for ( size_t i = 0; i < NUM_OF_PROCESS; i++ )
	{
		if ( !pro[ i ]->hasDone && pro[ i ]->startTime <= sys_time ) {
			if ( i == curr ) cout << "[running]";
			cout << "Process" << pro[ i ]->processID << "\t";
		}
	}
	cout << endl;
}

void initProcess( PCB* pro[] ) {
	srand( 10 );
	for ( int i = 0; i < NUM_OF_PROCESS; i++ ) {
		int start_time = rand( ) % 5;
		int execute_time = rand( ) % 5;
		int priority = rand( ) % 10;
		pro[ i ] = new PCB( i, start_time, execute_time,priority );
	}
	sort( pro );

}

void RoundRobinDispatch(){
	PCB *process[ NUM_OF_PROCESS ];
	initProcess( process );
	for ( int i = 0; i < NUM_OF_PROCESS; i++ ) {
		cout << "Process" << process[ i ]->processID << "\tstart:" << process[ i ]->startTime << "\texecute:" << process[ i ]->excuteTime << endl;
	}

	int curr = 0;
	int sys_time = 0;
	int total = 0;

	cout << "轮转调度-----------------------------------------------------------------------------------\n";
	while ( available( process ) ) {
		if ( process[ curr ]->startTime <= sys_time ) {
			printQueue( process, sys_time, curr );
			process[ curr ]->timeSplit--;  process[ curr ]->excuteTime--;

			if ( process[ curr ]->excuteTime == 0 ) {
				cout << "Process" << process[ curr ]->processID << "finish\n";
				total += ( sys_time - process[ curr ]->startTime + 1);
				process[ curr ]->hasDone = true;
				nextProcess( process, curr, sys_time );
			}
			else if ( process[ curr ]->timeSplit == 0 )nextProcess( process, curr, sys_time );
		}
		Sleep( 100 );
		sys_time++;
	}
	cout << "average timeout is:" << total*1.0 / NUM_OF_PROCESS << endl<<endl;
	for ( int i = 0; i < NUM_OF_PROCESS; i++ )delete process[ i ];
}

void PriorityDispatch( ) {
	PCB *process[ NUM_OF_PROCESS ];
	initProcess( process );
	for ( int i = 0; i < NUM_OF_PROCESS; i++ ) {
		cout << "Process" << process[ i ]->processID << "\tstart:" << process[ i ]->startTime << "\texcute:" << process[ i ]->excuteTime <<"\tpriority:"<<process[i]->priority<< endl;
	}

	int range = 0,head=0;
	int sys_time = 0;
	int total = 0;

	cout << "剥夺式动态优先级调度--------------------------------------------------------------------------\n";
	while ( available( process ) ) {
		while ( range < NUM_OF_PROCESS && process[ range ]->startTime <= sys_time ) {
			process[ range ]->timeSplit = process[ range ]->priority*TIME_SPLIT;
			range++;
		}
		sort( process,head, range );
		if ( head < range ) {
			printQueue( process, sys_time, head );
			process[ head ]->timeSplit--;  process[ head ]->excuteTime--;

			if ( process[ head ]->excuteTime == 0 ) {
				cout << "Process" << process[ head ]->processID << "finish\n";
				total += ( sys_time - process[ head ]->startTime + 1 );
				process[ head ]->hasDone = true;
				head++;
			}
			else if ( process[ head ]->timeSplit == 0 ) {
				process[ head ]->priority--;
				process[ head ]->timeSplit = process[ head ]->priority*TIME_SPLIT;
			}
		}
		Sleep( 100 );
		sys_time++;
	}
	cout << "average timeout is:" << total*1.0 / NUM_OF_PROCESS << endl;;
	for ( int i = 0; i < NUM_OF_PROCESS; i++ )delete process[ i ];

}


int main()
{	
	RoundRobinDispatch( );
	PriorityDispatch( );

	system( "pause" );
    return 0;
}

