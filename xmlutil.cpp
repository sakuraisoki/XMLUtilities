//============================================================================
// Name        : xmlutil.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "XMLLoader.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	XMLNode* node;
	XMLLoader loader(&node, argv[1]);
//	cout << node->getTagName() << endl;
	node->dump(cout);
	return 0;
}
