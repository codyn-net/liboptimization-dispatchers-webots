#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	WorkerResponse response;
	
	response.set_id(0);
	response.add_fitness(0);
	response.set_status(WorkerResponse::Success);

	writeResponse(response);
	return true;
}
