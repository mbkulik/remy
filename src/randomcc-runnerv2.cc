
#include <cstdio>
#include <vector>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "configrange.hh"
#include "middlelayer.hh"

using namespace std;

vector<NetConfig> makeConfigs(ConfigRange range) {

  vector<NetConfig> _configs;

  _configs.push_back( NetConfig().set_link_ppt( range.link_packets_per_ms.first ).set_delay( range.rtt_ms.first ).set_num_senders( range.max_senders ).set_on_duration( range.mean_on_duration ).set_off_duration( range.mean_off_duration ) );

  if ( range.lo_only ) {
    return _configs;
  }

  _configs.push_back( NetConfig().set_link_ppt( range.link_packets_per_ms.first ).set_delay( range.rtt_ms.second ).set_num_senders( range.max_senders ).set_on_duration( range.mean_on_duration ).set_off_duration( range.mean_off_duration ) );
  _configs.push_back( NetConfig().set_link_ppt( range.link_packets_per_ms.second ).set_delay( range.rtt_ms.first ).set_num_senders( range.max_senders ).set_on_duration( range.mean_on_duration ).set_off_duration( range.mean_off_duration ) );
  _configs.push_back( NetConfig().set_link_ppt( range.link_packets_per_ms.second ).set_delay( range.rtt_ms.second ).set_num_senders( range.max_senders ).set_on_duration( range.mean_on_duration ).set_off_duration( range.mean_off_duration ) );

  /* now load some random ones just for fun */
  for ( int i = 0; i < 12; i++ ) {
    boost::random::uniform_real_distribution<> link_speed( range.link_packets_per_ms.first, range.link_packets_per_ms.second );
    boost::random::uniform_real_distribution<> rtt( range.rtt_ms.first, range.rtt_ms.second );
    boost::random::uniform_int_distribution<> num_senders( 1, range.max_senders );

    _configs.push_back( NetConfig().set_link_ppt( link_speed( global_PRNG() ) ).set_delay( rtt( global_PRNG() ) ).set_num_senders( num_senders( global_PRNG() ) ).set_on_duration( range.mean_on_duration ).set_off_duration( range.mean_off_duration ) );
  }

  return _configs;
}

int main(int argc, char *argv[]) {

  unsigned int num_senders = 2;
  double link_ppt = 1.0;
  double delay = 100.0;
  double mean_on_duration = 5000.0;
  double mean_off_duration = 5000.0;
  double score = 0.0;

  int num_runs = 1;
	vector< NetConfig > configs;

  for ( int i = 1; i < argc; i++ ) {
    string arg( argv[ i ] );
     if ( arg.substr( 0, 5 ) == "nsrc=" ) {
      num_senders = atoi( arg.substr( 5 ).c_str() );
      fprintf( stderr, "Setting num_senders to %d\n", num_senders );
    } else if ( arg.substr( 0, 5 ) == "link=" ) {
      link_ppt = atof( arg.substr( 5 ).c_str() );
      fprintf( stderr, "Setting link packets per ms to %f\n", link_ppt );
    } else if ( arg.substr( 0, 4 ) == "rtt=" ) {
      delay = atof( arg.substr( 4 ).c_str() );
      fprintf( stderr, "Setting delay to %f ms\n", delay );
    } else if ( arg.substr( 0, 3 ) == "on=" ) {
      mean_on_duration = atof( arg.substr( 3 ).c_str() );
      fprintf( stderr, "Setting mean_on_duration to %f ms\n", mean_on_duration );
    } else if ( arg.substr( 0, 4 ) == "off=" ) {
      mean_off_duration = atof( arg.substr( 4 ).c_str() );
      fprintf( stderr, "Setting mean_off_duration to %f ms\n", mean_off_duration );
    }
  }

  ConfigRange configuration_range;
  configuration_range.link_packets_per_ms = make_pair( link_ppt, 0 ); /* 1 Mbps to 10 Mbps */
  configuration_range.rtt_ms = make_pair( delay, 0 ); /* ms */
  configuration_range.max_senders = num_senders;
  configuration_range.mean_on_duration = mean_on_duration;
  configuration_range.mean_off_duration = mean_off_duration;
  configuration_range.lo_only = true;

  configs = makeConfigs(configuration_range);

  for(int i = 0; i < num_runs; i++) {
    PRNG generator = global_PRNG();
    boost::random::uniform_int_distribution<> dist(0, 256);
    boost::random::uniform_int_distribution<> dist1(25, 300); 

    Simulator sim = Simulator();

    vector<pair<Observation,Action>> observationActionMap;

    for( auto &x: configs ) {

          Action a = Action( dist(generator), (dist1(generator) / 100.0));
          Observation o = Observation();

          observationActionMap.emplace_back(o,a);

          sim.simulate(observationActionMap, x);

          Reward r = sim.reward();

          score += r.getValue();
      
    }
  }

  printf("score: %f\n", score / num_runs);
  /*double norm_score = 0;

  for ( auto &run : throughputs_delays ) {
    printf( "===\nconfig: %s\n", run.first.str().c_str() );
    for ( auto &x : run.second ) {
      printf( "sender: [tp=%f, del=%f]\n", x.first / run.first.link_ppt, x.second / run.first.delay );
      norm_score += log2( x.first / run.first.link_ppt ) - log2( x.second / run.first.delay );
    }
  }

  printf( "normalized_score = %f\n", norm_score );



    double tmp_score = 0.0;
    PRNG generator = global_PRNG();
    boost::random::uniform_int_distribution<> dist(0, 256);
    boost::random::uniform_int_distribution<> dist1(25, 300); 

    Simulator sim = Simulator();

    vector<pair<Observation,Action>> observationActionMap;

    for( auto &x: configs ) {

      for(int i = 0; i < num_runs; i++) {

          Action a = Action( dist(generator), (dist1(generator) / 100.0));
          Observation o = Observation();

          observationActionMap.emplace_back(o,a);

          sim.simulate(observationActionMap, x);

          Reward r = sim.reward();

          tmp_score += r.getValue();
      }
    }


  printf("score: %f\n", (tmp_score / (num_runs * configs.size())));*/
	

    








		//PRNG run_prng = global_PRNG();
    //double tmp_score = 0.0;

  //		for ( auto &x : configs ) {
    		/* run once */
  	//		Network<RandomCC, RandomCC> network1( RandomCC(), run_prng, x);

    //		network1.run_simulation( ticks_to_run );

   // 		tmp_score += network1.senders().utility();
    		//the_outcome.throughputs_delays.emplace_back( x, network1.senders().throughputs_delays() );
  //		}

   //   printf("run %d score: %f\n", i, (tmp_score / configs.size()));

   //   score += (tmp_score / configs.size());

	//}

	//printf("score: %f\n", (score / num_runs));
}