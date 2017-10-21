#include "stdafx.h"

#define  _USE_MATH_DEFINES 

#include "ImageProcess.h"
#include <math.h>
#include <vector>
using namespace std;

float BOUND ( float val, float min, float max ){
	if ( val < min ) return min;
	if ( val > max ) return max;
	return val;
}

float Sinxx ( float value ) {
	if ( value < 0 ) value = -value;

	if ( value < 1.0 ) {
		float temp = value * value;
		return 0.5 * temp * value - temp + 2.0 / 3.0;
	}
	else if ( value < 2.0 ) {
		value = 2.0 - value;
		value *= value * value;
		return value / 6.0;
	}
	else {
		return 0.0;
	}
}

 inline void CubicInterpolation ( CImage* sourceImage, float dblXX, float dblYY,int bitCount,byte rgb[3])
 {
	 byte* srcData = (byte*)sourceImage->GetBits ();

	 int dwWidthBytes =  sourceImage->GetPitch();

	 int X, Y;
	 X = floor ( dblXX );
	 Y = floor ( dblYY );

	 int xx[4], yy[4]; //邻域坐标单位距离
	 xx[0] = -1;  xx[1] = 0; xx[2] = 1; xx[3] = 2;
	 yy[0] = -1;  yy[1] = 0; yy[2] = 1; yy[3] = 2;
	 //保证合法
	 if ( ( X - 1 ) < 0 ) xx[0] = 0;
	 if ( ( X + 1 ) > ( sourceImage->GetWidth() - 1 ) ) xx[2] = 0;
	 if ( ( X + 2 ) > ( sourceImage->GetWidth() - 1 ) ) xx[3] = ( ( xx[2] == 0 ) ? 0 : 1 );

	 if ( ( Y - 1 ) < 0 ) yy[0] = 0;
	 if ( ( Y + 1 ) > ( sourceImage->GetHeight() - 1 ) ) yy[2] = 0;
	 if ( ( Y + 2 ) > ( sourceImage->GetHeight() - 1 ) ) yy[3] = ( ( yy[2] == 0 ) ? 0 : 1 );

	 //相邻像素的像素值
	 BYTE abyRed[4][4], abyGreen[4][4], abyBlue[4][4];
	 for ( int i = 0; i < 4; i++ )
	 {		 
		 BYTE* pbySrcBase = srcData + ( Y + yy[i] ) * dwWidthBytes;
		 for ( int j = 0; j < 4; j++ )
		 {
			 BYTE* pbySrc = pbySrcBase + ( X + xx[j] )*bitCount; 
			 abyBlue[i][j] = *pbySrc++;
			 abyGreen[i][j] = *pbySrc++;
			 abyRed[i][j] = *pbySrc;
		 }
	 }

	 float u, v;
	 u = dblXX - X;
	 v = dblYY - Y;
	 //领域值的权重
	 float afu[4];
	 float afv[4];
	 afu[0] = Sinxx ( 1.0f + u );
	 afu[1] = Sinxx ( u );
	 afu[2] = Sinxx ( 1.0f - u );
	 afu[3] = Sinxx ( 2.0f - u );

	 afv[0] = Sinxx ( 1.0f + v );
	 afv[1] = Sinxx ( v );
	 afv[2] = Sinxx ( 1.0f - v );
	 afv[3] = Sinxx ( 2.0f - v );

	 //矩阵乘向量的中间值
	 float afRed[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	 float afGreen[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	 float afBlue[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	 for ( int i = 0; i < 4; i++ )
	 {
		 for ( int j = 0; j < 4; j++ )
		 {
			 afRed[i] += afv[j] * abyRed[j][i];
			 afGreen[i] += afv[j] * abyGreen[j][i];
			 afBlue[i] += afv[j] * abyBlue[j][i];
		 }
	 }
	 rgb[0]= ( BYTE ) ( BOUND ( ( afu[0] * afRed[0] + afu[1] * afRed[1] + afu[2] * afRed[2] +
		 afu[3] * afRed[3] ), 0, 255 ) );
	 rgb[1] = ( BYTE ) ( BOUND ( ( afu[0] * afGreen[0] + afu[1] * afGreen[1] + afu[2] * afGreen[2] +
		 afu[3] * afGreen[3] ), 0, 255 ) );
	 rgb[2] = ( BYTE ) ( BOUND ( ( afu[0] * afBlue[0] + afu[1] * afBlue[1] + afu[2] * afBlue[2] +
		 afu[3] * afBlue[3] ), 0, 255 ) );
 }

 void mulTemplate ( CImage* sourceImage,CImage* dstImage, int X, int Y,  int bitCount, double matrix[3][3] )
 {
	 int dwWidthBytes = sourceImage->GetPitch();

	 byte* srcData = (byte*)sourceImage->GetBits ();
	 byte* dstData = ( byte* ) dstImage->GetBits ();
	 int xx[3], yy[3]; //邻域坐标单位距离
	 xx[0] = -1;  xx[1] = 0; xx[2] = 1;
	 yy[0] = -1;  yy[1] = 0; yy[2] = 1;
	 //保证合法
	 if ( ( X - 1 ) < 0 ) xx[0] = 0;
	 if ( ( X + 1 ) > ( sourceImage->GetWidth() - 1 ) ) xx[2] = 0;

	 if ( ( Y - 1 ) < 0 ) yy[0] = 0;
	 if ( ( Y + 1 ) > ( sourceImage->GetHeight() - 1 ) ) yy[2] = 0;

	 //像素值
	 BYTE   pixels[3][3] ;
	 for ( int i = 0; i < 3; i++ )
	 {
		 BYTE* pbySrcBase = srcData + ( Y + yy[i] ) * dwWidthBytes;
		 for ( int j = 0; j < 3; j++ )
		 {
			 BYTE* pbySrc = pbySrcBase + ( X + xx[j] )*bitCount;
			 pixels[i][j] = *pbySrc;
		 }
	 }

	 BYTE   pixel = 0;
	 for ( int i = 0; i < 3; i++ )
	 {
		 for ( int j = 0; j < 3; j++ )
		 {
			 pixel += ( pixels[i][j] * matrix[i][j] );
		 }
	 }
	 if (pixel > 255)
	 {
		 pixel = 255;
	 }
	 else if ( pixel < 0 )
	 {
		 pixel = 0;
	 }
	 *( dstData + Y * dwWidthBytes + X * bitCount ) =
		 *( dstData + Y * dwWidthBytes + X * bitCount + 1 ) =
		 *( dstData + Y * dwWidthBytes + X * bitCount + 2 ) = pixel;
 }

 void Histogram ( CImage *img, long hist[3][256] )
 {
	 long size = img->GetHeight ()* img->GetWidth ();
	 int maxWidth = img->GetWidth ();
	 int pit = img->GetPitch ();
	 int bitCount = img->GetBPP () / 8;
	 byte* data = ( byte* ) img->GetBits ();
	 for ( int i = 0; i < size; ++i )
	 {
		 int X = i%maxWidth;
		 int Y = i / maxWidth;
		 hist[0][data[Y * pit + X * bitCount]]++;
		 hist[1][data[Y * pit + X * bitCount + 1]]++;
		 hist[2][data[Y * pit + X * bitCount + 2]]++;
	 }
 }

 void getBounderValue ( long hist[3][256], byte *newmin, byte *newmax, byte *oldmin, byte *oldmax, double lowCut, double hightCut, long PixelAmount )
 {
	 long lowAmount = PixelAmount * lowCut;
	 long hightAmount = PixelAmount * hightCut;
	 long sum = 0;
	 for ( size_t i = 0; i < 3; ++i )
	 {
		 sum = 0;
		 for ( size_t j = 0; j < 256; ++j )
		 {
			 sum += hist[i][j];
			 if ( sum > lowAmount )
			 {
				 newmin[i] = j;
				 break;
			 }
		 }
		 sum = 0;
		 for ( size_t j = 0; j < 256; ++j )
		 {
			 sum += hist[i][j];
			 if ( sum != 0 )
			 {
				 oldmin[i] = j;
				 break;
			 }
		 }
		 sum = 0;
		 for ( size_t j = 255; j >= 0; --j )
		 {
			 sum += hist[i][j];
			 if ( sum > hightAmount )
			 {
				 newmax[i] = j;
				 break;
			 }
		 }
		 sum = 0;
		 for ( size_t j = 255; j >= 0; --j )
		 {
			 sum += hist[i][j];
			 if ( sum != 0 )
			 {
				 oldmax[i] = j;
				 break;
			 }
		 }
	 }
 }

void ImageProcess::scale ( void* p )
{
	ThreadParam* param = ( ThreadParam* ) p;

	int maxWidth = param->dst->GetWidth ();
	int maxHeight = param->dst->GetHeight ();
	double scaleRate = param->u.scale_rate;   //实际缩放率的倒数   dst img -> src img
	int dst_startIndex = param->startIndex;     //dst img对应的位置
	int dst_endIndex = param->endIndex;    

	byte* srcData = ( byte* ) param->src->GetBits ();
	byte* dstData = ( byte* ) param->dst->GetBits ();
	int src_pit = param->src->GetPitch ();
	int dst_pit = param->dst->GetPitch ();
	int bitCount = param->src->GetBPP () / 8;


	for ( int i = dst_startIndex; i <= dst_endIndex; ++i )
	{
		int X = i % maxWidth;  //dst img对应像素点坐标
		int Y = i / maxWidth;
		float fx = X * scaleRate; //src img对应像素点坐标
		float fy = Y * scaleRate;
		byte rgb[3];
		if ( int ( fx ) == fx && int ( fy ) == fy ) //可在src img找到对应像素点
		{
			rgb[0] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount + 2 );
			rgb[1] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount + 1 );
			rgb[2] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount );
		}
		else
		{			
			CubicInterpolation ( param->src, fx, fy,bitCount, rgb );
		}
		*( dstData + dst_pit*Y + X*bitCount + 2 ) = rgb[0];
		*( dstData + dst_pit*Y + X*bitCount + 1 ) = rgb[1];
		*( dstData + dst_pit*Y + X*bitCount ) = rgb[2];
	}
//	::PostMessage ( AfxGetMainWnd ()->GetSafeHwnd (), WM_MEDIAN_FILTER, 1, NULL );
}

void ImageProcess::rotate ( void* p )
{
	ThreadParam* param = ( ThreadParam* ) p;

	int src_width = param->src->GetWidth ();
	int src_height = param->src->GetHeight ();
	int dst_width = param->dst->GetWidth ();
	int dst_height = param->dst->GetHeight ();
	double rotate_arc = param->u.rotate_arc;  
	double sinval = sin ( rotate_arc );
	double cosval = cos ( rotate_arc );

	int dst_startIndex = param->startIndex;     //dst img对应的位置
	int dst_endIndex = param->endIndex;

	byte* srcData = ( byte* ) param->src->GetBits ();
	byte* dstData = ( byte* ) param->dst->GetBits ();
	int src_pit = param->src->GetPitch ();
	int dst_pit = param->dst->GetPitch ();
	int bitCount = param->src->GetBPP () / 8;

	double num1 = -0.5* dst_width *cosval - 0.5* dst_height *sinval + 0.5* src_width;
	double num2 = 0.5*dst_width* sinval - 0.5 * dst_height *cosval + 0.5 * src_height;

	for ( int i = dst_startIndex; i <= dst_endIndex; ++i )
	{
		int X = i % dst_width;
		int Y = i / dst_width;
		float fx = X * cosval + Y * sinval + num1;
		float fy = -X * sinval + Y * cosval + num2;
		byte rgb[3];

		if ( fx < 0 || fx >= src_width || fy < 0 || fy >= src_height)
		{
			rgb[0] = rgb[1] = rgb[2] = 1;
		}
		else if ( int ( fx ) == fx && int ( fy ) == fy )
		{
			rgb[0] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount + 2 );
			rgb[1] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount + 1 );
			rgb[2] = *( srcData + src_pit*( int ( fy ) ) + ( int ( fx ) ) *bitCount );
		}
		else
		{
			CubicInterpolation ( param->src, fx, fy,  bitCount, rgb );
		}
		*( dstData + dst_pit*Y + X*bitCount + 2 ) = rgb[0];
		*( dstData + dst_pit*Y + X*bitCount + 1 ) = rgb[1];
		*( dstData + dst_pit*Y + X*bitCount ) = rgb[2];
	}
	//::PostMessage ( AfxGetMainWnd ()->GetSafeHwnd (), WM_MEDIAN_FILTER, 1, NULL );
}

void ImageProcess::autoLevels ( void* p )
{
	autolevelThreadParam* param = ( autolevelThreadParam* ) p;

	int maxWidth = param->src->GetWidth ();

	int src_startIndex = param->startIndex;     //起始位置
	int src_endIndex = param->endIndex;

	byte* srcData = ( byte* ) param->src->GetBits ();
	byte* dstData = ( byte* ) param->dst->GetBits ();

	int bitCount = param->src->GetBPP () / 8;
	int pit = param->src->GetPitch ();

	byte* newmin = param->newmin;
	byte* newmax = param->newmax;
	byte* oldmin = param->oldmin;
	byte* oldmax = param->oldmax;

	for ( int i = src_startIndex; i <= src_endIndex; ++i )
	{
		byte color;

		int X = i % maxWidth;  //dst img对应像素点坐标
		int Y = i / maxWidth;

		for ( int j = 0; j < 3; j++ )
		{
			color = *( srcData + Y* pit + X * bitCount + j );

			if ( color < newmin[j] ) color = 0;
			else if ( color > newmax[j] ) color = 255;
			else color = ( color - oldmin[j] ) * ( newmax[j] - newmin[j] ) / ( oldmax[j] - oldmin[j] ) + newmin[j];
			*( dstData + Y* pit + X * bitCount + j ) = color;
		}
	}
}

void ImageProcess::autoWhiteBalance ( void* p )
{
	ThreadParam* param = ( ThreadParam* ) p;

	int maxWidth = param->src->GetWidth ();
	int maxHeight = param->src->GetHeight ();

	int src_startIndex = param->startIndex;     //起始位置
	int src_endIndex = param->endIndex;

	byte* srcData = ( byte* ) param->src->GetBits ();

	int bitCount = param->src->GetBPP () / 8;
	int pit = param->src->GetPitch ();

	long hist[3][256] = { 0 };
	Histogram ( param->src, hist );

	byte min[3];

	long lowAmount = maxHeight*maxWidth * 0.001;
	long sum = 0;
	for ( size_t i = 0; i < 3; ++i )
		{
			sum = 0;
			for ( size_t j = 0; j < 256; ++j )
			{
				sum += hist[i][j];
				if ( sum > lowAmount )
				{
					min[i] = j;
					break;
				}
			}
		}


	for ( int i = src_startIndex; i <= src_endIndex; ++i )
	{
		byte color;

		int X = i % maxWidth;  //dst img对应像素点坐标
		int Y = i / maxWidth;

		for ( int j = 0; j < 3; j++ )
		{
			color = *( srcData + Y* pit + X * bitCount + j );

			if ( color < min[j] ) color = 0;
			else if ( color > 255 ) color = 255;
			else color = color * ( 255 - min[j] ) / 255 + min[j];

			*( srcData + Y* pit + X * bitCount + j ) = color;
		}
	}
}

void ImageProcess::sketchTranslate ( void* p )
{
	ThreadParam* param = ( ThreadParam* ) p;

	int dst_startIndex = param->startIndex;     //dst img对应的起始位置
	int dst_endIndex = param->endIndex;
	int maxWidth = param->src->GetWidth ();
	int maxHeight = param->src->GetHeight ();
	byte* srcData = ( byte* ) param->src->GetBits ();
	byte* dstData = ( byte* ) param->dst->GetBits ();
	int bitCount = param->dst->GetBPP () / 8;
	int pit = param->dst->GetPitch ();

	//去色
	byte rgb[3];
	for ( int i = dst_startIndex; i <= dst_endIndex; ++i )
	{
		const	int X = i % maxWidth;  //dst img对应像素点坐标
		const    int Y = i / maxWidth;

		rgb[0] = *( srcData + Y*pit + X * bitCount + 2 );
		rgb[1] = *( srcData + Y*pit + X * bitCount + 1 );
		rgb[2] = *( srcData + Y*pit + X * bitCount + 0 );
		rgb[0] = ( rgb[0] + rgb[1] + rgb[2] ) / 3;
		*( srcData + Y*pit + X * bitCount + 2 ) =
			*( srcData + Y*pit + X * bitCount + 1 ) =
			*( srcData + Y*pit + X * bitCount + 0 ) = rgb[0];
		*( dstData + Y*pit + X * bitCount + 2 ) =
			*( dstData + Y*pit + X * bitCount + 1 ) =
			*( dstData + Y*pit + X * bitCount + 0 ) = ( 255 - rgb[0] );
	}
	//在八领域取最小值
	for ( int i = dst_startIndex; i <= dst_endIndex; ++i )
	{
		int X = i % maxWidth;
		int Y = i / maxWidth;

		vector<byte> neighbors;
		byte min = *( dstData + Y*pit + X * bitCount + 0 );
		int xx[3], yy[3]; //邻域坐标单位距离
		xx[0] = -1;  xx[1] = 0; xx[2] = 1;
		yy[0] = -1;  yy[1] = 0; yy[2] = 1;
		//保证合法
		if ( ( X - 1 ) < 0 ) xx[0] = 0;
		if ( ( X + 1 ) > ( maxWidth - 1 ) ) xx[2] = 0;

		if ( ( Y - 1 ) < 0 ) yy[0] = 0;
		if ( ( Y + 1 ) > ( maxHeight - 1 ) ) yy[2] = 0;

		for ( int i = 0; i < 3; i++ )
		{
			BYTE* pbySrcBase = srcData + ( Y + yy[i] ) * pit;
			for ( int j = 0; j < 3; j++ )
			{
				BYTE* pbySrc = pbySrcBase + ( X + xx[j] )*bitCount;
				if ( j != 1 && i != 1 )
					neighbors.push_back ( *pbySrc );
			}
		}

		for ( int p = 0; p < neighbors.size (); p++ )
		{
			if ( min > neighbors[p] )
			{
				min = neighbors[p];
			}
		}
		*( dstData + Y*pit + X* bitCount + 0 ) =
			*( dstData + Y*pit + X * bitCount + 1 ) =
			*( dstData + Y*pit + X* bitCount + 2 ) = min;
	}

	//计算融合色
	for ( int i = dst_startIndex; i <= dst_endIndex; ++i )
	{
		int X = i % maxWidth;  //dst img对应像素点坐标
		int Y = i / maxWidth;

		rgb[0] = *( srcData + Y*pit + X * bitCount + 2 ) ;

		byte dstrgb;

		dstrgb = *( dstData + Y*pit + X * bitCount + 2 );

		if ( rgb[0] == 255)
		{
			rgb[0] = 0;
		}
		dstrgb = dstrgb + ( rgb[0] * dstrgb ) / ( 255 - rgb[0] );
		if ( dstrgb < 0 )
		{
			dstrgb = 0;
		}
		if ( dstrgb > 255 )
		{
			dstrgb = 255;
		}

		*( dstData + Y*pit + X * bitCount + 2 ) =
			*( dstData + Y*pit + X * bitCount + 1 ) =
			*( dstData + Y*pit + X * bitCount ) = dstrgb;
	}
}

void ImageProcess::mixture ( void* p )
{
	mixThreadParam* param = ( mixThreadParam* ) p;

	int startIndex = param->startIndex;     //dst img对应的起始位置
	int endIndex = param->endIndex;

	int maxWidth = param->img1->GetWidth ();
	int maxHeight = param->img1->GetHeight ();

	double alpha = param->alpha;

	byte* imgData1 = ( byte* ) param->img1	->GetBits ();
	byte* imgData2 = ( byte* ) param->img2	->GetBits ();
	byte* imgData   = ( byte* ) param->dst		->GetBits ();
	int bitCount = param->img1->GetBPP () / 8;
	int pit = param->img1->GetPitch ();

	for ( int i = startIndex; i <= endIndex; ++i )
	{
		int X = i % maxWidth;  //dst img对应像素点坐标
		int Y = i / maxWidth;

		*( imgData + Y*pit + X * bitCount + 2 ) = alpha * *( imgData1 + Y*pit + X * bitCount + 2 ) + ( 1 - alpha ) * *( imgData2 + Y*pit + X * bitCount + 2 );
		*( imgData + Y*pit + X * bitCount + 1 ) = alpha * *( imgData1 + Y*pit + X * bitCount + 1 ) + ( 1 - alpha ) * *( imgData2 + Y*pit + X * bitCount + 1 );
		*( imgData + Y*pit + X * bitCount + 0 ) = alpha * *( imgData1 + Y*pit + X * bitCount + 0 ) + ( 1 - alpha ) * *( imgData2 + Y*pit + X * bitCount + 0 );
	}
}

void ImageProcess::Bilateral_Filter ( void* p )
{
	bbfThreadParam* param = ( bbfThreadParam* ) p;

	int startIndex = param->startIndex;     //dst img对应的起始位置
	int endIndex = param->endIndex;

	int maxWidth = param->src->GetWidth ();
	int maxHeight = param->src->GetHeight ();

	int r = param->r;  //内核中心到边界的距离

	byte* srcData = ( byte* ) param->src->GetBits ();
	byte* dstData = ( byte* ) param->dst->GetBits ();
	int bitCount = param->src->GetBPP () / 8;
	int pit = param->src->GetPitch ();

	const double sigmaSpace = 30;         //最大距离方差
	const double sigmaColor = 255;      //最大像素强度值方差
	double sigma2 = sigmaSpace * sigmaSpace;
	double max_var = ( float ) ( sigmaColor*sigmaColor );

	vector<float> space_weight ( 4 * r*r + 4 * r + 1 );  //距离权重

	//遍历整个内核，计算高斯距离权值 
	int idx = 0;
	for ( int y = -r; y <= r; y++ )
		for ( int x = -r; x <= r; x++ )
		{
		space_weight[idx++] = ( float ) exp ( -0.5*( x * x + y * y ) / sigma2 );
		}

	float var_b, var_g, var_r;
	int currVal_b, currVal_g, currVal_r;
	int sumVal_b = 0, sumVal_g = 0, sumVal_r = 0;
	int sumValSqr_b = 0, sumValSqr_g = 0, sumValSqr_r = 0;
	int currValCenter_b = 0, currValCenter_g = 0, currValCenter_r = 0;
	int currWRTCenter_b, currWRTCenter_g, currWRTCenter_r;
	float weight_b, weight_g, weight_r;
	float totalWeight_b = 0., totalWeight_g = 0., totalWeight_r = 0.;
	float tmpSum_b = 0., tmpSum_g = 0., tmpSum_r = 0.;
	byte *tptr;

	for ( int i = startIndex; i <= endIndex; ++i )
	{
		//要计算的像素位置  内核中心点
		int X = i % maxWidth;
		int Y = i / maxWidth;

		if ( X < r || X >= maxWidth -r  || Y < r || Y >= maxHeight - r) continue;

		sumVal_b = 0, sumVal_g = 0, sumVal_r = 0;
		sumValSqr_b = 0, sumValSqr_g = 0, sumValSqr_r = 0;
		totalWeight_b = 0., totalWeight_g = 0., totalWeight_r = 0.;
		tmpSum_b = 0., tmpSum_g = 0., tmpSum_r = 0.;

		int startLMJ = Y - r;
		int endLMJ = Y + r;
		int howManyAll = space_weight.size();

		//遍历内核，分别计算红、绿、蓝三个通道的相似度权值方差  
		for ( int y = startLMJ; y < endLMJ; y++ )
		{
			tptr = srcData + y*pit +( X - r  ) * bitCount;  //内核左边界
			for ( int x =0 ; x <= 2 * r; x++ )
			{
				currVal_b = tptr[bitCount* ( x ) ], currVal_g = tptr[bitCount*( x ) + 1], currVal_r = tptr[bitCount*( x ) + 2];
				sumVal_b += currVal_b;
				sumVal_g += currVal_g;
				sumVal_r += currVal_r;
				sumValSqr_b += ( currVal_b *currVal_b );
				sumValSqr_g += ( currVal_g *currVal_g );
				sumValSqr_r += ( currVal_r *currVal_r );
			}
		}
		var_b = ( ( sumValSqr_b * howManyAll ) - sumVal_b * sumVal_b ) / ( ( float ) ( howManyAll*howManyAll ) );
		var_g = ( ( sumValSqr_g * howManyAll ) - sumVal_g * sumVal_g ) / ( ( float ) ( howManyAll*howManyAll ) );
		var_r = ( ( sumValSqr_r * howManyAll ) - sumVal_r * sumVal_r ) / ( ( float ) ( howManyAll*howManyAll ) );

		if ( var_b < 0.01 )
			var_b = 0.01f;
		else if ( var_b > max_var )
			var_b = ( float ) ( max_var );

		if ( var_g < 0.01 )
			var_g = 0.01f;
		else if ( var_g > max_var )
			var_g = ( float ) ( max_var );

		if ( var_r < 0.01 )
			var_r = 0.01f;
		else if ( var_r > max_var )
			var_r = ( float ) ( max_var );

		currValCenter_b = srcData[Y*pit + X * bitCount], currValCenter_g = srcData[Y*pit + X * bitCount + 1], currValCenter_r = srcData[Y*pit + X * bitCount + 2];
		//再次遍历内核，计算最终的结果  
		for ( int y = startLMJ; y < endLMJ; y++ )
		{
			tptr = srcData + y*pit + ( X - r ) * bitCount;;  //内核左边界
			for ( int x = 0; x <= 2 * r; x++ )
			{
				currVal_b = tptr[bitCount*( x )]; 
				currVal_g = tptr[bitCount*( x ) +1];
				currVal_r = tptr[bitCount*( x ) +2];
				currWRTCenter_b = currVal_b - currValCenter_b;
				currWRTCenter_g = currVal_g - currValCenter_g;
				currWRTCenter_r = currVal_r - currValCenter_r;

				float cur_spw = space_weight[ ( y + r - Y ) * ( 2 * r + 1 ) + x];

				weight_b = exp ( -0.5f * currWRTCenter_b * currWRTCenter_b / var_b ) * cur_spw;
				weight_g = exp ( -0.5f * currWRTCenter_g * currWRTCenter_g / var_g ) * cur_spw;
				weight_r = exp ( -0.5f * currWRTCenter_r * currWRTCenter_r / var_r ) * cur_spw;
			
				tmpSum_b += ( ( float ) currVal_b * weight_b );
				tmpSum_g += ( ( float ) currVal_g * weight_g );
				tmpSum_r += ( ( float ) currVal_r * weight_r );
				totalWeight_b += weight_b, totalWeight_g += weight_g, totalWeight_r += weight_r;
			}
		}
		tmpSum_b /= totalWeight_b;
		tmpSum_g /= totalWeight_g;
		tmpSum_r /= totalWeight_r;
		*( dstData + Y * pit + X * bitCount ) = tmpSum_b;
		*( dstData + Y * pit + X * bitCount + 1 ) = tmpSum_g;
		*( dstData + Y * pit + X * bitCount + 2 ) = tmpSum_r;
	}
}


