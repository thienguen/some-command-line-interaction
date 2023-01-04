#include <iostream>
#include <vector>
#include <fstream>

struct Node
{
	int val;
	int width;
	int height;
	std::vector<Node *> child;
};

auto getWidth(int) -> int;
void input(Node *&, int &);
void visualize_straight(Node *, int, long long &, bool);
void visualize_inclined(Node *, int, long long &, bool, int &);
void visualize_tree(Node *, int, int, int, char **);
void initTree(Node *);
void center(int, char **, int, int, int);
void del(Node *&);

int main()
{
	/* Init */
	int style = 0;
	long long stat = 0;
	Node *tree = new Node;

	/* Input Phase */
	input(tree, style);

	if (style == 1)
	{
		visualize_straight(tree, 0, stat, 0);
	}
	else if (style == 2)
	{
		int lines = 0;
		visualize_inclined(tree, 0, stat, 0, lines);
	}
	else if (style == 3)
	{
		initTree(tree);

		char **res = new char *[tree->height];

		for (int i = 0; i < tree->height; i++)
		{
			res[i] = new char[tree->width + 1];

			for (int j = 0; j < tree->width; j++)
				res[i][j] = ' ';

			res[i][tree->width] = '\0';
		}

		visualize_tree(tree, 0, 0, tree->width - 1, res);

		std::cout << "\n[INFO] Tree:\n\n";

		for (int i = 0; i < tree->height; i++)
			std::cout << res[i] << "\n";

		std::cout << "\n";

		for (int i = 0; i < tree->height; i++)
			delete[] res[i];

		delete[] res;

		std::cout << "[INFO] res[][] deleted\n";
	}

	std::cout << "\n[INFO] Tree visualized\n\n";
	del(tree);
	std::cout << "\n\n[INFO] Tree deleted\n";
}

void tab(int l)
{
	std::cout << std::string(l, ' ');
}

/*
 * Parameters
 * tree: a pointer to the root node of the tree
 * depth: the current depth in the tree
 * stat: a reference to a long long integer that stores information about the indentation of the tree
 * isLast: a boolean value indicating whether the current node is the last child of its parent
 * Returns
 * Nothing.
 */

void visualize_straight(Node *tree, int depth, long long &stat, bool isLast)
{
	/* Return if the tree is empty */
	if (!tree)
		return;

	// Output the indentation for the current node
	for (int i = 0; i <= depth; i++)
	{
		if ((stat >> i) & 1)
			std::cout << "|  ";
		else
			std::cout << "   ";
	}
	std::cout << "\n";

	// Output the indentation for the current node's value
	for (int i = 0; i < depth - 1; i++)
	{
		if ((stat >> i) & 1)
			std::cout << "|  ";
		else
			std::cout << "   ";
	}

	// Output the value of the current node, with an extra "|__" if it is not the root
	if (depth)
		std::cout << "|__";
	std::cout << tree->val << "\n";

	// Clear the last child flag for the parent of the current node
	if (isLast)
		stat &= ~(1LL << (depth - 1));

	// Recursively output the children of the current node
	if ((tree->child).size())
	{
		/* 
			* The stat variable is updated by using the ^= operator, 
			* which is a compound assignment operator that performs a 
			* bitwise XOR operation and assigns the result back to the 
			* stat variable. This sets the depth-th least significant 
			* bit of stat to 1. This bit will be used later to determine 
			* whether to output an indentation with a vertical bar or with spaces.

			* The for loop iterates over all children of the current node. 
			* For each child, the visualize_straight function is called 
			* recursively with the child as the new root and an incremented 
			* depth value. The isLast parameter is set to true for the last 
			* child and false for all other children. This will be used later 
			* to clear the last child flag for the parent of the current node.	
		*/

		stat ^= 1LL << depth;

		for (size_t i = 0; i < (tree->child).size(); i++)
		{
			visualize_straight((tree->child)[i], depth + 1, stat, i == (tree->child).size() - 1);
		}
	}
}

/*
 * Parameters
 * tree: a pointer to the root node of the tree
 * depth: the current depth in the tree
 * stat: a reference to a long long integer that stores information about the indentation and connections of the tree
 * isLast: a boolean value indicating whether the current node is the last child of its parent
 * lines: a reference to an integer that keeps track of the number of lines output so far
 * Returns
 * Nothing.
 */

void visualize_inclined(Node *tree, int depth, long long &stat, bool isLast, int &lines)
{
	if (!tree)
		return;

	// Print the indentation for the current node
	tab(lines);

	// Print the connection lines between the 
	// current node and its parent node
	for (int i = 0; i <= depth; i++)
	{
		if ((stat >> i) & 1)
			std::cout << "\\  ";
		else
			std::cout << "   ";
	}

	std::cout << "\n";
	
	// Increment the number of lines printed
	lines++;

	// Print the indentation for the current node's value
	tab(lines);

	// Print the connection lines between the 
	// current node's value and the node itself
	for (int i = 0; i < depth - 1; i++)
	{
		if ((stat >> i) & 1)
			std::cout << "\\  ";
		else
			std::cout << "   ";
	}

	if (depth)
		std::cout << "\\__";

	std::cout << tree->val << "\n";

	lines++;

	// If the current node is the last child of its 
	// parent, clear the last child flag for the current node's parent
	if (isLast)
		stat &= ~(1LL << (depth - 1));

	// If the current node has children, print them recursively
	if ((tree->child).size())
	{
		stat ^= 1LL << depth;
		for (size_t i = 0; i < (tree->child).size(); i++)
			visualize_inclined((tree->child)[i], depth + 1, stat, i == (tree->child).size() - 1, lines);
	}
}

void initTree(Node *tree)
{
	if (!tree)
		return;

	tree->width = getWidth(tree->val);

	int w = 0, h = 0;

	for (size_t i = 0; i < (tree->child).size(); i++)
	{
		initTree((tree->child)[i]);
		w += ((tree->child)[i])->width + 2 * (i != 0);
		h = std::max(h, ((tree->child)[i])->height);
	}
	
	tree->width = std::max(tree->width, w);
	tree->height = h ? 4 + h : 1;
}

/*
 * Parameters
 * val: the value of the node
 * Returns
 * The width of the node in the graphical representation.
 */

int getWidth(int x)
{
	int res = (x == 0);
	while (x)
	{
		res++;
		x /= 10;
	}
	return res;
}

/* Center in range of spaces */
void center(int value, char **result, int row, int left_column, int right_column)
{
	int width = getWidth(value), total_width = right_column - left_column + 1, margin = (total_width - width) / 2;
	
	for (size_t i = width; i > 0; i--)
	{
		result[row][left_column + i + margin - 1] = static_cast<char>('0' + value % 10);
		value /= 10;
	}
}

/*
 * Parameters
 * tree: a pointer to the root node of the tree
 * x1: the left coordinate of the bounding box for the current subtree
 * x2: the right coordinate of the bounding box for the current subtree
 * y: the vertical coordinate of the current row
 * res: a double pointer to an array of strings that stores the graphical representation of the tree
 * Returns
 * Nothing.
 */

void visualize_tree(Node *tree, int depth, int left, int right, char **res)
{
    if (!tree) 
		return;

    center(tree->val, res, depth, left, right);
    int num_children = tree->child.size();
    
	if (num_children == 0)
        return;
    
	res[depth + 1][(left + right) / 2] = '|';

    if (num_children == 1)
    {
        res[depth + 2][(left + right) / 2] = '|';
        res[depth + 3][(left + right) / 2] = '|';
        visualize_tree(tree->child[0], depth + 4, left, right, res);
    }
    else if (num_children > 1)
    {
        int margin_left  = ((tree->child[0]->width - 1) / 2);
		int margin_right = (tree->child[num_children - 1]->width) / 2;

        for (int i = left + margin_left; i <= right - margin_right; i++)
            res[depth + 2][i] = '-';
    
		res[depth + 2][(left + right) / 2] = '^';

        for (int i = 0; i < num_children; i++)
        {
            res[depth + 3][left + (tree->child[i]->width - 1) / 2] = '|';
            res[depth + 2][left + (tree->child[i]->width - 1) / 2] = '.';
            visualize_tree(tree->child[i], depth + 4, left, tree->child[i]->width + left - 1, res);
            left += tree->child[i]->width + 2;
        }
    }
}

void input(Node *&tree, int &style)
{
	std::cout << "[MESSAGE] Where do you want to import?\n";
	std::cout << "1. std\n";
	std::cout << "2. file\n";

	/* Simple Input Phase */
	int prompt{};
	do
	{
		std::cin >> prompt;

		if (prompt < 1 || prompt > 2)
		{
			std::cout << "[ERROR] Selection is in valid, please try again.\n";
			continue;
		}
		break;
	} while (1);

	/* User Options */
	if (prompt == 1)
	{
		int n, par;
		
		/* The amount of nodes */
		std::cout << "[INPUT] Type number of nodes: "; std::cin >> n;

		/* Allocate pointer point to a bunch of pointer pointing to a node */	
		/* The array is initialized with the memory addresses of the newly created Node objects */
		Node **nodes = new Node *[n];

		for (int i = 0; i < n; i++)
			nodes[i] = new Node;

		for (int i = 0; i < n; i++)
		{
			std::cout << "[INPUT] Type value of node " << i << ": "; std::cin >> ((nodes[i])->val);
			std::cout << "[INPUT] Type parent of node " << i << " (-1 means the root): "; std::cin >> par;

			/* Child */
			if (par != -1)
			{
				((nodes[par])->child).push_back(nodes[i]);
			}
			/* Root */
			else
				tree = nodes[i];
		}
		std::cout << "[INFO] Input completed\n";
	}
	else
	{
		std::ifstream inFile;
		std::cout << "\n[MESSAGE] 1. Importing from file \"tree.txt\"\n";
		std::cout << "\n[MESSAGE] 2. Or importing from file \"tree2.txt\"\n";
		
		std::cin >> prompt;

		if (prompt == 1)
			inFile.open("tree.txt");
		else
			inFile.open("tree2.txt");
		
		if (!inFile.is_open())
		{
			std::cout << "[ERROR] File not found!\n";
			return;
		}

		int n, parent;

		/* The amount of nodes */
		inFile >> n;

		/* Allocate pointer point to a bunch of pointer pointing to a node */
		/* The array is initialized with the memory addresses of the newly created Node objects */
		Node **nodes = new Node *[n];

		for (int i = 0; i < n; i++)
			nodes[i] = new Node;

		for (int i = 0; i < n; i++)
		{
			inFile >> nodes[i]->val;
			inFile >> parent;

			/* Child */
			if (parent != -1)
			{
				((nodes[parent])->child).push_back(nodes[i]);
			}
			/* Root */
			else
				tree = nodes[i];
		}

		inFile.close();
		std::cout << "[INFO] Input completed\n";
	}

	std::cout << "[MESSAGE] Which style of the tree do you want?\n";
	std::cout << "1. Vertical\n";
	std::cout << "2. Inclined\n";
	std::cout << "3. Tree\n";

	do
	{
		std::cin >> prompt;

		if (prompt < 1 || prompt > 3)
		{
			std::cout << "[ERROR] SElection is invalid, please try again.\n";
		}
		else
			break;

	} while (true);

	style = prompt;
}

void del(Node *&tree)
{
	if (!tree) return;

	for (size_t i = 0; i < (tree->child).size(); i++)
		del((tree->child)[i]);

	// Clear the node in the vector
	(tree->child).clear();

	// Delete the node, or that damn memory leak will happen
	delete tree;

	// Set the pointer to nullptr
	tree = nullptr;
}
