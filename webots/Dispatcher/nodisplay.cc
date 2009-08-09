#include "dispatcher.ih"

bool Dispatcher::noDisplay() const
{
	return !setting("display");
}
