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


class XMLNodeException{
public:
	XMLNodeException(int type){
		std::cerr << "XMLNodeException::" << type << std::endl;
	}
};

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

private:
//	std::vector<std::string> splitString(std::string str, char delimiter=' ') const{
//		std::vector<std::string> strs;
//		uint32_t size = str.size();
//		int pos = 0;
//		for(uint32_t i=0;i<size;i++){
//			if(str[i]==delimiter){
//				if(pos<i){
//					strs.push_back(str.substr(pos,i-pos));
//					pos = i+1;
//				}
//			}
//		}
//		if(pos!=size){
//			strs.push_back(str.substr(pos));
//		}
//		return strs;
//	}
	std::string cutFirstQuery(std::string& str, const char delimiter = ' ') const {
		uint32_t size = str.size();
		std::string query;
		for (uint32_t i = 0; i < size; i++) {
			if (str[i] == delimiter) {
				query = str.substr(0, i);
				uint32_t j;
				for(j=i+1;j<size;j++){
					if (str[j] != delimiter) {
						str = str.substr(j);
						break;
					}
				}
				if(j==size){
					str = "";
				}
				return query;
			}
		}
		query = str;
		str = "";
		return query;
	}


public:
//	XMLNode& operator ()(const char* querychars) {
//		std::string query(querychars);
//		return (*this)(query);
//	}

	XMLNode& operator ()(const std::string query) {
		std::string querystr;
		querystr = query;
//		std::cerr << querystr << std::endl;
		std::string query1st = cutFirstQuery(querystr);
//		std::cerr << querystr << std::endl;
		uint32_t size = query1st.size();
		if(size==0){
			throw XMLNodeException(1);
		}
		else{
			size_t delimitpos = query1st.find_first_of("#.");
			if(delimitpos!=std::string::npos){
//				std::cerr << query1st << std::endl;
				if(query1st[delimitpos]=='#'){
					std::string tag = query1st.substr(0,delimitpos);
					std::string id = query1st.substr(delimitpos+1);
					std::vector<XMLNode*> tags;
					if(tag.size()>0){
						tags = this->getChildren(tag);
					}
					else{
						tags = this->getAllChildren();
					}
					for(uint32_t itag = 0;itag<tags.size();itag++){
						if(tags[itag]->getAttribute("id")==id){
							XMLNode& currenttarget = *(tags[itag]);
							if(querystr.size()>0){
								return currenttarget(querystr);
							}
							else{
								return currenttarget;
							}
						}
						else{
							std::cout << tags[itag]->getAttribute("id") << std::endl;
						}
					}
					throw XMLNodeException(2);
				}
				else if(query1st[delimitpos]=='.'){
					std::string tag = query1st.substr(0,delimitpos);
					std::string classname = query1st.substr(delimitpos+1);
					std::vector<XMLNode*> tags;
					if(tag.size()>0){
						tags = this->getChildren(tag);
					}
					else{
						tags = this->getAllChildren();
					}
					for(uint32_t itag = 0;itag<tags.size();itag++){
						if(tags[itag]->getAttribute("class")==classname){
							XMLNode& currenttarget = *(tags[itag]);
							if(querystr.size()>0){
								return currenttarget(querystr);
							}
							else{
								return currenttarget;
							}
						}
					}
					throw XMLNodeException(3);
				}
			}
			else{
//				std::cerr << query1st << std::endl;
				std::vector<XMLNode*> tags = this->getChildren(query1st);
				if(tags.size()==1){
					XMLNode& currenttarget = *(tags[0]);
					if(querystr.size()>0){
						return currenttarget(querystr);
					}
					else{
						return currenttarget;
					}
				}
				else if(tags.size()==0){
					throw XMLNodeException(4);
				}
				else{
					throw XMLNodeException(5);
				}
			}
		}
        throw XMLNodeException(-1);
	}

	XMLNode& attr(std::string attrName, std::string attrValue) {
		this->addAttribute(attrName, attrValue);
		return *this;
	}
	std::string attr(std::string attrName) {
		return this->getAttribute(attrName);
	}

	XMLNode& addClass(std::string className) {
		std::string classnames = attributes["class"];
		if(classnames==""){
			attributes["class"] = className;
			NumberOfAttributes++;
		}
		else{
			attributes["class"] = classnames+std::string(" ")+className;
		}
		return *this;
	}

	XMLNode& val(const std::string value) {
		this->setValue(value);
		return *this;
	}
	std::string val() {
		return this->getValue();
	}

	XMLNode& append(XMLNode& node) {
		XMLNode* newchild = node.clone();
		this->addchild(newchild->getTagName(), newchild);
		return *this;
	}

public:
	void destroy() {
		clearAll();
		delete this;
	}

	void clearAll() {
		std::map<std::string, XMLNode*>::iterator end = children.end();
		for (std::map<std::string, XMLNode*>::iterator itr = children.begin(); itr != end; itr++) {
			delete itr->second;
		}
	}

	XMLNode* clone() {
		XMLNode* cloned = new XMLNode(nodename, value);

		std::map<std::string, std::string>::iterator itrattrend = attributes.end();
		for (std::map<std::string, std::string>::iterator itrattr = attributes.begin(); itrattr != itrattrend;
				itrattr++) {
			cloned->addAttribute(itrattr->first, itrattr->second);
		}

		std::map<std::string, XMLNode*>::iterator end = children.end();
		for (std::map<std::string, XMLNode*>::iterator itr = children.begin(); itr != end; itr++) {
			XMLNode* child = (XMLNode*) itr->second;
			XMLNode* clonedchild = child->clone();
			cloned->addchild(itr->first, clonedchild);
		}

		return cloned;
	}

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
	void setValue(const std::string valuename) {
		value = valuename;
		iselement = true;
	}

	std::string getValue() const {
		return value;
	}
	signed char getValueAsInt8() const {
		return Caster::strToInt8(value);
	}
	unsigned char getValueAsUInt8() const {
		return Caster::strToUInt8(value);
	}
	short getValueAsInt16() const {
		return Caster::strToInt16(value);
	}
	unsigned short getValueAsUInt16() const {
		return Caster::strToUInt16(value);
	}
	int getValueAsInt32() const {
		return Caster::strToInt32(value);
	}
	unsigned int getValueAsUInt32() const {
		return Caster::strToUInt32(value);
	}
	float getValueAsFloating32() const {
		return Caster::strToFloating32(value);
	}
	double getValueAsFloating64() const {
		return Caster::strToFloating64(value);
	}
	long double getValueAsFloating128() const {
		return Caster::strToFloating128(value);
	}

	std::string getTagName() const {
		return nodename;
	}
	int getNumberOfChildren() const {
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

	XMLNode* getDescendant(std::string tag_hierarchy) {
		std::string whole_tag = tag_hierarchy;
		if (whole_tag.substr(whole_tag.size(), 1) == ".") {
			return NULL;
		}

		std::string value = "";

		if (whole_tag.substr(0, 1) == ".") {
			whole_tag.erase(1, 0);
		}
		XMLNode* target = this;

		std::string current_tag;
		size_t dot_pos = 0;
		int depth = 0;
//		while((dot_pos = whole_tag.find(".",0))!=std::string::npos){
		while (true) {
			dot_pos = whole_tag.find(".", 0);
			if (dot_pos != std::string::npos) {
				current_tag = whole_tag.substr(0, dot_pos);
			} else {
				current_tag = whole_tag;
			}
			if (target == NULL) {
				return NULL;
			} else {
				if (current_tag.find_first_of("[", 0) != std::string::npos) {
					int state = 0;
					size_t bracestart, equalstart, braceend;
					for (unsigned int i = 0; i < current_tag.size(); i++) {
						if (state == 0) {
							if (current_tag[i] == '[') {
								state = 1;
								bracestart = i;
							}
						} else if (state == 1) {
							if (current_tag[i] == ':') {
								state = 2;
								equalstart = i;
							}
						} else if (state == 2) {
							if (current_tag[i] == ']') {
								state = 3;
								braceend = i;
							}
						}
					}
					if (state != 3) {
						return NULL;
					}
					std::string atrname = current_tag.substr(bracestart + 1, equalstart - bracestart - 1);
					std::string atrval = current_tag.substr(equalstart + 1, braceend - equalstart - 1);
					current_tag = current_tag.substr(0, bracestart);
					std::vector<XMLNode*> candidates = target->getChildren(current_tag);
					unsigned int j;
					for (j = 0; j < candidates.size(); j++) {
						if (candidates[j]->getAttribute(atrname) == atrval) {
							target = candidates[j];
							break;
						}
					}
					if (j >= candidates.size()) {
						return NULL;
					}
				} else if (current_tag.find_first_of("{", 0) != std::string::npos) {
					int state = 0;
					size_t bracestart, equalstart, braceend;
					for (unsigned int i = 0; i < current_tag.size(); i++) {
						if (state == 0) {
							if (current_tag[i] == '{') {
								state = 1;
								bracestart = i;
							}
						} else if (state == 1) {
							if (current_tag[i] == ':') {
								state = 2;
								equalstart = i;
							}
						} else if (state == 2) {
							if (current_tag[i] == '}') {
								state = 3;
								braceend = i;
							}
						}
					}
					if (state != 3) {
						return NULL;
					}
					std::string atrname = current_tag.substr(bracestart + 1, equalstart - bracestart - 1);
					std::string atrval = current_tag.substr(equalstart + 1, braceend - equalstart - 1);
					current_tag = current_tag.substr(0, bracestart);
					std::vector<XMLNode*> candidates = target->getChildren(current_tag);
					unsigned int j;
					for (j = 0; j < candidates.size(); j++) {
						if (candidates[j]->getAttribute(atrname) == atrval) {
							target = candidates[j];
							break;
						}
					}
					if (j >= candidates.size()) {
						return NULL;
					}
				} else {
					target = target->getChild(current_tag);
				}
			}
			if (dot_pos == std::string::npos) {
				break;
			}
			whole_tag.erase(0, dot_pos + 1);
			depth++;
		}
//		current_tag = whole_tag;
//		target = target->getChild(current_tag);
		return target;

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

	std::vector<XMLNode*> getAllChildren() {
		std::vector<XMLNode*> ret;
		ret.clear();

		std::multimap<std::string, XMLNode*>::iterator it = children.begin();
		while (it != children.end()) {
			ret.push_back(it->second);
			it++;
		}

		return ret;
	}

	void clearChild() {
		children.clear();
	}

	void addAttribute(const std::string attrname, const std::string attrvalue) {
		std::map<std::string, std::string>::iterator itr;
		itr = attributes.find(attrname);
		if (itr != attributes.end()) {
			attributes.erase(itr);
		}
		attributes.insert(std::pair<std::string, std::string>(attrname, attrvalue));
		NumberOfAttributes++;
	}

	std::string getAttribute(std::string attrname) {
		std::map<std::string, std::string>::iterator itr;
		itr = attributes.find(attrname);
		if (itr != attributes.end()) {
			return attributes[attrname];
		} else {
			return "";
		}
	}

};

//class XMLNodes : public std::vector<XMLNode*>{
//public:
//
//};

#endif /* XMLNODE_HPP_ */
