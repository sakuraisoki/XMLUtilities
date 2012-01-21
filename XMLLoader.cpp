/*
 * XMLLoader.hpp
 *
 *  Created on: 2011/06/12
 *      Author: sakurai
 */

#ifndef NSXML

#include "XMLLoader.h"

#include "XMLNode.hpp"

XMLLoader::XMLLoader(XMLNode** ptop, const char* fname){
	handler = NULL;
	parser = NULL;
	ptopnode = ptop;

	std::ifstream file(fname);
	if(!file.is_open()){
		file.close();
		throw XMLLoaderException(NoSuchFile);
	}
	file.close();

	filename = std::string(fname);

	load();
}

XMLLoader::~XMLLoader(){
	if(handler!=NULL&&parser!=NULL){
		delete handler;
		delete parser;
	}
}

void XMLLoader::load(){
	if(filename=="")
		*ptopnode = NULL;

	parser = new XMLParserX(std::string(filename));
	handler = new XMLHandlerX(ptopnode);
	parser->setHandler(handler);
	parser->setFileName(filename);
	parser->parse();
	if(*ptopnode==NULL){
		throw XMLLoaderException(NoGoodFile);
	}
}

void XMLLoader::setTopNode(XMLNode** ptop){
	ptopnode = ptop;
}

XMLNode* XMLLoader::getTopNode(){
	return *ptopnode;
}

std::vector<XMLNode*> XMLLoader::getTopNodes(){
	std::vector<XMLNode*> ret;
	ret.push_back(*ptopnode);
	return ret;
}

void XMLLoader::dumpAll(std::ostream& Stream){
	(*ptopnode)->dump(Stream,0);
}

std::string XMLLoader::getValueOf(std::string tag_hierarchy){
	if(*ptopnode==NULL)
		throw XMLLoaderException(NoSuchTag);

	std::string whole_tag = tag_hierarchy;
	if(whole_tag.substr(whole_tag.size(),1)=="/")
		throw XMLLoaderException(NoSuchTag);

	std::string value = "";

	if(whole_tag.substr(0,1)=="/")
		whole_tag.erase(1,0);
	XMLNode* target = *ptopnode;

	std::string current_tag;
	size_t slash_pos = 0;
	int depth = 0;
	while((slash_pos = whole_tag.find("/",0))!=std::string::npos){
		current_tag = whole_tag.substr(0,slash_pos);
		if(depth!=0){
			target = target->getChild(current_tag);
		}
		else if(current_tag!=target->getTagName()){
			throw XMLLoaderException(NoSuchTag);
		}
		whole_tag.erase(0,slash_pos+1);
		depth++;
	}
	size_t dot_pos = 0;
	if((dot_pos = whole_tag.find(".",0))!=std::string::npos){
		current_tag = whole_tag.substr(0,dot_pos);
		target = target->getChild(current_tag);
		whole_tag.erase(0,dot_pos+1);
		value = target->getAttribute(whole_tag);
	}
	else{
		current_tag = whole_tag.substr(0,dot_pos);
		target = target->getChild(current_tag);
		value = target->getValue();
	}
	return value;
}

std::vector<XMLNode*> XMLLoader::getNodes(std::string nodename){
	if(*ptopnode==NULL)
		throw XMLLoaderException(NoSuchTag);

	std::string whole_tag = nodename;
	if(whole_tag.substr(whole_tag.size(),1)=="/")
		throw XMLLoaderException(NoSuchTag);
	if(whole_tag.find(".",0)!=std::string::npos)
		throw XMLLoaderException(NoSuchTag);

	std::vector<XMLNode*> ret;
	ret.clear();

	if(whole_tag.substr(0,1)=="/")
		whole_tag.erase(1,0);
	XMLNode dummy;
	dummy.addchild((*ptopnode)->getTagName(),*ptopnode);
	XMLNode* target = &dummy;

	std::string current_tag;
	size_t slash_pos = 0;
	int depth = 0;
	while((slash_pos = whole_tag.find("/",0))!=std::string::npos){
		current_tag = whole_tag.substr(0,slash_pos);
		target = target->getChild(current_tag);
		if(current_tag!=target->getTagName()){
			throw XMLLoaderException(NoSuchTag);
		}
		whole_tag.erase(0,slash_pos+1);
		depth++;
	}
	return target->getChildren(whole_tag);
}

std::vector<XMLNode*> XMLLoader::operator[](std::string nodename){
	return getNodes(nodename);
}

#endif /* XMLLOADER_CPP_ */
