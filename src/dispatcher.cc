/*
 * dispatcher.cc
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


#include "dispatcher.hh"

#include <optimization/messages.hh>
#include <glibmm.h>
#include <jessevdk/base/base.hh>
#include <signal.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>

#include "config.hh"

using namespace std;
using namespace webots;
using namespace jessevdk::os;
using namespace jessevdk::network;
using namespace optimization::messages;
using namespace jessevdk::base;

size_t Dispatcher::PingTimeoutSeconds = 30;
size_t Dispatcher::KillTimeoutSeconds = 2;

Dispatcher::~Dispatcher()
{
	Kill();
}

void
Dispatcher::Stop()
{
	if (d_webotsOutputData != "")
	{
		cerr << d_webotsOutputData << endl;
		d_webotsOutputData = "";
	}

	Kill();
}

Dispatcher::Dispatcher()
:
	d_hasResponse(false),
	d_pid(0),
	d_stopping(false),
	d_pidBuilder(0)
{
	Config::Initialize(PREFIXDIR "/libexec/liboptimization-dispatchers-2.0/webots.conf");
}

Dispatcher::Override::Override()
:
	Value(""),
	Overridden(false)
{
}

Dispatcher::Override::Override(string const &value)
:
	Value(value),
	Overridden(false)
{
}

void
Dispatcher::KillWebots()
{
	if (d_pid == 0)
	{
		return;
	}

	GPid pid = d_pid;

	d_pid = 0;
	cerr << "Killing webots ourselves" << endl;

	d_terminator.Terminate(pid, true, false);
}

void
Dispatcher::KillBuilder()
{
	GPid pid = d_pidBuilder;

	// We don't want the builder to launch webots
	d_pidBuilder = 0;

	d_terminator.Terminate(pid, true, false);
}

void
Dispatcher::Kill()
{
	d_stopping = true;

	if (d_pidBuilder != 0)
	{
		KillBuilder();
	}
	else
	{
		KillWebots();
	}
}

bool
Dispatcher::Mode(string &m) const
{
	if (Setting("mode", m))
	{
		return true;
	}

	return false;
}

bool
Dispatcher::OnData(FileDescriptor::DataArgs &args)
{
	vector<task::Communication> comm;
	vector<task::Communication>::iterator iter;

	optimization::Messages::Extract(args, comm);

	for (iter = comm.begin(); iter != comm.end(); ++iter)
	{
		switch (iter->type())
		{
			case task::Communication::CommunicationResponse:
				d_hasResponse = true;

				WriteResponse(iter->response());

				if (!d_killTimeout && !d_stopping)
				{
					d_killTimeout = Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &Dispatcher::OnKillTimeout), KillTimeoutSeconds);
				}
			break;
			case task::Communication::CommunicationPing:
				if (d_pingTimeout)
				{
					d_pingTimeout.disconnect();
				}

				if (!d_killTimeout && d_stopping)
				{
					d_pingTimeout = Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &Dispatcher::OnPingTimeout), PingTimeoutSeconds);
				}
			break;
			default:
			break;
		}
	}

	return false;
}

bool
Dispatcher::OnWebotsData(FileDescriptor::DataArgs &args)
{
	d_webotsOutputData += args.data;
	return false;
}

bool
Dispatcher::OnWebotsError(FileDescriptor::DataArgs &args)
{
	d_webotsOutputData += args.data;
	return false;
}

bool
Dispatcher::OnBuilderData(FileDescriptor::DataArgs &args)
{
	d_builderText += args.data;
	return false;
}

bool
Dispatcher::OnBuilderError(FileDescriptor::DataArgs &args)
{
	d_builderErrorData += args.data;
	return false;
}

bool
Dispatcher::OnNewConnection(Client &connection)
{
	connection.OnData().Add(*this, &Dispatcher::OnData);

	// Send it the request
	task::Communication comm;
	comm.set_type(task::Communication::CommunicationTask);

	*(comm.mutable_task()) = Task();

	string serialized;

	if (optimization::Messages::Create(comm, serialized))
	{
		connection.Write(serialized);
	}

	if (!d_pingTimeout)
	{
		d_pingTimeout = Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &Dispatcher::OnPingTimeout), PingTimeoutSeconds);
	}

	return false;
}

bool
Dispatcher::OnKillTimeout()
{
	// Close webots
	Kill();
	return false;
}

bool
Dispatcher::OnPingTimeout()
{
	// Close webots
	cerr << "Did not receive ping, fail!" << endl;
	d_pingTimeout.disconnect();

	Stop();

	return false;
}

void
Dispatcher::Cleanup()
{
	// Cleanup temporary directory
	FileSystem::Remove(d_tmpHome, true);
	FileSystem::Remove(d_socketFile);

	if (d_builderText != "")
	{
		FileSystem::Remove(String(d_builderText).Strip());
		FileSystem::Remove("." + String(d_builderText).Strip() + ".project");
	}
}

void
Dispatcher::OnWebotsKilled(GPid pid, int ret)
{
	if (d_webotsOutputData != "")
	{
		cerr << d_webotsOutputData << endl;
		d_webotsOutputData = "";
	}

	if (!d_hasResponse)
	{
		cerr << "webots was killed without response: " << pid << " " << ret << endl;
	}

	Glib::spawn_close_pid(pid);

	d_server.Close();

	if (d_pingTimeout)
	{
		d_pingTimeout.disconnect();
	}

	if (d_killTimeout)
	{
		d_killTimeout.disconnect();
	}

	d_pid = 0;

	Cleanup();

	// Quit the main loop
	optimization::Dispatcher::Stop();
}

void
Dispatcher::OnBuilderKilled(GPid pid, int ret)
{
	Glib::spawn_close_pid(pid);

	if (d_builderErrorData != "")
	{
		cerr << "Builder error: " << d_builderErrorData << endl;
	}

	if (d_pidBuilder == 0)
	{
		return;
	}

	if (d_stopping)
	{
		Main()->quit();
	}

	d_pidBuilder = 0;
	d_builderPipe.ReadEnd().Close();

	if (!LaunchWebots())
	{
		d_server.Close();

		Cleanup();

		Main()->quit();
	}
}

string
Dispatcher::ResolveWebotsExecutable(std::string const &path)
{
	string ret = Glib::find_program_in_path(path);
	Config &config = Config::Instance();

	if (ret == "")
	{
		cerr << "Could not find webots executable: " << path << endl;
		return ret;
	}

	if (!config.Secure)
	{
		return ret;
	}

	// System webots is fine
	if (String(ret).StartsWith("/usr"))
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
		cerr << "Custom webots executable is not owned by the user: " << ret << endl;
		return "";
	}
	else if (!String(ret).StartsWith(homedir))
	{
		cerr << "Custom webots executable is not in user home directory: " << ret << endl;
		return "";
	}
	else
	{
		return ret;
	}
}

void
Dispatcher::PrepareWebotsRC(string const &source, string const &dest)
{
	fstream instr(source.c_str(), ios::in);

	if (!instr)
	{
		instr.open(DATADIR "/liboptimization2-dispatchers-webots/webotsrc", ios::in);
	}

	if (!instr)
	{
		return;
	}

	fstream outstr(dest.c_str(), ios::out);

	if (!outstr)
	{
		return;
	}

	map<string, Override>::iterator iter;

	for (iter = d_overrides.begin(); iter != d_overrides.end(); ++iter)
	{
		iter->second.Overridden = false;
	}

	string line;

	while (getline(instr, line))
	{
		vector<string> parts = String(line).Split(":", 2);

		if (parts.size() == 2)
		{
			iter = d_overrides.find(parts[0]);

			if (iter != d_overrides.end())
			{
				iter->second.Overridden = true;
				outstr << parts[0] << ": " << iter->second.Value << endl;
			}
			else
			{
				outstr << line << endl;
			}
		}
		else
		{
			outstr << line << endl;
		}
	}

	for (iter = d_overrides.begin(); iter != d_overrides.end(); ++iter)
	{
		if (!iter->second.Overridden)
		{
			outstr << iter->first << ": " << iter->second.Value << endl;
		}
	}

	outstr.close();
	instr.close();
}

bool
Dispatcher::RunTask()
{
	int f = Glib::file_open_tmp(d_socketFile, "optimization");

	if (f == -1)
	{
		return false;
	}

	InitRCOverrides();

	::close(f);
	::unlink(d_socketFile.c_str());

	map<string, string> envp = Environment::All();

	string environment;
	if (Setting("environment", environment))
	{
		vector<string> vars = String(environment).Split(",");

		for (vector<string>::iterator iter = vars.begin(); iter != vars.end(); ++iter)
		{
			vector<string> parts = String(*iter).Split("=", 2);

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

	string oldhome = "";
	Environment::Variable("HOME", oldhome);

	envp["OLDHOME"] = oldhome;

	if (f != -1)
	{
		::close(f);
		::unlink(d_tmpHome.c_str());
	}

	FileSystem::Mkdirs(d_tmpHome);

	// Copy .webotsrc and override some things
	string source = Glib::build_filename(Glib::get_home_dir(), ".webotsrc");
	string dest = Glib::build_filename(d_tmpHome, ".webotsrc");

	PrepareWebotsRC(source, dest);

	// Copy over .Xauthority
	source = Glib::build_filename(Glib::get_home_dir(), ".Xauthority");
	dest = Glib::build_filename(d_tmpHome, ".Xauthority");

	FileSystem::Copy(source, dest);

	d_server = UnixServer(d_socketFile);

	if (!d_server.Listen())
	{
		return false;
	}

	d_server.OnNewConnection().Add(*this, &Dispatcher::OnNewConnection);

	d_environment = Environment::Convert(envp);
	string builder;

	if (ResolveBuilderPath(builder))
	{
		return LaunchWorldBuilder(builder);
	}
	else
	{
		return LaunchWebots();
	}
}

bool
Dispatcher::ResolveBuilderPath(string &builder)
{
	string path;

	if (!Setting("worldBuilderPath", path))
	{
		return false;
	}

	builder = Glib::find_program_in_path(path);
	Config &config = Config::Instance();

	if (builder == "")
	{
		cerr << "Could not find world builder executable: " << path << endl;
		return false;
	}

	if (!config.Secure)
	{
		return true;
	}

	// System webots is fine
	if (String(builder).StartsWith("/usr"))
	{
		return true;
	}

	// Otherwise, must be owned by the user, and in his/her home directory
	struct stat buf;

	if (stat(builder.c_str(), &buf) != 0)
	{
		cerr << "World builder executable does not exist: "  << builder << endl;
		return false;
	}

	struct passwd *pwd = getpwuid(getuid());
	string homedir = pwd->pw_dir;

	if (buf.st_uid != getuid())
	{
		cerr << "World builder is not owned by the user: " << builder << endl;
		return false;
	}
	else if (!String(builder).StartsWith(homedir))
	{
		cerr << "World builder is not in user home directory: " << builder << endl;
		return false;
	}

	return true;
}

bool
Dispatcher::LaunchWorldBuilder(string const &builder)
{
	vector<string> argv;
	argv.push_back(builder);

	int sin;
	int sout;
	int serr;

	try
	{
		Glib::spawn_async_with_pipes(d_tmpHome,
		                             argv,
		                             d_environment,
		                             Glib::SPAWN_DO_NOT_REAP_CHILD |
		                             Glib::SPAWN_SEARCH_PATH,
		                             sigc::slot<void>(),
		                             &d_pidBuilder,
		                             &sin,
		                             &sout,
		                             &serr);
	}
	catch (Glib::SpawnError &e)
	{
		cerr << "Error while spawning world maker: " << e.what() << endl;
		return false;
	}

	Glib::signal_child_watch().connect(sigc::mem_fun(*this, &Dispatcher::OnBuilderKilled), d_pidBuilder, Glib::PRIORITY_LOW);

	// Write task
	string serialized;
	optimization::messages::task::Communication comm;

	comm.set_type(optimization::messages::task::Communication::CommunicationTask);
	(*comm.mutable_task()) = Task();

	optimization::Messages::Create(comm, serialized);

	d_builderError = FileDescriptor(serr);
	d_builderError.Attach();
	d_builderError.OnData().Add(*this, &Dispatcher::OnBuilderError);

	d_builderPipe = Pipe(sout, sin);
	d_builderPipe.ReadEnd().OnData().Add(*this, &Dispatcher::OnBuilderData);

	d_builderPipe.WriteEnd().Write(serialized);
	d_builderPipe.WriteEnd().Close();

	return true;
}

bool
Dispatcher::LaunchWebots()
{
	string wd;

	if (!World(wd))
	{
		return false;
	}

	// Launch webots
	vector<string> argv;
	string path = WebotsPath();

	path = ResolveWebotsExecutable(path);

	if (path == "")
	{
		cerr << "Could not find webots executable" << endl;
		return false;
	}

	Config &config = Config::Instance();

	string ver;

	if (Setting("webotsVersion", ver))
	{
		config.WebotsVersion = ver;
	}

	argv.push_back(path);
	string md;

	bool forceBatch = Config::Instance().ForceBatch;
	size_t version[3];
	config.WebotsNumericVersion(version);

	bool isbatch = false;

	if (Mode(md) && !forceBatch)
	{
		if (md == "" || md == "run")
		{
			argv.push_back("--mode=run");
		}
		else if (md == "fast")
		{
			argv.push_back("--mode=fast");
		}
		else if (md == "batch" || md == "minimize")
		{
			if (version[1] >= 2)
			{
				argv.push_back("--minimize");
			}
			else
			{
				argv.push_back("--batch");
			}

			argv.push_back("--mode=fast");
			isbatch = true;
		}
		else if (md == "stop")
		{
			argv.push_back("--mode=stop");
		}
	}
	else if (forceBatch)
	{
		if (version[1] >= 2)
		{
			argv.push_back("--minimize");
		}
		else
		{
			argv.push_back("--batch");
		}

		argv.push_back("--mode=fast");
		isbatch = true;
	}
	else
	{
		argv.push_back("--mode=run");
	}

	// Redirect both STDOUT and STDERR so we can pass them along, this is
	// not reliable on < 6.2
	if (version[1] >= 2 && isbatch && config.ForwardOutput)
	{
		d_environment.push_back("WEBOTS_STDOUT=1");
		d_environment.push_back("WEBOTS_STDERR=1");
	}

	argv.push_back(wd);

	int serr;
	int sout;

	try
	{
		Glib::spawn_async_with_pipes(d_tmpHome,
		                             argv,
		                             d_environment,
		                             Glib::SPAWN_DO_NOT_REAP_CHILD |
		                             Glib::SPAWN_SEARCH_PATH,
		                             sigc::slot<void>(),
		                             &d_pid,
		                             0,
		                             &sout,
		                             &serr);
	}
	catch (Glib::SpawnError &e)
	{
		cerr << "Error while spawning webots: " << e.what() << endl;
		return false;
	}

	d_webotsError = FileDescriptor(serr);
	d_webotsError.Attach();
	d_webotsError.OnData().Add(*this, &Dispatcher::OnWebotsError);

	d_webotsOutput = FileDescriptor(sout);
	d_webotsOutput.Attach();
	d_webotsOutput.OnData().Add(*this, &Dispatcher::OnWebotsData);

	Glib::signal_child_watch().connect(sigc::mem_fun(*this, &Dispatcher::OnWebotsKilled), d_pid, Glib::PRIORITY_LOW);
	return true;
}

string
Dispatcher::WebotsPath() const
{
	string val;

	if (Setting("webotsPath", val))
	{
		return val;
	}
	else if (Setting("webotsVersion", val))
	{
		string path = string("webots") + String(val).Replace(".", "");

		if (Glib::find_program_in_path(path) != "")
		{
			return path;
		}
	}

	return "webots";
}

bool
Dispatcher::World(string &w) const
{
	string set;
	Config &config = Config::Instance();

	if (d_builderText != "")
	{
		set = String(d_builderText).Strip();
	}
	else if (!Setting("world", set))
	{
		cerr << "Webots world is not set, please provide the path to the webots world file in the dispatcher settings..." << endl;
		return false;
	}

	string resolved;

	if (!FileSystem::Realpath(set, resolved))
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

	if (!config.Secure)
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
	else if (!String(resolved).StartsWith(home))
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

void
Dispatcher::InitRCOverrides()
{
	d_overrides["displayWelcomeDialog"] = Override("FALSE");
	d_overrides["version"] = Override(Config::Instance().WebotsVersion);

	String overrides = Config::Instance().RCOverrides;

	vector<string> parts = overrides.Split(";");

	for (vector<string>::iterator iter = parts.begin(); iter != parts.end(); ++iter)
	{
		vector<string> val = String(*iter).Split("=", 2);

		if (val.size() == 2)
		{
			d_overrides[val[0]] = Override(val[1]);
		}
	}
}
