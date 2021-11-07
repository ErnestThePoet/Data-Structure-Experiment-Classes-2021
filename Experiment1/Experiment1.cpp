#include <iostream>
#include <vector>

#include "binary_tree_presenter.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	BinaryTreePresenter<char> presenter('.');

	cout << "[CREATING TREE]: Please enter the tree height:" << endl;

	size_t tree_height = 0;
	cin >> tree_height;

	size_t node_count = (1U << tree_height) - 1;

	cout << "Enter " << node_count << " values:" << endl;

	vector<char> nodes;

	for (size_t i = 0; i < node_count; i++)
	{
		char current_node_value = 0;
		cin >> current_node_value;
		nodes.push_back(std::move(current_node_value));
	}

	presenter.CreateTree(nodes);

	cout << endl;
	cout << "[TREE DISPLAY]" << endl;
	presenter.PrintTree();

	cout << "[PREORDER TRAVERSAL RECURSIVE]" << endl;
	presenter.PreOrderTraversalRecursive();
	cout << endl << endl;

	cout << "[PREORDER TRAVERSAL ITERATIVE]" << endl;
	presenter.PreOrderTraversalIterative();
	cout << endl << endl;

	cout << "[INORDER TRAVERSAL RECURSIVE]" << endl;
	presenter.InOrderTraversalRecursive();
	cout << endl << endl;

	cout << "[INORDER TRAVERSAL ITERATIVE]" << endl;
	presenter.InOrderTraversalIterative();
	cout << endl << endl;

	cout << "[POSTORDER TRAVERSAL RECURSIVE]" << endl;
	presenter.PostOrderTraversalRecursive();
	cout << endl << endl;

	cout << "[POSTORDER TRAVERSAL ITERATIVE]" << endl;
	presenter.PostOrderTraversalIterative();
	cout << endl << endl;

	cout << "[LEVEL ORDER TRAVERSAL]" << endl;
	presenter.LevelOrderTraversal();
	cout << endl << endl;

	cout << "[IS COMPLETE BINARY TREE]" << endl;
	cout << std::boolalpha << presenter.IsComplete() << std::noboolalpha;
	cout << endl;

	return 0;
}