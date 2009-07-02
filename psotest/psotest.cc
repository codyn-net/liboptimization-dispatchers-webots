#include "Dispatcher/dispatcher.hh"

int main(int argc, char const* argv[])
{
	psotest::Dispatcher psotest;
	
	if (psotest.run())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
