#include "Randomise.h"
#include <random>
#include <chrono>

Randomise ranndomise;
std::default_random_engine ran;

Randomise::Randomise()
{
	ran = std::default_random_engine((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
}

int Randomise::Int(int lower, int upper)
{
	std::uniform_int_distribution<int> num(lower, upper);
	return num(ran);
}

float Randomise::Real(float lower, float upper)
{
	std::uniform_real_distribution<float> num(lower, upper);
	return num(ran);
}
