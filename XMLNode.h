/*
 * XMLNode.h
 *
 *  Created on: 2012/01/19
 *      Author: sakurai
 */

#ifndef XMLNODE_HPP_
#define XMLNODE_HPP_

#include "Caster.h"

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>


class XMLNode {
public:
	XMLNode(std::string name = "", std::string valuestr = "") ;

	~XMLNode();

	void clearAll();

private:
	std::string nodename;
	std::multimap<std::string, XMLNode*> children;
	int NumberOfChildren;

	std::map<std::string, std::string> attributes;
	int NumberOfAttributes;

	std::string value;
	bool iselement;

public:
	void dump(std::ostream& Stream, int depth = 0);

	bool isElement();

	void setValue(std::string valuename);
	std::string getValue();

	signed char getValueAsInt8();
	unsigned char getValueAsUInt8();
	short getValueAsInt16();
	unsigned short getValueAsUInt16();
	int getValueAsInt32();
	unsigned int getValueAsUInt32();
	float getValueAsFloating32();
	double getValueAsFloating64();
	long double getValueAsFloating128();

	std::string getTagName();

	int getNumberOfChildren();

	void addchild(std::string childname, XMLNode* childnode);

	XMLNode* getChild(std::string childname);
	std::vector<XMLNode*> getChildren(std::string childname);
	void clearChild();

	void addAttribute(std::string attrname, std::string attrvalue);

	std::string getAttribute(std::string attrname);
};

#endif /* XMLNODE_HPP_ */
