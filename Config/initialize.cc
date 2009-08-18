#include "config.ih"

Config &Config::initialize(string const &filename)
{
	if (!s_instance)
	{
		s_instance = new Config();
	}
	
	s_instance->read(filename);
	return *s_instance;
}
