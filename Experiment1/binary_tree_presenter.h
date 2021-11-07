#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <memory>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::stack;
using std::shared_ptr;
using std::make_shared;

// 二叉树展示类模板，模板参数决定结点数据类型
// 由于此类为模板类，故将声明与实现置于同一个文件中
template <typename T = int>
class BinaryTreePresenter final
{
private:
	// 二叉树结点类
	class TreeNode
	{
	public:
		T value;
		shared_ptr<TreeNode> left_child;
		shared_ptr<TreeNode> right_child;

		TreeNode(const T& value) :
			value(value), left_child(nullptr), right_child(nullptr)
		{}
		TreeNode(
			const T& value,
			const shared_ptr<TreeNode>& left_child,
			const shared_ptr<TreeNode>& right_child) :
			value(value), left_child(left_child), right_child(right_child)
		{}
		~TreeNode() = default;
	};

	using TreeNodePtr = shared_ptr<TreeNode>;

public:
	// 构造函数传入使用者定义的表示结点为空的序列元素值
	BinaryTreePresenter(const T& empty_node_value) :
		empty_node_value_(empty_node_value)
	{}

	// 构建二叉树
	// node_values: 二叉树各结点数据(含所有空结点)的层序遍历顺序序列。
	// 值等于empty_node_value_表示空节点
	void CreateTree(const vector<T>& node_values);

	// 打印二叉树
	// 每一行打印一层的结点，一对兄弟结点的值在一对圆括号内显示，
	// 圆括号前的方括号内指示其父结点的值
	void PrintTree() const;

	// 先序遍历的递归、迭代实现
	void PreOrderTraversalRecursive() const;
	void PreOrderTraversalIterative() const;

	// 中序遍历的递归、迭代实现
	void InOrderTraversalRecursive() const;
	void InOrderTraversalIterative() const;

	// 后序遍历的递归、迭代实现
	void PostOrderTraversalRecursive() const;
	void PostOrderTraversalIterative() const;

	// 层序遍历
	void LevelOrderTraversal() const;

	// 判断是否为完全二叉树
	bool IsComplete() const;

private:
	// 读入序列以建立二叉树时，表示结点为空的序列元素值
	T empty_node_value_;
	// 当前实例中所保存的二叉树的根节点
	TreeNodePtr root_;

	// 先序遍历、中序遍历、后序遍历递归版本的实际实现函数
	void PreOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
	void InOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
	void PostOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
};

// 构建二叉树
template <typename T>
void BinaryTreePresenter<T>::CreateTree(const vector<T>& node_values)
{
	// 传入序列中根节点为空，则直接置root_成员为空指针
	if (node_values[0] == empty_node_value_)
	{
		root_.reset();
		return;
	}

	// 下面基于层序遍历建立二叉树

	TreeNodePtr root = make_shared<TreeNode>(node_values[0]);

	// 由于输入序列是以满二叉树的形式组织的，因此此队列也必须以满二叉树的形式存储元素，
	// 即在循环中，对空的儿子结点，不能略过不入队，而必须入队一个空指针占位
	queue<TreeNodePtr> level_order_queue;

	level_order_queue.push(root);

	size_t current_index = 0;

	while (current_index < node_values.size() - 1)
	{
		TreeNodePtr current_node = level_order_queue.front();
		level_order_queue.pop();

		current_index++;
		// 如果当前父结点不为空而且传入的序列中当前元素的值不等于代表空结点的值，
		// 则为当前父结点建立左儿子结点
		if (current_node && node_values[current_index] != empty_node_value_)
		{
			current_node->left_child =
				make_shared<TreeNode>(node_values[current_index]);
			level_order_queue.push(current_node->left_child);
		}
		// 否则只将空指针入队
		else
		{
			level_order_queue.push(nullptr);
		}

		current_index++;
		// 如果当前父结点不为空而且传入的序列中当前元素的值不等于代表空结点的值，
		// 则为当前父结点建立右儿子结点
		if (current_node && node_values[current_index] != empty_node_value_)
		{
			current_node->right_child =
				make_shared<TreeNode>(node_values[current_index]);
			level_order_queue.push(current_node->right_child);
		}
		// 否则只将空指针入队
		else
		{
			level_order_queue.push(nullptr);
		}
	}

	root_ = root;
}

// 打印二叉树
template <typename T>
void BinaryTreePresenter<T>::PrintTree() const
{
	if (!root_)
	{
		cout << "<EMPTY TREE>" << endl;
		return;
	}

	queue<TreeNodePtr> level_traversal_queue;

	level_traversal_queue.push(root_);

	cout << root_->value << endl;

	// 每次进入循环，level_traversal_queue队列元素为当前层的所有结点，它们已经被打印过，
	// 循环中要打印它们的孩子结点，然后将它们移出level_traversal_queue
	while (!level_traversal_queue.empty())
	{
		size_t current_level_node_count = level_traversal_queue.size();

		for (size_t i = 0; i < current_level_node_count; i++)
		{
			TreeNodePtr current_node = level_traversal_queue.front();

			level_traversal_queue.pop();

			if (current_node->left_child || current_node->right_child)
			{
				cout << '[' << current_node->value << "](";

				if (current_node->left_child)
				{
					level_traversal_queue.push(current_node->left_child);
					cout << current_node->left_child->value;
				}

				if (current_node->right_child)
				{
					level_traversal_queue.push(current_node->right_child);
					cout << ',' << current_node->right_child->value;
				}

				cout << ") ";
			}
		}

		cout << endl;
	}
}

// 先序遍历的递归实现
template<typename T>
void BinaryTreePresenter<T>::PreOrderTraversalRecursive() const
{
	PreOrderTraversalRecursiveImp(root_);
}

// 先序遍历的迭代实现
template<typename T>
void BinaryTreePresenter<T>::PreOrderTraversalIterative() const
{
	stack<TreeNodePtr> preorder_stack;

	TreeNodePtr current_node = root_;

	while (!preorder_stack.empty() || current_node)
	{
		if (current_node)
		{
			cout << current_node->value << ' ';
			preorder_stack.push(current_node);
			current_node = current_node->left_child;
		}
		else
		{
			current_node = preorder_stack.top()->right_child;
			preorder_stack.pop();
		}
	}
}

// 中序遍历的递归实现
template<typename T>
void BinaryTreePresenter<T>::InOrderTraversalRecursive() const
{
	InOrderTraversalRecursiveImp(root_);
}

// 中序遍历的迭代实现
template<typename T>
void BinaryTreePresenter<T>::InOrderTraversalIterative() const
{
	stack<TreeNodePtr> inorder_stack;

	TreeNodePtr current_node = root_;

	while (!inorder_stack.empty() || current_node)
	{
		if (current_node)
		{
			inorder_stack.push(current_node);
			current_node = current_node->left_child;
		}
		else
		{
			current_node = inorder_stack.top();
			inorder_stack.pop();
			cout << current_node->value << ' ';
			current_node = current_node->right_child;
		}
	}
}

// 后序遍历的递归实现
template<typename T>
void BinaryTreePresenter<T>::PostOrderTraversalRecursive() const
{
	PostOrderTraversalRecursiveImp(root_);
}

// 后序遍历的迭代实现
template<typename T>
void BinaryTreePresenter<T>::PostOrderTraversalIterative() const
{
	stack<TreeNodePtr> postorder_stack;

	TreeNodePtr current_node = root_;

	TreeNodePtr previous_visited_node = nullptr;

	while (!postorder_stack.empty() || current_node)
	{
		if (current_node)
		{
			postorder_stack.push(current_node);
			current_node = current_node->left_child;
		}
		else
		{
			TreeNodePtr parent_node = postorder_stack.top();

			if (parent_node->right_child
				&& previous_visited_node != parent_node->right_child)
			{
				current_node = parent_node->right_child;
			}
			else
			{
				cout << parent_node->value << ' ';
				previous_visited_node = parent_node;
				postorder_stack.pop();
			}
		}
	}
}

// 层序遍历
template<typename T>
void BinaryTreePresenter<T>::LevelOrderTraversal() const
{
	if (!root_)
	{
		return;
	}

	queue<TreeNodePtr> level_order_queue;

	level_order_queue.push(root_);

	while (!level_order_queue.empty())
	{
		TreeNodePtr current_node = level_order_queue.front();
		level_order_queue.pop();

		cout << current_node->value << ' ';

		if (current_node->left_child)
		{
			level_order_queue.push(current_node->left_child);
		}

		if (current_node->right_child)
		{
			level_order_queue.push(current_node->right_child);
		}
	}
}

// 判断是否为完全二叉树
template<typename T>
bool BinaryTreePresenter<T>::IsComplete() const
{
	if (!root_)
	{
		cout << "<EMPTY TREE>" << endl;
		return true;
	}

	// 使用层次遍历进行判断

	// 指示已遍历到的结点的左右儿子是否都非空
	bool is_previous_all_full = true;

	queue<TreeNodePtr> level_order_queue;

	level_order_queue.push(root_);

	while (!level_order_queue.empty())
	{
		TreeNodePtr current_node = level_order_queue.front();
		level_order_queue.pop();

		// 任一结点若无左儿子而有右儿子，一定不是完全二叉树
		if (!current_node->left_child && current_node->right_child)
		{
			return false;
		}

		// 已经排除了当前结点无左儿子而有右儿子的情况，
		// 且已遍历到的结点的左右儿子都非空的情况下，如果当前结点：
		// 1.有左儿子而无右儿子；
		// 2.左右儿子均没有；
		// 也即左右儿子结点有任意一个为空，
		// 那么标记已遍历部分不是完全二叉树
		if (is_previous_all_full)
		{
			if (!current_node->left_child || !current_node->right_child)
			{
				is_previous_all_full = false;
			}
		}
		// 否则，只允许后续结点左右儿子都没有
		else
		{
			if (current_node->left_child || current_node->right_child)
			{
				return false;
			}
		}

		if (current_node->left_child)
		{
			level_order_queue.push(current_node->left_child);
		}

		if (current_node->right_child)
		{
			level_order_queue.push(current_node->right_child);
		}
	}

	return true;
}

// 先序遍历递归版本的实际实现函数
template<typename T>
void BinaryTreePresenter<T>::
PreOrderTraversalRecursiveImp(const TreeNodePtr& node) const
{
	if (node)
	{
		cout << node->value << ' ';

		PreOrderTraversalRecursiveImp(node->left_child);
		PreOrderTraversalRecursiveImp(node->right_child);
	}
}

// 中序遍历递归版本的实际实现函数
template<typename T>
void BinaryTreePresenter<T>::
InOrderTraversalRecursiveImp(const TreeNodePtr& node) const
{
	if (node)
	{
		InOrderTraversalRecursiveImp(node->left_child);

		cout << node->value << ' ';

		InOrderTraversalRecursiveImp(node->right_child);
	}
}

// 后序遍历递归版本的实际实现函数
template<typename T>
void BinaryTreePresenter<T>::
PostOrderTraversalRecursiveImp(const TreeNodePtr& node) const
{
	if (node)
	{
		PostOrderTraversalRecursiveImp(node->left_child);
		PostOrderTraversalRecursiveImp(node->right_child);

		cout << node->value << ' ';
	}
}