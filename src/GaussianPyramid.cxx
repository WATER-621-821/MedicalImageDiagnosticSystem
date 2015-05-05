
#include "GaussianPyramid.h"

GaussianPyramid :: GaussianPyramid(string img, string otImg)
{
	this->inputImage = img;
	this->outputImage = otImg;

} 		

void GaussianPyramid :: execute()
{
  typedef    float    InputPixelType;
  typedef    float    OutputPixelType;

  const unsigned int dimension =3;
  typedef itk::Image< InputPixelType,  dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, dimension >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  typedef itk::SmoothingRecursiveGaussianImageFilter<					//Do we need to sharpen the image for watershed segmentation?
                        InputImageType, OutputImageType >  FilterType;   // how about laplacian Gaussian Filter?
		
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(this->inputImage);

  FilterType::Pointer filter = FilterType::New();

  filter->SetNormalizeAcrossScale( false );

  filter->SetInput( reader->GetOutput() );

  const double sigma = 1.0;

  filter->SetSigma( sigma );

  filter->Update();

  typedef  unsigned char                             WritePixelType;
  typedef itk::Image< WritePixelType, dimension >            WriteImageType;
  typedef itk::RescaleIntensityImageFilter<
                   OutputImageType, WriteImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255); 
  typedef itk::ImageFileWriter< WriteImageType >  WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( this->outputImage );
  rescaler->SetInput( filter->GetOutput() );

 
  typedef itk::ShrinkImageFilter <WriteImageType, WriteImageType>
          ShrinkImageFilterType;
 
  ShrinkImageFilterType::Pointer shrinkFilter
          = ShrinkImageFilterType::New();
  shrinkFilter->SetInput(rescaler->GetOutput());
  shrinkFilter->SetShrinkFactor(0, 2); // shrink the first dimension by a factor of 2 (i.e. 100 gets changed to 50)
  shrinkFilter->SetShrinkFactor(1, 2); // shrink the second dimension by a factor of 2 (i.e. 100 gets changed to 50)
  shrinkFilter->SetShrinkFactor(2, 2);
  shrinkFilter->Update();
  //filter->SetInput(shrinkFilter->GetOutput());
  //need to sharpen the output image from the shrinkFiler
  writer->SetInput( shrinkFilter->GetOutput() );
  writer->Update();
	
	
}