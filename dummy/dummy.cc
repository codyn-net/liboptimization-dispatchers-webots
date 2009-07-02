#include "Dispatcher/dispatcher.hh"

int main(int argc, char const* argv[])
{
	dummy::Dispatcher dummy;
	
	if (dummy.run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
