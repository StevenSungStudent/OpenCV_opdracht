/*
 * ShapeDetection.h
 *
 *  Created on: Sep 21, 2023
 *      Author: steven
 */

#ifndef SHAPEDETECTION_H_
#define SHAPEDETECTION_H_

#include "Colours.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <iostream>


class ShapeDetection {
public:
	ShapeDetection();
	virtual ~ShapeDetection();

	cv::Mat imgMask;

	//TODO: maybe remove
	cv::Scalar lowerLimit;
	cv::Scalar upperLimit;

	void getContours(cv::Mat inputImg, cv::Mat outputImg, Colour requestedColour, std::string requestedShape);
	void showSliders();


private:
	//TODO: make these percent.
	const unsigned short MIN_AREA_SIZE = 2000;

	//TODO: set in initialiser list.

	int hueMin = 0;
	int saturationMin = 0;
	int valueMin = 0;

	int hueMax = 179;
	int saturationMax = 255;
	int valueMax = 255;

	cv::Mat prepocessing(cv::Mat inputImg, Colour objectColour);
	std::string determineShape(std::vector<cv::Point> objectPoly, float objectArea, cv::Rect boundRect);

};

#endif /* SHAPEDETECTION_H_ */
