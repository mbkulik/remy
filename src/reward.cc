
#include "middlelayer.hh"

Reward::Reward() :
	score(0)
{
}

Reward::Reward(double v) :
	score(v)
{
}

double Reward::getValue()
{
	return score;
}