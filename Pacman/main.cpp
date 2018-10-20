#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <set>
#include <queue>

enum Object : char
{
	START = '1',
	BOB_START = '2',
	WALL = '#',
	EMPTY = '.',
	CHERRY = '@',
};

enum Move : char
{
	UP = 'U',
	DOWN = 'D',
	LEFT = 'L',
	RIGHT = 'R',
	WAIT = 'W',
};

enum Turn
{
	MINE = 0,
	JUDGE = 1,
};

struct Point
{
	int x, y;

	Point(int x, int y) : x(x), y(y)
	{

	}

	Point() : Point(0, 0)
	{

	}
};


bool operator<(Point lhs, Point rhs)
{
	if (lhs.x < rhs.x)
	{
		return true;
	}
	else if (lhs.x == rhs.x)
	{
		return lhs.y < rhs.y;
	}
	else
	{
		return false;
	}
}

bool operator==(Point lhs, Point rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}


std::ostream& operator<<(std::ostream& out, Point pt)
{
	return (out << "(" << pt.x << ", " << pt.y << ")");
}

struct Route
{
	std::vector<Point> points;

	void add(Point p)
	{
		points.push_back(p);
	}

	void add_front(Point p)
	{
		points.insert(points.begin(), p);
	}

	Point destination() const
	{
		assert(points.size() > 0);
		return points.back();
	}

	Point start() const
	{
		assert(points.size() > 0);
		return points.front();
	}

	Point operator[](size_t i) const
	{
		assert(points.size() > 0);
		return points[i+1];
	}

	size_t size() const noexcept
	{
		if (!points.empty())
		{
			return points.size()-1;
		}
		else
		{
			return 0;
		}
	}
};


struct Pathing
{
	struct PathNode
	{
		Point point;
		std::vector<PathNode*> path;

		std::map<Point, Route> routes;

		PathNode(Point p) : point(p), path(), routes()
		{

		}
	};

	std::map<Point, PathNode*> nodes;
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

struct GameMap;
Route djikstra_search(Point start, Point end, bool* success, const GameMap* map);




class GameMap
{
	private:

	public:
	int w, h;

	char** field;

	GameMap(int w, int h) : w(w), h(h), field(nullptr)
	{
		field = new char*[w];
		for (int x = 0; x < w; ++x)
		{
			field[x] = new char[h];
		}
	}

	
	Object operator[](Point p) const noexcept
	{
		return at(p);
	}

	Object at(Point p) const noexcept
	{
		return at(p.x, p.y);
	}

	Object at(int x, int y) const noexcept
	{
		if (x < 0 || y < 0 || x >= w || y >= h)
		{
			return WALL;
		}
		return (Object)field[x][y];
	}

	Point myStart() const noexcept
	{
		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j)
			{
				if (at(i, j) == START)
				{
					return Point(i, j);
				}
			}
		}
		
		assert(false);
	}

	Point judgeStart() const noexcept
	{
		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j)
			{
				if (at(i, j) == BOB_START)
				{
					return Point(i, j);
				}
			}
		}
		
		assert(false);
	}


	GameMap* clone() const
	{
		GameMap* other = new GameMap(w, h);

		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j)
			{
				other->field[i][j] = this->field[i][j];
			}
		}

		return other;
	}

	size_t reachable(Point origin, size_t moves, std::set<Point>& points) const
	{
		assert(origin.x <= w);
		assert(origin.y <= h);
		points.clear();
		std::queue<Point> q;
		std::queue<Point> q2;
		q.push(origin);
		points.insert(origin);

		
		Point tmp;
		for (size_t n = 0; n < moves; ++n)
		{
			while (!q.empty())
			{
				Point pt = q.front();
				q.pop();

				tmp = Point(pt.x + 1, pt.y);
				if (at(tmp) != WALL && points.count(tmp) == 0)
				{
					q2.push(tmp);
					points.insert(tmp);
				}

				tmp = Point(pt.x - 1, pt.y);
				if (at(tmp) != WALL && points.count(tmp) == 0)
				{
					q2.push(tmp);
					points.insert(tmp);
				}

				tmp = Point(pt.x, pt.y + 1);
				if (at(tmp) != WALL && points.count(tmp) == 0)
				{
					q2.push(tmp);
					points.insert(tmp);
				}

				tmp = Point(pt.x, pt.y - 1);
				if (at(tmp) != WALL && points.count(tmp) == 0)
				{
					q2.push(tmp);
					points.insert(tmp);
				}
			}

			q = q2;
		}

		return points.size();
	}

	std::set<Point> reachable(Point origin, size_t moves) const
	{
		std::set<Point> points;
		reachable(origin, moves, points);
		return points;
	}



	Route generate_route(Point start, Point end) const
	{
		
		bool success;
		Route r = djikstra_search(start, end, &success, this);


		return r;
	}

	std::vector<Point> cherries() const noexcept
	{
		std::vector<Point> cherries;

		for (auto i = 0; i < w; ++i)
		{
			for (auto j = 0; j < h; ++j)
			{
				if (at(i, j) == CHERRY)
				{
					cherries.emplace_back(i, j);
				}
			}
		}

		return cherries;
	}
};






struct Game
{
	int tick;
	Point me;
	Point judge;
	GameMap* map;
	Turn turn;

	int my_points;
	int judge_points;

	Game(GameMap* map) : tick(0), me(map->myStart()), judge(map->judgeStart()), map(map), turn(MINE), my_points(0), judge_points(0)
	{

	}

	

	Game copy() const noexcept
	{
		Game other(this->map->clone());
		other.tick = tick;
		other.me = me;
		other.judge = judge;
		other.turn = turn;
		other.my_points = my_points;
		other.judge_points = judge_points;
		return other;
	}
};


void build_pathing(GameMap*, Pathing*);
std::vector<Move> build_plan(GameMap*, Pathing*);




GameMap* read_map()
{
	int w, h;
	std::cin >> w;
	std::cin >> h;
	w = (2*w + 1);
	h = (2*h + 1);
	GameMap* field = new GameMap(w, h);

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			std::cin >> field->field[j][i];
		}
	}

	return field;
}

// // Pop DAT cherry
// int main()
// {
// 	int w, h;
// 	std::cin >> w;
// 	std::cin >> h;
// 	w = (2*w + 1);
// 	h = (2*h + 1);
// 	GameMap field(w, h);

// 	std::cout << w << ", " << h << std::endl;
	

	

// 	for (int i = 0; i < h; ++i)
// 	{
// 		for (int j = 0; j < w; ++j)
// 		{
// 			std::cin >> field.field[j][i];
// 		}
// 	}

// 	Pathing* pathing = new Pathing();
// 	build_pathing(&field, pathing);
// 	auto moves = build_plan(&field, pathing);

// 	for (auto move : moves)
// 	{
// 		std::cout << (char)move << std::endl;
// 	}




// 	return 0;


// 	auto game1 = new Game(&field);

	



// 	std::set<Point> points;
// 	field.reachable(field.myStart(), 4, points);

// 	for (auto& pt : points)
// 	{
// 		std::cout << "(" << pt.x << ", " << pt.y << ")" << std::endl;
// 	}

// }







// Route djikstra_search(Point start, Point end, bool* success, const GameMap* map)
// {
// 	if (success)
// 	{
// 		*success = false;
// 	}
// 	std::priority_queue<cost_container<int, Point>, std::vector<cost_container<int, Point>>, cost_container_compare<int, Point>> nodes;




// 	nodes.push(cost_container<int, Point> { 0, start });
	
// 	std::set<Point> checked;
// 	checked.insert(start);

// 	Point current = start;
// 	int currentCost;
// 	while (!nodes.empty())
// 	{
// 		auto next = nodes.top();
// 		nodes.pop();
// 		current = next.value;
// 		currentCost = next.cost;

// 		std::set<Point> neighbors;
// 		map->reachable(current, 1, neighbors);
// 		for (const auto& edge : neighbors)
// 		{
// 			if (edge == current)
// 			{
// 				continue;
// 			}

// 			if (edge == end)
// 			{
// 				if (success)
// 				{
// 					*success = true;
// 				}
// 				return currentCost + 1;
// 			}
// 			else if (!checked.count(edge))
// 			{
// 				nodes.push(cost_container<Cost, Node*> { currentCost + 1, edge.first });
// 				checked.insert(edge.first);
// 			}
// 		}
// 	}
	
// 	return 0;
// }



void add_neighbor_node(GameMap* map, Pathing* pathing, Pathing::PathNode* node, Point pt)
{
	if (map->at(pt) != WALL)
	{
		auto other_node = pathing->nodes[pt];
		assert(other_node);
		Route r;
		r.add(node->point);
		r.add(pt);
		node->routes[pt] = r;
	}
}

bool any_incomplete(Pathing* pathing)
{
	for (auto& node_pair : pathing->nodes)
	{
		auto node = node_pair.second;
		if (node->routes.size() < (pathing->nodes.size() - 1))
		{
			std::cout << "Incomplete Node: " << node->point << std::endl;

			std::cout << std::endl << std::endl;
			for (const auto& route_pair : node->routes)
			{
				std::cout << route_pair.first << std::endl;
			}
			std::cout << std::endl << std::endl;
			return true;
		}
	}

	return false;
}

void build_pathing(GameMap* map, Pathing* pathing)
{
	std::vector<Point> valid_points;
	for (int i = 0; i < map->w; ++i)
	{
		for (int j = 0; j < map->h; ++j)
		{
			if (map->at(i, j) != WALL)
			{
				valid_points.push_back(Point(i, j));
			}
		}
	}


	for (auto& pt : valid_points)
	{
		pathing->nodes[pt] = new Pathing::PathNode(pt);
	}
	
	for (auto& node_pair : pathing->nodes)
	{
		auto node = node_pair.second;
		auto pt = node_pair.first;
		pt.x += 1;
		add_neighbor_node(map, pathing, node, pt);


		pt = node_pair.first;
		pt.x -= 1;
		add_neighbor_node(map, pathing, node, pt);

		pt = node_pair.first;
		pt.y += 1;
		add_neighbor_node(map, pathing, node, pt);

		pt = node_pair.first;
		pt.y -= 1;
		add_neighbor_node(map, pathing, node, pt);
	}

	int added = 1;
	while (added > 0)
	{
		added = 0;
		for (auto& node_pair : pathing->nodes)
		{
			auto node = node_pair.second;

			for (const auto& route : node->routes)
			{
				auto other_node = pathing->nodes[route.second.destination()];
				assert(other_node);
				for (const auto& next_route : other_node->routes)
				{
					if (next_route.second.destination() == node->point)
					{
						continue;
					}
					if (node->routes.count(next_route.second.destination()) == 0)
					{
						auto route_copy = next_route.second;
						route_copy.add_front(node->point);
						node->routes[next_route.second.destination()] = route_copy;
						++added;
					}
				}
			}
		}
		std::cout << "Added: " << added << std::endl;
	}

	std::cout << "Finished!" << std::endl;
}





Route build_plan_route(GameMap*, Pathing*);

std::vector<Move> build_plan_from_route(const Route&);

std::vector<Move> build_plan(GameMap* map, Pathing* pathing)
{

	


	std::vector<Move> moves;
	auto r = build_plan_route(map, pathing);
	Point it = map->myStart();

	std::cout << "Movement Plan: " << std::endl;
	for (size_t i = 0; i < r.size(); ++i)
	{
		std::cout << r[i] << std::endl;
	}

	std::cout << "\n\n\n\n" << std::endl;
	std::cout << "Plan printed." << std::endl;

	return build_plan_from_route(r);



	for (int i = 0; i < r.size(); ++i)
	{
		auto next = r[i];
		if (next.x - it.x == 1)
		{
			moves.push_back(RIGHT);
		}
		else if (next.x - it.x == -1)
		{
			moves.push_back(LEFT);
		}
		else if (next.y - it.y == 1)
		{
			moves.push_back(DOWN);
		}
		else if (next.y - it.y == -1)
		{
			moves.push_back(UP);
		}
		else
		{
			std::cout << "Next: " << next << std::endl;
			std::cout << "Current: " << it << std::endl;
			assert(false);
		}
		it = next;
	}

	return moves;
}

template <class T>
struct length_compare
{
	bool operator()(const T& lhs, const T& rhs) const noexcept
	{
		return lhs.size() < rhs.size();
	}
};

Route build_plan_route(GameMap* map, Pathing* pathing)
{
	std::set<Point> gathered;
	auto _cherries = map->cherries();
	std::set<Point> cherries(_cherries.begin(), _cherries.end());
	auto start = map->myStart();

	Route r;
	r.add(start);

	auto start_node = pathing->nodes[start];

	Point current_start = start;

	
	while (cherries.size() > 0)
	{
		auto node = pathing->nodes[current_start];
		Point cherry;
		size_t current_len = 99999999999999;

		for (auto& cherry_it : cherries)
		{
			if (node->routes.count(cherry_it))
			{
				auto& route = node->routes[cherry_it];
				if (route.size() < current_len)
				{
					cherry = route.destination();
					assert(route.destination() == cherry_it);
					current_len = route.size();
				}
			}
		}

		if (current_len == 99999999999999)
		{
			break;
		}

		cherries.erase(cherry);
		const auto& route = node->routes[cherry];
		assert(route.start() == current_start);
		for (size_t i = 0; i < route.size(); ++i)
		{
			r.add(route[i]);
		}
		assert(route[route.size()-1] == route.destination());

	}

	return r;
}

std::vector<Move> build_plan_from_route(const Route& r)
{
	std::vector<Move> moves;
	Point it = r.start();



	for (int i = 0; i < r.size(); ++i)
	{
		auto next = r[i];
		if (next.x - it.x == 1)
		{
			moves.push_back(RIGHT);
		}
		else if (next.x - it.x == -1)
		{
			moves.push_back(LEFT);
		}
		else if (next.y - it.y == 1)
		{
			moves.push_back(DOWN);
		}
		else if (next.y - it.y == -1)
		{
			moves.push_back(UP);
		}
		else
		{
			std::cout << "Next: " << next << std::endl;
			std::cout << "Current: " << it << std::endl;
			assert(false);
		}
		it = next;
	}

	return moves;
}













struct SpotEdge;

struct SpotNode
{
	Point point;
	std::vector<SpotEdge*> edges;


	SpotNode(Point pt) : point(pt), edges()
	{
		
	}
};

struct SpotEdge
{
	SpotNode* left;
	SpotNode* right;
	int cost;
	Route pathway;
};





struct Routes
{
	std::map<Point, std::map<Point, Route>> routes;
};

struct SpotNodes
{
	std::map<Point, SpotNode*> nodes;
};


SpotEdge* djikstra_search(SpotNodes*, GameMap*, Point start, Point end, Route& r);


SpotNodes* build_route_graph(GameMap* map)
{
	std::set<Point> gathered;
	auto cherries = map->cherries();

	std::set<Point> node_points;
	for (auto& cherry : cherries)
	{
		node_points.insert(cherry);
	}
	node_points.insert(map->myStart());

	SpotNodes* node_set = new SpotNodes();

	for (auto& point : node_points)
	{
		node_set->nodes[point] = new SpotNode(point);
	}

	bool skip;

	for (auto& point : node_points)
	{
		auto node = node_set->nodes[point];
		for (auto& other_point : node_points)
		{
			if (point == other_point)
			{
				continue;
			}

			auto other_node = node_set->nodes[other_point];
			assert(other_node != node);

			skip = false;

			for (auto& edge : node->edges)
			{
				if (edge->right == other_node || edge->left == other_node)
				{
					skip = true;
					break;
				}
			}

			for (auto& edge : other_node->edges)
			{
				if (edge->right == node)
				{
					node->edges.push_back(edge);
					skip = true;
					break;
				}
			}
			if (skip)
			{
				continue;
			}



			Route r;
			auto edge = djikstra_search(node_set, map, point, other_point, r);
			if (edge)
			{
				node->edges.push_back(edge);
				other_node->edges.push_back(edge);
			}
		}
	}


	return node_set;
}


struct PartialRoute
{
	int cost;
	PartialRoute* prev;
	Point location;


	PartialRoute() : PartialRoute(0, Point())
	{

	}

	PartialRoute(PartialRoute* prev, Point loc) : cost(prev->cost + 1), prev(prev), location(loc)
	{

	}

	PartialRoute(int cost, Point loc) : cost(cost), prev(nullptr), location(loc)
	{

	}
};

template <class Q>
void djikstra_add_neighbor_node(GameMap* map, std::map<Point, PartialRoute*>& mappings, Q* q, PartialRoute* pr, Point pt)
{
	if (map->at(pt) != WALL)
	{
		auto next_partial = new PartialRoute(pr, pt);
		q->push(next_partial);
		mappings[pt] = next_partial;
	}
}

void build_route(Route& r, PartialRoute* pr)
{
	if (!pr)
	{
		return;
	}

	build_route(r, pr->prev);
	r.add(pr->location);
}

SpotEdge* djikstra_search(SpotNodes* nodes, GameMap* map, Point start, Point end, Route& r)
{
	struct djikstra_search_compare
	{
		bool operator()(const PartialRoute* lhs, const PartialRoute* rhs) const
		{
			return lhs->cost < rhs->cost;
		}
	};

	auto start_node = nodes->nodes[start];
	auto end_node = nodes->nodes[end];
	std::priority_queue<PartialRoute*, std::vector<PartialRoute*>, djikstra_search_compare> q;
	std::map<Point, PartialRoute*> mappings;

	mappings[start] = new PartialRoute (0, start);

	{
		auto reachable = map->reachable(start, 1);
		for (auto& point : reachable)
		{
			if (point == end)
			{
				assert(false);
			}

			auto partial = new PartialRoute(mappings[start], point);
			q.push(partial);
		}
	}

	PartialRoute* found_route = nullptr;

	while (!q.empty())
	{
		auto p_route = q.top();
		q.pop();

		if (p_route->location == end)
		{
			found_route = p_route;
			break;
		}

		auto pt = p_route->location;
		pt.x += 1;
		if (!mappings.count(pt))
		{
			djikstra_add_neighbor_node(map, mappings, &q, p_route, pt);
		}
		

		pt = p_route->location;
		pt.x -= 1;
		if (!mappings.count(pt))
		{
			djikstra_add_neighbor_node(map, mappings, &q, p_route, pt);
		}

		pt = p_route->location;
		pt.y += 1;
		if (!mappings.count(pt))
		{
			djikstra_add_neighbor_node(map, mappings, &q, p_route, pt);
		}

		pt = p_route->location;
		pt.y -= 1;
		if (!mappings.count(pt))
		{
			djikstra_add_neighbor_node(map, mappings, &q, p_route, pt);
		}
	}

	if (found_route)
	{
		build_route(r, found_route);
		auto edge = new SpotEdge();
		edge->left = start_node;
		edge->right = end_node;
		edge->cost = found_route->cost;
		edge->pathway = r;
		return edge;
	}
	else
	{
		return nullptr;
	}


	
}



std::vector<Move> combine_routing(GameMap* map, SpotNodes*);

void print_moves(const std::vector<Move>&);


int main()
{
	auto map = read_map();
	auto nodes = build_route_graph(map);
	std::vector<Move> moves;
	if (nodes->nodes[map->myStart()]->edges.size() != 1)
	{
		moves = combine_routing(map, nodes);
	}
	else
	{
		moves = build_plan_from_route(nodes->nodes[map->myStart()]->edges[0]->pathway);
	}


	
	print_moves(moves);
	return 0;
}


void print_moves(const std::vector<Move>& moves)
{
	for (auto move : moves)
	{
		std::cout << (char)move << std::endl;
	}
}


std::vector<Move> combine_routing(GameMap* map, SpotNodes* nodes)
{

	assert(false);
}