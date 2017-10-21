#include <opencv2/ml.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include "opencv2/nonfree/nonfree.hpp" 
using namespace cv;

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <io.h>
using namespace std;

//opencv�ṩ���㷨  ����ʵ����ģ�����
char SIFT[] = "SIFT";
char SURF[] = "SURF";
char ORB[] = "ORB";

Size size ( 256 , 256 );
string objDir = "../data/";
string saveDir = "../result/";
int eigenVecLength = 0;

//������⺯��( ģ�����ֵ����)
template<char const *FEATURE>
bool getFeature ( Mat& img , Mat &descriptors )
{
	vector<KeyPoint> keyPoints;
	const Ptr<FeatureDetector> detector = FeatureDetector::create ( FEATURE );//�������������  
	const Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create ( FEATURE );//������������������  
	
	if ( detector.empty () || descriptor_extractor.empty ()  )
		return false;
	detector->detect ( img , keyPoints );//���img�е������㣬�洢��keypoints��  
	descriptor_extractor->compute ( img , keyPoints , descriptors );

	if ( keyPoints.empty () || descriptors.empty () )
		return false;
	return true;
}

bool getHogFeature ( Mat& img , Mat &descriptors )
{
	HOGDescriptor hog ( ::size , cvSize ( 24 , 24 ) , cvSize ( 8 , 8 ) , cvSize ( 8 , 8 ) , 9 );

	vector<float> descrip;
	hog.compute ( img , descrip);
	descriptors = Mat(descrip);

	return true;
}

typedef bool ( *EigenFunc ) ( Mat& img , Mat &descriptors );


//ͼƬԤ����    ��С��һ��
bool preprocess ( const Mat& src , Mat &dst )
{
	Mat tmp;
	resize ( src , tmp ,::size);
// 	cvtColor ( tmp , dst , CV_RGB2GRAY );
// 	equalizeHist ( dst , tmp );
	dst = tmp;
	return true;
}

//��ѵ����������������ת��Ϊ��������   һ�ж�Ӧһ����������������    ���������������ά����Ϊ�������������  ����Ĳ�0
static  Mat formatImages ( const vector<Mat> &data )
{
	int maxLength = 0;//�����������ά��
	for ( unsigned int i = 0; i < data.size (); i++ )
		if ( data[i].size().area() > maxLength )
			maxLength = data[i].size ().area();

	eigenVecLength = maxLength;

	Mat dst = Mat::zeros( static_cast< int >( data.size () ) , maxLength , CV_32F );

	for ( unsigned int i = 0; i < data.size (); i++ )
	{
		Mat image_row = data[i].clone ().reshape ( 1 , 1 );
		image_row.convertTo ( image_row , CV_32F );
		dst.row ( i ).colRange( Range(0,image_row.cols) ) += image_row;
	}
	return dst;
}


//���ڲ���ѵ�����
bool trainAndTestF ( vector<Mat> trainSet , vector<int> trainLabels , vector<Mat> testSet , vector<int> testLabels , string modelName , EigenFunc func )
{
	CvSVM svm;

	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat& img = trainSet[i];
		Mat tmp;
		preprocess ( img , tmp );
		func ( tmp , img );
	}

	Mat train = formatImages ( trainSet );

  	for ( int i = 0; i < train.rows; ++i )
  	{
  		trainSet[i] = train.row ( i );
  	}

	Mat label ( trainLabels );

	CvSVMParams param;	//svm ѵ���ĳ�ʼ����
	CvTermCriteria criteria; //svm ѵ�������ı�׼
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//��֧�������ķ���С��ָ��Epsilon ������
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //֧�ַ��������˹�˺���


	svm.train_auto ( train , label , Mat () , Mat () , param );
	svm.save ( ( saveDir + modelName ).c_str () );

	
	int iCorrectPrediction = 0;
	int predictedLabel;
	int testPhotoNumber = trainSet.size ();
	ofstream out ( saveDir +  + "Result.txt" , ios::out );

	if ( !out )
	{
		cout << "can't open " << modelName + "Result.txt\n";
		getchar ();
	}

	out << modelName << " : ";

	for ( int i = 0; i < testPhotoNumber; i++ )
	{
	
		Mat& testImage = trainSet[i];

		predictedLabel = svm.predict ( testImage );

		if ( predictedLabel == trainLabels[i] )
		{
			iCorrectPrediction++;
		}
		else
		{
			out << trainLabels[i]<< '\t' << predictedLabel << "\n";
		}

	}
	out << "correct:" << iCorrectPrediction << "\tTotal:" << testPhotoNumber << "\tcorrectRate" << iCorrectPrediction*1.f / testPhotoNumber << endl;
	return true;
}


//����ѵ��ģ�ͺͲ��Է�������PCA����������ά����ͬά�ȣ�
bool trainAndTestPca ( vector<Mat> trainSet , vector<int> trainLabels , vector<Mat> testSet , vector<int> testLabels , string modelName , EigenFunc func )
{
	CvSVM svm;

	//��ÿ��ѵ��ͼƬ����Ԥ������ȡ����
	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat& img = trainSet[i];
		Mat tmp;
		preprocess ( img , tmp );
		func ( tmp , img );
	}

	//������������תΪ��������
	Mat train = formatImages ( trainSet );

	//pca ��ά
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer ();
	for ( int i = 0; i < train.rows; ++i )
	{
		trainSet[i] = train.row ( i );
	}
	model->train ( trainSet , trainLabels );
	Mat  eigenvalues = model->getMat ( "eigenvalues" );
	Mat W = model->getMat ( "eigenvectors" );
	W.convertTo ( W , CV_32FC1 );
	train *= W;


	Mat label ( trainLabels );

	//svm����ѧϰ
	CvSVMParams param;	//svm ѵ���ĳ�ʼ����
	CvTermCriteria criteria; //svm ѵ�������ı�׼
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//��֧�������ķ���С��ָ��Epsilon ������
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //֧�ַ��������˹�˺���


	svm.train_auto ( train , label , Mat () , Mat () , param );
	svm.save ( ( saveDir + modelName ).c_str () );


	int iCorrectPrediction = 0;
	int predictedLabel;
	int testPhotoNumber = testSet.size ();
	ofstream out ( saveDir  + "Result.txt" , ios::app );
	 
	 	if ( !out )
	 	{
	 		cout << "can't open " << modelName + "Result.txt\n";
	 		getchar ();
	 	}

		out << modelName << " :";

	//���в��� �� ��ÿ�Ų���ͼƬ  Ԥ����� �� ��ȡ���� �� תΪ������ �� ��һ����ָ����������ά�ȣ��ٵĲ�0  ��Ľ�ȥ�� �� ��pcaģ������������˽�ά �� svmģ��Ԥ��
 	for ( int i = 0; i < testPhotoNumber; i++ )
	{
		
		Mat& testImage = testSet[i];
		Mat tmp;
		preprocess ( testImage , tmp );
		func ( tmp , testImage );

		tmp = Mat::zeros ( 1 , eigenVecLength , CV_32FC1 );
		testImage = testImage.reshape ( 1 , 1 );
		testImage.convertTo ( testImage , CV_32FC1 );
		tmp.colRange ( Range ( 0 , min ( testImage.cols , eigenVecLength ) ) ) += testImage.colRange ( Range ( 0 , min ( testImage.cols , eigenVecLength ) ) );
		tmp *= W;

		predictedLabel = svm.predict ( tmp );

		if ( predictedLabel == testLabels[i] )
		{
			iCorrectPrediction++;
		}
		else
		{
			//			out << testLabels[i] << '\t' << predictedLabel << "\n";
		}

	}
	out << "correct:" << iCorrectPrediction << "\tTotal:" << testPhotoNumber << "\tcorrectRate" << iCorrectPrediction*1.f/testPhotoNumber << endl;
	return true;
}


//����ѵ��ģ�ͺͲ��Է�������BOW �õ�ͳһά�ȵ�����������
bool trainAndTestBOW ( vector<Mat> trainSet , vector<int> trainLabels , vector<Mat> testSet , vector<int> testLabels , string modelName , EigenFunc func )
{
	CvSVM svm;

	const Ptr<FeatureDetector> detector = FeatureDetector::create ( modelName );//�������������  
	const Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( modelName );//������������������  
	const Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create ( "FlannBased" );


	//���ڴӲ���ͼƬ��ȡbow��������������
	BOWImgDescriptorExtractor bowDE ( descriptor , matcher );

	//���ڶ����в���������ȡ���������������������������  ��������ͼƬ�����о���  �õ�bow�ĵ��ʱ�
	//��Ϊʹ��sift/surf ��ȡͼƬ����ʱ ÿ��ͼƬ������ȡ�����������������ȷ��������ʹ��bow������ÿ��ͼƬ�õ�ͳһά�ȵ���������  ������Ϊ200����
	int dictionarySize = 200;
	BOWKMeansTrainer bowTrainer ( dictionarySize );

	vector<vector<KeyPoint> > keyPoints;
	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat& img = trainSet[i];
		Mat tmp , eigen;
		vector<KeyPoint> keypoint;

		preprocess ( img , tmp );
		img = tmp;

		detector->detect ( tmp , keypoint );//���img�е������㣬�洢��keypoints��  
		descriptor->compute ( tmp , keypoint , eigen );
		keyPoints.push_back ( keypoint );
		//��ÿ��������������������о���  
		bowTrainer.add ( eigen );
	}
	//����õ��ʻ��ÿһ��Ϊһ���ʻ��� ��
	Mat dictionary = bowTrainer.cluster ();
	bowDE.setVocabulary ( dictionary );

	//��ѵ�������ôʻ�����������������������Ƿ��иôʻ���  �ʻ����Ƶ�ʵȵ�һ������   ά�ȵ��ڴʻ���С��
	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat bow_descriptor;

		bowDE.compute ( trainSet[i] , keyPoints[i] , bow_descriptor );
		trainSet[i] = bow_descriptor;
	}

	Mat train = formatImages ( trainSet );

	Mat label ( trainLabels );

	//svm����ѧϰ
	CvSVMParams param;	//svm ѵ���ĳ�ʼ����
	CvTermCriteria criteria; //svm ѵ�������ı�׼
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//��֧�������ķ���С��ָ��Epsilon ������
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //֧�ַ��������˹�˺���


	svm.train_auto ( train , label , Mat () , Mat () , param );
	svm.save ( ( saveDir + modelName ).c_str () );


	int iCorrectPrediction = 0;
	int predictedLabel;
	int testPhotoNumber = testSet.size ();
	ofstream out ( saveDir + "Result.txt" , ios::out );

	if ( !out )
	{
		cout << "can't open " << modelName + "Result.txt\n";
		getchar ();
	}
	for ( int i = 0; i < testPhotoNumber; i++ )
	{

		Mat& testImage = testSet[i];
		Mat tmp;
		vector<KeyPoint> keypoint;
		preprocess ( testImage , tmp );

		detector->detect ( tmp , keypoint );
		bowDE.compute ( tmp , keypoint , testImage );

		predictedLabel = svm.predict ( testImage );

		if ( predictedLabel == testLabels[i] )
		{
			iCorrectPrediction++;
		}
		else
		{
			out << testLabels[i] << '\t' << predictedLabel << "\n";
		}

	}
	out << "correct:" << iCorrectPrediction << "\tTotal:" << testPhotoNumber << "\tcorrectRate" << iCorrectPrediction*1.f / testPhotoNumber << endl;
	return true;
}


//��ȡ dataĿ¼�µ�����  ÿ������ʹ��30����Ϊѵ����  ʣ�µ���Ϊ���Լ�
bool getImgs ( vector<Mat>& trainSet , vector<int>& trainLabels , vector<Mat>& testSet , vector<int>& testLabels , string rootDir )
{
	Directory dir;
	auto list = dir.GetListFolders ( rootDir );

	for ( int i = 1; i <= list.size(); ++i )
	{
		auto path = list[i-1];
		if ( path == "." || path == ".." )
		{
			continue;
		}
		auto imgsPathList = dir.GetListFiles ( path , "*.jpg" );

		//ѵ����
		for ( int j = 0; j < 30; ++j )
		{
			Mat tmp = imread ( imgsPathList[j] );
			//Ҫ������ǲ�ɫͼ  �Ե�ͨ��ͼƬ  תΪ��ͨ��
 			if ( tmp.channels () < 3 )
 			{
 				Mat planes[] = { tmp , tmp , tmp };
 				merge ( planes , 1 , tmp );
 			}
			trainSet.push_back ( tmp );
			trainLabels.push_back ( i );
		}
		//���Լ�
		for ( int j = 30; j < imgsPathList.size(); ++j )
		{
			Mat tmp = imread ( imgsPathList[j] );
			if ( tmp.channels () < 3 )
			{
				Mat planes[] = { tmp , tmp , tmp };
				merge ( planes , 1 , tmp );
			}
			testSet.push_back ( tmp );
			testLabels.push_back ( i );
		}
	}

	return true;
}

#define trainAndTest trainAndTestF

int main ()
{
	initModule_nonfree ();
	vector<Mat> trainSet;
	vector<int> trainLabels;
	vector<Mat> testSet;
	vector<int> testLabels;

	getImgs ( trainSet , trainLabels , testSet , testLabels , objDir );

	EigenFunc  fun;
	fun = getHogFeature;

	 	trainAndTest ( trainSet , trainLabels , testSet , testLabels , "Hog" , fun );
// 
//    	fun = getFeature < ::SIFT > ;
//  	trainAndTest ( trainSet , trainLabels , testSet , testLabels , ::SIFT , fun );
//  
//   	fun = getFeature < ::SURF > ;
//   	trainAndTest ( trainSet , trainLabels , testSet , testLabels , ::SURF , fun );

//   	fun = getFeature < ::ORB > ;
// 	trainAndTest ( trainSet , trainLabels , testSet , testLabels , ::ORB , fun );



	return 0;
}