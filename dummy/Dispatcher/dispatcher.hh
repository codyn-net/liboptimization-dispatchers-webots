#ifndef __DUMMY_DISPATCHER_H__
#define __DUMMY_DISPATCHER_H__

#include <dispatcher/Dispatcher/dispatcher.hh>

namespace dummy
{
	class Dispatcher : public worker::dispatcher::Dispatcher
	{
		public:
			bool runSolution();
		protected:
			virtual bool useMainLoop() const;
	};
	
	inline bool Dispatcher::useMainLoop() const
	{
		return false;
	}
}

#endif /* __DUMMY_DISPATCHER_H__ */
