#pragma once

#ifndef UTILS_H 
#define UTILS_H 
#include <string> 
class Utils { 
	public: 
		static float getRandom(float from=-4, float to=4) {
			int mod = (to - from)*100;
			return float(rand()%mod + 100*from)/100;}

}; 
#endif 