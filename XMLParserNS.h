//
//  NSParser.h
//  MyXML
//
//  Created by Soki Sakurai on 12/01/21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "XMLNode.hpp"

@interface XMLParserNS: NSObject {
	XMLNode** topnode;
	std::string filename;
	int filesize;
	std::stack<XMLNode*> nodestack;
	std::string buf;
	
	NSXMLParser* parser;
}

- (void)setFileName: (char *)fname;
- (std::string)getFileName;
- (void)setTopNode: (XMLNode**)ptop; 
- (XMLNode*)getTopNode;
-(int) getFileSize;
- (void)parse;
@end 


#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>

@implementation XMLParserNS

- (void)setFileName: (char *)fname{
	filename = std::string(fname);
}

- (std::string)getFileName{
	return filename;
}


-(int) getFileSize{
	return filesize;
}

- (void)setTopNode: (XMLNode**)ptop{
	topnode = ptop;
}

- (XMLNode*)getTopNode{
	return *topnode;
}

- (void)parse{
	XMLNode* dummytopnode = new XMLNode("DUMMYTOPNODE");
	nodestack.push(dummytopnode);

	NSString* pathandfile = [[NSString alloc] initWithCString:filename.c_str() encoding:NSUTF8StringEncoding];
	NSData *data = [NSData dataWithContentsOfFile:pathandfile];
	parser = [[NSXMLParser alloc] initWithData:data];

	filesize = [data length];

	[parser setDelegate:self];
	[parser setShouldProcessNamespaces:NO];
	[parser setShouldReportNamespacePrefixes:NO];
//	[parser setShouldResolveExternalEntites:NO];
	[parser parse];
}

-(void)parserDidStartDocument:(NSXMLParser *)parser {
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict {
	char* name = (char*)[elementName UTF8String];
	XMLNode* nexttopnode = new XMLNode(std::string(name));
	XMLNode* currenttopnode = nodestack.top();
	currenttopnode->addchild(std::string(name),nexttopnode);
	nodestack.push(nexttopnode);

	int max_attributes = [attributeDict count];
	NSArray* keys = [attributeDict allKeys];
	for(int i = 0;i<max_attributes;i++){
		NSString* keystr = [keys objectAtIndex:i];
		NSString* valstr = [attributeDict objectForKey:keystr];
		char* keyname = (char *)[keystr UTF8String];
		char* valuename = (char *)[valstr UTF8String];

		nexttopnode->addAttribute(std::string(keyname),std::string(valuename));
	}
	buf = "";
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string {
	char* name = (char*)[string UTF8String];
	std::string val = std::string(name);
	buf = buf+val;
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName {
	XMLNode* currenttopnode = nodestack.top();
	buf.erase(0,buf.find_first_not_of(" \t\n",0));
	buf = buf.substr(0,buf.find_last_not_of(" \t\n",-1)+1);
	currenttopnode->setValue(buf);
	if(nodestack.size()==2){
		*topnode = nodestack.top();
		nodestack.pop();
		XMLNode* dummytopnode = nodestack.top();
		delete dummytopnode;
	}
	nodestack.pop();
	buf = "";
}

- (void)parserDidEndDocument:(NSXMLParser *)parser {
}

- (void)parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError {
	//	throw XMLLoader::XMLLoaderException(XMLLoader::XMLLoaderException::NoGoodFile);
}

@end
