#ifndef IMG_MATCH_ALGORITHM_FACTORY_
#define IMG_MATCH_ALGORITHM_FACTORY_
#include "ImgMatchAlgorithm.h"
#include <vector>
using namespace std;

class ImgMatchAlgorithmFactory
{
public:
	~ImgMatchAlgorithmFactory (){};

	static ImgMatchAlgorithmFactory* getInstance ();
	ImgMatchAlgorithm* getImgMatchAlgorithm ( string featureMethod, string resolveOutliersMethod = "2NN", string matchMethod = "without ransac" );
	vector<string> getAlgorithmNames ( string algorithmScore );

private:
	static ImgMatchAlgorithmFactory* instance;
	ImgMatchAlgorithmFactory (){};
	ImgMatchAlgorithmFactory ( const ImgMatchAlgorithmFactory & ){};
	ImgMatchAlgorithmFactory* operator=( ImgMatchAlgorithmFactory & ){};
};



#endif
