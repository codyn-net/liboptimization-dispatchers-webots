#include "dispatcher.ih"

bool Dispatcher::world(string &w) const
{
	return setting("world", w);
}
