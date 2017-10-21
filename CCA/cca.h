#include <opencv2/opencv.hpp>

class CCA
{
public:
	//x  y   每一列为一个样本   样本特征向量以列向量形式存储
	CCA(cv::Mat &X, cv::Mat &Y);
	~CCA();

	void calc();
	cv::Mat predict(const cv::Mat &x);

	cv::Mat A , B;
private:
	cv::Mat calc_center(const cv::Mat &X); // calc col_wise center of mass
	void centration ( cv::Mat &X , cv::Mat &center );
	void normalizeVariance(cv::Mat &X, const cv::Mat Sigma);

	cv::Mat X, Y;
	cv::Mat S, G;
	cv::Mat Sxx, Sxy, Syx, Syy;
	cv::Mat center_x, center_y;
	int num_of_data;
};