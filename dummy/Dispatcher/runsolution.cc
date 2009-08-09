#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	worker::Response response;
	
	response.set_id(0);
	
	worker::Response::Fitness *fitness = response.add_fitness();
	fitness->set_name("fitness");
	fitness->set_value(0);

	response.set_status(worker::Response::Success);

	writeResponse(response);
	return true;
}
