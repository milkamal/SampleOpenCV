# Configure our Required Version
cmake_minimum_required(VERSION 3.0)
project(pub-sub-cpp)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Define the output library to be a shared file (.so or .dll) and output it as "(lib)hello-cpp.so"
add_library(pub-sub-cpp SHARED Image_Publisher.cpp pch.cpp Image_Publisher.h pch.h)
target_link_libraries( pub-sub-cpp ${OpenCV_LIBS} )

