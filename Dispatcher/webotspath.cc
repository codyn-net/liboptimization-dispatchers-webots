#include "dispatcher.ih"

string Dispatcher::webotsPath() const
{
	string val;
	
	if (setting("webotsPath", val))
		return val;
	
	return "webots";
}
