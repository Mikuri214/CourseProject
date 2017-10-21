#include "egg_vessel_detect.h"
#include "ai_egg_embryo_vessel_detection.h"
#include <vector>
#include <string>
#include <queue>
#include <sys/timeb.h>
#include <exception>
using namespace cv;
using namespace std;

//helper function
//--------------------------------------------------------------------------------------------------//

void adaptivethreshold ( const cv::Mat  src, cv::Mat& dst )
{
	medianBlur ( src, dst, 41);   
	cv::Mat tmp;
	tmp  = dst - src - 3 ;    
	dst = tmp != 0;
}

//在查找表里查找对应的值
void applylutc ( const Mat &in, Mat& out, uchar* lut )
{
	static int weights3[3][3] = { { 1, 8, 64 }, { 2, 16, 128 }, { 4, 32, 256 } };
	int numRows = in.rows - 1,
		numCols = in.cols - 1;
	int y, x;

	Mat in2 = ( in != 0 ) / 255;
	int offset;
	const uchar* data_last = NULL;
	const uchar* data = NULL;
	const uchar* data_next = NULL;
	for ( x = 1; x < numRows; ++x )
	{
		data_last = in2.ptr<uchar> ( x - 1 );
		data = in2.ptr<uchar> ( x );
		data_next = in2.ptr<uchar> ( x + 1 );

		for ( y = 1; y < numCols; ++y )
		{
			offset = 0;

			offset += data_last[y - 1] * weights3[0][0];
			offset += data_last[y] * weights3[0][1];
			offset += data_last[y + 1] * weights3[0][2];
			offset += data[y - 1] * weights3[1][0];
			offset += data[y] * weights3[1][1];
			offset += data[y + 1] * weights3[1][2];
			offset += data_next[y - 1] * weights3[2][0];
			offset += data_next[y] * weights3[2][1];
			offset += data_next[y + 1] * weights3[2][2];

			out.at<uchar> ( x, y ) = ( uchar ) ( *( lut + offset ) );
		}
	}
	Scalar zero ( 0 );
	out.row ( 0 ) = zero;
	out.row ( numRows ) = zero;
	out.col ( 0 ) = zero;
	out.col ( numCols ) = zero;
}

bool equal ( const Mat&a, const Mat&b )
{
	if ( a.size () != b.size () )  return false;

	int i, j;
	const uchar* data1 = NULL;
	const uchar* data2 = NULL;
	for ( i = 0; i < a.rows; i++ )
	{
		data1 = a.ptr<uchar> ( i );
		data2 = b.ptr<uchar> ( i );
		for ( j = 0; j < a.cols; j++ )
		{
			if ( data1[j] != data2[j] )
				return false;
		}
	}
	return true;
}

//骨架化  zhang快速算法
void Skeletonization ( cv::Mat &srcimage )//单通道、二值化后的图像  骨架化
{
	vector<Point> deletelist1;
	int Zhangmude[9];
	int nl = srcimage.rows;
	int nc = srcimage.cols;
	while ( true )
	{
		for ( int j = 1; j < ( nl - 1 ); j++ )
		{
			uchar* data_last = srcimage.ptr<uchar> ( j - 1 );
			uchar* data = srcimage.ptr<uchar> ( j );
			uchar* data_next = srcimage.ptr<uchar> ( j + 1 );
			for ( int i = 1; i < ( nc - 1 ); i++ )
			{
				if ( data[i] == 255 )
				{
					Zhangmude[0] = 1;
					if ( data_last[i] == 255 ) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if ( data_last[i + 1] == 255 ) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if ( data[i + 1] == 255 ) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if ( data_next[i + 1] == 255 ) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if ( data_next[i] == 255 ) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if ( data_next[i - 1] == 255 ) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if ( data[i - 1] == 255 ) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if ( data_last[i - 1] == 255 ) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for ( int k = 1; k < 9; k++ )
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ( ( whitepointtotal >= 2 ) && ( whitepointtotal <= 6 ) )
					{
						int ap = 0;
						if ( ( Zhangmude[1] == 0 ) && ( Zhangmude[2] == 1 ) ) ap++;
						if ( ( Zhangmude[2] == 0 ) && ( Zhangmude[3] == 1 ) ) ap++;
						if ( ( Zhangmude[3] == 0 ) && ( Zhangmude[4] == 1 ) ) ap++;
						if ( ( Zhangmude[4] == 0 ) && ( Zhangmude[5] == 1 ) ) ap++;
						if ( ( Zhangmude[5] == 0 ) && ( Zhangmude[6] == 1 ) ) ap++;
						if ( ( Zhangmude[6] == 0 ) && ( Zhangmude[7] == 1 ) ) ap++;
						if ( ( Zhangmude[7] == 0 ) && ( Zhangmude[8] == 1 ) ) ap++;
						if ( ( Zhangmude[8] == 0 ) && ( Zhangmude[1] == 1 ) ) ap++;
						if ( ap == 1 )
						{
							if ( ( Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0 ) && ( Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0 ) )
							{
								deletelist1.push_back ( Point ( i, j ) );
							}
						}
					}
				}
			}
		}
		if ( deletelist1.size () == 0 ) break;
		for ( size_t i = 0; i < deletelist1.size (); i++ )
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar> ( tem.y );
			data[tem.x] = 0;
		}
		deletelist1.clear ();

		for ( int j = 1; j < ( nl - 1 ); j++ )
		{
			uchar* data_last = srcimage.ptr<uchar> ( j - 1 );
			uchar* data = srcimage.ptr<uchar> ( j );
			uchar* data_next = srcimage.ptr<uchar> ( j + 1 );
			for ( int i = 1; i < ( nc - 1 ); i++ )
			{
				if ( data[i] == 255 )
				{
					Zhangmude[0] = 1;
					if ( data_last[i] == 255 ) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if ( data_last[i + 1] == 255 ) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if ( data[i + 1] == 255 ) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if ( data_next[i + 1] == 255 ) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if ( data_next[i] == 255 ) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if ( data_next[i - 1] == 255 ) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if ( data[i - 1] == 255 ) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if ( data_last[i - 1] == 255 ) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for ( int k = 1; k < 9; k++ )
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ( ( whitepointtotal >= 2 ) && ( whitepointtotal <= 6 ) )
					{
						int ap = 0;
						if ( ( Zhangmude[1] == 0 ) && ( Zhangmude[2] == 1 ) ) ap++;
						if ( ( Zhangmude[2] == 0 ) && ( Zhangmude[3] == 1 ) ) ap++;
						if ( ( Zhangmude[3] == 0 ) && ( Zhangmude[4] == 1 ) ) ap++;
						if ( ( Zhangmude[4] == 0 ) && ( Zhangmude[5] == 1 ) ) ap++;
						if ( ( Zhangmude[5] == 0 ) && ( Zhangmude[6] == 1 ) ) ap++;
						if ( ( Zhangmude[6] == 0 ) && ( Zhangmude[7] == 1 ) ) ap++;
						if ( ( Zhangmude[7] == 0 ) && ( Zhangmude[8] == 1 ) ) ap++;
						if ( ( Zhangmude[8] == 0 ) && ( Zhangmude[1] == 1 ) ) ap++;
						if ( ap == 1 )
						{
							if ( ( Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0 ) && ( Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0 ) )
							{
								deletelist1.push_back ( Point ( i, j ) );
							}
						}
					}
				}
			}
		}
		if ( deletelist1.size () == 0 ) break;
		for ( size_t i = 0; i < deletelist1.size (); i++ )
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar> ( tem.y );
			data[tem.x] = 0;
		}
		deletelist1.clear ();
	}
}

//分支点图
void branchpoints ( const Mat& src, Mat &dst )
{
	static uchar lut1[512] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	static uchar lut2[512] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 2, 1, 1, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 3, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 2, 2, 3, 2, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 3, 2, 3, 2, 2, 3, 3, 4, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 3, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 2, 2, 3, 2, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 2, 1, 1, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 3, 3, 4, 3, 3, 2, 2, 3, 2, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 3, 2, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 2, 2, 3, 2, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 2, 1, 1, 2, 2, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0
	};
	static uchar lut3[512] =
	{
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	Mat C; C.create ( src.size (), src.type () );
	Mat B; B.create ( src.size (), src.type () );
	Mat E; E.create ( src.size (), src.type () );
	Mat D; D.create ( src.size (), src.type () );
	Mat Vp; Vp.create ( src.size (), src.type () );
	Mat Vq; Vq.create ( src.size (), src.type () );

	// 	% Initial branch point candidates
	// 		C = bwlookup ( bw, images.internal.lutbranchpoints () );
	applylutc ( src, C, lut1 );

	// 	% Background 4 - Connected Object Count ( Vp )
	// 		B = bwlookup ( bw, images.internal.lutbackcount4 () );	
	applylutc ( C, B, lut2 );

	// 	% End Points ( Vp = 1 )
	// 		E = ( B == 1 );  	
	E = ( B == 1 );
	// 	% Final branch point candidates
	// 		FC = ~E.*C;
	dst = ( ~E ).mul ( C );
	// 	% Generate mask that defines pixels for which Vp = 2 and no
	// 		% foreground neighbor q for which Vq > 2
	// 
	// 		% Vp = 2 Mask
	// 		Vp = ( ( B == 2 ) & ~E );
	Vp = ( ( B == 2 ) & ( ~E ) );
	// 	% Vq > 2 Mask
	Vq = ( ( B > 2 ) & ( ~E ) );

	// 	% Dilate Vq
	// 		D = bwlookup ( Vq, images.internal.lutdilate () );
	applylutc ( Vq, D, lut3 );

	// 	% Intersection between dilated Vq and final candidates w / Vp = 2
	// 		M = ( FC & Vp ) & D;
	C = ( dst & Vp ) & D;
	//% Final Branch Points
	dst = dst & ( ~C );
}

//端点图
void endpoints (  Mat& src, Mat &dst )
{
	static uchar lut1[512] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0,
	};

	applylutc ( src, dst, lut1 );

	dst = dst != 0;
}

Size	computeFilterSizeFromSigma ( const double sigma )
{
	int size = 2 * ceil ( 2 * sigma ) + 1;
	return Size ( size, size );
}

//消除小于指定大小的联通区域
void bwareaopen ( const Mat& src, Mat&dst, const int pixels )
{
	if ( &dst != &src )
		dst = src.clone ();

	Scalar zero ( 0 ), maxVal ( 255 );
	vector<vector<Point> > contours;
	findContours ( src, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE );

	double tmpArea;
	for ( int i = 0; i < contours.size (); ++i )
	{
		tmpArea = fabs ( contourArea ( contours[i] ) );
		if ( tmpArea < pixels ) //面积小于指定像素 填充为背景
			drawContours ( dst, contours, i, zero, CV_FILLED );
		else
			drawContours ( dst, contours, i, maxVal, CV_FILLED );
	}
}

//获取矩阵中所有非0的坐标对 （col ,  row）
vector<Point> find ( const Mat& src )
{
	vector<Point> result;
	int i, j;
	const uchar*  data = NULL;
	for ( i = 0; i < src.rows; ++i )
	{
		data = src.ptr<uchar> ( i );
		for ( j = 0; j < src.cols; ++j )
		{
			//for a point  : col-->x  row-->y				for mat  :  at function visit by  (  row , col )
			if ( data[j] )
				result.push_back ( Point ( j, i ) );
		}
	}
	return result;
}

//获取src对应mask位置中的最小值
int minDist ( const cv::Mat src, const vector<Point> mask )
{
	int min = INT_MAX;
	int cur;
 
	for ( int i = 0; i < mask.size (); ++i )
	{
		cur = src.at<int> ( mask[i] );
	//	src.at<int> ( mask[i] ) = 0;
		if ( cur < min ) min = cur;
	}
	return min;
}

//计算指定点在src的测地距离矩阵
Mat bwdistgeodesic ( const Mat &src, Point  point )
{
	Mat distance = Mat::ones ( src.size (), CV_32SC1 ) * INT_MAX;

	queue<Point> next;
	next.push ( point );
	distance.at<int> ( point ) = 0;

	Point curPos;
	int xx, yy;
	while ( !next.empty () )//bfs
	{
		curPos = next.front ();
		const int curVal = distance.at<int> ( curPos );//当前处理的点的测地距离
		next.pop ();
		//for a point  : col-->x  row-->y				for mat  :  at function visit by  (  row , col )
		//处理当前点的八领域  
		const uchar* mask_data = NULL;
		int* dist_data = NULL;
		for ( int i = curPos.y - 1; i <= curPos.y + 1; ++i )
		{
			mask_data = src.ptr<uchar> ( i );
			dist_data = distance.ptr<int> ( i );
			for ( int j = curPos.x - 1; j <= curPos.x + 1; ++j )
			{
				if ( mask_data[j] && dist_data[j] == INT_MAX )//当前点属于地且未处
				{
					dist_data[j] = curVal + 1;
					next.push ( Point ( j, i ) );
				}
			}
		}
	}

	return distance;
}
 
//重建端点和最近分支点的路径  从骨架图减去   实现消除毛刺  
void reconstructionPath ( const cv::Mat branchpoints, const cv::Mat  endpoints, const cv::Mat src, cv::Mat &dst )
{
	vector<Point> Epoints = find ( endpoints );
	vector<Point> Bpoints = find ( branchpoints );
	dst = cv::Mat::zeros ( src.size (), src.type () );

	cv::Mat dist;
	int nearestDist;

	for ( int i = 0; i < Epoints.size() ; i++ )
	{
		dist = bwdistgeodesic ( src, Epoints[i] );
		nearestDist = minDist ( dist, Bpoints );
		if ( nearestDist != INT_MAX )
			dst = dst | ( dist < nearestDist );
	}

	dst = ( src - dst ) != 0;
}

//--------------------------------------------------------------------------------------------------//




//main  procedural functions
//--------------------------------------------------------------------------------------------------//
void enhance ( const cv::Mat src, cv::Mat & dst, const double clipLimit = 8, const int sigma = 2 )
{
	cv::Mat tmp;

	//adapthiseq      
	Ptr<CLAHE> clahe = createCLAHE ( clipLimit ,Size(8,8)  );
	clahe->apply ( src, tmp );

	//imgaussfilt   
	GaussianBlur ( tmp, tmp, computeFilterSizeFromSigma ( sigma ), sigma );

	//bw = adaptivethreshold(hblur,40,0.01,1);
	adaptivethreshold ( tmp, dst );

}

void trimap (const cv::Mat bw,const cv::Mat v, cv::Mat& bw_out)
{
	cv::Mat bw_egg = v >= 20 ;
	cv::Mat bw_roi,bw_dist;

	distanceTransform (  bw_egg , bw_dist, CV_DIST_L2, CV_DIST_MASK_PRECISE );
	bw_roi = bw_dist > 30;
	
	//范围限制  
	Scalar zero ( 0 );
	for ( size_t i = 0; i < 100; i++ )
	{
		bw_roi.row ( i ) = zero;
		bw_roi.row ( bw_roi.rows - i - 1 ) = zero;
	}
	for ( size_t i = 0; i < 100; i++ )
	{
		bw_roi.col ( i ) = zero;
		bw_roi.col ( bw_roi.cols - i - 1 ) = zero;
	}

	bw_out = bw.mul( bw_roi);
}

void postProcess (const cv::Mat& src, cv::Mat& dst)
{
	cv::Mat B;//branch points
	cv::Mat E;// end points
	//去除区域大小小于300个像素的联通区域
	cv::Mat skel = src.clone ();
	bwareaopen ( src, skel, 300 );    
	//骨架化			 
	Skeletonization ( skel );     
	//找到分支点
	B.create ( skel.size (), skel.type () );
	branchpoints ( skel, B );
	//找到端点
	E.create ( skel.size (), skel.type () );
	endpoints ( skel, E );

	//根据端点和最近分支点重建路径（即毛刺）， 再从骨架图中减去 ， 实现去毛刺
	reconstructionPath ( B, E, skel, dst );
}

//将找出的血管在原图上标出 （红色）
void markOut ( Mat& src, Mat& mask )
{
	uchar* mask_data = NULL;
	Vec3b* data = NULL;

	for ( int i = 0; i < src.rows; i++ )
	{
		mask_data = mask.ptr<uchar> ( i );
		data = src.ptr <Vec3b> ( i );
		for ( int j = 0; j < src.cols; j++ )
		{
			if ( mask_data[j] )
			{
				data[j][0] = 0;
				data[j][1] = 0;
				data[j][2] = 255;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------//

uint egg_vessel_detection ( void* p, int  size)
{
	ai_egg_embryo_vessel_detection* param = ( ai_egg_embryo_vessel_detection* ) p;

	if ( param == NULL  || param->imageSrc.empty() )
		return AlgorithmResult::AR_EXTERNAL;

	cv::Mat origin, hsv, h, v, tmp[3], bw, bw2;
	struct timeb startTime, endTime;

	origin = param->imageSrc.clone ();
	try{
		ftime ( &startTime );
		resize ( origin, origin, Size( 800, 600 ) );
		cvtColor ( origin, hsv, COLOR_BGR2HSV );
		split ( hsv, tmp );
		h = tmp[0];
		v = tmp[2];

		enhance ( h, bw );
		trimap ( bw, v, bw2 );
		postProcess ( bw2, bw );
		markOut ( origin, bw );
		ftime ( &endTime );
		param->procTime =( endTime.time - startTime.time)* 1000 + ( endTime.millitm - startTime.millitm);
		param->imgDst = origin.clone();
	}catch( Exception e)
	{
		return AlgorithmResult::AR_INTERNAL;
	}
	return AlgorithmResult::AR_DONE;
}

