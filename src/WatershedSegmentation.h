#ifndef WatershedSegmentation_H_
#define WatershedSegmentation_H_


using namespace std;

#include "Segmentation.h"

class WatershedSegmentation : public Segmentation
{
	private:
		static const unsigned int dimension3d =3;
		static const unsigned int dimension2d= 2;
		string inputFileName;
		string outputFileName;

	
	public:
	    int dimension;
		float threshold;
		float level;

		WatershedSegmentation(string filename, float thes, float lev);
		WatershedSegmentation(string filename, string otflname, float thres, float lev);
		void performSegmentation() ;
		

};


#endif 