#include "dispatcher.ih"

bool Dispatcher::mode(string &m) const
{
	if (setting("mode", m))
	{
		m = string("--mode=") + m;
		return true;
	}
	
	return false;
}
