cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (openCV_opdrachtV2
	VERSION 0.1
	DESCRIPTION "A Hello World Project")

add_compile_options(-Wall -Wextra -Wconversion)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# This project will output an executable file
add_executable(${PROJECT_NAME}
   src/openCV_opdrachtV2.cpp
   src/ShapeDetection.h
   src/ShapeDetection.cpp
   src/Colours.h
   src/Parser.h
   src/Parser.cpp
)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

# Create a simple configuration header
configure_file(config.h.in config.h)

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
