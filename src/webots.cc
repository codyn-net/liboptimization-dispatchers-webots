#include "dispatcher.hh"

int main (int argc, char const* argv[])
{
	webots::Dispatcher webots;
	
	if (webots.Run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
