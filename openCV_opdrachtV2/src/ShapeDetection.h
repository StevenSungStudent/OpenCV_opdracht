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
#include <cmath>
#include <iostream>


class ShapeDetection {
public:
	ShapeDetection();
	virtual ~ShapeDetection();

	cv::Mat imgMask;

	/**
		 * @brief This function finds contours in the image.
	 *
	 * @param inputImg, the image to be 'read'
	 * @param outputImg, the output image after finding the contours.
	 * @param requestedColour, the colour of the object which it searches for..
	 * @param requestedShape, the shape of the object which it searches for.
	 */
	void getContours(cv::Mat inputImg, cv::Mat outputImg, Colour requestedColour, const std::string& requestedShape);

	/**
		 * @brief Shows the sliders for calibrating the colour limits.
	 *
	 */
	void showSliders();

private:
	const unsigned short MIN_AREA_SIZE = 2000;

	cv::Scalar lowerLimit;
	cv::Scalar upperLimit;

	int hueMin = 0;
	int saturationMin = 0;
	int valueMin = 0;

	int hueMax = 179;
	int saturationMax = 255;
	int valueMax = 255;

	/**
		 * @brief Does the pre-processing step.
	 *
	 * @param inputImg, input image.
	 * @param objectColour, the colour which it wil NOT filter out.
	 * @return The modified image.
	 */
	cv::Mat prepocessing(cv::Mat inputImg, Colour objectColour);

	/**
		 * @brief Determines the shape in an image.
	 *
	 * @param objectPoly, the polygons of the object.
	 * @param objectArea, the area of the object.
	 * @param boundRect, the bounding rectangle of the object.
	 * @return Ohe object type.
	 */
	std::string determineShape(std::vector<cv::Point> objectPoly, float objectArea, cv::Rect boundRect);

};

#endif /* SHAPEDETECTION_H_ */
