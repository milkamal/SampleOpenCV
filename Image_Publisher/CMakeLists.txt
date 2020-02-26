# Configure our Required Version
cmake_minimum_required(VERSION 3.0)
# Define the name of our project
project(pub-sub-cpp)
# Define the output library to be a shared file (.so or .dll) and output it as "(lib)hello-cpp.so"
add_library(pub-sub-cpp SHARED Image_Publisher.cpp pch.cpp Image_Publisher.h pch.h)
