#include "algorithmInterface.h"

class  ai_egg_embryo_vessel_detection : public AlgorithmInterface
{
public:
	cv::Mat imageSrc;    
	cv::Mat imgDst;
	double procTime;//ms
public:
	ai_egg_embryo_vessel_detection ( )
	{
		init ();
	}
	~ai_egg_embryo_vessel_detection ( )
	{
		destroy ();
	}
	virtual void init ()
	{
		AlgorithmInterface::init ();
		procTime = 0.0;
	}
	virtual void destroy (){}
};
