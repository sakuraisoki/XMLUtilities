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

class SOO {
public:
	SOO(){
		g = 22;
	}
	int getInt(){return g;}
private:
	int g;
};

class ZOO {
public:
	ZOO(){
		sio = new SOO();
	}
	SOO* getSOO(){return sio;}
private:
	SOO* sio;
};


int main(int argc, const char *argv[]){
	if(argc<2){
		cout<<"Specify an XMLfile." << endl;
		return -1;
	}

	XMLLoader xml = XMLLoader(argv[1]);

	std::vector<XMLNode*> rev;
	rev = xml.getNodes("Configurations/Configuration");

	int max=rev.size();
	for(int i=0;i<max;i++){
		cout << "IP = " << rev.at(i)->getChild("IPaddress")->getValue() << endl;
	}

	/*
	cout << "IP address = " << xml.getValueOf("Configurations/IPaddress") << endl;
	cout << "IP version = " << xml.getValueOf("Configurations/IPaddress.version") << endl;

	cout << "*****Dump All*****" << endl;
	xml.dumpAll(cout);
	 */

	return 0;
}
