#ifndef __WEBOTS_DISPATCHER_H__
#define __WEBOTS_DISPATCHER_H__

#include <optimization/dispatcher.hh>
#include <jessevdk/network/network.hh>
#include <jessevdk/os/os.hh>

#include <map>

namespace webots
{
	class Dispatcher : public optimization::Dispatcher
	{
		struct Override
		{
			std::string Value;
			bool Overridden;

			Override();
			Override(std::string const &value);
		};

		bool d_hasResponse;

		std::map<std::string, Override> d_overrides;

		jessevdk::network::UnixServer d_server;
		sigc::connection d_pingTimeout;
		sigc::connection d_killTimeout;
		Glib::Pid d_pid;
		bool d_stopping;
		bool d_enablePing;

		std::string d_socketFile;
		std::string d_tmpHome;
		std::vector<std::string> d_environment;

		Glib::Pid d_pidBuilder;
		jessevdk::os::Pipe d_builderPipe;
		jessevdk::os::FileDescriptor d_builderError;
		std::string d_builderText;
		std::string d_builderErrorData;

		jessevdk::os::Terminator d_terminator;

		static size_t PingTimeoutSeconds;
		static size_t KillTimeoutSeconds;

		jessevdk::os::FileDescriptor d_webotsError;
		jessevdk::os::FileDescriptor d_webotsOutput;
		std::string d_webotsOutputData;

		bool d_hasBuilder;

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
			bool OnBuilderError(jessevdk::os::FileDescriptor::DataArgs &args);

			bool OnWebotsData(jessevdk::os::FileDescriptor::DataArgs &args);
			bool OnWebotsError(jessevdk::os::FileDescriptor::DataArgs &args);

			std::string ResolveWebotsExecutable(std::string const &path);

			void Cleanup();

			void InitRCOverrides();
			void PrepareWebotsRC(std::string const &source, std::string const &dest);

			bool EnablePing() const;
	};
}

#endif /* __WEBOTS_DISPATCHER_H__ */
