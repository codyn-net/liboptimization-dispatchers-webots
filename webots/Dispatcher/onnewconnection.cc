#include "dispatcher.ih"

bool Dispatcher::onNewConnection(network::Client &connection) 
{
	connection.onData().add(*this, &Dispatcher::onData);
	
	/* Send it the request */
	string serialized;

	if (optimization::Messages::create(request(), serialized))
		connection.write(serialized);
	
	return false;
}
