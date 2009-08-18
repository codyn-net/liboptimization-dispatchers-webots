#ifndef __WEBOTS_DISPATCHER_H__
#define __WEBOTS_DISPATCHER_H__

#include <optimization/Dispatcher/dispatcher.hh>
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
			virtual bool runSolution();
		protected:
			virtual std::string webotsPath() const;
			virtual bool mode(std::string &m) const;
			virtual bool timeout(size_t &tm) const;
			
			virtual bool world(std::string &w) const;
			
		private:
			/* Private functions */
			void killWebots();
			void onWebotsKilled(GPid pid, int ret);
			bool onTimeout();

			bool onData(os::FileDescriptor::DataArgs &args);
			bool onNewConnection(network::Client &connection);

			std::string resolveWebotsExecutable(std::string const &path);
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
