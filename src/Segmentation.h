#ifndef __Segmentation_H
#define __Segmentation_H

#include <stdio.h>
#include "itkWatershedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"



class Segmentation
{
	
	public:
		typedef float InternalPixelType;
        typedef itk::RGBPixel<unsigned char> RGBPixelType; 


 
    public:  
		
		virtual void performSegmentation() =0;	

};


#endif
