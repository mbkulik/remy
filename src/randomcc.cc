#include <algorithm>
#include <limits>
#include <cstdlib>
#include <cstdio>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "randomcc.hh"

using namespace std;

RandomCC::RandomCC(void)
  :  
     _packets_sent( 0 ),
     _packets_received( 0 ),
     _last_send_time( 0 ),
     _the_window( 0 ),
     _intersend_time( 0 ),
     _flow_id( 0 ),
     _largest_ack( -1 ),
     _generator(time(0))
{
}

void RandomCC::packets_received( const vector< Packet > & packets ) {
  _packets_received += packets.size();
  /* Assumption: There is no reordering */
  _largest_ack = max( packets.at( packets.size() - 1 ).seq_num, _largest_ack );

  boost::random::uniform_int_distribution<> dist(0, 256);
  boost::random::uniform_int_distribution<> dist1(25, 300);

  _the_window = dist(_generator);
  _intersend_time = (dist1(_generator) / 100.0);
}

void RandomCC::reset( const double & )
{
  //_memory.reset();
  _last_send_time = 0;
  _the_window = 0;
  _intersend_time = 0;
  _flow_id++;
  _largest_ack = _packets_sent - 1; /* Assume everything's been delivered */
  assert( _flow_id != 0 );
}

double RandomCC::next_event_time( const double & tickno ) const
{
  if ( _packets_sent < _largest_ack + 1 + _the_window ) {
    if ( _last_send_time + _intersend_time <= tickno ) {
      return tickno;
    } else {
      return _last_send_time + _intersend_time;
    }
  } else {
    /* window is currently closed */
    return std::numeric_limits<double>::max();
  }
}