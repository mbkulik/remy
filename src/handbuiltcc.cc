#include <algorithm>
#include <limits>
#include <iostream>

#include "handbuiltcc.hh"

using namespace std;

HandBuiltCC::HandBuiltCC( void )
  :   _memory(),
     _packets_sent( 0 ),
     _packets_received( 0 ),
     _last_send_time( 0 ),
     _the_window( 0 ),
     _intersend_time( 0 ),
     _flow_id( 0 ),
     _largest_ack( -1 ),
     _last_rewma(0),
     _last_sewma(0),
     lowAction(1, 0.5),
     highAction(256,3)
{
}

void HandBuiltCC::packets_received( const vector< Packet > & packets ) {
  _packets_received += packets.size();
  /* Assumption: There is no reordering */
  _largest_ack = max( packets.at( packets.size() - 1 ).seq_num, _largest_ack );
  _memory.packets_received( packets, _flow_id );


  double rewma = _memory.field(1);
  //double sewma = _memory.field(0);

  if( rewma >= _last_rewma ) {
    _the_window  = highAction.getWindowSize();
    _intersend_time = highAction.getIntersendTime();
  } else {
    _the_window = lowAction.getWindowSize();
    _intersend_time = lowAction.getIntersendTime();
  }
  
  _last_rewma = rewma;
}

void HandBuiltCC::reset( const double & )
{
  _memory.reset();
  _last_send_time = 0;
  _the_window = 0;
  _intersend_time = 0;
  _flow_id++;
  _largest_ack = _packets_sent - 1; /* Assume everything's been delivered */
  assert( _flow_id != 0 );
}

double HandBuiltCC::next_event_time( const double & tickno ) const
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