#ifndef __DicomSeriesTo3dImage_H
#define __DicomSeriesTo3dImage_H

#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

using namespace std;

class DicomSeriesTo3dImage
{
	
	private:
		string dicomInputFolder;
		string outputFolder;
		string outputFileName;
		string outputFileFormat;

		// We define the pixel type and dimension of the image to be read. In this
		// particular case, the dimensionality of the image is 3, and we assume a
		// \code{signed short} pixel type that is commonly used for X-Rays CT scanners.
		//
		// The image orientation information contained in the direction cosines
		// of the DICOM header are read in and passed correctly down the image processing
		// pipeline.
		typedef signed short    PixelType;
		static const unsigned int      Dimension = 3;
		typedef itk::Image< PixelType, Dimension >         ImageType;
		// We use the image type for instantiating the type of the series reader and
		// for constructing one object of its type.
		typedef itk::ImageSeriesReader< ImageType >        ReaderType;
		// A GDCMImageIO object is created and connected to the reader. This object is
		// the one that is aware of the internal intricacies of the DICOM format.
 		typedef itk::GDCMImageIO       ImageIOType;
 
    public:  
		DicomSeriesTo3dImage();
		void setInputFolder(string inputFol);
		void setOutputFolder(string outputFol);
		void setOutputFileName(string file);
		void setOutputFileFormat(string format);

		string getInputFolder();
		string getOutputFolder();
		string getOutputFileName();
		string getOutputFileFormat();

		void convertTo3dFormat();
		void convertToAll3dFormat();
		


};


#endif