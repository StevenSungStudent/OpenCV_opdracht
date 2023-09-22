/*
 * Parser.h
 *
 *  Created on: Sep 22, 2023
 *      Author: steven
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include "Colours.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <utility>



class Parser {
public:
	Parser();
	virtual ~Parser();

	void parseFile(std::string fileName, std::string &shape, Colour &colour);
	void parseInput(std::string inputText, std::string &shape, Colour &colour);
};

#endif /* SRC_PARSER_H_ */
