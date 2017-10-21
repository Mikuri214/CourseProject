#ifndef GENERIC_IMG_MATCH_ALGORITHM_H_
#define GENERIC_IMG_MATCH_ALGORITHM_H_

#include "ImgMatchAlgorithm.h"
#include <string>
using namespace cv;
using namespace std;


typedef bool ( *DetectFunc )( Mat& img, vector<KeyPoint> &keyPoints, Mat &descriptors );
typedef bool ( *RemoveOutliersFunc )( vector<KeyPoint> &objKeypoints, Mat &objDescriptors, vector<KeyPoint> & sceneKeypoints, Mat &sceneDescriptors, vector<DMatch> &matches ,double,double );
typedef bool ( *MatchFunc )( vector<KeyPoint> &objKeypoints, vector<KeyPoint> &sceneKeypoints, vector<DMatch> &matchPairs );


class GenericImgMatchAlgorithm:public ImgMatchAlgorithm
{
public:
	GenericImgMatchAlgorithm (){};
	virtual ~GenericImgMatchAlgorithm (){};

	DetectFunc detectFunc = nullptr;
	RemoveOutliersFunc removeOutliersFunc = nullptr;
	MatchFunc matchFunc = nullptr;

protected:
	virtual bool detect (){
		if ( detectFunc )
		{
			bool result_status = true;
			result_status &= detectFunc ( object, objKeypoints, objDescriptor );
			result_status &= detectFunc ( scene, sceneKeypoints, sceneDescriptor );
			return result_status;
		}
		return false;
	}

	virtual bool removeOutliers (){
		if ( removeOutliersFunc )
		{
			return removeOutliersFunc ( objKeypoints,objDescriptor, sceneKeypoints ,sceneDescriptor ,matchPairs, 0.6,0.8);
		}
		return false;
	}

	virtual bool match (){
		if ( matchFunc)
		{
			return matchFunc ( objKeypoints, sceneKeypoints, matchPairs );
		}
		return false;
	}

	virtual bool drawMatch(){
		//在匹配图画出匹配点连线   未匹配的特征点为绿色    匹配点及连线为蓝色
		drawMatches ( object, objKeypoints, scene, sceneKeypoints, matchPairs,*result ,Scalar(255,0,0),Scalar(0,255,0) );
		//在obj图像标出特征点
		drawKeypoints ( object,objKeypoints, *obj_with_key_point);
		//在场景图标出特征点
		drawKeypoints ( scene,sceneKeypoints ,*scene_with_key_point);

		return true;
	}

	virtual AlgorithmResult process ( void* param )
	{
		bool exec_status = true;
		exec_status &= preprocess ( param );
		if ( exec_status ) exec_status &= detect ();
		if ( exec_status ) exec_status &= removeOutliers ();
		if ( exec_status ) exec_status &= match ();
		if ( exec_status ) exec_status &= drawMatch ();
		if ( exec_status )
			return AlgorithmResult::AR_DONE;
		else
			return AlgorithmResult::AR_INTERNAL;
	}

private:
	Mat objDescriptor;
	Mat sceneDescriptor;
	vector<DMatch> matchPairs;
};



//特征检测函数( 模板参数值重载)
template<char const *FEATURE>
bool feature( Mat& img, vector<KeyPoint> &keyPoints, Mat &descriptors )
{
	initModule_nonfree ();
	const Ptr<FeatureDetector> detector = FeatureDetector::create ( FEATURE );//创建特征检测器  
	const Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create ( FEATURE );//创建特征向量生成器  
	if ( detector.empty () || descriptor_extractor.empty () )
		return false;
	detector->detect ( img, keyPoints );//检测img1中的SIFT特征点，存储到keypoints1中  
	descriptor_extractor->compute ( img, keyPoints, descriptors );

	if ( keyPoints.empty () || descriptors.empty () )
		return false;
	return true;
}

//野点消除函数
template<char const *TWONNMETHOD>
bool Two_NN ( vector<KeyPoint> &objKeypoints, Mat &objDescriptors, vector<KeyPoint> & sceneKeypoints, Mat &sceneDescriptors, vector<DMatch> &matches, double ONE_NN_CONSTRAIN = 0.6, double TWO_NN_CONSTRAIN = 0.8 )
{
	const Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create ( TWONNMETHOD );//创建匹配器
	if ( objDescriptors.type () != CV_32FC1 ){
		objDescriptors.convertTo ( objDescriptors, CV_32FC1 );
		sceneDescriptors.convertTo ( sceneDescriptors, CV_32FC1 );
	}
	vector<vector<DMatch>> matches2;
	descriptor_matcher->knnMatch ( objDescriptors, sceneDescriptors, matches2, 2 );

	double max_dist = 0;
	double min_dist = 100;
	int n = matches2.size ();
	int i;
	for ( i = 0; i < n; i++ )
	{
		double dist = matches2[i][0].distance;
		if ( dist < min_dist ) min_dist = dist;
		if ( dist > max_dist ) max_dist = dist;
	}

	max_dist = max_dist * ONE_NN_CONSTRAIN;
	n = matches2.size ();
	for ( i = 0; i < n; ++i )
	{
		if ( matches2[i][0].distance <  matches2[i][1].distance * TWO_NN_CONSTRAIN && matches2[i][0].distance < max_dist )
		{
			matches.push_back ( matches2[i][0] );
		}
	}
	if ( matches.size () > 0 )
		return true;
	return false;
}

template<const char *TWONNMETHOD>
bool One_NN ( vector<KeyPoint> &objKeypoints, Mat &objDescriptors, vector<KeyPoint> & sceneKeypoints, Mat &sceneDescriptors, vector<DMatch> &matches, double ONE_NN_CONSTRAIN = 0.6, double TWO_NN_CONSTRAIN = 0.8 )
{
	const Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create ( TWONNMETHOD );//创建匹配器
	if ( objDescriptors.type () != CV_32FC1 ){
		objDescriptors.convertTo ( objDescriptors, CV_32FC1 );
		sceneDescriptors.convertTo ( sceneDescriptors, CV_32FC1 );
	}
	vector<DMatch> matches2;
	descriptor_matcher->match ( objDescriptors, sceneDescriptors, matches2 );

	//计算匹配结果中距离的最大和最小值  
	//距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近  
	double max_dist = 0;
	double min_dist = 100;
	int n = matches2.size ();
	int i;
	for ( i = 0; i < n; i++ )
	{
		double dist = matches2[i].distance;
		if ( dist < min_dist ) min_dist = dist;
		if ( dist > max_dist ) max_dist = dist;
	}

	max_dist = max_dist * ONE_NN_CONSTRAIN;
	for ( i = 0; i < n; ++i )
	{
		if ( matches2[i].distance < max_dist )
		{
			matches.push_back ( matches2[i]);
		}
	}
	if ( matches.size () > 0 )
		return true;
	return false;
}

#endif
