#include "DicomSeriesTo3dImage.h"
	
DicomSeriesTo3dImage :: DicomSeriesTo3dImage()
{

}

void DicomSeriesTo3dImage ::  setInputFolder(string inputFol)
{
	this->dicomInputFolder = inputFol;		

}

void DicomSeriesTo3dImage :: setOutputFolder(string outputFol)
{
	this->outputFolder = outputFol;

}
		
void DicomSeriesTo3dImage :: setOutputFileName(string file)
{
	this->outputFileName = file;	

}

void DicomSeriesTo3dImage :: setOutputFileFormat(string format)
{
	this->outputFileFormat = format;
}


string DicomSeriesTo3dImage:: getInputFolder()
{
	return this->dicomInputFolder;
}

string DicomSeriesTo3dImage:: getOutputFolder()
{
	return this->outputFolder;
}
		
string DicomSeriesTo3dImage:: getOutputFileName()
{
	return this->outputFileName;
}

string DicomSeriesTo3dImage:: getOutputFileFormat()
{
	return this->outputFileFormat;
}

void DicomSeriesTo3dImage :: convertTo3dFormat()
{
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO( dicomIO );

	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails( true );
	nameGenerator->AddSeriesRestriction("0008|0021" );
	nameGenerator->SetDirectory( this->dicomInputFolder );

	try
    {
		std::cout << std::endl << "The directory: " << std::endl;
		std::cout << std::endl << this->dicomInputFolder << std::endl << std::endl;
		std::cout << "Contains the following DICOM Series: ";
		std::cout << std::endl << std::endl;


// The GDCMSeriesFileNames object first identifies the list of DICOM series
// present in the given directory. We receive that list in a reference
// to a container of strings and then we can do things like print out all
// the series identifiers that the generator had found. Since the process of
// finding the series identifiers can potentially throw exceptions, it is
// wise to put this code inside a \code{try/catch} block.

		typedef std::vector< std::string >    SeriesIdContainer;
		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
		SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
		while( seriesItr != seriesEnd )
		{
			std::cout << seriesItr->c_str() << std::endl;
			++seriesItr;
		}


		// Given that it is common to find multiple DICOM series in the same directory,
		// we must tell the GDCM classes what specific series we want to read. In
		// this example we do this by checking first if the user has provided a series
		// identifier in the command line arguments. If no series identifier has been
		// passed, then we simply use the first series found during the exploration of
		// the directory.

		std::string seriesIdentifier;
		//if( argc > 3 ) // If no optional series identifier
		 // {
		  //seriesIdentifier = argv[3];
		  //}
		//else
		  ///{
		seriesIdentifier = seriesUID.begin()->c_str();
		  //}

		std::cout << std::endl << std::endl;
		std::cout << "Now reading series: " << std::endl << std::endl;
		std::cout << seriesIdentifier << std::endl;
		std::cout << std::endl << std::endl;

		// We pass the series identifier to the name generator and ask for all the
		// filenames associated to that series. This list is returned in a container of
		// strings by the \code{GetFileNames()} method.
		//
		// \index{itk::GDCMSeriesFileNames!GetFileNames()}

		typedef std::vector< std::string >   FileNamesContainer;
		FileNamesContainer fileNames;
		fileNames = nameGenerator->GetFileNames( seriesIdentifier );

		// The list of filenames can now be passed to the \doxygen{ImageSeriesReader}
		// using the \code{SetFileNames()} method.
		//
		//  \index{itk::ImageSeriesReader!SetFileNames()}

		reader->SetFileNames( fileNames );

		// Finally we can trigger the reading process by invoking the \code{Update()}
		// method in the reader. This call as usual is placed inside a \code{try/catch}
		// block.

		try
		{
			reader->Update();
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
			
		}

		// At this point, we have a volumetric image in memory that we can access by
		// invoking the \code{GetOutput()} method of the reader.
	
		// We proceed now to save the volumetric image in another file, as specified by
		// the user in the command line arguments of this program. Thanks to the
		// ImageIO factory mechanism, only the filename extension is needed to identify
		// the file format in this case.

		typedef itk::ImageFileWriter< ImageType > WriterType;
		WriterType::Pointer writer = WriterType::New();
		writer->SetFileName( this->outputFileName+"."+this->outputFileFormat) ;
		writer->SetInput( reader->GetOutput() );

		std::cout  << "Writing the image as " << std::endl << std::endl;
		std::cout  << this->outputFileName << std::endl << std::endl;


		// The process of writing the image is initiated by invoking the
		// \code{Update()} method of the writer.
		try{
			writer->Update();
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
		
		}
		
    }
	catch (itk::ExceptionObject &ex)
    {
		std::cout << ex << std::endl;
	
    }
	
}


void DicomSeriesTo3dImage :: convertToAll3dFormat()
{
	int i;
	string fileFormats[] = {"vtk","mha"};

	for(i=0; i<2; i++)
	{
		this->outputFileFormat = fileFormats[i];
		this->convertTo3dFormat();
	}

}