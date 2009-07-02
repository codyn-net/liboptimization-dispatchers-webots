#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	vector<vector<double> > solutions = extractSolutions();
	WorkerRequest::Dispatch const &dispatch = request();

	/* Calculate maximum possible distance */
	double ddmax = 0;
	
	for (int i = 0; i < dispatch.boundaries_size(); ++i)
	{
		WorkerRequest::Dispatch::Boundary const &boundary = dispatch.boundaries(i);
		double mm = boundary.max() - boundary.min();
		
		ddmax += mm * mm;
	}
	
	ddmax = ::sqrt(ddmax);
	double finalfitness = 0;
	
	for (size_t j = 0; j < solutions.size(); ++j)
	{
		double fitness = 0;
		
		for (int i = 0; i < dispatch.solution_size(); ++i)
		{
			double dd = solutions[j][i] - dispatch.solution(i);
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
