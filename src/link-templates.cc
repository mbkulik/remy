#include <utility>
#include <cstdio>

#include "link.hh"

using namespace std;

template <class NextHop>
void Link::tick( NextHop & next, const double & tickno )
{
  _pending_packet.tick( next, tickno );

  if ( _pending_packet.empty() ) {
    if ( not _buffer.empty() ) {
      _pending_packet.accept( _buffer.front(), tickno );
      _buffer.pop();
      _size_statistics[ _buffer.size() ] += tickno - _last_change_tick; 
      _last_change_tick = tickno;
    }
  }
}
