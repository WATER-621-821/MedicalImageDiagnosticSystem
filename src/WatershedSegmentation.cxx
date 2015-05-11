
#include <sstream>
#include  "WatershedSegmentation.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"
#include <itkLaplacianRecursiveGaussianImageFilter.h>

#include "itkGradientImageFilter.h"

WatershedSegmentation :: WatershedSegmentation( string filename,float thres, float lev)
{
	this->inputFileName = filename;
	this->threshold = thres;
	this->level = lev;
	this->dimension = 3;
}


WatershedSegmentation :: WatershedSegmentation( string filename, string otflname, float thres, float lev)
{
	this->inputFileName = filename;
	this->threshold = thres;
	this->level = lev;
	this->dimension = 3;

	std::ostringstream thresholdbuff;
	std::ostringstream levelbuff;
	thresholdbuff<<thres;
	levelbuff<<level;

	this->outputFileName = otflname;
    /*std::size_t pos = otflname.find(".");     
    std::string extension =  otflname.substr (pos); 

	this->outputFileName = otflname.substr(0, pos);
    this->outputFileName.append(thresholdbuff.str());
	this->outputFileName.append("-");
	this->outputFileName.append(levelbuff.str());
	this->outputFileName.append(extension);*/
}

void WatershedSegmentation:: performSegmentation()
{
	typedef itk::Image< InternalPixelType,  WatershedSegmentation::dimension3d >  InternalImageType;
  	typedef itk::Image< RGBPixelType, WatershedSegmentation::dimension3d>  RGBImageType;

	typedef  itk::ImageFileReader< InternalImageType   >  ReaderType;
	typedef  itk::ImageFileReader< RGBImageType   >  RGBReaderType;
	typedef  itk::ImageFileWriter< RGBImageType  >  RGBWriterType;
	typedef  itk::ImageFileWriter< InternalImageType >  WriterType;

	InternalImageType::Pointer segmentedImage;
	RGBImageType::Pointer coloredSegmentedImage;

    ReaderType::Pointer reader = ReaderType::New();
	RGBWriterType::Pointer writer = RGBWriterType::New();


	reader->SetFileName( this->inputFileName );
	writer->SetFileName(this->outputFileName);		 
    InternalImageType::Pointer image = reader->GetOutput();
		
			  //
  //  Instantiate the GradientMagnitude image filter
  //itk::GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
	
	//giving error failed to allocate memory for large 3d image like the target 1. 
	// might need to compress the image to some appropriate level.
	typedef  itk::RecursiveMultiResolutionPyramidImageFilter< InternalImageType,
                                                     InternalImageType> PyramidFilterType;
    typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<
                                                     InternalImageType,
                                                     InternalImageType
                                                          > GradientMagnitudeFilterType;

    GradientMagnitudeFilterType::Pointer gradienMagnitudeFilter = GradientMagnitudeFilterType::New();
	//PyramidFilterType::Pointer pyramidFilter = PyramidFilterType::New();

	//pyramidFilter->SetInput(image);

    gradienMagnitudeFilter->SetInput( image );
    gradienMagnitudeFilter->SetSigma( 1.0 );

	//cout<<"gradientFilter is Set\n";
	gradienMagnitudeFilter->Update();
	//cout<<"gradienMagnitudeFilter->Update();\n";

  //
  //  Instantiate the Watershed filter
  //
	//pyramidFilter->SetNumberOfLevels(4);
	//pyramidFilter->SetUseShrinkImageFilter(true);
	//pyramidFilter->Update();
    typedef  itk::WatershedImageFilter<
                              InternalImageType
                                            > WatershedFilterType;

    WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();

	watershedFilter->SetInput( gradienMagnitudeFilter->GetOutput());
	

    watershedFilter->SetThreshold( this->threshold );
    watershedFilter->SetLevel( this->level );
	
	//cout<<"watershedFilter is Set\n";

  //
  //  Instantiate the filter that will encode the label image
  //  into a color image (random color attribution).
  //

    typedef itk::Functor::ScalarToRGBPixelFunctor<
                                           unsigned long
                                                    > ColorMapFunctorType;

    typedef WatershedFilterType::OutputImageType  LabeledImageType;

    typedef itk::UnaryFunctorImageFilter<
                                LabeledImageType,
                                RGBImageType,
                                ColorMapFunctorType
                                                > ColorMapFilterType;

    ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
	watershedFilter->Update();
	//cout<<"watershedFilter is updated\n";
    colorMapFilter->SetInput(  watershedFilter->GetOutput() );
	coloredSegmentedImage =  colorMapFilter->GetOutput() ;


    writer->SetInput(coloredSegmentedImage );
    try
    {
		//cout<<"About to write image \n";
        writer->Update();
    }
    catch( itk::ExceptionObject & excep )
    {
        std::cerr << "Exception caught !" << std::endl;
        std::cerr << excep << std::endl;
    }

}
