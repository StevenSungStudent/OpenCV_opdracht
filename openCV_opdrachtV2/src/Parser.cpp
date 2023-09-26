/*
 * Parser.cpp
 *
 *  Created on: Sep 22, 2023
 *      Author: steven
 */

#include "Parser.h"
#include <mutex>

Parser::Parser() {
}

Parser::~Parser() {
}


bool Parser::parseFile(const std::string& fileName, std::string &shape, Colour &colour){
	std::string line;
	std::ifstream myfile (fileName);
	if (myfile.is_open()){
		while ( std::getline (myfile,line) ){
			if(!line.empty()){
				if(line == "exit"){
					myfile.close();
					return (true);
				}
				if(line.at(0) != '#'){
					parseInput(line, shape, colour);
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
		}
		myfile.close();
	}

	return (false);
}

void Parser::parseInput(const std::string& inputText, std::string &shape, Colour &colour){
	std::mutex mtx;
	std::stringstream ssText(inputText);
	std::string shapeString;
	std::string colourString;

	Colour requestedColour = UNKNOWN;
	std::string requestedShape = "UNKNOWN";

	std::string token;
	std::getline(ssText, token, ' ');
	shapeString = token;

	if(token == "halve"){
		std::getline(ssText, token, ' ');
		shapeString.append(" " + token);
	}
	std::getline(ssText, colourString);

	if(colourString == "groen"){
		requestedColour = GREEN;
	}else if(colourString == "roze"){
		requestedColour = PINK;

	}else if(colourString == "geel"){
		requestedColour = YELLOW;

	}else if(colourString == "oranje"){
		requestedColour = ORANGE;

	}else{
		requestedColour = UNKNOWN;
		std::cout << "Unknown colour"  << std::endl;
	}

	if(shapeString == "cirkel"){
		requestedShape = "Circle";
	}else if(shapeString == "halve cirkel"){
		requestedShape = "Semi-circle";

	}else if(shapeString == "vierkant"){
		requestedShape = "Square";

	}else if(shapeString == "rechthoek"){
		requestedShape = "Rectangle";

	}else if(shapeString == "driehoek"){
		 requestedShape = "Triangle";

	}else{
		 requestedShape = "INVALID";
		std::cout << "Unknown shape: " << std::endl;
	}

//	if(requestedColour != UNKNOWN && requestedShape != "UNKNOWN"){
	std::lock_guard<std::mutex> lock(mtx);
	colour = requestedColour;
	shape = requestedShape;
//	}

}
