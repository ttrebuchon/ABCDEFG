#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


struct Node
{
	char value;
	Node *left, *right, *parent;
};

Node** gen_nodes(string str, size_t& i)
{
	Node** array = new Node*[str.length()];
	i = 0;
	for (auto& v : str)
	{
		array[i] = new Node;
		array[i]->value = v;
		++i;
	}

	return array;
}



int max_depth(Node* n)
{
	if (!n)
	{
		return 0;
	}
	else
	{
		return 1+max_depth(n);
	}
}

int max_width(Node* n)
{
	return pow(3, max_depth(n));
}

int width_at_depth(Node* n, int* w, int depth)
{

}

void _tree_to_string(Node* n, std::vector<std::string>& lines, int depth, int max_depth)
{
	if (!n)
	{
		if (depth < max_depth)
		{
			_tree_to_string(nullptr, lines, depth+1, max_depth);
		}
		lines[depth] += "   ";
		return;
	}

	lines[depth] += " ";
	int sn = 3*(pow(2, max_depth) - pow(2, depth));

	lines[depth] += string(' ', (sn - 1)/2);
	lines[depth] += n->value;
	lines[depth] += string(' ', (sn - 1)/2);



	
}

string tree_to_string(Node* n)
{
	int d = max_depth(n);
	std::vector<std::string> lines(d);
	_tree_to_string(n, lines, 0, d);


	string str = "";
	for (auto& line : lines)
	{
		str += line + "\n";
	}
	return str;
}

// #define PREFIX_MAX_STAGE

// int prefix_recurse(Node** nodes, size_t n, Node* current, Node* root, int stage)
// {
// 	if (n == 0)
// 	{
// 		print_tree(root);
// 		return -1;
// 	}

// 	current->left = nodes[0];
// 	int stage = 0;
// 	while ((stage = prefix_recurse(nodes+1, n-1, current->left, root, stage)) < PREFIX_MAX_STAGE)
// 	{

// 	}
	
// 	if (n > 1)
// 	{
// 		current->right = nodes[1];
// 		prefix_recurse(nodes+2, n-2, current->right, root);
// 	}

// 	current->left = nullptr;
// 	current->right = nodes[0];
// 	prefix_recurse(nodes+1, n-1, current->right, root);


// 	current->right = nullptr;
// 	return;
	
// }

// Node** gen_prefix(string str)
// {
// 	size_t n;
// 	Node** arr = gen_nodes(str, n);

// 	auto root = arr[0];
// 	prefix_recurse(arr, n, root, root);

// }

// Node** gen_infix(string str)
// {
// 	size_t n;
// 	Node** arr = gen_nodes(str, n);



// }





int main()
{
	std::string line1;
	std::string line2;
	std::getline(std::cin, line1);
	std::getline(std::cin, line2);

	// Node** array1 = new Node*[line1.length()];
	// Node** array2 = new Node*[line1.length()];

	// int i = 0;
	// for (auto& v : line1)
	// {
	// 	array1[i] = new Node;
	// 	array1[i]->value = v;
	// 	array2[i] = new Node;
	// 	array2[i]->value = v;
	// 	++i;
	// }

	size_t n;
	Node** array1 = gen_nodes(line1, n);

	array1[0]->right = array1[1];
	array1[1]->left = array1[2];
	//print_tree_structure(array1[0], 0);
	std::cout << tree_to_string(array1[0]);

	//std::cout << line1 << std::endl << line2 << std::endl;
}