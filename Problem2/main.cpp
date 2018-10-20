#include <map>
#include <vector>
#include <iostream>
#include <set>
#include <queue>
#include <cassert>


#define MOD_VALUE 998244353


struct Currency;


typedef unsigned long long quantity_t;

struct Conversion
{
	Currency* currency;
	quantity_t rate;

	Conversion() : Conversion(nullptr, 0)
	{

	}

	Conversion(Currency* currency, quantity_t rate) : currency(currency), rate(rate)
	{

	}
};




struct Currency
{
	std::string Name;
	std::map<Currency*, Conversion> conversions;
};



struct Value
{
	long Scalar;
	Currency* currency;
};

template <class Cost, class T>
struct cost_container
{
	Cost cost;
	T value;
};

template <class Cost, class T>
struct cost_container_compare
{
	typedef cost_container<Cost, T> value_type;
	
	bool operator()(const value_type& lhs, const value_type& rhs) const noexcept
	{
		return lhs.cost > rhs.cost;
	}
};

template <class Node, class Cost>
Cost djikstra_search(Node* start, Node* end, bool* success)
{
	if (success)
	{
		*success = false;
	}
	std::priority_queue<cost_container<Cost, Node*>, std::vector<cost_container<Cost, Node*>>, cost_container_compare<Cost, Node*>> nodes;


	nodes.push(cost_container<Cost, Node*> { 1, start });
	nodes.push(cost_container<Cost, Node*> { 100, end });
	assert(nodes.top().value == start);


	nodes.push(cost_container<Cost, Node*> { 1, start });
	
	std::set<Node*> checked;
	checked.insert(start);

	Node* current = start;
	Cost currentCost;
	while (!nodes.empty())
	{
		auto next = nodes.top();
		nodes.pop();
		current = next.value;
		currentCost = next.cost;


		for (const auto& edge : current->conversions)
		{
			if (edge.first == end)
			{
				if (success)
				{
					*success = true;
				}
				return currentCost*edge.second.rate;
			}
			else if (!checked.count(edge.first))
			{
				nodes.push(cost_container<Cost, Node*> { currentCost*edge.second.rate, edge.first });
				checked.insert(edge.first);
			}
		}
	}
	
	return 0;
}

int main()
{


	struct ParsedConversion
	{
		std::string currency1, currency2;
		quantity_t rate;
		Currency *obj1, *obj2;
	};

	struct ParsedQuery
	{
		std::string currency1, currency2;
	};

	std::vector<ParsedConversion> conversions;

	std::string tmp;
	std::getline(std::cin, tmp);
	int count = std::stoi(tmp);
	for (int i = 0; i < count; ++i)
	{
		ParsedConversion conversion;
		conversion.obj1 = conversion.obj2 = nullptr;
		std::getline(std::cin, tmp);
		size_t j;
		for (j = 0; j < tmp.length(); ++j)
		{
			if (tmp[j] == ' ')
			{
				conversion.currency1 = tmp.substr(0, j);
				break;
			}
		}

		size_t k;
		for (k = j+1; k < tmp.length(); ++k)
		{
			if (tmp[k] == ' ')
			{
				conversion.currency2 = tmp.substr(j+1, (k - j - 1));
				break;
			}
		}

		tmp = tmp.substr(k+1);
		conversion.rate = strtoull(tmp.c_str(), nullptr, 10);
		conversions.push_back(conversion);
	}

	std::map<std::string, Currency*> currencies;
	for (auto& line : conversions)
	{
		Currency *obj1, *obj2;

		obj1 = currencies[line.currency1];
		if (!obj1)
		{
			obj1 = new Currency { line.currency1 };
			currencies[line.currency1] = obj1;
		}

		obj2 = currencies[line.currency2];
		if (!obj2)
		{
			obj2 = new Currency { line.currency2 };
			currencies[line.currency2] = obj2;
		}

		obj1->conversions[obj2] = Conversion(obj2, line.rate);
	}


	std::getline(std::cin, tmp);
	count = std::stoi(tmp);

	std::vector<ParsedQuery> queries;

	for (int i = 0; i < count; ++i)
	{
		ParsedQuery query;
		std::cin >> query.currency1;
		std::cin >> query.currency2;
		queries.push_back(query);
	}



	



	for (auto& query : queries)
	{
		auto start = currencies[query.currency1];
		auto end = currencies[query.currency2];

		if (start == nullptr || end == nullptr)
		{
			std::cout << -1 << std::endl;
			continue;
		}

		if (start == end)
		{
			std::cout << 1 << std::endl;
			continue;
		}

		bool success;
		auto result = djikstra_search<Currency, quantity_t>(start, end, &success);
		if (success)
		{
			std::cout << (result % MOD_VALUE) << std::endl;
		}
		else
		{
			std::cout << -1 << std::endl;
		}
	}
}
