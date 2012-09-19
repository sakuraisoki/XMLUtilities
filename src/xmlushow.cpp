//============================================================================
// Name        : xmlushow.cpp
// Author      : Soki Sakurai
// Version     :
// Copyright   : free for all
// Description : A sample program using XMLUtilities.
//============================================================================

//If using Cocoa (NeXTSTEP) library, uncomment the line below.
//#define XMLUTILITIES_NSXMLPARSER
#include "XMLLoader.hpp"

#include <readline/readline.h>
#include <readline/history.h>

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

class XMLShowManager{
private:
	bool initialized;
	std::string filename;
	static XMLNode* topnode;

	XMLShowManager(){
		filename = "";
		topnode = NULL;
		initialized = false;
	}

public:
	static XMLShowManager& getInstance(){
		static XMLShowManager manager;
		return manager;
	}

	void setXMLFileName(std::string fname){
		filename = fname;
		initialized = false;
	}

	void initialize(){
		XMLLoader loader(&topnode,filename.c_str());
		initialized = true;
	}

	void startSession(){
		if(!initialized){
			initialize();
		}

		char *line;
		rl_attempted_completion_function = on_complete;
		using_history();
		std::string fpath = std::getenv("HOME");
		fpath = fpath+"/.xmlushow_history";
		read_history(fpath.c_str());
		while(true){
			line = readline("Tag>> ");
			std::string com = std::string(line);
			if(com==".quit"||com==".q"||com==".exit"){
				break;
			}
			while(com[com.size()-1]==' ' || com[com.size()-1]=='\t'){
				com = com.substr(0,com.size()-1);
			}
			XMLNode* target = topnode->getDescendant(com);
			if(target!=NULL){
				target->dump(std::cout);
				add_history(line);
				free(line);
			}
			else{
				std::cout<<"No such tag."<<std::endl;
				add_history(line);
				free(line);
			}
		}
		write_history(fpath.c_str());
	}

	bool showValue(std::string tagname){
		if(!initialized){
			initialize();
		}

		XMLNode* target = topnode->getDescendant(tagname);
		if(target!=NULL){
			target->dump(std::cout);
			return true;
		}
		else{
			return false;
		}
	}

	static char* word_generator(const char* text, int state){
		static unsigned int index, wordlen;
		char* name;

		if(state==0){  // state = 0は初出の単語なので初期化
			wordlen = strlen(text);
			index = 0;
		}

		XMLNode* target = topnode;
		std::string whole(text);
		std::string pattern = whole;
		size_t finaldotpos = whole.find_last_of(".");
//		cout << "PAT=" << pattern << endl;
		if(finaldotpos!=std::string::npos){
			whole = whole.substr(0,finaldotpos+1);
		}
		else{
			whole = "";
		}
		size_t dotpos=0;
		while((dotpos = pattern.find_first_of(".",0))!=std::string::npos){
			std::string tag = pattern.substr(0,dotpos);
//			cout << "TAG=" << tag << " " << dotpos << endl;
			std::string atrname="";
			std::string atrval="";
			if(tag.find_first_of("[",0)!=std::string::npos){
				int state=0;
				size_t bracestart, equalstart, braceend;
				for(unsigned int i=0;i<tag.size();i++){
					if(state==0){
						if(tag[i]=='['){
							state = 1;
							bracestart = i;
						}
					}
					else if(state==1){
						if(tag[i]==':'){
							state = 2;
							equalstart = i;
						}
					}
					else if(state==2){
						if(tag[i]==']'){
							state = 2;
							braceend = i;
						}
					}
				}
				if(state!=2){
					return NULL;
				}
				atrname = tag.substr(bracestart+1,equalstart-bracestart-1);
				atrval = tag.substr(equalstart+1,braceend-equalstart-1);
				tag = tag.substr(0,bracestart);
				std::vector<XMLNode*> candidates = target->getChildren(tag);
				unsigned int j;
				for(j=0;j<candidates.size();j++){
					if(candidates[j]->getAttribute(atrname)==atrval){
						target = candidates[j];
						break;
					}
				}
				if(j>=candidates.size()){
					return NULL;
				}
			}
			else{
				std::vector<XMLNode*> candidates = target->getChildren(tag);
				if(candidates.size()!=0){
					target = candidates[0];
				}
				else{
					return NULL;
				}
			}
			pattern = pattern.substr(dotpos+1);
		}
//		if(finaldotpos!=std::string::npos){
//			pattern = whole.substr(finaldotpos+1);
////			std::string tagh = whole.substr(0,finaldotpos);
////			for(unsigned int i=0;i<tagh.size();i++){
////				if(tagh[i]==' '){
////					tagh[i] = '.';
////				}
////			}
////			target = target->getDescendant(tagh);
//			target = target->getDescendant(whole.substr(0,finaldotpos));
//			whole = whole.substr(0,finaldotpos+1);
//		}
//		else{
//			pattern = whole;
//			whole = "";
//		}
		unsigned int size = pattern.size();
		std::vector<XMLNode*> children = target->getAllChildren();
		while(index<children.size()){
			std::string tagname = children[index]->getTagName();
			index++;
			if(pattern==tagname.substr(0,size)){
				whole = whole+tagname;
				name = (char*)whole.c_str();
				return strdup(name);
			}
			else{
			}
		}
		return NULL;
	}

	static char** on_complete(const char* text, int start, int end){
		if(start==0){ //先頭の文字列だけ
			return rl_completion_matches(text,word_generator);
		}
		return NULL;
	}
};

XMLNode* XMLShowManager::topnode = NULL;


int main(int argc, char* argv[]){
	XMLShowManager& xmm = XMLShowManager::getInstance();

	if(argc < 1){
		cout << "Specify an XML file." << endl;
	}
	else if(argc < 3){
		xmm.setXMLFileName(string(argv[1]));
		xmm.initialize();
		xmm.startSession();
	}
	else if(argc >=2){
		xmm.setXMLFileName(string(argv[1]));
		if(!xmm.showValue(string(argv[2]))){
			cout << "No such tag." << endl;
		}
	}
	return 1;
}
