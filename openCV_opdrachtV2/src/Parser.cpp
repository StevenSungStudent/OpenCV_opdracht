/*
 * Parser.cpp
 *
 *  Created on: Sep 22, 2023
 *      Author: steven
 */

#include "Parser.h"

Parser::Parser() {
	// TODO Auto-generated constructor stub

}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

void Parser::parseFile(std::string fileName, std::string &shape, Colour &colour){
	std::string line;
	std::ifstream myfile (fileName);
	if (myfile.is_open()){
		while ( std::getline (myfile,line) ){
			std::cout << line << std::endl;
			if(!line.empty()){
				if(line == "exit"){
					exit(0);
				}
				if(line.at(0) != '#'){
					parseInput(line, shape, colour);
					std::this_thread::sleep_for(std::chrono::seconds(2));
				}
			}
		}
		myfile.close();
	}

}

void Parser::parseInput(std::string inputText, std::string &shape, Colour &colour){
	std::stringstream ssText(inputText);

	std::pair<std::string, Colour> shapeColour;
	std::string shapeString;
	std::string colourString;

	Colour requestedColour;
	std::string requestedShape;


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
		std::cout << "Unknown shape" << std::endl;
	}

	shape = requestedShape;
	colour = requestedColour;
}

