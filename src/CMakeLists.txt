cmake_minimum_required(VERSION 2.8)

PROJECT( diagnosticSystem )

#CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/Segmentation.h
 #               ${CMAKE_CURRENT_BINARY_DIR}/Segmentation.h
#)
#
#  Define the list of source files
#
INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR} 
)

#
#  Define the list of source files
#
SET(APPLICATION_SOURCES
  WatershedSegmentation.cxx
  FolderReader.cxx
  DicomSeriesTo3dImage.cxx
  GaussianPyramid.cxx
  ImageCropper.cxx
  Registration.cxx
  main.cxx
  )
set(HEADER_FILES Segmentation.h  WatershedSegmentation.h FolderReader.h 
DicomSeriesTo3dImage.h GaussianPyramid.h ImageCropper.h Registration.h)


find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()
 
add_library( ${HEADER_FILES})
add_executable(diagnosticSystem MACOSX_BUNDLE WatershedSegmentation.cxx FolderReader.cxx
DicomSeriesTo3dImage.cxx main.cxx GaussianPyramid.cxx ImageCropper.cxx 
Registration.cxx ${HEADER_FILES})

target_link_libraries(diagnosticSystem
  ${Glue}  ${VTK_LIBRARIES} ${ITK_LIBRARIES})
SET_TARGET_PROPERTIES(Segmentation.h PROPERTIES LINKER_LANGUAGE C++)
