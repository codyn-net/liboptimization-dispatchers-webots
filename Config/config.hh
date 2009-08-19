#ifndef __WEBOTS_CONFIG_H__
#define __WEBOTS_CONFIG_H__

#include <base/Config/config.hh>
#include <glibmm.h>

namespace webots
{
	class Config : public base::Config
	{
		static Config *s_instance;

		public:
			bool secure;
			
			/* Constructor/destructor */
			static Config &initialize(std::string const &filename);
			static Config &instance();
		
			/* Public functions */
		private:
			/* Private functions */
			Config();
	};
}

#endif /* __WEBOTS_CONFIG_H__ */
