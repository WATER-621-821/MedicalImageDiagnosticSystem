#include "ImageCropper.h"

ImageCropper::ImageCropper(string inImg, string otImg)
{
	this->inputImage = inImg;
	this->outputImage = otImg;	
}

void ImageCropper:: crop()
{
  	typedef    float    InputPixelType;
  	typedef    float    OutputPixelType;

  	const unsigned int dimension =3;
  	typedef itk::Image< InputPixelType,  dimension >   InputImageType;
  	typedef itk::Image< OutputPixelType, dimension >   OutputImageType;

  	typedef itk::ImageFileReader< InputImageType >  ReaderType;
  	typedef itk::ImageFileWriter< OutputImageType >  WriterType;
  	typedef itk::CropImageFilter <InputImageType, OutputImageType> CropImageFilterType;

  	ReaderType::Pointer reader = ReaderType::New();
  	WriterType::Pointer writer = WriterType::New();
  	CropImageFilterType::Pointer cropFilter
    	= CropImageFilterType::New();

    InputImageType::SizeType cropSize;

    reader->SetFileName(this->inputImage);
	cropSize[0]=0;
	cropSize[1]=0;
	cropSize[2]=55;

  	cropFilter->SetInput(reader->GetOutput());
  // The SetBoundaryCropSize( cropSize ) method specifies the size of
  // the boundary to be cropped at both the uppper & lower ends of the
  // image eg. cropSize/2 pixels will be removed at both upper & lower
  // extents
  	cropFilter->SetBoundaryCropSize(cropSize);
  // The below three lines are equivalent to the above two lines:
  //ImageType::SizeType halfCropSize = {5,7};
  //cropFilter->SetUpperBoundaryCropSize(halfCropSize);
  //cropFilter->SetLowerBoundaryCropSize(halfCropSize);
 	
 	writer->SetFileName(this->outputImage);	
 	writer->SetInput(cropFilter->GetOutput());
 	writer->Update();
}