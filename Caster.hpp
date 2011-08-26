/*
 * Caster.hpp
 *
 *  Created on: 2011/08/24
 *      Author: sakurai
 */

#ifndef CASTER_HPP_
#define CASTER_HPP_

#include <sstream>

class Caster{
public:
	static unsigned long long HexaexpToUInteger64(std::string str){
		//suppose "0x" is already cut (str=12FF, for example.)
		int size = str.size();
		if(size==0)
			return 0;
		char c;
		unsigned long long ret = 0;
		unsigned long long order = 1;
		for(int i = size-1;i>=0;i--){
			c = str[i];
			if(c<58){
				if(47<c){
					ret += (c-48)*order;
				}
				else{
					return 0;
				}
			}
			else if(c<71){
				if(64<c){
					ret += (c-55)*order;
				}
				else{
					return 0;
				}
			}
			else if(c<103){
				if(96<c){
					ret += (c-87)*order;
				}
				else{
					return 0;
				}
			}
			order = order<<4;
		}
		return ret;
	}

	static unsigned long long BinaryToUInteger64(std::string str){
		//suppose "b" is already cut (str=1011, for example.)
		int size = str.size();
		if(size==0)
			return 0;
		char c;
		unsigned long long ret = 0;
		unsigned long long order = 1;
		for(int i = size-1;i>=0;i--){
			c = str[i];
			if(c=='0' || c=='1'){
				ret += (c-48)*order;
			}
			else{
				return 0;
			}
			order = order<<1;
		}
		return ret;
	}

	static signed char strToInt8(std::string str){
		if(str.find_first_of("eE",0)!=std::string::npos){
			return (signed char)atof(str.c_str());
		}
		std::istringstream stream(str);
		signed char ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static unsigned char strToUInt8(std::string str){
		int size = str.size();
		if(size==0){
			return 0;
		}
		else if(size > 1){
			if(str[0]=='0' && str[1]=='x'){
				return (unsigned char)HexaexpToUInteger64(str.substr(2));
			}
			else if(str[size-1]=='h'){
				return (unsigned char)HexaexpToUInteger64(str.substr(0,size-1));
			}
			else if(str[size-1]=='b'){
				return (unsigned char)BinaryToUInteger64(str.substr(0,size-1));
			}
		}

		if(str.find_first_of("eE",0)!=std::string::npos){
			return (unsigned char)atof(str.c_str());
		}
		std::istringstream stream(str);
		unsigned char ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static short strToInt16(std::string str){
		if(str.find_first_of("eE",0)!=std::string::npos){
			return (short)atof(str.c_str());
		}

		std::istringstream stream(str);
		short ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static unsigned short strToUInt16(std::string str){
		int size = str.size();
		if(size==0){
			return 0;
		}
		else if(size > 1){
			if(str[0]=='0' && str[1]=='x'){
				return (unsigned short)HexaexpToUInteger64(str.substr(2));
			}
			else if(str[size-1]=='h'){
				return (unsigned short)HexaexpToUInteger64(str.substr(0,size-1));
			}
			else if(str[size-1]=='b'){
				return (unsigned short)BinaryToUInteger64(str.substr(0,size-1));
			}
		}

		if(str.find_first_of("eE",0)!=std::string::npos){
			return (unsigned short)atof(str.c_str());
		}
		std::istringstream stream(str);
		unsigned short ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static int strToInt32(std::string str){
		if(str.find_first_of("eE",0)!=std::string::npos){
			return (int)atof(str.c_str());
		}
		std::istringstream stream(str);
		int ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static unsigned int strToUInt32(std::string str){
		int size = str.size();
		if(size==0){
			return 0;
		}
		else if(size > 1){
			if(str[0]=='0' && str[1]=='x'){
				return (unsigned int)HexaexpToUInteger64(str.substr(2));
			}
			else if(str[size-1]=='h'){
				return (unsigned int)HexaexpToUInteger64(str.substr(0,size-1));
			}
			else if(str[size-1]=='b'){
				return (unsigned int)BinaryToUInteger64(str.substr(0,size-1));
			}
		}

		if(str.find_first_of("eE",0)!=std::string::npos){
			return (unsigned int)atof(str.c_str());
		}
		std::istringstream stream(str);
		unsigned int ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static long strToInt64(std::string str){
		std::istringstream stream(str);
		long long ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static unsigned long long strToUInt64(std::string str){
		int size = str.size();
		if(size==0){
			return 0;
		}
		else if(size > 1){
			if(str[0]=='0' && str[1]=='x'){
				return HexaexpToUInteger64(str.substr(2));
			}
			else if(str[size-1]=='h'){
				return HexaexpToUInteger64(str.substr(0,size-1));
			}
			else if(str[size-1]=='b'){
				return BinaryToUInteger64(str.substr(0,size-1));
			}
		}

		std::istringstream stream(str);
		unsigned long long ret = 0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static float strToFloating32(std::string str){
		std::istringstream stream(str);
		float ret = 0.0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static double strToFloating64(std::string str){
		std::istringstream stream(str);
		double ret = 0.0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	static long double strToFloating128(std::string str){
		std::istringstream stream(str);
		long double ret = 0.0;
		if(str.size()!=0){
			stream>>ret;
		}
		return ret;
	}

	/*
	 static int strToINT32(std::string str){
	 int startpoint = str.size()-1;
	 int I32 = 0;
	 int order = 1;

	 size_t dotpos = str.find_first_of(".",0);
	 if(dotpos!=std::string::npos){
	 startpoint = dotpos-1;
	 }

	 if(startpoint<0){
	 return 0;
	 }

	 if(str[0]=='-'){
	 for(int i = startpoint;i>=1;i++){
	 if(str[i]<48||str[i]>57)
	 return 0;
	 I32 += ((int)str[i]-48)*order;
	 order *= 10;
	 }
	 return -I32;
	 }
	 else{
	 for(int i = startpoint;i>=0;i--){
	 if(str[i]<48||str[i]>57)
	 return 0;
	 I32 += ((int)str[i]-48)*order;
	 order *= 10;
	 }
	 return I32;
	 }
	 }

	 static unsigned int strToUINT32(std::string str){
	 int startpoint = str.size()-1;
	 int UI32 = 0;
	 unsigned int order = 1;

	 size_t dotpos = str.find_first_of(".",0);
	 if(dotpos!=std::string::npos){
	 startpoint = dotpos-1;
	 }

	 if(startpoint<0||str[0]=='-'){
	 return 0;
	 }
	 for(int i = startpoint;i>=0;i--){
	 if(str[i]<48||str[i]>57)
	 return 0;
	 UI32 += ((int)str[i]-48)*order;
	 order *= 10;
	 }
	 return UI32;
	 }

	 static double strToFloating64(std::string str){
	 return atof(str.c_str());
	 }

	 */
};

#endif /* CASTER_HPP_ */
