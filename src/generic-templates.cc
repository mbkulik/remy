#include <cassert>
#include <utility>

#include "generic.hh"

using namespace std;

template <class NextHop>
void GenericCC::send( const unsigned int id, NextHop & next, const double & tickno,
		const int packets_sent_cap )
{
  assert( _packets_sent >= _largest_ack + 1 );

  if ( _the_window == 0 ) {
    /* initial window and intersend time */
    _the_window = 2;
    _intersend_time = 3;

  }

  if ( (_packets_sent < _largest_ack + 1 + _the_window)
       and (_last_send_time + _intersend_time <= tickno) ) {

    /* Have we reached the end of the flow for now? */
    if ( _packets_sent >= packets_sent_cap ) {
      return;
    }

    Packet p( id, _flow_id, tickno, _packets_sent );
    _packets_sent++;
    _memory.packet_sent( p );
    next.accept( p, tickno );
    _last_send_time = tickno;
  }
}