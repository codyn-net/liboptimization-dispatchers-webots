#include "dispatcher.ih"

bool Dispatcher::world(string &w) const
{
	string set;

	if (!setting("world", set))
	{
		return false;
	}

	string resolved;
	
	if (!FileSystem::realpath(set, resolved))
	{
		return false;
	}

	// Check if world is owned by user and in home dir
	struct stat buf;

	if (stat(resolved.c_str(), &buf) != 0)
	{
		return false;
	}

	struct passwd *pw = getpwuid(getuid());
	string home = pw->pw_dir;

	if (buf.st_uid == getuid() && String(resolved).startsWith(home))
	{
		w = resolved;
		return true;
	}
	else
	{
		return false;
	}
}
