#pragma once

struct ThreadParam
{
	CImage * src;
	CImage * dst;
	int startIndex;
	int endIndex;

	union 
	{
		double scale_rate;
		double rotate_arc;
	}u;
};

struct autolevelThreadParam
{
	CImage * src;
	CImage * dst;
	int startIndex;
	int endIndex;
	byte*  newmin;
	byte*  newmax;
	byte*  oldmin;
	byte*  oldmax;
};

struct mixThreadParam{
	CImage* img1;
	CImage* img2;
	CImage* dst;

	int startIndex;
	int endIndex;

	double alpha;
};

struct bbfThreadParam
{
	CImage* src;
	CImage* dst;

	int startIndex;
	int endIndex;

	int r;
};


void Histogram ( CImage *img, long hist[3][256] );
void getBounderValue ( long hist[3][256], byte* newmin, byte* newmax, byte* oldmin, byte* oldmax, double lowCut, double hightCut, long PixelAmount );

class ImageProcess
{
public:
	static void  scale ( void* param );
	static void  rotate ( void* param );
	static void  autoLevels ( void* param );
	static void  autoWhiteBalance ( void* param );
	static void  sketchTranslate ( void* param );
	static void  mixture ( void* param );
	static void	 Bilateral_Filter ( void* param );
};

