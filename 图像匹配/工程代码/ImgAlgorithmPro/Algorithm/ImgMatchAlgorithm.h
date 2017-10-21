#ifndef IMG_MATCH_ALGORITHM_H_
#define IMG_MATCH_ALGORITHM_H_

#define API _declspec( dllexport )

#include "AlgorithmInterface.h"

#include <opencv2/opencv.hpp>
#include "opencv2/nonfree/nonfree.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/legacy/legacy.hpp"
using namespace std;
using namespace cv;


class ImgMatchAlgorithm:public AlgorithmInterface
{
public:
	ImgMatchAlgorithm (){};
	virtual ~ImgMatchAlgorithm ()=0{};

	virtual void init ( void* );
	virtual void destroy ();
	virtual AlgorithmResult process ( void* param = nullptr );

	Mat *obj_with_key_point, *scene_with_key_point;
	Mat *result;

	class ImgMatchAlgorithmParam:public AlgorithmInterface::AlgorithmParam
	{
	public:
		ImgMatchAlgorithmParam (){};
		virtual ~ImgMatchAlgorithmParam (){};
		
		Mat obj;
		Mat scene;
		Mat *obj_with_key_point , *scene_with_key_point;
		Mat *result;
	};

protected:
	virtual bool preprocess ( void* param )
	{ 
		ImgMatchAlgorithmParam *ptr = ( ImgMatchAlgorithmParam* ) param;
		object = ptr->obj;
		scene = ptr->scene;

		if ( object.empty () || scene.empty () ) 
			return false;

		resize ( object, object, Size( 200, 150) );
		resize ( scene, scene, Size(400,300 ) );
		obj_with_key_point = ptr->obj_with_key_point;
		scene_with_key_point = ptr->scene_with_key_point;
		result = ptr->result;
		result->release ();
		result->create ( max( object.rows ,scene.rows ),  object.cols + scene.cols , CV_8UC3 );
		
		//将结果图片设为 对象图和场景图的水平组合图片   当处理失败时 显示该图片  否则 成功时会调用相应绘制函数进行覆盖 
		Mat part ( *result , Rect ( 0 , 0 , object.cols , object.rows ) );
		object.copyTo ( part );
		part = Mat ( *result , Rect ( object.cols , 0 , scene.cols , scene.rows ) );
		scene.copyTo ( part );

		return true; 
	};
	virtual bool detect ( void* param = nullptr ){ return true; };
	virtual bool removeOutliers ( void* param = nullptr ){ return true; };
	virtual bool match ( void* param = nullptr ){ return true; };
	virtual bool drawMatch ( void* param = nullptr ){ return true; };


	vector<KeyPoint> objKeypoints, sceneKeypoints; //关键点
	Mat object;
	Mat scene;
};

#endif // !IMG_MATCH_ALGORITHM_H_