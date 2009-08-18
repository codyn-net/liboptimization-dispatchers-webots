#include "dispatcher.ih"

void Dispatcher::killWebots()
{
	if (d_pid == 0)
	{
		return;
	}

	cerr << "Killing webots ourselves" << endl;

	::kill(d_pid, SIGTERM);
	usleep(50000);
	::kill(d_pid, SIGKILL);

	onWebotsKilled(d_pid, 0);
}
