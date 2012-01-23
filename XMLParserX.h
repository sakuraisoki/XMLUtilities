/*
 * XMLParser.h
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef XMLPARSERX_H_
#define XMLPARSERX_H_

#ifndef NSXML


#include "XMLNode.h"
#include "XMLHandlerX.h"

#include <iostream>
#include <fstream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

class XMLParserX{
public:
	XMLParserX(std::string fname="");
	virtual ~XMLParserX();

private:
	xercesc::SAX2XMLReader* parser;
	XMLHandlerX *handler;
	std::string filename;

public:
	void parse();
	void setHandler(XMLHandlerX* phandler);
	void setFileName(std::string fname);

public:
	class XMLParserException{
	public:
		XMLParserException(int type){};
		~XMLParserException(){}
		enum {
			NoSuchFile,
			NoHandler
		};
	};
};

#endif

#endif /* XMLPARSERX_H_ */
