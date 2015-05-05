#ifndef Registration_H_
#define Registration_H_


#include <iostream>
#include <cstdlib>

// ITK IO includes
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

// ITK Registration includes
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"

using namespace std;


class Registration
{
	private:
		static const unsigned int dimension3d =3;
		static const unsigned int dimension2d= 2;
		string fixedImage;
		string movingImage;
		string outputImage;

	public:
		 unsigned int RmsCounter;
	     double MaxRmsE[4];
													

	
	public:
	    int dimension;
		Registration(string fdImage,string mvImage, string otImage);
		void performRegistration();
		

};


#endif 