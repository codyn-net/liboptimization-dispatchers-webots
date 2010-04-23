#include "dispatcher.hh"

#include <signal.h>

webots::Dispatcher dispatcher_webots;

static void
nicely_stop(int sig)
{
	dispatcher_webots.Stop();
}

int main (int argc, char const* argv[])
{
	struct sigaction new_action;

	new_action.sa_handler = nicely_stop;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction(SIGTERM, &new_action, NULL);

	if (dispatcher_webots.Run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
