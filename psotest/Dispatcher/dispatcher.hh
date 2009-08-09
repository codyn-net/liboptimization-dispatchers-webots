#ifndef __PSOTEST_DISPATCHER_H__
#define __PSOTEST_DISPATCHER_H__

#include <optimization/Dispatcher/dispatcher.hh>

namespace psotest
{
	class Dispatcher : public optimization::Dispatcher
	{
		public:
			bool runSolution();
		protected:
			virtual bool useMainLoop() const;
		private:
			std::vector<std::vector<double> > extractSolutions();
	};
	
	inline bool Dispatcher::useMainLoop() const
	{
		return false;
	}
}

#endif /* __PSOTEST_DISPATCHER_H__ */
