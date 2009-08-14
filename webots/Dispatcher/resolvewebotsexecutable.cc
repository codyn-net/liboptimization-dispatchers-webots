#include "dispatcher.ih"

string Dispatcher::resolveWebotsExecutable(std::string const &path)
{
	string ret = Glib::find_program_in_path(path);

	if (ret == "")
	{
		cerr << "Could not find webots executable: " << path << endl;
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
		cerr << "Webots executable does not exist: "  << ret << endl;
		return "";
	}

	struct passwd *pwd = getpwuid(getuid());
	string homedir = pwd->pw_dir;

	if (buf.st_uid != getuid())
	{
		cerr << "Custom webots executable is not owned by the used" << endl;
		return "";
	}
	else if (String(ret).startsWith(homedir))
	{
		cerr << "Custom webots executable is not in user home directory" << endl;
		return "";
	}
	else
	{
		return ret;
	}
}
