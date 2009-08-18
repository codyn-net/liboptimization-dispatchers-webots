#include "config.ih"

Config &Config::instance()
{
	return *s_instance;
}
