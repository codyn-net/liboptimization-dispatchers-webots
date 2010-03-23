#ifndef __WEBOTS_DISPATCHER_H__
#define __WEBOTS_DISPATCHER_H__

#include <optimization/dispatcher.hh>
#include <jessevdk/network/network.hh>
#include <jessevdk/os/os.hh>

namespace webots
{
	class Dispatcher : public optimization::Dispatcher
	{
		jessevdk::network::UnixServer d_server;
		sigc::connection d_timeout;
		Glib::Pid d_pid;

		std::string d_socketFile;
		std::string d_tmpHome;
		std::vector<std::string> d_environment;

		Glib::Pid d_pidBuilder;
		jessevdk::os::Pipe d_builderPipe;
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

			bool OnData(jessevdk::os::FileDescriptor::DataArgs &args);
			bool OnNewConnection(jessevdk::network::Client &connection);
			
			bool OnBuilderData(jessevdk::os::FileDescriptor::DataArgs &args);

			std::string ResolveWebotsExecutable(std::string const &path);

			void KillAll(GPid parent);
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
