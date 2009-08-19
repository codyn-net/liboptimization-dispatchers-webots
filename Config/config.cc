#include "config.ih"

Config::Config()
:
	secure(true)
{
	registerProperty("secure", secure);
}
