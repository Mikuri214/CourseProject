#ifndef FACE_GENDER_RECOGNIZER_H_
#define FACE_GENDER_RECOGNIZER_H_

#include "common.h"

#include "cv.h"
#include "highgui.h"
#include "ml.hpp"
using namespace cv;

#include "face_detection.h"
using namespace seeta;

#include <string>
using namespace std;

class FGR_API FaceGenderRecognizer
{
public:
	/*
	model_path: 用此sdk训练保存的性别识别模型文件的路径
	seetaface_detector_model_path: seetaface  人脸检测模块的模型文件的路径
	性别识别模型可以延后加载，但seetaface 人脸识别模型必须在构造时加载
	*/
	FaceGenderRecognizer ( const char* seetaface_detector_model_path, const char* model_path = nullptr);
	virtual ~FaceGenderRecognizer ();

	/*
	src:输入人脸图片（包含一张人脸）
	返回值：true--male   false--female  对一张输入图片（无论是否有人脸或有多个人脸），一定有且只有一个输出
	*/
	bool predict ( const Mat& src );

	/*
	标签; female - 0  male - 1   
	标签与图片必须是一对一的，集合大小相等
	trainSet:训练图片的路径集合
	*/
	bool train ( const vector<string>& trainSet,const vector<int>& trainLabels );
	/*
	trainSet:训练图片的集合
	*/
	bool train ( const vector<Mat>& trainSet, const vector<int>& trainLabels );
	/*
	将性别模型保存到指定路径path  eg.  C:/dir/xxModel.bin
	path不能只是文件夹路径 ， 要指定模型文件名
	*/
	bool save ( const string path );
	/*
	加载以前训练好的性别模型文件，会替换掉当前性别模型
	性别模型必须为用此sdk训练保存的
	path不能是文件夹路径，要指定模型文件名
	*/
	bool load ( const string path );

private:
	CvSVM* svm = NULL;
	FaceDetection *face_detector = NULL;

	/*
	预处理：将人脸图片中的人脸区域提取出来，统一大小
	*/
	bool preprocess (const Mat& src, Mat &dst );
};

#endif