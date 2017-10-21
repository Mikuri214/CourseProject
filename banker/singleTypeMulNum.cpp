#include<iostream>
#include<string>
#include<fstream>
#include<ctime>

#include<vector>
using namespace  std;

#define  resource_total 5
#define  biggest_demand_for_per_process 5
#define PROCESS 5  //进程数

void print( vector<int> j, vector<int> p, int i ) {
	for ( int i = 0; i < PROCESS; i++ ) {
		cout <<i<<" "<< j[ i ] << ' ' << p[ i ];
		cout << endl;
	}
	cout << "available :";
	cout << i << ' ';
	cout << endl << endl;

}
ofstream f( "./out.txt", ios::out );

bool isFinish( vector<bool> finish ) {
	for ( int i = 0; i < PROCESS; i++ ) if ( !finish[ i ] ) return false;
	return true;
}


bool isSafe( vector<int> res_hold, vector<int> res_demand, int res_avaid ,vector<bool> finish ) {
	
	while ( !isFinish( finish ) ) {
		int i;
		for ( i = 0; i < PROCESS; i++ ) {
			if ( !finish[ i ] &&  res_demand[ i ] <= res_avaid) {
				res_avaid += res_hold[ i ];
				finish[ i ] = true;

				break;
			}	
		}
		if ( i == PROCESS ) return false;
	}
	return true;
}


void init( vector<int> &res_demand, int &res_avaid ) {
	res_avaid = resource_total;
	for ( int i = 0; i < PROCESS; i++ )     res_demand[ i ] = rand( ) % biggest_demand_for_per_process + 1;
}


void getRequst( int &req, int &process, vector<bool> finish, const vector<int>&res_demand ) {
	do {
		process = rand( ) % PROCESS;
	} while ( finish[ process ] );

	if ( res_demand[ process ] != 0 )
		req = rand( ) % res_demand[ process ] + 1;
}


void changeData( int &res_hold, int &res_demand, int &res_avaid, int req ) {
		res_avaid -= req;
		res_demand -= req;
		res_hold += req;
}


void restore( int &res_hold, int &res_demand, int &res_avaid, int req) {
		res_avaid += req;
		res_demand += req;
		res_hold -= req;
}


void log( int process, int request, string response ) {
	f << process << "\t"
	 << request << " "
	 << response << "\n";
}

void banker( vector<int> res_hold, vector<int> res_demand, int res_avaid ) {
	vector<bool> finish( PROCESS, false ); 
	int processID;
	int requst;

	while ( !isFinish( finish ) ) {
		getRequst( requst, processID, finish, res_demand );

		if ( requst > res_avaid ) continue;  cout << endl << "process " << processID << " request" << requst << endl; print( res_hold, res_demand, res_avaid );

		changeData( res_hold[ processID ], res_demand[ processID ], res_avaid, requst );	

		if ( !isSafe( res_hold, res_demand, res_avaid, finish) ) {
			cout << "roll back\n";
			restore( res_hold[ processID ], res_demand[ processID ], res_avaid, requst );
			log( processID, requst, "refuse" );
		}
		else {
			cout << "success\n";
			log( processID, requst, "success" );
			if ( res_demand[processID] == 0 ) {
				res_avaid += res_hold[processID];
				finish[ processID ] = true;
			}
			
		}	       
	}
}

int main( ) {
	vector<int> res_hold( PROCESS  );
	vector<int> res_demand( PROCESS );
	int res_avaid;
	srand( time( NULL ) );
	vector<bool> finish( PROCESS, false );
	do {
		for each ( bool var in finish ) var = false;
		init( res_demand, res_avaid );
	} while ( !isSafe( res_hold, res_demand, res_avaid,finish) );

	
	print( res_hold, res_demand, res_avaid );

	banker( res_hold, res_demand, res_avaid );
	system( "pause" );
	return 0;
}

