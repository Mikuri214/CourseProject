#ifndef DATA_MODEL_H_
#define DATA_MODEL_H_

#include <vector>
using namespace std;

#include <highgui.hpp>
using namespace cv;

class DataModel
{
public:
	~DataModel ();
	static DataModel* getInstance ();
	static bool deleteInstance ();

	bool loadData ( string dir );
	Mat getData ( int index );
private:
	static DataModel* m_instance;
	vector<string> fileLists;

	DataModel (){};
	DataModel ( const DataModel& ){};
	DataModel& operator =( DataModel& ){};
};


#endif
