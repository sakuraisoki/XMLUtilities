#ifndef NSXML

#include "XMLParserX.h"

XMLParserX::XMLParserX(std::string fname){
	// TODO Auto-generated constructor stub
	xercesc::XMLPlatformUtils::Initialize();
	parser = xercesc::XMLReaderFactory::createXMLReader();
	filename = fname;
	handler = NULL;
}

XMLParserX::~XMLParserX(){
	// TODO Auto-generated destructor stub
}

void XMLParserX::parse(){
	if(filename==""){
		throw XMLParserException(XMLParserException::NoSuchFile);
	}
	else if(handler==NULL){
		throw XMLParserException(XMLParserException::NoHandler);
	}
	parser->parse(filename.c_str());
}

void XMLParserX::setHandler(XMLHandlerX* phandler){
	handler = phandler;
	parser->setContentHandler(phandler);
}

void XMLParserX::setFileName(std::string fname){
	filename = fname;
}
#endif
