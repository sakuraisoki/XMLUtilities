/*
 * XMLHandler.cpp
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef NSXML

#include "XMLNode.hpp"
#include "XMLHandlerX.h"


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

#endif /* XMLHANDLERX_CPP_ */
