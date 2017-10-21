#include "ImgMatchAlgorithmFactory.h"

//�����ƥ���㷨
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

//���ṩ���㷨
//�µ��㷨Ҫ�ڴ˴�����㷨��
vector<string> FeatureMethod = { "SIFT", "SURF", "ORB", "FERNS" };
vector<string> OutliersResolveMethod = { "1NN", "2NN" };
vector<string> MatchMethod = { "without ransac", "with ransac" };

//opencv�ṩ���㷨  ����ʵ����ģ�����
char FLANN_BASE[] = "FlannBased";
char SIFT[] = "SIFT";
char SURF[] = "SURF";
char ORB[] = "ORB";


//ƥ�亯��
bool noMatch ( vector<KeyPoint> &objKeypoints , vector<KeyPoint> &sceneKeypoints , vector<DMatch> &matchPairs )
{
	return true;
}

bool ransac ( vector<KeyPoint> &objKeypoints , vector<KeyPoint> &sceneKeypoints , vector<DMatch> &matchPairs )
{
	vector<uchar> m_RANSACStatus;       // ����������ڴ洢RANSAC��ÿ�����״̬     

	int matchNum = matchPairs.size ();
	Mat p1 ( matchNum , 2 , CV_32F );
	Mat p2 ( matchNum , 2 , CV_32F );

	// ��Keypointת��ΪMat     
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
		if ( m_RANSACStatus[i] )// ״̬Ϊ0��ʾҰ��  Ϊ1��ʾ�ڵ�
		{
			inliersMatches.push_back ( matchPairs[i] );
		}
	}
	if ( inliersMatches.size () == 0 )
		return false;//û���ҵ�ƥ��

	matchPairs.clear ();
	matchPairs = inliersMatches;
	return true;
}

//��װ�㷨   ���㷨���Ҫ������ע��
ImgMatchAlgorithm* ImgMatchAlgorithmFactory::getImgMatchAlgorithm ( string featureMethod, string resolveOutliersMethod , string matchMethod )
{
	ImgMatchAlgorithm* ptr = nullptr;
	switch ( find( FeatureMethod.begin(),FeatureMethod.end(),featureMethod  ) - FeatureMethod.begin())
	{
	case 0:
	{
		ptr = new GenericImgMatchAlgorithm ();
		GenericImgMatchAlgorithm* tmp = dynamic_cast< GenericImgMatchAlgorithm* >( ptr );
		tmp->detectFunc = feature < ::SIFT > ; //��ģ�����ֵ������
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
		return ptr;  // fernsƥ�䲻��Ҫ��һ������  ֱ�ӷ���
	}
		break;

	default:
		return nullptr;//δָ����������  ����ʧ��  ���ؿ�ָ��
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
		tmp->removeOutliersFunc = Two_NN <FLANN_BASE>; //Ĭ��ʹ��2nn
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

