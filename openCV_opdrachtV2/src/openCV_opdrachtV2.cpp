#include "ShapeDetection.h"
#include "Parser.h"
#include "Colours.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <iostream>
#include <mutex>

std::mutex mtx;
Colour requestedColour = CALIBRATE;
std::string requestedShape = "All";
std::string fileName;
bool batchMode = false;
bool running = true;

void setRunning(bool value){
	 std::lock_guard<std::mutex> lock(mtx);
	running = value;
}

bool isRunning(){
	 std::lock_guard<std::mutex> lock(mtx);
	return (running);
}

Colour getRequestedColour() {
    std::lock_guard<std::mutex> lock(mtx);
    return (requestedColour);
}

std::string getRequestedShape() {
    std::lock_guard<std::mutex> lock(mtx);
    return (requestedShape);
}

bool isBatchMode() {
    std::lock_guard<std::mutex> lock(mtx);
    return (batchMode);
}

void setBatchMode(bool batch) {
    std::lock_guard<std::mutex> lock(mtx);
    batchMode = batch;
}

std::string getFileName() {
    std::lock_guard<std::mutex> lock(mtx);
    return (fileName);
}

void setFileName(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);
    fileName = name;
}

void handleInput() {
	Parser parser;
    std::string input;
    while (true) {
    	if(isBatchMode()){
    		 if(parser.parseFile(getFileName(), requestedShape, requestedColour)){
    			 setRunning(false);
    			 return;
    		 }
    	}else{
			std::cout << "Enter shape and colour: ";
			std::getline(std::cin, input);

			parser.parseInput(input, requestedShape, requestedColour);
    	}
    }
}

int main(int argc, char **argv) {
	ShapeDetection shapeDetection;
	cv::Mat rawImg;

	if(argc > 1){
		setFileName(argv[1]);
		setBatchMode(true);

	}

	cv::VideoCapture cap(2, cv::CAP_V4L2);

	if (!cap.isOpened()) {
		std::cout << "Error: Cannot open camera." << std::endl;
		return (1);
	}
	std::thread InputThread(handleInput);

	shapeDetection.showSliders();

	while(isRunning()){
		cap.read(rawImg);
		cv::Mat outputImg = rawImg;

		shapeDetection.getContours(rawImg, outputImg, getRequestedColour(), getRequestedShape());

		cv::imshow("Mask image", shapeDetection.imgMask);
		cv::imshow("Output image",outputImg);
		cv::waitKey(1);
	}

	InputThread.join();

    cap.release();
    cv::destroyAllWindows();

	return (0);
}
