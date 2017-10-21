#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<math.h>
using namespace std;
class smithwatman
{
public:
	smithwatman(string A,string B,float extgap,float opengap,float mismatch,float match);
	smithwatman(void);
	~smithwatman(void);
	//float **H;
	string A,B;
	vector <vector <float>> H; 
	float S(char a, char b);
	float W(int k);
	void Align(void);
	void traceback(void);
	float extgap;
	float opengap;
	float mismatch;
	float match;
};

