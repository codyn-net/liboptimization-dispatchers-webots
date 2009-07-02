#include "dispatcher.ih"

vector<vector<double> > Dispatcher::extractSolutions() 
{
	/* Get 'solutions' setting if any */
	string str;
	
	size_t numbounds = request().parameters_size();

	vector<vector<double> > empty;
	vector<double> items(numbounds, 0);
	empty.push_back(items);
		
	if (setting("solution", str))
	{
		vector<string> parts = String(str).split(" ");

		if (parts.size() % numbounds == 0)
		{
			vector<vector<double> > ret;

			size_t num = parts.size() / numbounds;
			size_t idx = 0;

			for (size_t i = 0; i < num; ++i)
			{
				vector<double> items;

				for (size_t j = 0; j < numbounds; ++j)
				{
					stringstream ext(parts[idx++]);
					double ptr = 0;

					if (!(ext >> ptr))
						return empty;

					items.push_back(ptr);
				}

				ret.push_back(items);
			}
			
			return ret;
		}
	}
	
	return empty;
}
