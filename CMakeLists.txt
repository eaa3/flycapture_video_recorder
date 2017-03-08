cmake_minimum_required(VERSION 2.8)
project( FlyCaptureOpenCV )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules/)

find_package( OpenCV REQUIRED )
find_package( FlyCapture2 REQUIRED )



include_directories(${FlyCapture2_INCLUDE_DIRS})

#set(FLYCAPTURE_LIBRARY_DIRS "NOTFOUND")
#set(FLYCAPTURE_LIBRARY_DIRS /usr/lib/flycapture)
#find_library(FLYCAPTURE_LIBRARY NAMES libflycapture.so
#             HINTS /usr/lib/ /usr/lib)

#message("Found Libraries: ${FLYCAPTURE_LIBRARY}")
add_executable( cv_flycapture src/flycapture_opencv.cpp )
target_link_libraries( cv_flycapture ${OpenCV_LIBS} ${FlyCapture2_LIBRARIES} )
