#include "cca.h"

CCA::CCA(cv::Mat &_X, cv::Mat &_Y){
	assert(_X.cols == _Y.cols);
	X = _X.clone();
	Y = _Y.clone();
	num_of_data = X.cols;

	center_x = calc_center(X);
	center_y = calc_center(Y);

	centration(X,center_x);
	centration(Y,center_y);
}

CCA::~CCA(){

}

void CCA::calc()
{
	Sxx = X*X.t();	
	Sxy = X*Y.t();	
	Syx = Y*X.t();
	Syy = Y*Y.t();

	cv::Mat Sa = Sxx.inv()*Sxy*Syy.inv()*Syx;
	cv::Mat Sb = Syy.inv()*Syx*Sxx.inv()*Sxy;	


	cv::SVD svda(Sa);
	cv::SVD svdb(Sb);

	A = cv::Mat(X.rows, Y.rows, svda.u.type());
	for(int i=0; i<Y.rows; i++){
		A.col(i) = svda.u.col(i);
		svda.u.col(i).copyTo(A.col(i));
	}

	B = svdb.u;		

	cv::Mat diag = svda.w;
	S = cv::Mat::zeros(Y.rows, Y.rows, A.type());//svda.w;
	for(int i=0; i<Y.rows; i++){
		S.at<double>(i, i) = diag.at<double>(i, 0);
	}

	normalizeVariance(A, Sxx);
	normalizeVariance(B, Syy);	

	G = B.t().inv()*S*A.t();
}

cv::Mat CCA::predict(const cv::Mat &x){

	return G*(x-center_x)+center_y;
}

cv::Mat CCA::calc_center(const cv::Mat &X){
	cv::Mat one = cv::Mat::ones(X.cols, 1, CV_64FC1);
	cv::Mat center = X*one;
	center /= X.cols;
	return center;
}

void CCA::centration(cv::Mat &X ,cv::Mat &center){
	for(int i=0; i<X.cols; i++){
		X.col(i) -= center;
	}
}

void CCA::normalizeVariance(cv::Mat &X, const cv::Mat Sigma){
	cv::Mat V = X.t()*Sigma*X;
	for(int i=0; i<X.cols; i++){
		//std::cout << V.at<float>(i, i) << std::endl;
		X.col(i) /= V.at<double>(i, i);
	}
}