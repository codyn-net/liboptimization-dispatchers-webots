#include "dispatcher.ih"

bool Dispatcher::onData(os::FileDescriptor::DataArgs &args) 
{
	vector<worker::Response> response;
	vector<worker::Response>::iterator iter;
	
	optimization::Messages::extract(args, response);
	
	for (iter = response.begin(); iter != response.end(); ++iter)
	{
		writeResponse(*iter);
		
		if (!setting("multiResponse")  && !d_timeout)
		{
			// If we only expect one response, set a timeout just to make
			// sure we actually kill webots
			d_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Dispatcher::onTimeout), 2000);
		}
	}
	
	return false;
}
