#include "face_gender_recognizer.h"
#include <io.h>



const HOGDescriptor hog( cvSize ( 64, 64 ), cvSize ( 24, 24 ),cvSize ( 8, 8 ), cvSize ( 8, 8 ), 9 );

const Size size ( 64, 64 );

FaceGenderRecognizer::FaceGenderRecognizer ( const char* seetaface_detector_model_path,const char* model_path  )
{
	//�Ա�ʶ��ģ�Ϳ����Ӻ���أ���seetaface ����ʶ��ģ�ͱ����ڹ���ʱ����
	assert ( seetaface_detector_model_path != nullptr && ( _access ( seetaface_detector_model_path, 0 ) != -1 ) );

	svm = new CvSVM ();
	if ( model_path != nullptr && ( _access(model_path,0)!= -1) )
		svm->load ( model_path );

	face_detector = new FaceDetection (seetaface_detector_model_path);
	face_detector->SetMinFaceSize ( 40 );
	face_detector->SetScoreThresh ( 2.f );
	face_detector->SetImagePyramidScaleFactor ( 0.8f );
	face_detector->SetWindowStep ( 4, 4 );
}

FaceGenderRecognizer::~FaceGenderRecognizer ( )
{
	if ( face_detector != NULL )
	{
		delete face_detector;
		face_detector = NULL;
	}
	if ( svm != NULL )
	{
		delete face_detector;
		svm = NULL;
	}
}

bool FaceGenderRecognizer::predict (const  Mat& src )
{
	if ( src.empty () || ( svm->get_support_vector_count () < 1 ) ){
		cerr<< "Invalide picture or didn't load model yet."<<endl;
		return false;
	}
	Mat face;
	
	preprocess ( src, face );

	vector<float> descriptor;
	//����ͼƬHOG��������
	hog.compute ( face, descriptor, Size ( 1, 1 ), Size ( 0, 0 ) );

	//������������  ������
	Mat testHog = Mat ( 1, descriptor.size (), CV_32FC1, descriptor.data () );

	return svm->predict ( testHog );
}

bool FaceGenderRecognizer::train ( const vector<string>& trainSet, const vector<int>& trainLabels )
{
	if ( trainSet.size () != trainLabels.size () ){
		cerr << "The number of trainLables must equal to trainSet." << endl;
		return false;
	}
	else if ( trainSet.size () < 2 )
	{
		cerr << "The number of trainSet must more than 1." << endl;
		return false;
	}

	try{
		//////////////////////////////////////////// ѵ������  //////////////////////////////////////////////////////////////
		Mat trainDataHog;//�����HOG�����������������洢����һ�ж�Ӧһ��������HOG����������

		Mat trainLabel = Mat ( 1, trainSet.size (), CV_32SC1, ( void* ) trainLabels.data () ).t ();;//����ѵ����ǩ����  ������
		trainLabel.convertTo ( trainLabel, CV_32FC1 );

		//�ȸ��ݵ�һ��ͼƬ�ڵ�ǰHog��������ȡ������������������ʼ�� ����Hog�������󣬱�����ѭ���д���
		{
			vector<float> descriptor;
			Mat curr = imread ( trainSet[0] );
			preprocess ( curr, curr );
			hog.compute ( curr, descriptor, Size ( 1, 1 ), Size ( 0, 0 ) );

			trainDataHog = Mat::zeros ( trainSet.size (), descriptor.size (), CV_32FC1 );
			Mat testHog = Mat ( 1, descriptor.size (), CV_32FC1, descriptor.data () );
			trainDataHog.row ( 0 ) += testHog;
		}

#pragma  omp parallel for
		for ( int i = 1; i < trainSet.size (); i++ )
		{
			Mat curr = imread ( trainSet[i] );
			preprocess ( curr, curr );

			vector<float> descriptor;
			hog.compute ( curr, descriptor, Size ( 1, 1 ), Size ( 0, 0 ) );

			//����ǰ������HOG����������������HOG��������
			Mat testHog = Mat ( 1, descriptor.size (), CV_32FC1, descriptor.data () );
			trainDataHog.row ( i ) += testHog;
		}

		////////////////////////////////////////////  ѵ��ģ��  ////////////////////////////////////////////////////////////
		CvSVMParams param;	//svm ѵ���ĳ�ʼ����
		CvTermCriteria criteria; //svm ѵ�������ı�׼
		criteria = cvTermCriteria ( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );//��֧�������ķ���С��ָ��Epsilon ������
		param = CvSVMParams ( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria ); //֧�ַ��������˹�˺���
		svm->train ( trainDataHog, trainLabel, Mat (), Mat (), param );
	}
	catch ( ... ){
		return false;//�����쳣ʱ��ѵ��ʧ��
	}
	return true;
}

bool FaceGenderRecognizer::train ( const vector<Mat>& trainSet, const vector<int>& trainLabels )
{
	if ( trainSet.size () != trainLabels.size ()  ){
		cerr << "The number of trainLables must equal to trainSet."<<endl;
		return false;
	}
	else if ( trainSet.size () < 2  )
	{
		cerr << "The number of trainSet must more than 1." << endl;
		return false;
	}

	try{
		//////////////////////////////////////////// ѵ������  //////////////////////////////////////////////////////////////
		Mat trainDataHog;//�����HOG���������������洢����һ�ж�Ӧһ��������HOG����������

		Mat trainLabel = Mat ( 1, trainSet.size (), CV_32SC1, ( void* ) trainLabels.data () ).t ();//����ѵ����ǩ������
		trainLabel.convertTo ( trainLabel, CV_32FC1 );

		//�ȸ��ݵ�һ��ͼƬ�ڵ�ǰHog��������ȡ������������������ʼ�� ����Hog�������󣬱�����ѭ���д���
		{
			vector<float> descriptor;
			Mat curr = trainSet[0];
			preprocess ( curr, curr );
			hog.compute ( curr, descriptor, Size ( 1, 1 ), Size ( 0, 0 ) );

			trainDataHog = Mat::zeros ( trainSet.size (), descriptor.size (), CV_32FC1 );
			Mat testHog = Mat ( 1, descriptor.size (), CV_32FC1, descriptor.data () );
			trainDataHog.row ( 0 ) += testHog;
		}

#pragma  omp parallel for
		for ( int i = 1; i < trainSet.size (); i++ )
		{
			Mat curr =  trainSet[i];
			preprocess ( curr, curr );

			vector<float> descriptor;
			hog.compute ( curr, descriptor, Size ( 1, 1 ), Size ( 0, 0 ) );

			//����ǰ������HOG����������������HOG��������
			Mat testHog = Mat ( 1, descriptor.size (), CV_32FC1, descriptor.data () );
			trainDataHog.row ( i ) += testHog;
		}


		//////////////////////////////////////////// ѵ��ģ�� /////////////////////////////////////////////////////////////
		CvSVMParams param;	//svm ѵ���ĳ�ʼ����
		CvTermCriteria criteria; //svm ѵ�������ı�׼
		criteria = cvTermCriteria ( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );//��֧�������ķ���С��ָ��Epsilon ������
		param = CvSVMParams ( CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria ); //֧�ַ��������˹�˺���
		svm->train ( trainDataHog, trainLabel, Mat (), Mat (), param );

	}
	catch ( ... ){
		return false;//�����쳣ʱ��ѵ��ʧ��
	}
	return true;
}

bool FaceGenderRecognizer::save ( const string path )
{
	if ( svm->get_support_vector_count() < 1  )
	{
		cerr << "Did not load or train model yet.Can't save model."<<endl;
		return false;
	}

	try{
		svm->save ( path.c_str() );
	}
	catch ( ... )
	{
		return false;
	}
	return true;
}

bool FaceGenderRecognizer::load ( const string path )
{
	if ( path.empty () || ( ( _access ( path.c_str (), 0 ) ) == -1 )){
		cerr<<"Invalide path."<<endl;
		return false;
	}
	CvSVM* tmp = NULL;
	try{
		//���سɹ���  ��ָ���µ�ģ��   ���򣬼���ʧ�ܣ���Ӱ�쵱ǰģ�͵�ʹ��
		tmp = new CvSVM ();
		tmp->load ( path.c_str () );

		delete svm;
		svm = tmp;
	}
	catch ( ... ){
		if ( tmp != NULL )
			delete tmp;
		return false;
	}
	return true;
}

bool FaceGenderRecognizer::preprocess ( const Mat& src, Mat &dst )
{
	if ( src.empty())
	{
		cerr << "src cannt be empty."<<endl;
		return false;
	}

	try{
		Mat gray, face;

		//���srcΪ��ɫͼ  תΪ�Ҷ�ͼ
		if ( src.channels () > 1 )
			cvtColor ( src, gray, CV_RGB2GRAY );
		else
			gray = src;

		seeta::ImageData image_data;
		image_data.data = gray.data;
		image_data.width = gray.cols;
		image_data.height = gray.rows;
		image_data.num_channels = 1;

		std::vector<seeta::FaceInfo> faces = face_detector->Detect ( image_data );
		int32_t face_num = static_cast< int32_t >( faces.size () );//seetaface��⵽��������������  ����ֻ�����һ��

		//û�м�⵽��������ʱ����Ϊ����ͼƬ����������
		if ( face_num == 0 )
			face = src;
		else{
			//�����������򣬽���������������ԭͼƬ��Χ�� ��seetaface���������������Ǽ���õ��ģ������и�ֵ����С���ܳ���ԭͼƬ��
			faces[0].bbox.x = min ( max ( 0, faces[0].bbox.x ), src.cols );
			faces[0].bbox.y = min ( max ( 0, faces[0].bbox.y ), src.rows );
			if( faces[0].bbox.width + faces[0].bbox.x > src.cols )
				faces[0].bbox.width = src.cols - faces[0].bbox.x;
			if ( faces[0].bbox.height + faces[0].bbox.y > src.rows )
				faces[0].bbox.height = src.rows - faces[0].bbox.y;

			face = Mat ( src, cv::Rect ( faces[0].bbox.x, faces[0].bbox.y, faces[0].bbox.width, faces[0].bbox.height ) );
		}

		resize ( face, dst, size );
 	}
 	catch ( ... ){
		dst = src;
 		return false;
 	}
	return true;
}
