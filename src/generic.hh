#ifndef GENERIC_CC_H
#define GENERIC_CC_H

#include <vector>
#include <string>
#include <limits>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include "packet.hh"
#include "memory.hh"
#include "middlelayer.hh"

class GenericCC
{
private:
  Memory _memory;

  int _packets_sent, _packets_received;

  double _last_send_time;

  unsigned int _flow_id;
  int _largest_ack;

  int _the_window;
  double _intersend_time;

  std::vector<std::pair<Observation, Action>> _actionMap;


public:
  GenericCC();
  GenericCC(std::vector<std::pair<Observation,Action>>);

  Observation getObservation();

  void packets_received( const std::vector< Packet > & packets );
  void reset( const double & tickno ); /* start new flow */

  template <class NextHop>
  void send( const unsigned int id, NextHop & next, const double & tickno,
	     const int packets_sent_cap = std::numeric_limits<int>::max() );

  GenericCC & operator=( const GenericCC & ) { assert( false ); return *this; }

  double next_event_time( const double & tickno ) const;

  const int & packets_sent( void ) const { return _packets_sent; }
};


#endif