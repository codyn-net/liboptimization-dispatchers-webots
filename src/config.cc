/*
 * config.cc
 * This file is part of dispatcher-webots
 *
 * Copyright (C) 2009 - Jesse van den Kieboom
 *
 * dispatcher-webots is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dispatcher-webots is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dispatcher-webots; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "config.hh"

using namespace std;
using namespace webots;
using namespace jessevdk;

Config *Config::s_instance = 0;

Config::Config()
:
	Secure(false),
	WebotsVersion("6.3.1"),
	ForceBatch(false),
	RCOverrides(""),
	ForwardOutput(false)
{
	Register("secure", Secure);
	Register("webots version", WebotsVersion);
	Register("force batch", ForceBatch);
	Register("rc overrides", RCOverrides);
	Register("forward output", ForwardOutput);

	d_version[0] = 0;
	d_version[1] = 0;
	d_version[2] = 0;
}

Config &
Config::Initialize(string const &filename)
{
	if (!s_instance)
	{
		s_instance = new Config();
	}
	
	s_instance->Read(filename);
	return *s_instance;
}

Config &
Config::Instance()
{
	return *s_instance;
}

void
Config::WebotsNumericVersion(size_t version[3])
{
	if (d_version[0] == 0)
	{
		vector<string> parts = base::String(WebotsVersion).Split(".");

		if (parts.size() == 3)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				stringstream s(parts[i]);
				s >> d_version[i];
			}
		}
	}

	for (size_t i = 0; i < 3; ++i)
	{
		version[i] = d_version[i];
	}
}
