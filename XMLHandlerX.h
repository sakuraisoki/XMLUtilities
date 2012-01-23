/*
 * XMLHandlerX.h
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef XMLHANDLERX_HPP_
#define XMLHANDLERX_HPP_


#ifndef NSXML

#include "XMLNode.h"

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

#endif

#endif /* XMLHANDLERX_HPP_ */
