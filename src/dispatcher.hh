#ifndef __WEBOTS_DISPATCHER_H__
#define __WEBOTS_DISPATCHER_H__

#include <optimization/dispatcher.hh>
#include <network/UnixServer/unixserver.hh>
#include <network/Client/client.hh>
#include <os/Pipe/pipe.hh>

namespace webots
{
	class Dispatcher : public optimization::Dispatcher
	{
		network::UnixServer d_server;
		sigc::connection d_timeout;
		Glib::Pid d_pid;

		std::string d_socketFile;
		std::string d_tmpHome;
		std::vector<std::string> d_environment;

		Glib::Pid d_pidBuilder;
		os::Pipe d_builderPipe;
		std::string d_builderText;

		public:
			Dispatcher();
			~Dispatcher();

			/* Public functions */
			virtual bool RunTask();
			virtual void Stop();
		protected:
			virtual std::string WebotsPath() const;
			virtual bool Mode(std::string &m) const;
			
			virtual bool World(std::string &w) const;
			
		private:
			/* Private functions */
			void KillWebots();
			void OnWebotsKilled(GPid pid, int ret);
			void OnBuilderKilled(GPid pid, int ret);
			
			bool LaunchWebots();
			bool LaunchWorldBuilder(std::string const &builder);
			bool ResolveBuilderPath(std::string &builder);
			
			bool OnTimeout();

			bool OnData(os::FileDescriptor::DataArgs &args);
			bool OnNewConnection(network::Client &connection);
			
			bool OnBuilderData(os::FileDescriptor::DataArgs &args);

			std::string ResolveWebotsExecutable(std::string const &path);
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
