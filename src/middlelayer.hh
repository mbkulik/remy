#ifndef MIDDLELAYER_H
#define MIDDLELAYER_H

#include <vector>


#include "memory.hh"
#include "network.hh"
#include "utility.hh"

class Observation {

private:
	Memory _mem;

public:
	Observation();
	Observation(Memory);

	double getSendEWMA();
	double getRecEWMA();
	double getRTTRatio();
	double getSlowRecEWMA();
};

class Action {

private:
	int _the_window;
  	double _intersend_time;
public:
	Action();
	Action(int, double);
	int getWindowSize(void);
	double getIntersendTime(void);
};

class Reward {

private:
	double score;

public:
	Reward();
	Reward(double);
	double getValue();
};


class Simulator {

private:
	Observation _observation;
	Action _action;
	Reward _reward;

public:
	Simulator();

	Observation  observation();
	Action  action();
	Reward reward();

	void simulate(std::vector<std::pair<Observation,Action>>, NetConfig);
};

#endif