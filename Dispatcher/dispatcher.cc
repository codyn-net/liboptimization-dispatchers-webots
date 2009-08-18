#include "dispatcher.ih"

Dispatcher::Dispatcher()
{
	Config::initialize(PREFIXDIR "/libexec/liboptimization-dispatchers-0.1/webots.conf");

	d_pid = 0;
}
