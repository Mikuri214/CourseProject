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
	model_path: �ô�sdkѵ��������Ա�ʶ��ģ���ļ���·��
	seetaface_detector_model_path: seetaface  �������ģ���ģ���ļ���·��
	�Ա�ʶ��ģ�Ϳ����Ӻ���أ���seetaface ����ʶ��ģ�ͱ����ڹ���ʱ����
	*/
	FaceGenderRecognizer ( const char* seetaface_detector_model_path, const char* model_path = nullptr);
	virtual ~FaceGenderRecognizer ();

	/*
	src:��������ͼƬ������һ��������
	����ֵ��true--male   false--female  ��һ������ͼƬ�������Ƿ����������ж����������һ������ֻ��һ�����
	*/
	bool predict ( const Mat& src );

	/*
	��ǩ; female - 0  male - 1   
	��ǩ��ͼƬ������һ��һ�ģ����ϴ�С���
	trainSet:ѵ��ͼƬ��·������
	*/
	bool train ( const vector<string>& trainSet,const vector<int>& trainLabels );
	/*
	trainSet:ѵ��ͼƬ�ļ���
	*/
	bool train ( const vector<Mat>& trainSet, const vector<int>& trainLabels );
	/*
	���Ա�ģ�ͱ��浽ָ��·��path  eg.  C:/dir/xxModel.bin
	path����ֻ���ļ���·�� �� Ҫָ��ģ���ļ���
	*/
	bool save ( const string path );
	/*
	������ǰѵ���õ��Ա�ģ���ļ������滻����ǰ�Ա�ģ��
	�Ա�ģ�ͱ���Ϊ�ô�sdkѵ�������
	path�������ļ���·����Ҫָ��ģ���ļ���
	*/
	bool load ( const string path );

private:
	CvSVM* svm = NULL;
	FaceDetection *face_detector = NULL;

	/*
	Ԥ����������ͼƬ�е�����������ȡ������ͳһ��С
	*/
	bool preprocess (const Mat& src, Mat &dst );
};

#endif