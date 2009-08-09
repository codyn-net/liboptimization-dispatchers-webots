#include "dispatcher.ih"

bool Dispatcher::runSolution() 
{
	vector<vector<double> > solutions = extractSolutions();
	worker::Request::Dispatch const &dispatch = request();

	/* Calculate maximum possible distance */
	double ddmax = 0;
	
	for (int i = 0; i < dispatch.parameters_size(); ++i)
	{
		worker::Request::Dispatch::Parameter const &parameter = dispatch.parameters(i);
		double mma = parameter.max() * parameter.max();
		double mmi = parameter.min() * parameter.min();
		
		ddmax += math::max(mma, mmi);
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
	
	worker::Response response;
	
	response.set_id(0);
	
	worker::Response::Fitness *f = response.add_fitness();
	f->set_name("fitness");
	f->set_value(finalfitness);

	response.set_status(worker::Response::Success);

	writeResponse(response);
	return true;
}
