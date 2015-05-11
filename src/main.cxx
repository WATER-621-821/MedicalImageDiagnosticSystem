#include "WatershedSegmentation.h"
#include "DicomSeriesTo3dImage.h"
#include "FolderReader.h"
#include "GaussianPyramid.h"
#include "ImageCropper.h"
#include <time.h>
#include <stack>
#include <string>
#include "Registration.h"

string fixedImage;
string movingImage;
string outputImageName;
string segmentedImage;

// this is used to create unique name for dicom converted 3d images.
static int dicomConversionCount=0;
static string inputSignal = string("==>");

/**
	starts interactive Mode of the diagnosis system
**/
void interactiveMode();

/**
	This perfomrs a full diagnosis of a CT image
		First performs registration
		Second performs segmentation
**/
void fullDiagnosis();

/**
	This gives option to etierrh perform registration or segmentation
**/
void partialDiagnosis();

/**
	@return d if input type is dicom series
	 or else returns 3 for a 3d image input
**/
string getInputType();

/**
	Prompts user for input images (fixedImage, movingImage)	
	Sets the global input images
**/
void setInputImage();

/**
	Prompts user for the dicom series directory
	Creates a 3d image from the dicom series
	@return the 3d image name as a string 
**/
string getInputDicomSeries();

/**
	Prompts user for a 3d image with proper path
	@return a 3d image as a string
**/
string getInput3dImage();

/**
	Prompts user for output image name
	@return an output image name as a string
**/
string getOutputImageName();

/**
   Promps dicom series for input
   Creates a 3d image from the series
   Then segments the 3d image
**/
void threeDimensionDicomSeriesSegment();

/**
	Prompts 3d image for input
	Then segments the 3d image
**/
void segment3dImage();

/**
	@param fixedIm is the name of the 3d fixed Image
	@param movingIm is the name of the 3d moving Image
	@param outputIm is the name of the output image
	 In the case for input images need full path along with name if in a 
	    different directory or else just the name.
	Performs Registration on the two 3d images.
**/
void executeRegistration(string fixedIm, string movingIm , string outputIm);

/**
	@param inputFile is the name of the 3d input image that is to be segmented
	@param outputFile is the name of the output image or the segmented image
	@param downsample needs to be true if you want to downsample the input image for segmentation
	@param crop needs to be true if you want to crop the input image for segmentation
	
	 In the case for input images need full path along with name if in a 
	    different directory or else just the name.
	Performs Segmentation on the inputImage (indicated by inputFile).
**/
void executeSegmentation(string inputFile, string outputFile, bool downSample, bool crop);

double sec();

int main(int argc, char ** argv)
{	

	if(argc<5)
	{
		interactiveMode();
		return 1;

	}
	
	DicomSeriesTo3dImage converter = DicomSeriesTo3dImage();
	string outputFile = argv[2];

	converter.setInputFolder(argv[1]);
	converter.setOutputFileName(outputFile+"-original");
	converter.setOutputFileFormat("mha");
	converter.convertTo3dFormat();

	
	WatershedSegmentation seg (converter.getOutputFileName()+"."+converter.getOutputFileFormat(), 
		(outputFile+".mha"),atof(argv[3]), atof(argv[4]));

	seg.performSegmentation();
	
	
   return 0;
}

void interactiveMode()
{
	string userSelection = "";
	cout<<"-----------------------------------------------------------------------"<<endl;
	cout<<"-----------------------------------------------------------------------"<<endl;
	cout<<"| Welcome to our CT Image Diagnostic System                           |"<<endl;
	cout<<"| We will help you diagnose and analyse Lungs in a CT scan            |"<<endl;
	cout<<"| NOTE: Currently we only work with CT scan images for Lungs          |"<<endl;
	cout<<"-----------------------------------------------------------------------"<<endl;
	cout<<"-----------------------------------------------------------------------\n\n"<<endl;

	do
	{
		
		cout<<"\nPlease select from the following:"<<endl;
		cout<<"   Enter 1 for a full diagnosis of a CT image"<<endl;
		cout<<"   Enter 2 to separately evaluate registration and segmentation of a CT image"<<endl;
		cout<<"   Enter e to exit"<<endl;
		cout<<inputSignal;
		cin >> userSelection;
		if(userSelection.compare("e") == 0) break;
		else if(userSelection.compare("1") == 0) fullDiagnosis();
		else if(userSelection.compare("2") == 0) partialDiagnosis();


	}while(1);
}

void fullDiagnosis()
{

	setInputImage();
	outputImageName = getOutputImageName();
	executeRegistration(fixedImage, movingImage, outputImageName);
	executeSegmentation(outputImageName, "segmented_"+outputImageName, false, false);
	cout<<"\nImage Processing Complete!"<<endl;
}

void partialDiagnosis()
{
	string userSelection = "";

	cout<<"\n   Enter 1 for registration"<<endl;
	cout<<"   Enter 2 for segmentation"<<endl;
	cout<<inputSignal;
	cin >> userSelection;


	if(userSelection.compare("1") == 0)
	{
		setInputImage();
		outputImageName = getOutputImageName();
		executeRegistration(fixedImage, movingImage, outputImageName);
	}
	else if(userSelection.compare("2") == 0) 
	{
		string inputType = getInputType();
		if(inputType.compare("d")==0) threeDimensionDicomSeriesSegment();
		else segment3dImage();
	}
	cout<<"\nImage Processing Complete!"<<endl;

}

string getInputType()
{
	string userSelection = "";

	cout<<"\n   Enter 1 to input dicom series"<<endl;
	cout<<"   Enter 2 to input a 3d image"<<endl;
	cout<<inputSignal;
	cin >> userSelection;

	if(userSelection.compare ("1")==0) userSelection = "d";
	else userSelection = "3";

	return userSelection;

}

void setInputImage()
{
	string userSelection = "";

	userSelection = getInputType();

	if(userSelection.compare("d") == 0)
	{ 
		cout<<"      Input fixed Image"<<endl;
		fixedImage = getInputDicomSeries();
		cout<<"      Input Moving Image"<<endl;
		movingImage =  getInputDicomSeries();
		
	}
	else if(userSelection.compare("3") == 0) 
	{
		cout<<"      Input fixed Image"<<endl;
		fixedImage = getInput3dImage();
		cout<<"      Input Moving Image"<<endl;
		movingImage =  getInput3dImage();
	}
}

string getInputDicomSeries()
{
	string inputFolder;
	string outputFile;
	string outputFileFormat;

	cout<<"   Enter the dicom series folder:"<<endl;
	cout<<inputSignal;
	cin>>inputFolder;


	DicomSeriesTo3dImage converter = DicomSeriesTo3dImage();
	

	converter.setInputFolder(inputFolder);
	outputFile = std::to_string(dicomConversionCount);
	converter.setOutputFileName(outputFile+"-original");
	converter.setOutputFileFormat("mha");
	converter.convertTo3dFormat();

	dicomConversionCount++;
	outputFile = converter.getOutputFileName()+"."+converter.getOutputFileFormat();
	
	return outputFile;

}

string getInput3dImage()
{
	string inputFile;

	cout<<"   Enter the complete 3d input file (along with format):"<<endl;
	cout<<inputSignal;
	cin>>inputFile;

	return inputFile;

}

string getOutputImageName()
{
	string outputFile;
	string outputFileFormat;

	cout<<"   Enter the output file name(not with format):"<<endl;
	cout<<inputSignal;
	cin>>outputFile;
	cout<<"   Enter the output file format:"<<endl;
	cout<<inputSignal;
	cin>>outputFileFormat;

	return (outputFile+"."+outputFileFormat);
}

void threeDimensionDicomSeriesSegment()
{
	string inputImage;
	string outputFileName;
	inputImage = getInputDicomSeries();
	outputFileName = getOutputImageName();

	executeSegmentation(inputImage, 
		(outputImageName), true, true);
	

}


void segment3dImage()
{
	string inputImage;
	string outputFileName;
	inputImage = getInput3dImage();
	outputFileName = getOutputImageName();

	executeSegmentation(inputImage, 
		(outputImageName), true, true);
}

void executeRegistration(string fixedIm, string movingIm , string outputIm)
{
	GaussianPyramid gm = GaussianPyramid(fixedIm, "down_sampled_"+fixedIm);
	gm.execute();
	gm.inputImage = movingIm;
	gm.outputImage = "down_sampled_"+movingIm;
	gm.execute();

	ImageCropper cropper = ImageCropper( "down_sampled_"+fixedIm, "down_sampled_"+fixedIm);
	cropper.crop();
	cropper.inputImage = "down_sampled_"+movingIm;
	cropper.outputImage = "down_sampled_"+movingIm;
	cropper.crop();

	Registration reg = Registration("down_sampled_"+fixedIm, "down_sampled_"+movingIm, outputIm);
	reg.performRegistration();
}

void executeSegmentation(string inputFile, string outputFile, bool downSample, bool crop)
{
	float threshold;
	float level;
	double time0, time1;
	time0=0; time1=0;

	cout<<"\n   Enter the lower threshold level :"<<endl;
	cout<<inputSignal;
	cin>>threshold;
	cout<<"   Enter the scale level:"<<endl;
	cout<<inputSignal;
	cin>>level;

	if(downSample)
	{
		GaussianPyramid gm = GaussianPyramid(inputFile, "down_sampled_"+inputFile);
		gm.execute();
		inputFile =  "down_sampled_"+inputFile;
	}

	if(crop)
	{
		ImageCropper cropper = ImageCropper( inputFile, inputFile);
		cropper.crop();
	}
	
	WatershedSegmentation seg (inputFile, outputFile, threshold, level);

	time0 = sec();
	//cout<<"Starting Segmentation\n";
	seg.performSegmentation();
	time1 = sec();

	cout<<"\nTime for Segmentation: "<<(time1-time0)<<" secs"<<endl;
}

double sec()
{
	return (double)clock()/(double)CLOCKS_PER_SEC;
}

