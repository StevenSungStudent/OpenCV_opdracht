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
	/**
		 * @brief Reads and parses a given file.
	 *
	 * @param fileName, the name of the file it will try to read.
	 * @param shape, a reference to the shape variable which it will change.
	 * @param colour, a referecne to the colour variable that it will change.
	 */
	static bool parseFile(const std::string& fileName, std::string &shape, Colour &colour);

	/**
		 * @brief Parses a line of text.
	 *
	 * @param inputText, the input string.
	 * @param shape, a reference to the shape variable which it will change.
	 * @param colour, a referecne to the colour variable that it will change.
	 */
	static void parseInput(const std::string& inputText, std::string &shape, Colour &colour);
};

#endif /* SRC_PARSER_H_ */
