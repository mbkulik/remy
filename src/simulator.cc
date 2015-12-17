
#include "middlelayer.hh"
#include "generic.hh"
#include "network.cc"
#include "generic-templates.cc"

using namespace std;	

Simulator::Simulator() :
	_observation(),
	_action(),
	_reward()
{
}

Observation Simulator::observation()
{
	return _observation;
}

Action Simulator::action()
{
	return _action;
}
	
Reward Simulator::reward()
{
	return _reward;
}

void Simulator::simulate(vector<pair<Observation,Action>> OtoAMap, NetConfig x)
{
	PRNG run_prng = global_PRNG();
	int ticks_to_run = 1000000;
	double tmp_score = 0.0;
	
	GenericCC genie = GenericCC(OtoAMap);

	Network<GenericCC, GenericCC> network1( genie, run_prng, x);

    network1.run_simulation( ticks_to_run );

    tmp_score = network1.senders().utility();
  	
  	_reward = Reward(tmp_score);
  	_observation = genie.getObservation();
}
