/*
 * Caster.h
 *
 *  Created on: 2011/08/24
 *      Author: sakurai
 */

#ifndef CASTER_HPP_
#define CASTER_HPP_

#include <sstream>

class Caster{
public:
	static unsigned long long HexaexpToUInteger64(std::string str);
	static unsigned long long BinaryToUInteger64(std::string str);

	static signed char strToInt8(std::string str);

	static unsigned char strToUInt8(std::string str);
	static short strToInt16(std::string str);
	static unsigned short strToUInt16(std::string str);
	static int strToInt32(std::string str);
	static unsigned int strToUInt32(std::string str);
	static long strToInt64(std::string str);
	static unsigned long long strToUInt64(std::string str);
	static float strToFloating32(std::string str);
	static double strToFloating64(std::string str);
	static long double strToFloating128(std::string str);

	static std::string IntToStr(long long integer);
	static std::string UIntToStr(unsigned long long uinteger);
	static std::string FloatingToStr(long double db);
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
