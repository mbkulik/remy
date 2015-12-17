
#include "middlelayer.hh"

Action::Action(void) :
	_the_window(0),
	_intersend_time(0)
{
}

Action::Action(int w, double i) :
	_the_window(w),
	_intersend_time(i)
{
}

int Action::getWindowSize(void)
{
	return _the_window;
}

double Action::getIntersendTime(void)
{
	return _intersend_time;
}
