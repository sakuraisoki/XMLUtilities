//============================================================================
// Name        : main_example.cpp
// Author      : Soki Sakurai
// Version     :
// Copyright   : free for all
// Description : A sample program using XMLUtilities.
//============================================================================

//If using Cocoa (NeXTSTEP) library, uncomment the line below.
//#define XMLUTILITIES_NSXMLPARSER
#include "XMLLoader.hpp"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]){
	string filename = string(argv[1]);

	XMLNode* node;
	XMLLoader loader(&node,filename.c_str());

	node->dump(cout);		//Dump as XML format.
	cout<<endl;			//Here possibly the same as input file.

	//Below, the value of "port" tag in each "Configuration" tag is set to "10032" (see config.xml).
	vector<XMLNode*> configurationNodes = node->getChildren("Configuration");
	for(unsigned int i = 0;i<configurationNodes.size();i++){
		XMLNode* configurationNode = configurationNodes[i];
		XMLNode* portNode = configurationNode->getChild("port");
		portNode->setValue("10032");
	}

	cout<<"Revised one is..."<<endl;
	node->dump(cout);

	return 0;
}
