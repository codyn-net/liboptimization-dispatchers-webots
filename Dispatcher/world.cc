#include "dispatcher.ih"

bool Dispatcher::world(string &w) const
{
	string set;
	Config &config = Config::instance();

	if (!setting("world", set))
	{
		cerr << "Webots world is not set, please provide the path to the webots world file in the dispatcher settings..." << endl;
		return false;
	}

	string resolved;
	
	if (!FileSystem::realpath(set, resolved))
	{
		cerr << "Webots world could not be found: " << set << endl;
		return false;
	}

	// Check if world is owned by user and in home dir
	struct stat buf;

	if (stat(resolved.c_str(), &buf) != 0)
	{
		cerr << "Webots world could not be found: " << set << endl;
		return false;
	}

	if (!config.secure)
	{
		w = resolved;
		return true;
	}

	struct passwd *pw = getpwuid(getuid());
	string home = pw->pw_dir;

	if (buf.st_uid != getuid())
	{
		cerr << "Webots world is not owned by the user: " << resolved << endl;
		return false;
	}
	else if (!String(resolved).startsWith(home))
	{
		cerr << "Webots world is not in user home directory: " << resolved << endl;
		return false;
	}
	else
	{
		w = resolved;
		return true;
	}
}
