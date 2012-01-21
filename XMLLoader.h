/*
 * XMLLoader.h
 *
 *  Created on: 2011/06/12
 *      Author: sakurai
 */

#ifndef XMLLOADER_H_
#define XMLLOADER_H_

//#define NSXML

#include "XMLNode.hpp"

#ifdef NSXML
#import "NSParser.h"
#else
#include "XMLParserX.h"
#include "XMLHandlerX.h"
#endif

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>


class XMLLoader {
public:
	XMLLoader(XMLNode** ptop, const char* fname);
	~XMLLoader();

private:
	XMLNode** ptopnode;
	std::string filename;

#ifdef NSXML
	NSParser *parser;
	std::string buf;
#else
	XMLParserX* parser;
	XMLHandlerX *handler;
#endif


public:
	void load();

	void setTopNode(XMLNode** ptop);
	XMLNode* getTopNode();

	std::string getFileName();

	std::vector<XMLNode*> getTopNodes();
	void dumpAll(std::ostream& Stream);
	std::string getValueOf(std::string tag_hierarchy);
	std::vector<XMLNode*> getNodes(std::string nodename);

public:

	std::vector<XMLNode*> operator[](std::string nodename);

public:
	class XMLLoaderException {
	public:
		XMLLoaderException(int type) {
		}
	};
	enum ExceptionType{
		NoSuchFile, NoSuchTag,NoGoodFile
	};
};


#endif /* XMLLOADER_H_ */
