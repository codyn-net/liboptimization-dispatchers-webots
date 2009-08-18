#include "dispatcher.ih"

bool Dispatcher::onTimeout() 
{
	// Close webots
	killWebots();
	return false;
}
