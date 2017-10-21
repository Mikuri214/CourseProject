#include "face_gender_recognizer.h"

#include <opencv2/highgui.hpp>
using namespace cv;

#include <fstream>
#include <iostream>
using namespace std;


#define M 1
#define F 0

void read_csv ( String& csvPath, vector<String>& trainPath, vector<int>& label, char separator = ';' )
{
	string line, path, classLabel;
	ifstream file ( csvPath.c_str (), ifstream::in );

	while ( getline ( file, line ) )
	{
		stringstream lines ( line );
		getline ( lines, path, separator );
		getline ( lines, classLabel, separator );
		if ( !path.empty () && !classLabel.empty () )
		{
			trainPath.push_back ( path );
			label.push_back ( atoi ( classLabel.c_str () ) );
		}
	}
}

int main (){

	FaceGenderRecognizer recognizer ( "../../dependency/seetaface/FaceDetection/model/seeta_fd_frontal_v1.0.bin" );
	int choice = 0;
	while ( true)
	{
		cout << "ѡ��Ҫʹ�õĹ��ܣ�\n\t1.�����Ա�ģ��\n\t2.�Ա�ʶ��\n\t3.ѵ���Ա�ģ��\n\t4.���浱ǰ�Ա�ģ��" << endl;
		cin >> choice;

		switch ( choice )
		{
		case 1:
			cout << "������ģ��·��(c��·��)" << endl;
			{
				string path;
				cin >> path;
				if ( recognizer.load ( path ) ){
					cout << "successful." << endl;
				}else
				{
					cout << "failed." << endl;
				}
			}
			break;
		case 2:
			cout << "����������ͼƬ�ļ�·��(c��·��)" << endl;
			{
				string path;
				cin >> path;
				Mat src = imread ( path );
				int res = recognizer.predict ( src );
				cout << ( res?"��" :"Ů") << endl;
			}
			break;
		case 3:
			cout << "����������ͼƬ��csv�ļ�·��(c��·��)" << endl;
			{
				string path;
				cin >> path;
				vector<String> trainPath;
				vector<int> label;
				try {
					read_csv ( path, trainPath, label );
				}
				catch ( cv::Exception& e ) {
					cerr << "Error opening file " << path << ". Reason: " << e.msg << endl;
					// �ļ������⣬����ɶҲ�������ˣ��˳���
					exit ( 1 );
				}
				
				if ( recognizer.train ( trainPath, label ) )
					cout << "successful." << endl;
				else
					cout << "failed." << endl;
			}
			break;
		case 4:
			cout << "�����뱣���ļ�·��(����ģ���ļ���)" << endl;
			{
				string path;
				cin >> path;

				if ( recognizer.save ( path ) )
					cout << "successful." << endl;
				else
					cout << "failed." << endl;
			}
			break;

		default:
			cout << "Invalide choice." << endl;
			break;
		}
	}
	

// 	if ( recognizer.load ( "../../model/gender_model.txt" ) )
// 	{
// 
// 		recognizer.save ( "../../model/gender_model.bin" );
// 	}
// 
// 
// 	Mat src = imread ( "../../data/M1.JPG" );
// 	assert ( recognizer.predict ( src ) == M );
// 
// 	src = imread ( "../../data/F2.JPG" );
// 	assert ( recognizer.predict ( src ) == F );

	return 0;
}