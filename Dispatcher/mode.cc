#include "dispatcher.ih"

bool Dispatcher::mode(string &m) const
{
	if (setting("mode", m))
	{
		return true;
	}
	
	return false;
}
