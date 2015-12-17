#ifndef HANDBUILTCC_H
#define HANDBUILTCC_H

#include <vector>
#include <string>
#include <limits>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include "packet.hh"
#include "memory.hh"
#include "middlelayer.hh"

class HandBuiltCC
{
private:
  Memory _memory;

  int _packets_sent, _packets_received;

  double _last_send_time;

  int _the_window;
  double _intersend_time;

  unsigned int _flow_id;
  int _largest_ack;

  double _last_rewma;
  double _last_sewma;

  Action lowAction;
  Action highAction;

public:
  HandBuiltCC();

  void packets_received( const std::vector< Packet > & packets );
  void reset( const double & tickno ); /* start new flow */

  template <class NextHop>
  void send( const unsigned int id, NextHop & next, const double & tickno,
	     const int packets_sent_cap = std::numeric_limits<int>::max() );

  HandBuiltCC & operator=( const HandBuiltCC & ) { assert( false ); return *this; }

  double next_event_time( const double & tickno ) const;

  const int & packets_sent( void ) const { return _packets_sent; }
};


#endif