#pragma once

#ifndef UTILS_H 
#define UTILS_H 

#include <string> 
#include "texture.h"
#include "constant.h"
#include "material.h"

class Utils { 
	public: 
		static float getRandom(float from=-4, float to=4) {
			int mod = (to - from)*100;
			return float(rand()%mod + 100*from)/100;
		}
}; 


class Textures {
	public:
		Textures(const Textures&) = delete;

		static Textures& Get(){
			static Textures instance;
			return instance;
		} 

		static Texture* Brick(){return Get().IBrick();}
		static Texture* Dirt(){return Get().IDirt();}
		static Texture* White(){return Get().IWhite();}
		static Texture* Wood(){return Get().IWood();}
		static Texture* Hulk(){return Get().IHulk();}
		static Texture* Bird(){return Get().IBird();}

	private:
		Textures(){}

		Texture* IBrick(){return &brick;}
		Texture* IDirt(){return &dirt;}
		Texture* IWhite(){return &white;}
		Texture* IWood(){return &wood;}
		Texture* IHulk(){return &hulk;}
		Texture* IBird(){return &bird;}

		Texture brick = Texture(image::brick); 
		Texture dirt  = Texture(image::dirt); 
		Texture white = Texture(image::white);
		Texture wood  = Texture(image::wood);
		Texture hulk  = Texture(image::hulk);
		Texture bird  = Texture(image::bird);

};


class Materials {
	public:
		Materials(const Materials&) = delete;

		static Materials& Get(){
			static Materials instance;
			return instance;
		} 

		static Material* Shiny(){return Get().IShiny();}
		static Material* Dull(){return Get().IDull();}
		static Material* Empty(){return Get().IEmpty();}

	private:
		Materials(){}

		Material* IShiny(){return &shiny;}
		Material* IDull(){return &dull;}
		Material* IEmpty(){return &empty;}

		Material shiny = Material(4.0f, 256.0f); 
    	Material dull  = Material(0.3f, 4.0f); 
    	Material empty = Material(0.0f, 0.0f); 

};


//https://stackoverflow.com/questions/45613707/proper-setter-and-getter-for-static-member-variable-in-header-only-library
class Time 
{
    public:

        // The setter uses the fact that speed()
        // returns a non-const reference,
        // so we can assign to it.
        static void setSpeed(const float _v){
			oldSpeed() = getSpeed();
            speed() = _v;
        }           

        // A true getter.
        // Returns const float&, so we cannot assign to it.
        static const float& getSpeed(){
            return speed();
        }		
		
		static const float& getOldSpeed(){
            return oldSpeed();
        }

	private:

        static float& speed(){
            static float v = 1;
            return v;
        }
		
		static float& oldSpeed(){
			static float old = 1;
            return old;
		}
};

#endif 
