#pragma once

#include <string> 
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
#include "material.h"
#include "constant.h"

class Utils { 
	public: 
		static float getRandom(float from=0, float to=1) {
			int mod = (to - from)*100;
			return float(rand()%mod + 100*from)/100;
		}

		static glm::vec3 getRandom3(float from=0, float to=1){
			return glm::vec3(getRandom(from, to), getRandom(from, to),getRandom(from, to));
		} 

		static glm::vec3 RainbowColor(float t) {
			t = fmod(t, 1.0f);
			float h = t * 6.0f;
			float r, g, b;
			if (h < 1.0f) {
				r = 1.0f;
				g = h;
				b = 0.0f;
			} else if (h < 2.0f) {
				r = 2.0f - h;
				g = 1.0f;
				b = 0.0f;
			} else if (h < 3.0f) {
				r = 0.0f;
				g = 1.0f;
				b = h - 2.0f;
			} else if (h < 4.0f) {
				r = 0.0f;
				g = 4.0f - h;
				b = 1.0f;
			} else if (h < 5.0f) {
				r = h - 4.0f;
				g = 0.0f;
				b = 1.0f;
			} else {
				r = 1.0f;
				g = 0.0f;
				b = 6.0f - h;
			}
			return glm::vec3(r, g, b);
		}
}; 


class Textures {
	public:
		Textures(const Textures&) = delete;

		static Texture* Get(std::string name){
			if (name == "brick") {return Get().IBrick();}
			else if (name == "dirt") {return Get().IDirt();}
			else if (name == "anubis") {return Get().IDirt();}
			else if (name == "white") {return Get().IWhite();}
			else if (name == "wood") {return Get().IWood();}
			else if (name == "concrete") {return Get().IConcrete();}
			else if (name == "container") {return Get().IContainer();}
			else if (name == "damier") {return Get().IDamier();}
			else if (name == "point1") {return Get().IPoint1();}
			else if (name == "point2") {return Get().IPoint2();}
			else if (name == "brickwall") {return Get().IBrickwall();}
			else if (name == "brick2") {return Get().IBrick2();}
			else if (name == "toy") {return Get().IToy();}
			else {return Get().IWhite();}
		} 

		static Texture* Brick(){return Get().IBrick();}
		static Texture* Dirt(){return Get().IDirt();}
		static Texture* Anubis(){return Get().IAnubis();}
		static Texture* White(){return Get().IWhite();}
		static Texture* Wood(){return Get().IWood();}
		static Texture* Concrete(){return Get().IConcrete();}
		static Texture* Container(){return Get().IContainer();}
		static Texture* Damier(){return Get().IDamier();}
		static Texture* Point1(){return Get().IPoint1();}
		static Texture* Point2(){return Get().IPoint2();}
		static Texture* Brickwall(){return Get().IBrickwall();}
		static Texture* Brick2(){return Get().IBrick2();}
		static Texture* Toy(){return Get().IToy();}


	private:
		Textures(){}

		static Textures& Get(){
			static Textures instance;
			return instance;
		} 

		Texture* IBrick(){return &brick;}
		Texture* IDirt(){return &dirt;}
		Texture* IAnubis(){return &anubis;}
		Texture* IWhite(){return &white;}
		Texture* IWood(){return &wood;}
		Texture* IConcrete(){return &concrete;}
		Texture* IContainer(){return &container;}
		Texture* IDamier(){return &damier;}
		Texture* IPoint1(){return &point1;}
		Texture* IPoint2(){return &point2;}
		Texture* IBrickwall(){return &brickwall;}
		Texture* IBrick2(){return &brick2;}
		Texture* IToy(){return &toy;}


		Texture brick = Texture(image::brick, "brick", true, false); 
		Texture dirt  = Texture(image::dirt, "dirt", false, false); 
		Texture anubis  = Texture(image::dirt, "anubis", true, true); 
		Texture white = Texture(image::white, "white", false, false);
		Texture wood  = Texture(image::wood, "wood", false, false);
		Texture concrete  = Texture(image::concrete, "concrete", false, false);
		Texture container  = Texture(image::container, "container", false, false);
		Texture damier  = Texture(image::damier, "damier", true, true);
		Texture point1  = Texture(image::point1, "point1", true, true);
		Texture point2  = Texture(image::point2, "point2", true, true);
		Texture brickwall  = Texture(image::brickwall, "brickwall", false, false, image::brickwalln);
		Texture brick2  = Texture(image::brick2, "brick2", false, false, image::brick2n, image::brick2d); // paralax
		Texture toy  = Texture(image::toy, "toy", false, false, image::toyn, image::toyd); // paralax
};

class Materials {
	public:
		Materials(const Materials&) = delete;

		static Material* Get(std::string name){
			if (name == "shiny") {return Get().IShiny();}
			else if (name == "dull") {return Get().IDull();}
			else if (name == "empty") {return Get().IEmpty();}
			else {return Get().IEmpty();}
		} 

		static Material* Shiny(){return Get().IShiny();}
		static Material* Dull(){return Get().IDull();}
		static Material* Empty(){return Get().IEmpty();}

	private:
		Materials(){}

		static Materials& Get(){
			static Materials instance;
			return instance;
		} 

		Material* IShiny(){return &shiny;}
		Material* IDull(){return &dull;}
		Material* IEmpty(){return &empty;}

		Material shiny = Material(4.0f, 256.0f, "shiny"); 
    	Material dull  = Material(0.3f, 4.0f, "dull"); 
    	Material empty = Material(0.0f, 0.0f, "empty"); 

};


//https://stackoverflow.com/questions/45613707/proper-setter-and-getter-for-static-member-variable-in-header-only-library
struct Day {
    int hour;
    int minute;
    int second;
};

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

		static void updateTime()
        {
			if (!pause()) {
				deltaTime() = glfwGetTime() - glfwOldTime();
				time() = time() + speed() * deltaTime();
				glfwOldTime() = glfwGetTime();
			}
			else {
				glfwOldTime() = glfwGetTime();
			}
        }

		static const float getTime(){
			return fmod(time(), Ttime::maxTime);
        }


		static void setTime(const float _t)
        {
			if (pause()) {
				deltaTime() = _t - oldTime();
				time() = _t;
				oldTime() = _t;
			}
			else {
				oldTime() = _t;
			}
        }

		static const Day getDate()
        {
			float hour = fmod(24 * time() / Ttime::maxTime, 24);
        	float minute = 60*(hour - floor(hour));
        	float second = 60*(minute - floor(minute));
			return Day{(int) hour, (int) minute, (int) second};
        }

		static const bool& pause(){
			return isPause();
        }

		static void setPause(const bool _p){
            isPause() = _p;
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

		static bool& isPause(){
			static bool pause = false;
            return pause;
		}

		static float& time(){
			static float time = 0;
            return time;
		}

		static float& glfwOldTime(){
			static float glfwOldTime = -Ttime::startTime;
            return glfwOldTime;
		}

		static float& oldTime(){
			static float oldTime = 0;
            return oldTime;
		}

		static float& deltaTime(){
			static float deltaTime = 0;
            return deltaTime;
		}
};

class Optic 
{
    public:
        static void setReflectivity(const float _reflec){
            reflectivity() = _reflec;
        }           
        static const float& getReflectivity(){
            return reflectivity();
        }	

        static void setRefractivity(const float _refrac){
            refractivity() = _refrac;
        }           
        static const float& getRefractivity(){
            return refractivity();
        }	

        static void setCoefRefractivity(const float _coefRefrac){
            coefRefractivity() = _coefRefrac;
        }           
        static const float& getCoefRefractivity(){
            return coefRefractivity();
        }		


	private:

        static float& reflectivity(){
            static float reflec = optic::reflectivity;
            return reflec;
        }

		static float& refractivity(){
            static float refrac = optic::refractivity;
            return refrac;
        }

		static float& coefRefractivity(){
			static float coefRefrac = optic::coefRefraction;
            return coefRefrac;
		}
};