#include "ShapeDetection.h"
#include "Parser.h"
#include "Colours.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <utility>
#include <mutex>

std::mutex mtx;
Colour requestedColour = CALIBRATE;
std::string requestedShape = "";
bool batchMode = false;
std::string fileName;

//void setColour(Colour color)
//{
//	std::lock_guard<std::mutex> lock(mtxColour);
//	requestedColour = color;
//}

void handleInput() {
	Parser parser;
    std::string input;
    while (true) {


    	if(batchMode){
    		std::lock_guard<std::mutex> lock(mtx);
    		parser.parseFile(fileName, requestedShape, requestedColour);
    		std::lock_guard<std::mutex> unlock(mtx);
    	}else{
			std::cout << "Enter shape and colour: ";
			std::getline(std::cin, input);

			std::lock_guard<std::mutex> lock(mtx);
			parser.parseInput(input, requestedShape, requestedColour);
			std::lock_guard<std::mutex> unlock(mtx);
    	}


    }
}

int main(int argc, char **argv) {
	ShapeDetection shapeDetection;
	cv::Mat rawImg;

	if(argc > 1){
		fileName = argv[1];
		Parser parser;
		batchMode = true;

	}

	cv::VideoCapture cap(2, cv::CAP_V4L2);

	if (!cap.isOpened()) {
		std::cout << "Error: Cannot open camera." << std::endl;
		return 1;
	}
	std::thread InputThread(handleInput);

	shapeDetection.showSliders();

	while(true){
		cap.read(rawImg);
		cv::Mat outputImg = rawImg;

		shapeDetection.getContours(rawImg, outputImg, requestedColour, requestedShape);

		cv::imshow("Mask image", shapeDetection.imgMask);
		cv::imshow("Output image",outputImg);
		cv::waitKey(1);
	}
	InputThread.join();

    cap.release();
    cv::destroyAllWindows();

	return 0;
}
