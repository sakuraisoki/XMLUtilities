//============================================================================
// Name        : xmlparser.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "XMLLoader.hpp"
using namespace std;

int main(int argc, const char *argv[]){
	if(argc<2){
		cout<<"Specify an XMLfile." << endl;
		return -1;
	}
	XMLLoader xml = XMLLoader(argv[1]);
	cout << "IP address = " << xml.getValueOf("Configurations/IPaddress") << endl;
	cout << "IP version = " << xml.getValueOf("Configurations/IPaddress.version") << endl;

	cout << "*****Dump All*****" << endl;

	xml.dumpAll(cout);

	return 0;
}
