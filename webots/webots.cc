#include <dispatcher/Webots/webots.hh>

int main (int argc, char const* argv[])
{
	worker::dispatcher::Webots webots;
	
	if (webots.run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
