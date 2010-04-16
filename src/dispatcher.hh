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
		sigc::connection d_pingTimeout;
		sigc::connection d_killTimeout;
		Glib::Pid d_pid;
		bool d_stopping;

		std::string d_socketFile;
		std::string d_tmpHome;
		std::vector<std::string> d_environment;

		Glib::Pid d_pidBuilder;
		jessevdk::os::Pipe d_builderPipe;
		std::string d_builderText;

		jessevdk::os::Terminator d_terminator;

		static size_t PingTimeoutSeconds;
		static size_t KillTimeoutSeconds;

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
			void Kill();
			void KillWebots();
			void KillBuilder();

			void OnWebotsKilled(GPid pid, int ret);
			void OnBuilderKilled(GPid pid, int ret);
			
			bool LaunchWebots();
			bool LaunchWorldBuilder(std::string const &builder);
			bool ResolveBuilderPath(std::string &builder);
			
			bool OnKillTimeout();
			bool OnPingTimeout();

			bool OnData(jessevdk::os::FileDescriptor::DataArgs &args);
			bool OnNewConnection(jessevdk::network::Client &connection);
			
			bool OnBuilderData(jessevdk::os::FileDescriptor::DataArgs &args);

			std::string ResolveWebotsExecutable(std::string const &path);
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
