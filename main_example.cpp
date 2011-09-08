//============================================================================
// Name        : xmlparser.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "XMLLoader.hpp"
#include "Caster.hpp"
using namespace std;

int main(){
	XMLLoader xml = XMLLoader("config.xml");

	std::vector<XMLNode*> rev;
	rev = xml.getNodes("Configurations/Configuration");
//	rev = xml["Configurations/Configuration"];		//This format is also available.


	int max=rev.size();
	for(int i=0;i<max;i++){
		cout << "port = " << dec << rev.at(i)->getChild("port")->getValueAsUInt32() << endl;
		cout << "DLA = " << hex << (int)rev.at(i)->getChild("DestinationLogicalAddress")->getValueAsUInt8() << endl;
	}

	return 0;
}
