#ifndef GaussianPyramid_H_
#define GaussianPyramid_H_


using namespace std;

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkSmartPointer.h"
#include <string>

class GaussianPyramid
{



	public:
		string inputImage;
		string outputImage;
		/**
 			@parameter img is the name of the input image
        		if the image is not in the current directory then full path along with the name must be given
 			@parameter otImg is the name of the output image
				output image will be saved in the current directory where the executable resides.
		**/
		GaussianPyramid(string inImg, string otImg);


		/**
			DownSamples the input image
			Creates the new down sampled image with the set output Image name in the current directory of the executable.
		**/	
		void execute();

		

};


#endif 