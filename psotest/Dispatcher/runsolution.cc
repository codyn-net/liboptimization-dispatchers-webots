#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	vector<vector<double> > solutions = extractSolutions();
	WorkerRequest::Dispatch const &dispatch = request();

	/* Calculate maximum possible distance */
	double ddmax = 0;
	
	for (int i = 0; i < dispatch.parameters_size(); ++i)
	{
		WorkerRequest::Dispatch::Parameter const &parameter = dispatch.parameters(i);
		double mm = parameter.max() - parameter.min();
		
		ddmax += mm * mm;
	}
	
	ddmax = ::sqrt(ddmax);
	double finalfitness = 0;
	
	for (size_t j = 0; j < solutions.size(); ++j)
	{
		double fitness = 0;
		
		for (int i = 0; i < dispatch.parameters_size(); ++i)
		{
			double dd = solutions[j][i] - dispatch.parameters(i).value();
			fitness += dd * dd;
		}
		
		fitness = ddmax - ::sqrt(fitness);
		
		if (fitness > finalfitness)
		{
			finalfitness = fitness;
		}
	}
	
	WorkerResponse response;
	
	response.set_id(0);
	response.add_fitness(finalfitness);
	response.set_status(WorkerResponse::Success);

	writeResponse(response);
	return true;
}
