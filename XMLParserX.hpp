/*
 * XMLParser.h
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef XMLPARSERX_H_
#define XMLPARSERX_H_


#include "XMLNode.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>


#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>


struct XMLHandlerX: public xercesc::DefaultHandler {
	XMLHandlerX(XMLNode** top);

	void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname,
			const xercesc::Attributes& attrs);
	void characters(const XMLCh* const chars, const XMLSize_t length);
	void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);

private:
	std::stack<XMLNode*> nodestack;
	XMLNode** topnode;
	std::string buf;
};


XMLHandlerX::XMLHandlerX(XMLNode** top){
	topnode = top;
	XMLNode* dummytopnode = new XMLNode("DUMMYTOPNODE");
	nodestack.push(dummytopnode);
}

void XMLHandlerX::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname,
		const xercesc::Attributes& attrs){
	char* name = xercesc::XMLString::transcode(localname);
	XMLNode* nexttopnode = new XMLNode(std::string(name));
	XMLNode* currenttopnode = nodestack.top();
	currenttopnode->addchild(std::string(name),nexttopnode);
	nodestack.push(nexttopnode);

	int max_attributes = attrs.getLength();
	for(int i = 0;i<max_attributes;i++){
		char* locname = xercesc::XMLString::transcode(attrs.getLocalName(i));
		char* value = xercesc::XMLString::transcode(attrs.getValue(i));
		nexttopnode->addAttribute(std::string(locname),std::string(value));
		xercesc::XMLString::release(&locname);
		xercesc::XMLString::release(&value);
	}
	buf = "";
	xercesc::XMLString::release(&name);
}

void XMLHandlerX::characters(const XMLCh* const chars, const XMLSize_t length){
	char* name = xercesc::XMLString::transcode(chars);
	std::string val = std::string(name);
	buf = buf+val;
	xercesc::XMLString::release(&name);
}

void XMLHandlerX::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname){
	XMLNode* currenttopnode = nodestack.top();
	buf.erase(0,buf.find_first_not_of(" \t\n",0));
	buf = buf.substr(0,buf.find_last_not_of(" \t\n",-1)+1);
	currenttopnode->setValue(buf);
	buf = "";
	if(nodestack.size()==2){
		*topnode = nodestack.top();
		nodestack.pop();
		XMLNode* dummytopnode = nodestack.top();
		delete dummytopnode;
	}
	nodestack.pop();
}




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

#endif /* XMLPARSERX_H_ */
