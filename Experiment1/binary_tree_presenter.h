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

// ������չʾ��ģ�壬ģ��������������������
// ���ڴ���Ϊģ���࣬�ʽ�������ʵ������ͬһ���ļ���
template <typename T = int>
class BinaryTreePresenter final
{
private:
	// �����������
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
	// ���캯������ʹ���߶���ı�ʾ���Ϊ�յ�����Ԫ��ֵ
	BinaryTreePresenter(const T& empty_node_value) :
		empty_node_value_(empty_node_value)
	{}

	// ����������
	// node_values: ���������������(�����пս��)�Ĳ������˳�����С�
	// ֵ����empty_node_value_��ʾ�սڵ�
	void CreateTree(const vector<T>& node_values);

	// ��ӡ������
	// ÿһ�д�ӡһ��Ľ�㣬һ���ֵܽ���ֵ��һ��Բ��������ʾ��
	// Բ����ǰ�ķ�������ָʾ�丸����ֵ
	void PrintTree() const;

	// ��������ĵݹ顢����ʵ��
	void PreOrderTraversalRecursive() const;
	void PreOrderTraversalIterative() const;

	// ��������ĵݹ顢����ʵ��
	void InOrderTraversalRecursive() const;
	void InOrderTraversalIterative() const;

	// ��������ĵݹ顢����ʵ��
	void PostOrderTraversalRecursive() const;
	void PostOrderTraversalIterative() const;

	// �������
	void LevelOrderTraversal() const;

	// �ж��Ƿ�Ϊ��ȫ������
	bool IsComplete() const;

private:
	// ���������Խ���������ʱ����ʾ���Ϊ�յ�����Ԫ��ֵ
	T empty_node_value_;
	// ��ǰʵ����������Ķ������ĸ��ڵ�
	TreeNodePtr root_;

	// ��������������������������ݹ�汾��ʵ��ʵ�ֺ���
	void PreOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
	void InOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
	void PostOrderTraversalRecursiveImp(const TreeNodePtr& node) const;
};

// ����������
template <typename T>
void BinaryTreePresenter<T>::CreateTree(const vector<T>& node_values)
{
	// ���������и��ڵ�Ϊ�գ���ֱ����root_��ԱΪ��ָ��
	if (node_values[0] == empty_node_value_)
	{
		root_.reset();
		return;
	}

	// ������ڲ����������������

	TreeNodePtr root = make_shared<TreeNode>(node_values[0]);

	// ����������������������������ʽ��֯�ģ���˴˶���Ҳ������������������ʽ�洢Ԫ�أ�
	// ����ѭ���У��ԿյĶ��ӽ�㣬�����Թ�����ӣ����������һ����ָ��ռλ
	queue<TreeNodePtr> level_order_queue;

	level_order_queue.push(root);

	size_t current_index = 0;

	while (current_index < node_values.size() - 1)
	{
		TreeNodePtr current_node = level_order_queue.front();
		level_order_queue.pop();

		current_index++;
		// �����ǰ����㲻Ϊ�ն��Ҵ���������е�ǰԪ�ص�ֵ�����ڴ���ս���ֵ��
		// ��Ϊ��ǰ����㽨������ӽ��
		if (current_node && node_values[current_index] != empty_node_value_)
		{
			current_node->left_child =
				make_shared<TreeNode>(node_values[current_index]);
			level_order_queue.push(current_node->left_child);
		}
		// ����ֻ����ָ�����
		else
		{
			level_order_queue.push(nullptr);
		}

		current_index++;
		// �����ǰ����㲻Ϊ�ն��Ҵ���������е�ǰԪ�ص�ֵ�����ڴ���ս���ֵ��
		// ��Ϊ��ǰ����㽨���Ҷ��ӽ��
		if (current_node && node_values[current_index] != empty_node_value_)
		{
			current_node->right_child =
				make_shared<TreeNode>(node_values[current_index]);
			level_order_queue.push(current_node->right_child);
		}
		// ����ֻ����ָ�����
		else
		{
			level_order_queue.push(nullptr);
		}
	}

	root_ = root;
}

// ��ӡ������
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

	// ÿ�ν���ѭ����level_traversal_queue����Ԫ��Ϊ��ǰ������н�㣬�����Ѿ�����ӡ����
	// ѭ����Ҫ��ӡ���ǵĺ��ӽ�㣬Ȼ�������Ƴ�level_traversal_queue
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

// ��������ĵݹ�ʵ��
template<typename T>
void BinaryTreePresenter<T>::PreOrderTraversalRecursive() const
{
	PreOrderTraversalRecursiveImp(root_);
}

// ��������ĵ���ʵ��
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

// ��������ĵݹ�ʵ��
template<typename T>
void BinaryTreePresenter<T>::InOrderTraversalRecursive() const
{
	InOrderTraversalRecursiveImp(root_);
}

// ��������ĵ���ʵ��
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

// ��������ĵݹ�ʵ��
template<typename T>
void BinaryTreePresenter<T>::PostOrderTraversalRecursive() const
{
	PostOrderTraversalRecursiveImp(root_);
}

// ��������ĵ���ʵ��
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

// �������
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

// �ж��Ƿ�Ϊ��ȫ������
template<typename T>
bool BinaryTreePresenter<T>::IsComplete() const
{
	if (!root_)
	{
		cout << "<EMPTY TREE>" << endl;
		return true;
	}

	// ʹ�ò�α��������ж�

	// ָʾ�ѱ������Ľ������Ҷ����Ƿ񶼷ǿ�
	bool is_previous_all_full = true;

	queue<TreeNodePtr> level_order_queue;

	level_order_queue.push(root_);

	while (!level_order_queue.empty())
	{
		TreeNodePtr current_node = level_order_queue.front();
		level_order_queue.pop();

		// ��һ�����������Ӷ����Ҷ��ӣ�һ��������ȫ������
		if (!current_node->left_child && current_node->right_child)
		{
			return false;
		}

		// �Ѿ��ų��˵�ǰ���������Ӷ����Ҷ��ӵ������
		// ���ѱ������Ľ������Ҷ��Ӷ��ǿյ�����£������ǰ��㣺
		// 1.������Ӷ����Ҷ��ӣ�
		// 2.���Ҷ��Ӿ�û�У�
		// Ҳ�����Ҷ��ӽ��������һ��Ϊ�գ�
		// ��ô����ѱ������ֲ�����ȫ������
		if (is_previous_all_full)
		{
			if (!current_node->left_child || !current_node->right_child)
			{
				is_previous_all_full = false;
			}
		}
		// ����ֻ�������������Ҷ��Ӷ�û��
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

// ��������ݹ�汾��ʵ��ʵ�ֺ���
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

// ��������ݹ�汾��ʵ��ʵ�ֺ���
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

// ��������ݹ�汾��ʵ��ʵ�ֺ���
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