/*
 * XMLNode.h
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef XMLNODE_HPP_
#define XMLNODE_HPP_

#include "Caster.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>

class XMLNode {
private:
	std::string nodename;
	std::multimap<std::string, XMLNode*> children;
	int NumberOfChildren;

	std::map<std::string, std::string> attributes;
	int NumberOfAttributes;

	std::string value;
	bool iselement;

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

	void dump(std::ostream& Stream, int depth=0) {
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

#endif /* XMLNODE_HPP_ */
