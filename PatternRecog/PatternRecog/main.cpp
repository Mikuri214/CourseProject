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

//opencv提供的算法  用于实例化模板参数
char SIFT[] = "SIFT";
char SURF[] = "SURF";
char ORB[] = "ORB";

Size size ( 256 , 256 );
string objDir = "../data/";
string saveDir = "../result/";
int eigenVecLength = 0;

//特征检测函数( 模板参数值重载)
template<char const *FEATURE>
bool getFeature ( Mat& img , Mat &descriptors )
{
	vector<KeyPoint> keyPoints;
	const Ptr<FeatureDetector> detector = FeatureDetector::create ( FEATURE );//创建特征检测器  
	const Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create ( FEATURE );//创建特征向量生成器  
	
	if ( detector.empty () || descriptor_extractor.empty ()  )
		return false;
	detector->detect ( img , keyPoints );//检测img中的特征点，存储到keypoints中  
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


//图片预处理    大小归一化
bool preprocess ( const Mat& src , Mat &dst )
{
	Mat tmp;
	resize ( src , tmp ,::size);
// 	cvtColor ( tmp , dst , CV_RGB2GRAY );
// 	equalizeHist ( dst , tmp );
	dst = tmp;
	return true;
}

//将训练集的特征向量集转化为特征矩阵   一行对应一个样本的特征向量    以最长的特征向量的维度作为特征矩阵的列数  不足的补0
static  Mat formatImages ( const vector<Mat> &data )
{
	int maxLength = 0;//特征向量最大维度
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


//用于测试训练误差
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

	CvSVMParams param;	//svm 训练的初始参数
	CvTermCriteria criteria; //svm 训练收敛的标准
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//以支持向量的方差小于指定Epsilon 来收敛
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //支持分类机，高斯核函数


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


//用于训练模型和测试泛化误差（用PCA将特征矩阵降维到相同维度）
bool trainAndTestPca ( vector<Mat> trainSet , vector<int> trainLabels , vector<Mat> testSet , vector<int> testLabels , string modelName , EigenFunc func )
{
	CvSVM svm;

	//对每张训练图片进行预处理并提取特征
	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat& img = trainSet[i];
		Mat tmp;
		preprocess ( img , tmp );
		func ( tmp , img );
	}

	//将特征向量集转为特征矩阵
	Mat train = formatImages ( trainSet );

	//pca 降维
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

	//svm分类学习
	CvSVMParams param;	//svm 训练的初始参数
	CvTermCriteria criteria; //svm 训练收敛的标准
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//以支持向量的方差小于指定Epsilon 来收敛
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //支持分类机，高斯核函数


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

	//进行测试 ： 对每张测试图片  预处理后 》 提取特征 》 转为行向量 》 归一化到指定特征向量维度（少的补0  多的截去） 》 与pca模型特征矩阵相乘降维 》 svm模型预测
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


//用于训练模型和测试泛化误差（用BOW 得到统一维度的特征向量）
bool trainAndTestBOW ( vector<Mat> trainSet , vector<int> trainLabels , vector<Mat> testSet , vector<int> testLabels , string modelName , EigenFunc func )
{
	CvSVM svm;

	const Ptr<FeatureDetector> detector = FeatureDetector::create ( modelName );//创建特征检测器  
	const Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( modelName );//创建特征向量生成器  
	const Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create ( "FlannBased" );


	//用于从测试图片提取bow描述的特征向量
	BOWImgDescriptorExtractor bowDE ( descriptor , matcher );

	//用于对所有测试样本提取的特征向量（特征点的描述向量  不是整张图片）进行聚类  得到bow的单词表
	//因为使用sift/surf 提取图片特征时 每张图片可以提取到的特征点的数量不确定，这里使用bow方法对每张图片得到统一维度的特征向量  这里设为200个类
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

		detector->detect ( tmp , keypoint );//检测img中的特征点，存储到keypoints中  
		descriptor->compute ( tmp , keypoint , eigen );
		keyPoints.push_back ( keypoint );
		//用每个特征点的描述向量进行聚类  
		bowTrainer.add ( eigen );
	}
	//聚类得到词汇表（每一类为一个词汇项 ）
	Mat dictionary = bowTrainer.cluster ();
	bowDE.setVocabulary ( dictionary );

	//将训练样本用词汇表描述样本的特征向量（是否有该词汇项  词汇项的频率等的一个向量   维度等于词汇表大小）
	for ( int i = 0; i < trainSet.size (); ++i )
	{
		Mat bow_descriptor;

		bowDE.compute ( trainSet[i] , keyPoints[i] , bow_descriptor );
		trainSet[i] = bow_descriptor;
	}

	Mat train = formatImages ( trainSet );

	Mat label ( trainLabels );

	//svm分类学习
	CvSVMParams param;	//svm 训练的初始参数
	CvTermCriteria criteria; //svm 训练收敛的标准
	criteria = cvTermCriteria ( CV_TERMCRIT_EPS , 1000 , FLT_EPSILON );//以支持向量的方差小于指定Epsilon 来收敛
	param = CvSVMParams ( CvSVM::NU_SVC , CvSVM::RBF , 10.0 , 0.09 , 1.0 , 10.0 , 0.5 , 1.0 , NULL , criteria ); //支持分类机，高斯核函数


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


//读取 data目录下的物体  每类物体使用30张作为训练集  剩下的作为测试集
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

		//训练集
		for ( int j = 0; j < 30; ++j )
		{
			Mat tmp = imread ( imgsPathList[j] );
			//要处理的是彩色图  对单通道图片  转为三通道
 			if ( tmp.channels () < 3 )
 			{
 				Mat planes[] = { tmp , tmp , tmp };
 				merge ( planes , 1 , tmp );
 			}
			trainSet.push_back ( tmp );
			trainLabels.push_back ( i );
		}
		//测试集
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