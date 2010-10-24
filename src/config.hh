#ifndef __WEBOTS_CONFIG_H__
#define __WEBOTS_CONFIG_H__

#include <jessevdk/base/base.hh>
#include <glibmm.h>

namespace webots
{
	class Config : public jessevdk::base::Config
	{
		static Config *s_instance;
		size_t d_version[3];

		public:
			bool Secure;
			std::string WebotsVersion;
			bool ForceBatch;
			std::string RCOverrides;
			bool ForwardOutput;

			/* Constructor/destructor */
			static Config &Initialize(std::string const &filename);
			static Config &Instance();

			/* Public functions */
			void WebotsNumericVersion(size_t version[3]);
		private:
			/* Private functions */
			Config();
	};
}

#endif /* __WEBOTS_CONFIG_H__ */
