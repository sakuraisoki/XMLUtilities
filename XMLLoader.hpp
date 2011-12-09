/*
 * XMLLoader.hpp
 *
 *  Created on: 2011/06/12
 *      Author: sakurai
 */

#ifndef XMLLOADER_HPP_
#define XMLLOADER_HPP_

#include "Caster.hpp"

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

class XMLNode {
public:
	XMLNode(std::string name = "", std::string valuestr = "") {
		nodename = name;
		children.clear();
		NumberOfChildren = 0;
		NumberOfAttributes = 0;
		if (valuestr != "") {
			value = valuestr;
			iselement = true;
		} else {
			value = "";
			iselement = false;
		}
	}

	~XMLNode() {
		/*
		 std::map<std::string,XMLNode*>::iterator end = children.end();
		 for(std::map<std::string,XMLNode*>::iterator itr = children.begin();itr!=end;itr++){
		 delete itr->second;
		 }
		 */
	}

	void clearAll() {
		std::map<std::string, XMLNode*>::iterator end = children.end();
		for (std::map<std::string, XMLNode*>::iterator itr = children.begin(); itr != end; itr++) {
			delete itr->second;
		}
	}

private:
	std::string nodename;
	std::multimap<std::string, XMLNode*> children;
	int NumberOfChildren;

	std::map<std::string, std::string> attributes;
	int NumberOfAttributes;

	std::string value;
	bool iselement;
public:
	void dump(std::ostream& Stream, int depth = 0) {
		std::string tabs = "";
		for (int i = 0; i < depth; i++) {
			tabs = tabs + std::string("\t");
		}
		Stream << tabs << "<" << nodename;

		if (attributes.size() > 0) {
			std::map<std::string, std::string>::iterator end_attr = attributes.end();
			for (std::map<std::string, std::string>::iterator itr = attributes.begin(); itr != end_attr; itr++) {
				Stream << " " << itr->first << "=\"" << itr->second << "\"";
			}
		}
		//		Stream<<">"<<std::endl;
		Stream << ">";

		if (value != "") {
			//			Stream<<tabs<<"\t"<<value<<std::endl;
			Stream << value;
		}

		std::map<std::string, XMLNode*>::iterator itr, end_chld = children.end();
		if ((itr = children.begin()) != end_chld) {
			Stream << std::endl;
			for (; itr != end_chld; itr++) {
				itr->second->dump(Stream, depth + 1);
			}
			Stream << tabs << "</" << nodename << ">" << std::endl;
		} else {
			Stream << "</" << nodename << ">" << std::endl;
		}
	}

	bool isElement() {
		return iselement;
	}
	void setValue(std::string valuename) {
		value = valuename;
		iselement = true;
	}

	std::string getValue() {
		return value;
	}
	signed char getValueAsInt8() {
		return Caster::strToInt8(value);
	}
	unsigned char getValueAsUInt8() {
		return Caster::strToUInt8(value);
	}
	short getValueAsInt16() {
		return Caster::strToInt16(value);
	}
	unsigned short getValueAsUInt16() {
		return Caster::strToUInt16(value);
	}
	int getValueAsInt32() {
		return Caster::strToInt32(value);
	}
	unsigned int getValueAsUInt32() {
		return Caster::strToUInt32(value);
	}
	float getValueAsFloating32() {
		return Caster::strToFloating32(value);
	}
	double getValueAsFloating64() {
		return Caster::strToFloating64(value);
	}
	long double getValueAsFloating128() {
		return Caster::strToFloating128(value);
	}

	std::string getTagName() {
		return nodename;
	}
	int getNumberOfChildren() {
		return NumberOfChildren;
	}
	void addchild(std::string childname, XMLNode* childnode) {
		/*
		 std::map<std::string,XMLNode*>::iterator itr;
		 itr = children.find(childname);
		 if(itr!=children.end()){
		 children.erase(itr);
		 }
		 */
		children.insert(std::pair<std::string, XMLNode*>(childname, childnode));
		NumberOfChildren++;
	}

	XMLNode* getChild(std::string childname) {
		std::pair<std::multimap<std::string, XMLNode*>::iterator, std::multimap<std::string, XMLNode*>::iterator> range;
		XMLNode* ret = NULL;
		children.size();
		range = children.equal_range(childname);
		/*
		 if(range.first==range.second){
		 std::cout << "non" << std::endl;
		 return NULL;
		 }
		 else{
		 range = children.equal_range(childname);
		 }
		 */
		if (children.count(childname) != 0) {
			ret = (range.first)->second;
			return ret;
		} else {
			return NULL;
		}
	}

	std::vector<XMLNode*> getChildren(std::string childname) {
		std::pair<std::multimap<std::string, XMLNode*>::iterator, std::multimap<std::string, XMLNode*>::iterator> range;
		std::vector<XMLNode*> ret;
		ret.clear();

		for (range = children.equal_range(childname); range.first != range.second; range.first++) {
			ret.push_back((range.first)->second);
		}
		return ret;
	}

	void clearChild() {
		children.clear();
	}

	void addAttribute(std::string attrname, std::string attrvalue) {
		std::map<std::string, std::string>::iterator itr;
		itr = attributes.find(attrname);
		if (itr != attributes.end()) {
			attributes.erase(itr);
		}
		attributes.insert(std::pair<std::string, std::string>(attrname, attrvalue));
		NumberOfAttributes++;
	}

	std::string getAttribute(std::string attrname) {
		return attributes[attrname];
	}
};

struct XMLHandler: public xercesc::DefaultHandler {
	XMLHandler(XMLNode** top) :
		topnode(top) {
		XMLNode* dummytopnode = new XMLNode("DUMMYTOPNODE");
		nodestack.push(dummytopnode);
	}

	void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname,
			const xercesc::Attributes& attrs) {
		char* name = xercesc::XMLString::transcode(localname);
		XMLNode* nexttopnode = new XMLNode(std::string(name));
		XMLNode* currenttopnode = nodestack.top();
		currenttopnode->addchild(std::string(name), nexttopnode);
		nodestack.push(nexttopnode);

		int max_attributes = attrs.getLength();
		for (int i = 0; i < max_attributes; i++) {
			char* locname = xercesc::XMLString::transcode(attrs.getLocalName(i));
			char* value = xercesc::XMLString::transcode(attrs.getValue(i));
			nexttopnode->addAttribute(std::string(locname), std::string(value));
			xercesc::XMLString::release(&locname);
			xercesc::XMLString::release(&value);
		}
		buf = "";
		xercesc::XMLString::release(&name);
	}

	void characters(const XMLCh* const chars, const XMLSize_t length) {
		char* name = xercesc::XMLString::transcode(chars);
		std::string val = std::string(name);
		buf = buf + val;
		xercesc::XMLString::release(&name);
	}

	void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname) {
		XMLNode* currenttopnode = nodestack.top();
		buf.erase(0, buf.find_first_not_of(" \t\n", 0));
		buf = buf.substr(0, buf.find_last_not_of(" \t\n", -1) + 1);
		currenttopnode->setValue(buf);
		buf = "";
		if (nodestack.size() == 2) {
			*topnode = nodestack.top();
			nodestack.pop();
			XMLNode* dummytopnode = nodestack.top();
			delete dummytopnode;
		}
		nodestack.pop();
	}

private:
	std::stack<XMLNode*> nodestack;
	XMLNode** topnode;
	std::string buf;
};

class XMLLoader {
public:
	XMLLoader(const char* filename = NULL) {
		handler = NULL;
		parser = NULL;
		topnode = NULL;

		std::ifstream file(filename);
		if (!file.is_open()) {
			file.close();
			throw XMLLoaderException(FileNonExistence);
		}
		file.close();

		xercesc::XMLPlatformUtils::Initialize();
		parser = xercesc::XMLReaderFactory::createXMLReader();
		handler = new XMLHandler(&topnode);
		parser->setContentHandler(handler);
		parser->parse(filename);
	}

	~XMLLoader() {
		if (handler != NULL && parser != NULL) {
			delete handler;
			delete parser;
		}
	}
private:
	XMLNode* topnode;
	xercesc::SAX2XMLReader* parser;
	XMLHandler *handler;

public:
	XMLNode* getTopNode() {
		return topnode;
	}

	std::vector<XMLNode*> getTopNodes() {
		std::vector<XMLNode*> ret;
		ret.push_back(topnode);
		return ret;
	}

	void dumpAll(std::ostream& Stream) {
		topnode->dump(Stream, 0);
	}

	std::string getValueOf(std::string tag_hierarchy) {
		if (topnode == NULL)
			throw XMLLoaderException(NoSuchTag);

		std::string whole_tag = tag_hierarchy;
		if (whole_tag.substr(whole_tag.size(), 1) == "/")
			throw XMLLoaderException(NoSuchTag);

		std::string value = "";

		if (whole_tag.substr(0, 1) == "/")
			whole_tag.erase(1, 0);
		XMLNode* target = topnode;

		std::string current_tag;
		size_t slash_pos = 0;
		int depth = 0;
		while ((slash_pos = whole_tag.find("/", 0)) != std::string::npos) {
			current_tag = whole_tag.substr(0, slash_pos);
			if (depth != 0) {
				target = target->getChild(current_tag);
			} else if (current_tag != target->getTagName()) {
				throw XMLLoaderException(NoSuchTag);
			}
			whole_tag.erase(0, slash_pos + 1);
			depth++;
		}
		size_t dot_pos = 0;
		if ((dot_pos = whole_tag.find(".", 0)) != std::string::npos) {
			current_tag = whole_tag.substr(0, dot_pos);
			target = target->getChild(current_tag);
			whole_tag.erase(0, dot_pos + 1);
			value = target->getAttribute(whole_tag);
		} else {
			current_tag = whole_tag.substr(0, dot_pos);
			target = target->getChild(current_tag);
			value = target->getValue();
		}
		return value;
	}

	std::vector<XMLNode*> getNodes(std::string nodename) {
		if (topnode == NULL)
			throw XMLLoaderException(NoSuchTag);

		std::string whole_tag = nodename;
		if (whole_tag.substr(whole_tag.size(), 1) == "/")
			throw XMLLoaderException(NoSuchTag);
		if (whole_tag.find(".", 0) != std::string::npos)
			throw XMLLoaderException(NoSuchTag);

		std::vector<XMLNode*> ret;
		ret.clear();

		if (whole_tag.substr(0, 1) == "/")
			whole_tag.erase(1, 0);
		XMLNode dummy;
		dummy.addchild(topnode->getTagName(), topnode);
		XMLNode* target = &dummy;

		std::string current_tag;
		size_t slash_pos = 0;
		int depth = 0;
		while ((slash_pos = whole_tag.find("/", 0)) != std::string::npos) {
			current_tag = whole_tag.substr(0, slash_pos);
			target = target->getChild(current_tag);
			if (current_tag != target->getTagName()) {
				throw XMLLoaderException(NoSuchTag);
			}
			whole_tag.erase(0, slash_pos + 1);
			depth++;
		}
		return target->getChildren(whole_tag);
	}

public:

	std::vector<XMLNode*> operator[](std::string nodename) {
		return getNodes(nodename);
	}

public:
	class XMLLoaderException {
	public:
		XMLLoaderException(int type) {
		}
	};
	enum ExceptionType {
		FileNonExistence, NoSuchTag
	};
};

#endif /* XMLLOADER_HPP_ */
