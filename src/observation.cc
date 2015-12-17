
#include "middlelayer.hh"


Observation::Observation() :
	_mem(MAX_MEMORY())
{
}

Observation::Observation(Memory m) :
	_mem(m)
{
}

double Observation::getSendEWMA()
{
	return _mem.field(0);
}

double Observation::getRecEWMA()
{
	return _mem.field(1);
}

double Observation::getRTTRatio()
{
	return _mem.field(2);
}

double Observation::getSlowRecEWMA()
{
	return _mem.field(3);
}