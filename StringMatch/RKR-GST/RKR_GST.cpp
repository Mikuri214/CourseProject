#include "stdafx.h"
#include <iostream>
#include "RKR_GST.h"

using namespace std;
 int main ()
 {
 	    string A,B; 
 	     A="abcd-efgh-ijklm";
 	 	B= "defgh-abcij--klm";
		vector<Title> matches; //���ڱ���ƥ��λ�� ������ ��Ϣ
		RKR_GST rs ( A, B,matches, 2 );

		for ( int i = 0; i < matches.size (); i++ )
			cout << matches[i].tStart << " " << matches[i].pStart<< " " << matches[i].len << endl;
		system ( "pause" );
 }