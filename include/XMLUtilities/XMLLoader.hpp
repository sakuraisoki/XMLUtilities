/*
 * XMLLoader.h
 *
 *  Created on: 2011/06/12
 *      Author: sakurai
 */

#ifndef XMLLOADER_H_
#define XMLLOADER_H_

#ifndef XMLUTILITIES_NSXMLPARSER
#define XMLUTILITIES_XERCERS
#endif

#include "XMLNode.hpp"

#ifdef XMLUTILITIES_NSXMLPARSER
#import "XMLParserNS.h"
#endif

#ifdef XMLUTILITIES_XERCERS
#include "XMLParserX.hpp"
#endif

#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>

class XMLLoader {
private:
	XMLNode** ptopnode;
	std::string filename;

#ifdef XMLUTILITIES_XERCERS
	XMLParserX* parser;
	XMLHandlerX *handler;
#endif

#ifdef XMLUTILITIES_NSXMLPARSER
	XMLParserNS *parser;
	std::string buf;
#endif

public:
	XMLLoader(XMLNode** ptop=NULL, const char* fname=NULL) {
	#ifdef XMLUTILITIES_XERCERS
		handler = NULL;
		parser = NULL;
	#endif
#ifdef XMLUTILITIES_NSXMLPARSER
		parser=NULL;
#endif
		if(ptop!=NULL){
			ptopnode = ptop;
		}
		else{
			return;
		}

		if(fname==NULL){
			filename = "";
			return;
		}
		std::ifstream file(fname);
		if (!file.is_open()) {
			file.close();
			throw XMLLoaderException(NoSuchFile);
		}
		file.close();

		filename = std::string(fname);

		load();
	}

	~XMLLoader() {
	#ifdef XMLUTILITIES_XERCERS
		if (handler != NULL && parser != NULL) {
			delete handler;
			delete parser;
		}
	#endif
	}

	void load() {
		if (filename == "")
			*ptopnode = NULL;

	#ifdef XMLUTILITIES_XERCERS
		if(parser==NULL){
			parser = new XMLParserX(std::string(filename));
		}
		if(handler==NULL){
			handler = new XMLHandlerX(ptopnode);
		}
		parser->setHandler(handler);
		parser->setFileName(filename);
		parser->parse();
		if (*ptopnode == NULL) {
			throw XMLLoaderException(NoGoodFile);
		}
	#endif

	#ifdef XMLUTILITIES_NSXMLPARSER
		parser = [[XMLParserNS alloc] init];
		[parser setFileName:(char*)filename.c_str()];
		[parser setTopNode:ptopnode];

		[parser parse];
	#endif

	}

	void setTopNode(XMLNode** ptop) {
		ptopnode = ptop;
	}

	XMLNode* getTopNode() {
		return *ptopnode;
	}

	std::vector<XMLNode*> getTopNodes() {
		std::vector<XMLNode*> ret;
		ret.push_back(*ptopnode);
		return ret;
	}

	void dumpAll(std::ostream& Stream) {
		(*ptopnode)->dump(Stream, 0);
	}

#ifdef XMLUTILITIES_XERCERS
	void loadFromString(XMLNode** ptop, std::string xmlstring){
		ptopnode = ptop;
		if(parser==NULL){
			parser = new XMLParserX();
		}
		if(handler==NULL){
			handler = new XMLHandlerX(ptopnode);
		}
		else{
			handler->setTopNodePointer(ptopnode);
		}
		parser->setHandler(handler);
		parser->parseString(xmlstring);
		if (*ptopnode == NULL) {
			throw XMLLoaderException(NoGoodFile);
		}
	}
    
#else
	void loadFromString(XMLNode** ptop, std::string xmlstring){

        ptopnode = ptop;
		if(parser==NULL){
            parser = [[XMLParserNS alloc] init];
		}
		[parser setTopNode:ptopnode];
        NSString* buf = [[NSString alloc] initWithCString:xmlstring.c_str() encoding:NSASCIIStringEncoding];
		[parser parseString:buf];
		if (*ptopnode == NULL) {
			throw XMLLoaderException(NoGoodFile);
		}
	}
#endif
    
    
	void loadFromFile(XMLNode** ptop, std::string fname){
		ptopnode = ptop;
		filename = std::string(fname);
		load();
	}

	std::string getValueOf(std::string tag_hierarchy) {
		if (*ptopnode == NULL)
			throw XMLLoaderException(NoSuchTag);

		std::string whole_tag = tag_hierarchy;
		if (whole_tag.substr(whole_tag.size(), 1) == "/")
			throw XMLLoaderException(NoSuchTag);

		std::string value = "";

		if (whole_tag.substr(0, 1) == "/")
			whole_tag.erase(1, 0);
		XMLNode* target = *ptopnode;

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
		if (*ptopnode == NULL)
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
		dummy.addchild((*ptopnode)->getTagName(), *ptopnode);
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
		NoSuchFile, NoSuchTag, NoGoodFile
	};
};
#endif /* XMLLOADER_H_ */
