#include "ImgMatchAlgorithmFactory.h"

//具体的匹配算法
#include "GenericImgMatchAlgorithm.h"
#include "FernsImgMatchAlgorithm.h"

#include <map>
using namespace std;

ImgMatchAlgorithmFactory* ImgMatchAlgorithmFactory::instance = nullptr;

ImgMatchAlgorithmFactory* ImgMatchAlgorithmFactory::getInstance ()
{
	if ( instance == nullptr)
	{
		instance = new ImgMatchAlgorithmFactory ();
	}
	return instance;
}

//可提供的算法
//新的算法要在此处添加算法名
vector<string> FeatureMethod = { "SIFT", "SURF", "ORB", "FERNS" };
vector<string> OutliersResolveMethod = { "1NN", "2NN" };
vector<string> MatchMethod = { "without ransac", "with ransac" };

//opencv提供的算法  用于实例化模板参数
char FLANN_BASE[] = "FlannBased";
char SIFT[] = "SIFT";
char SURF[] = "SURF";
char ORB[] = "ORB";


//匹配函数
bool noMatch ( vector<KeyPoint> &objKeypoints , vector<KeyPoint> &sceneKeypoints , vector<DMatch> &matchPairs )
{
	return true;
}

bool ransac ( vector<KeyPoint> &objKeypoints , vector<KeyPoint> &sceneKeypoints , vector<DMatch> &matchPairs )
{
	vector<uchar> m_RANSACStatus;       // 这个变量用于存储RANSAC后每个点的状态     

	int matchNum = matchPairs.size ();
	Mat p1 ( matchNum , 2 , CV_32F );
	Mat p2 ( matchNum , 2 , CV_32F );

	// 把Keypoint转换为Mat     
	Point2f pt;
	int i;
	for ( i = 0; i < matchNum; i++ )
	{
		pt = objKeypoints[matchPairs[i].queryIdx].pt;
		p1.at<float> ( i , 0 ) = pt.x;
		p1.at<float> ( i , 1 ) = pt.y;

		pt = sceneKeypoints[matchPairs[i].trainIdx].pt;
		p2.at<float> ( i , 0 ) = pt.x;
		p2.at<float> ( i , 1 ) = pt.y;
	}

	findFundamentalMat ( p1 , p2 , m_RANSACStatus , FM_RANSAC );

	vector<DMatch> inliersMatches;
	for ( i = 0; i < matchNum; i++ )
	{
		if ( m_RANSACStatus[i] )// 状态为0表示野点  为1表示内点
		{
			inliersMatches.push_back ( matchPairs[i] );
		}
	}
	if ( inliersMatches.size () == 0 )
		return false;//没有找到匹配

	matchPairs.clear ();
	matchPairs = inliersMatches;
	return true;
}

//组装算法   新算法组件要在这里注册
ImgMatchAlgorithm* ImgMatchAlgorithmFactory::getImgMatchAlgorithm ( string featureMethod, string resolveOutliersMethod , string matchMethod )
{
	ImgMatchAlgorithm* ptr = nullptr;
	switch ( find( FeatureMethod.begin(),FeatureMethod.end(),featureMethod  ) - FeatureMethod.begin())
	{
	case 0:
	{
		ptr = new GenericImgMatchAlgorithm ();
		GenericImgMatchAlgorithm* tmp = dynamic_cast< GenericImgMatchAlgorithm* >( ptr );
		tmp->detectFunc = feature < ::SIFT > ; //用模板参数值来重载
		ptr->algorithmName = featureMethod + "_Match";
	}	
		break;	
	case 1:
	{
		ptr = new GenericImgMatchAlgorithm ();
		GenericImgMatchAlgorithm* tmp = dynamic_cast< GenericImgMatchAlgorithm* >( ptr );
		tmp->detectFunc = feature < ::SURF > ;
		ptr->algorithmName = featureMethod + "_Match";
	}
		break;
	case 2:
	{
		ptr = new GenericImgMatchAlgorithm ();
		GenericImgMatchAlgorithm* tmp = dynamic_cast< GenericImgMatchAlgorithm* >( ptr );
		tmp->detectFunc = feature < ::ORB > ;
		ptr->algorithmName = featureMethod + "_Match";
	}
		break;
	case 3:
	{
		ptr = new FernsImgMatchAlgorithm ( "./model" );
		FernsImgMatchAlgorithm* tmp = dynamic_cast< FernsImgMatchAlgorithm* >( ptr );
		ptr->algorithmName = featureMethod + "_Match";
		return ptr;  // ferns匹配不需要进一步构造  直接返回
	}
		break;

	default:
		return nullptr;//未指定所用特征  构造失败  返回空指针
	}
	
	GenericImgMatchAlgorithm* tmp = dynamic_cast< GenericImgMatchAlgorithm* >( ptr );
	switch ( find ( OutliersResolveMethod.begin (), OutliersResolveMethod.end (), resolveOutliersMethod ) - OutliersResolveMethod.begin () )
	{
	case 0:	
		tmp->removeOutliersFunc = One_NN <FLANN_BASE> ;
		break;
	case  1:
		tmp->removeOutliersFunc = Two_NN < FLANN_BASE >;
		break;
	default:
		tmp->removeOutliersFunc = Two_NN <FLANN_BASE>; //默认使用2nn
	}

	switch ( find ( MatchMethod.begin (), MatchMethod.end (), matchMethod ) - MatchMethod.begin () )
	{
	case 0:
		tmp->matchFunc = noMatch;
		break;
	case 1:
		tmp->matchFunc = ransac;
		break;
	default:
		tmp->matchFunc = noMatch;
	}

	return ptr;
}


vector<string> ImgMatchAlgorithmFactory::getAlgorithmNames ( string algorithmScore )
{
	if ( algorithmScore == "Feature" )
	{
		return FeatureMethod;
	}
	if ( algorithmScore == "OutliersResolve" )
	{
		return OutliersResolveMethod;
	}
	if ( algorithmScore == "Match" )
	{
		return MatchMethod;
	}
}

