#ifndef FERNS_IMG_MATCH_ALGORITHM_H_
#define FERNS_IMG_MATCH_ALGORITHM_H_

#include "ImgMatchAlgorithm.h"

class FernsImgMatchAlgorithm : public ImgMatchAlgorithm
{
public:
	FernsImgMatchAlgorithm ( string modelDir ){ this->modelDir = modelDir;  };
	virtual ~FernsImgMatchAlgorithm (){};


	class FernsImgMatchAlgorithmParam:public ImgMatchAlgorithm::ImgMatchAlgorithmParam
	{
	public:
		FernsImgMatchAlgorithmParam (){};
		virtual ~FernsImgMatchAlgorithmParam (){};

		string objFileName;
	};

protected:
	virtual bool preprocess ( void* param = nullptr ){
		try{
			ImgMatchAlgorithm::preprocess ( param );
			Mat obj_gray, scene_gray;
			cvtColor ( object, obj_gray, CV_BGR2GRAY );
			cvtColor ( scene, scene_gray, CV_BGR2GRAY );

			int blurKSize = 3;
			double sigma = 0;
			GaussianBlur ( obj_gray, obj_gray, Size ( blurKSize, blurKSize ), sigma, sigma );
			GaussianBlur ( scene_gray, scene_gray, Size ( blurKSize, blurKSize ), sigma, sigma );
			buildPyramid ( obj_gray, objpyr, ldetector.nOctaves - 1 );
			buildPyramid ( scene_gray, scenepyr, ldetector.nOctaves - 1 );
			string objectFileName = (( FernsImgMatchAlgorithmParam * ) param)->objFileName ;
			string model_filename = modelDir + "/" + objectFileName + "_model.xml.gz";

			FileStorage fs ( model_filename, FileStorage::READ );
			if ( fs.isOpened () )
			{
				detector.read ( fs.getFirstTopLevelNode () );
			}
			else
			{
				ldetector.getMostStable2D ( obj_gray, objKeypoints, 100, gen );

				detector.train ( objpyr, objKeypoints, patchSize.width, 100, 11, 10000, ldetector, gen );
				if ( fs.open ( model_filename, FileStorage::WRITE ) )
					detector.write ( fs, "ferns_model" );
			}
			fs.release ();
		}
		catch ( exception &e ){
			cerr << e.what () << endl;
			return false;
		}

		return true;
	}

	virtual bool detect (){
		objKeypoints = detector.getModelPoints ();
		ldetector ( scenepyr, sceneKeypoints , 300 );

		return true;
	}

	virtual bool match (){
		pairs.clear();
		dst_corners.clear();
		return detector ( scenepyr, sceneKeypoints, H, dst_corners, &pairs );
	}

	virtual bool drawMatch (){
		int i;
		//在obj图像标出特征点
		for ( i = 0; i < ( int ) objKeypoints.size (); i++ )
		{
			circle ( object , objKeypoints[i].pt , 2 , Scalar ( 0 , 0 , 255 ) , -1 );
			circle ( object , objKeypoints[i].pt , ( 1 << objKeypoints[i].octave ) * 15 , Scalar ( 0 , 255 , 0 ) , 1 );
		}
		//在场景图标出特征点
		for ( i = 0; i < ( int ) sceneKeypoints.size (); i++ )
		{
			circle ( scene, sceneKeypoints[i].pt , 2 , Scalar ( 0 , 0 , 255 ) , -1 );
			circle ( scene, sceneKeypoints[i].pt , ( 1 << sceneKeypoints[i].octave ) * 15 , Scalar ( 0 , 255 , 0 ) , 1 );
		}

		Mat part ( *result , Rect ( 0 , 0 , object.cols , object.rows ) );
		object.copyTo ( part );
		part = Mat ( *result , Rect ( object.cols , 0 , scene.cols , scene.rows ) );
		scene.copyTo ( part );

		obj_with_key_point->release ();
		object.copyTo ( *obj_with_key_point );
		scene_with_key_point->release ();
		scene.copyTo ( *scene_with_key_point );

		//在场景图框出检测到的obj   边框线用红色
		for ( i = 0; i < 4; i++ )
		{
			Point r1 = dst_corners[i % 4];
			Point r2 = dst_corners[( i + 1 ) % 4];

			if ( r1.x < 0 )
				r1.x = 0;
			if ( r1.x > scene.cols )
				r1.x = scene.cols;
			if ( r2.x < 0 )
				r2.x = 0;
			if ( r2.x > scene.cols )
				r2.x = scene.cols;


			line ( *result , Point ( r1.x + object.cols  , r1.y ) ,
				   Point ( r2.x + object.cols , r2.y) , Scalar ( 0 , 0 , 255 ) );
		}
		//在匹配图画出匹配点连线  匹配用蓝色
		for ( i = 0; i < ( int ) pairs.size (); i += 2 )
		{
			line ( *result, objKeypoints[pairs[i]].pt,
				sceneKeypoints[pairs[i + 1]].pt + Point2f (( float ) object.cols , 0 ),
				Scalar ( 255, 0, 0 ) );
		}

		return true;
	}

	virtual AlgorithmResult process ( void* param /* = nullptr */ ){
		FernsImgMatchAlgorithmParam *ptr = ( FernsImgMatchAlgorithmParam* ) param;

		bool exec_status = true;
		exec_status &= preprocess ( ptr );
		if ( exec_status ) exec_status &= detect ();
		if ( exec_status ) exec_status &= match ();
		if ( exec_status ) exec_status &= drawMatch ();
		if ( exec_status )
			return AlgorithmResult::AR_DONE;
		else
			return AlgorithmResult::AR_INTERNAL;
	}

private:
	Mat H;
	vector<int> pairs;
	vector<Point2f> dst_corners;

	Size patchSize = Size( 32, 32 );
	LDetector ldetector = LDetector( 7, 20, 2, 2000, patchSize.width, 2 );
	PatchGenerator gen = PatchGenerator( 0, 256, 5, true, 0.8, 1.2, -CV_PI / 2, CV_PI / 2, -CV_PI / 2, CV_PI / 2 );
	PlanarObjectDetector detector;

	vector<Mat> objpyr, scenepyr; // 图像金字塔
	string modelDir;
};



#endif // !1
