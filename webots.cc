#include "Dispatcher/dispatcher.hh"

int main (int argc, char const* argv[])
{
	webots::Dispatcher webots;
	
	if (webots.run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
