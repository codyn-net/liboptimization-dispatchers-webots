#include "dispatcher.ih"

string Dispatcher::resolveWebotsExecutable(std::string const &path)
{
	string ret = Glib::find_program_in_path(path);

	if (ret == "")
	{
		return ret;
	}

	// System webots is fine
	if (String(ret).startsWith("/usr"))
	{
		return ret;
	}

	// Otherwise, must be owned by the user, and in his/her home directory
	struct stat buf;

	if (stat(ret.c_str(), &buf) != 0)
	{
		return "";
	}

	struct passwd *pwd = getpwuid(getuid());
	string homedir = pwd->pw_dir;

	if (buf.st_uid == getuid() && String(ret).startsWith(homedir))
	{
		return ret;
	}
	else
	{
		return "";
	}
}
