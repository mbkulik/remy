
#include <algorithm>
#include <limits>
#include <iostream>

#include "generic.hh"

using namespace std;

GenericCC::GenericCC( void )
  :   _memory(),
     _packets_sent( 0 ),
     _packets_received( 0 ),
     _last_send_time( 0 ),
     _flow_id( 0 ),
     _largest_ack( -1 ),
     _the_window(0),
     _intersend_time(0),
     _actionMap()
{
}

GenericCC::GenericCC(std::vector<std::pair<Observation,Action>> a)
:   _memory(),
     _packets_sent( 0 ),
     _packets_received( 0 ),
     _last_send_time( 0 ),
     _flow_id( 0 ),
     _largest_ack( -1 ),
     _the_window(0),
     _intersend_time(0),
     _actionMap(a)
     {
     }

Observation GenericCC::getObservation() {
	return Observation(_memory);
}

void GenericCC::packets_received( const vector< Packet > & packets ) {
  _packets_received += packets.size();
  /* Assumption: There is no reordering */
  _largest_ack = max( packets.at( packets.size() - 1 ).seq_num, _largest_ack );
  _memory.packets_received( packets, _flow_id );

  Action curAction = _actionMap.at(rand() % _actionMap.size()).second;

	_the_window = curAction.getWindowSize();
	_intersend_time = curAction.getIntersendTime();

}

void GenericCC::reset( const double & )
{
  _memory.reset();
  _last_send_time = 0;
  _flow_id++;
  _largest_ack = _packets_sent - 1; /* Assume everything's been delivered */
  assert( _flow_id != 0 );
}

double GenericCC::next_event_time( const double & tickno ) const
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