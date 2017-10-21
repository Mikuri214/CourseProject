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
		cout << "选择要使用的功能：\n\t1.加载性别模型\n\t2.性别识别\n\t3.训练性别模型\n\t4.保存当前性别模型" << endl;
		cin >> choice;

		switch ( choice )
		{
		case 1:
			cout << "请输入模型路径(c盘路径)" << endl;
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
			cout << "请输入人脸图片文件路径(c盘路径)" << endl;
			{
				string path;
				cin >> path;
				Mat src = imread ( path );
				int res = recognizer.predict ( src );
				cout << ( res?"男" :"女") << endl;
			}
			break;
		case 3:
			cout << "请输入人脸图片集csv文件路径(c盘路径)" << endl;
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
					// 文件有问题，我们啥也做不了了，退出了
					exit ( 1 );
				}
				
				if ( recognizer.train ( trainPath, label ) )
					cout << "successful." << endl;
				else
					cout << "failed." << endl;
			}
			break;
		case 4:
			cout << "请输入保存文件路径(包含模型文件名)" << endl;
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