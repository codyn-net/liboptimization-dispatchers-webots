#include "dispatcher.ih"

bool Dispatcher::timeout(size_t &tm) const
{
	string val;
	
	if (!setting("timeout", val))
		return false;
	
	stringstream s;
	s << val;
	
	tm = 0;
	s >> tm;
	return true;
}
