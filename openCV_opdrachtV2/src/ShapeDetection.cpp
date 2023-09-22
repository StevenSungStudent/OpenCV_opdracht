/*
 * ShapeDetection.cpp
 *
 *  Created on: Sep 21, 2023
 *      Author: steven
 */

#include "ShapeDetection.h"

float distance(cv::Point point1, cv::Point point2)
{
    // Calculating distance
    return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
}

template< class dataType>
float calculateCircleArea(dataType radius)
{
    return M_PI * pow(radius, 2);
}

std::vector<float> getSortedPolyList(std::vector<cv::Point> objectPoly){
	std::vector<float>polyList;

	for(unsigned long j = 0; j < objectPoly.size() - 1; ++j){
		float measuredPoly = distance(objectPoly.at(j), objectPoly.at(j + 1));
		polyList.push_back(measuredPoly);
	}
	polyList.push_back(distance(objectPoly.front(), objectPoly.back()));
	std::sort(polyList.begin(), polyList.end(), std::greater<float>());

	return polyList;
}

ShapeDetection::ShapeDetection() {
	// TODO Auto-generated constructor stub

}

ShapeDetection::~ShapeDetection() {
	// TODO Auto-generated destructor stub
}

void ShapeDetection::getContours(cv::Mat inputImg, cv::Mat outputImg, Colour requestedColour, std::string requestedShape){
	unsigned long long startTime = cv::getTickCount();
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat preparedImg = prepocessing(inputImg, requestedColour);

	cv::findContours(preparedImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for(unsigned long i = 0; i < contours.size(); ++i){
		float area = cv::contourArea(contours.at(i));
		std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
		std::vector<cv::Rect> boundRect(contoursPoly.size());




		if(area > MIN_AREA_SIZE){
			float perimeter = cv::arcLength(contours.at(i), true);
			cv::approxPolyDP(contours.at(i), contoursPoly.at(i), 0.02 *perimeter, true);
			boundRect.at(i) = cv::boundingRect(contoursPoly.at(i));

		    cv::Moments mu = cv::moments(contoursPoly.at(i), true);
		    cv::Point center;
		    center.x = mu.m10 / mu.m00;
		    center.y = mu.m01 / mu.m00;


			std::string objectType = determineShape(contoursPoly.at(i), area, boundRect.at(i));

			if(objectType == requestedShape || requestedShape == "All"){


				std::cout << "Area: " << area << std::endl;

			    cv::circle(outputImg, center, 2, cv::Scalar(0,0,255));
				cv::drawContours(outputImg, contoursPoly, i, cv::Scalar(255,0,255),2);
				cv::putText(outputImg, objectType, {boundRect.at(i).x, boundRect.at(i).y - 5 }, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,255,0),2);
			}
			unsigned long long dectectDuration = cv::getTickCount() - startTime;
			std::cout << "Duration: " << dectectDuration << std::endl;
		}
	}
}

void ShapeDetection::showSliders(){
	cv::namedWindow("sliders", (600, 200));
	cv::createTrackbar("hue min", "sliders", &hueMin, 179);
	cv::createTrackbar("hue max", "sliders", &hueMax, 179);
	cv::createTrackbar("saturation min", "sliders", &saturationMin, 255);
	cv::createTrackbar("saturation max", "sliders", &saturationMax, 255);
	cv::createTrackbar("value min", "sliders", &valueMin, 255);
	cv::createTrackbar("value max", "sliders", &valueMax, 255);
}


std::string ShapeDetection::determineShape(std::vector<cv::Point> objectPoly, float objectArea, cv::Rect boundRect){
	const double SQUARE_MARGIN = 30;
	const double CIRCLE_MARGIN = objectArea * 0.25;
	const double SEMI_CIRCLE_MARGIN = objectArea * 0.35;
	const double TRIANGLE_MARGIN = objectArea * 0.35;
	long unsigned int objectCorners = objectPoly.size();

	std::string objectType = "UNKOWN?";

	std::vector<float> sortedPolyList =  getSortedPolyList(objectPoly);

	if(objectCorners == 4){
		if(std::abs(distance(objectPoly.at(0), objectPoly.at(1)) - distance(objectPoly.at(1), objectPoly.at(2))) < SQUARE_MARGIN){
			objectType = "Square";
		}else{
			objectType = "Rectangle";
		}

	}else if(objectCorners > 4){
		float halfCircleRadius = 0;
		halfCircleRadius = sortedPolyList.at(0) / 2;
		halfCircleRadius = boundRect.width / 2;

		if(boundRect.height > halfCircleRadius * 2){
			halfCircleRadius = boundRect.height / 2;
		}else if(boundRect.width > halfCircleRadius * 2){
			halfCircleRadius = boundRect.width / 2;
		}

		if(std::abs(calculateCircleArea(halfCircleRadius) / 2 - objectArea) < SEMI_CIRCLE_MARGIN){
			objectType = "Semi-circle";
		}else if(std::abs(calculateCircleArea(boundRect.width / 2) - objectArea) < CIRCLE_MARGIN){
			objectType = "Circle";
		}else{
			objectType = "UNKOWN";
		}
	}

	//Heron's formula for calculating the area of a triangle with only the sides.
	if(objectPoly.size() >= 3){
		float a = sortedPolyList.at(0);
		float b = sortedPolyList.at(1);
		float c = sortedPolyList.at(2);
		float s = (a + b + c) / 2;

		float triangleArea = sqrt(s * (s - a) * (s - b) * (s - c));

		if(std::abs(triangleArea - objectArea) < TRIANGLE_MARGIN){
			objectType = "Triangle";
		}
	}


	return objectType;
}

cv::Mat ShapeDetection::prepocessing(cv::Mat inputImg, Colour objectColour){
	cv::Mat processedImg;
	cv::Mat workingImg;

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));

	switch(objectColour){


	case GREEN:
		lowerLimit = {57, 87, 24};
		upperLimit = {112, 244, 255};
		break;

	case PINK:
		lowerLimit = {101, 67, 31};
		upperLimit = {179, 255, 255};
		break;

	case YELLOW:
		lowerLimit = {16, 59, 45};
		upperLimit = {40, 255, 255};
		break;

	case ORANGE:
		lowerLimit = {18, 39, 0};
		upperLimit = {54, 181, 255};
		break;

	case CALIBRATE:
		lowerLimit = {hueMin, saturationMin, valueMin};
		upperLimit = {hueMax, saturationMax, valueMax};
		break;

	}

	cv::cvtColor(inputImg, workingImg, cv::COLOR_BGR2HSV);
	cv::inRange(workingImg, lowerLimit, upperLimit, imgMask);
    cv::GaussianBlur(imgMask, workingImg, cv::Size(3,3),4,0);
	cv::Canny(workingImg, workingImg, 25, 75);
	cv::dilate(workingImg, processedImg, kernel, cv::Point(-1,-1), 2);

	return processedImg;
}
