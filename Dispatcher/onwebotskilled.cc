#include "dispatcher.ih"

void Dispatcher::onWebotsKilled(GPid pid, int ret) 
{
	cerr << "webots was killed " << pid << " " << ret << endl;
	Glib::spawn_close_pid(pid);
	
	d_server.close();
	
	if (d_timeout)
		d_timeout.disconnect();
	
	d_pid = 0;
	main()->quit();
	
	// Cleanup temporary directory
	FileSystem::remove(d_tmpHome, true);
	FileSystem::remove(d_socketFile);
}
