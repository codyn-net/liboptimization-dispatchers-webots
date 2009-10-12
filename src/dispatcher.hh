#ifndef __WEBOTS_DISPATCHER_H__
#define __WEBOTS_DISPATCHER_H__

#include <optimization/dispatcher.hh>
#include <network/UnixServer/unixserver.hh>
#include <network/Client/client.hh>

namespace webots
{
	class Dispatcher : public optimization::Dispatcher
	{
		network::UnixServer d_server;
		sigc::connection d_timeout;
		Glib::Pid d_pid;
		std::string d_socketFile;
		std::string d_tmpHome;

		public:
			Dispatcher();
			~Dispatcher();

			/* Public functions */
			virtual bool RunSolution();
		protected:
			virtual std::string WebotsPath() const;
			virtual bool Mode(std::string &m) const;
			virtual bool Timeout(size_t &tm) const;
			
			virtual bool World(std::string &w) const;
			
		private:
			/* Private functions */
			void KillWebots();
			void OnWebotsKilled(GPid pid, int ret);
			bool OnTimeout();

			bool OnData(os::FileDescriptor::DataArgs &args);
			bool OnNewConnection(network::Client &connection);

			std::string ResolveWebotsExecutable(std::string const &path);
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
