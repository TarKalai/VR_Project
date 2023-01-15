#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max(); // Returns a number between 0 and 1.
        // if we want a random number between 0 and 10 we can do : Random::Float() * 10
        // random number between 0 and 100 = Random::Float() *100, ....
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};