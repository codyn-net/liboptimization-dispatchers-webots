#include "dispatcher.hh"

webots::Dispatcher dispatcher_webots;

static void
nicely_stop(int sig)
{
	dispatcher_webots.Stop();
}

int main (int argc, char const* argv[])
{
	signal(SIGTERM, nicely_stop);
	
	if (dispatcher_webots.Run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
