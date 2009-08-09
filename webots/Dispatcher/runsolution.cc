#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	/* Launch webots */
	vector<string> argv;
	string path = webotsPath();
	
	if (path != "")
		argv.push_back(path);
	else
		argv.push_back("webots");
	
	if (noDisplay())
		argv.push_back("--batch");

	string mode;

	if (this->mode(mode))
		argv.push_back(mode);
	
	string world;

	if (this->world(world))
	{
		argv.push_back(world);
	}
	else
	{
		cerr << "** [Dispatcher] Webots world is not set, please provide the path to the webots world file in the dispatcher settings..." << endl;
		return false;
	}
	
	int f = Glib::file_open_tmp(d_socketFile, "optimization");
	
	if (f == -1)
		return false;
	
	::close(f);
	::unlink(d_socketFile.c_str());

	map<string, string> envp = Environment::all();
	
	string environment;
	if (setting("environment", environment))
	{
		vector<string> vars = String(environment).split(",");
		
		for (vector<string>::iterator iter = vars.begin(); iter != vars.end(); ++iter)
		{
			vector<string> parts = String(*iter).split("=", 2);
			
			if (parts.size() == 2)
			{
				envp[parts[0]] = parts[1];
			}
			else if (parts.size() == 1)
			{
				envp[parts[0]] = "";
			}
		}
	}

	envp["OPTIMIZATION_UNIX_SOCKET"] = d_socketFile;
	
	// We make a temporary home directory because there is a race condition
	// somewhere which crashes webots when it is run on multiple pcs which
	// all mount the home directory from NFS
	f = Glib::file_open_tmp(d_tmpHome, "optimization-home");
	envp["HOME"] = d_tmpHome;
	
	if (f != -1)
	{
		::close(f);
		::unlink(d_tmpHome.c_str());
	}

	FileSystem::mkdirs(d_tmpHome);
	
	// Copy .webotsrc just because that's the nice thing to do
	string source = Glib::build_filename(Glib::get_home_dir(), ".webotsrc");
	string dest = Glib::build_filename(d_tmpHome, ".webotsrc");

	FileSystem::copy(source, dest);

	// Copy over .Xauthority
	source = Glib::build_filename(Glib::get_home_dir(), ".Xauthority");
	dest = Glib::build_filename(d_tmpHome, ".Xauthority");

	FileSystem::copy(source, dest);

	d_server = UnixServer(d_socketFile);
	
	if (!d_server.listen())
		return false;

	d_server.onNewConnection().add(*this, &Dispatcher::onNewConnection);
	int serr;

	try
	{
		Glib::spawn_async_with_pipes(d_tmpHome,
		                  argv,
		                  Environment::convert(envp),
		                  Glib::SPAWN_DO_NOT_REAP_CHILD |
		                  Glib::SPAWN_SEARCH_PATH,
		                  sigc::slot<void>(),
		                  &d_pid,
				  0,
				  &serr,
				  0);
	}
	catch (Glib::SpawnError &e)
	{
		cerr << "** [Dispatcher] Error while spawning webots: " << e.what() << endl;
		return false;
	}
	
	Glib::signal_child_watch().connect(sigc::mem_fun(*this, &Dispatcher::onWebotsKilled), d_pid);

	/* Install timeout */
	size_t tm;
	if (timeout(tm) && tm > 0)
	{
		d_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Dispatcher::onTimeout),
		                                           tm * 1000);
	}
	
	return true;
}
