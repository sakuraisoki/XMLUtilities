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
- (void)parseString: (NSString*)xmlstring;
@end
