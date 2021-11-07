#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <stack>
#include <utility>
//#include <iostream>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::stack;
using std::pair;

template<typename T>
class BinarySearchTree final
{
	struct BSTNode
	{
		T value;
		shared_ptr<BSTNode> left_child = nullptr;
		shared_ptr<BSTNode> right_child = nullptr;

		BSTNode(const T& value):value(value){}

		// ���ʹ�úϳɵ�������������ô����ո߶ȹ��ߵĶ��������ʱ����Ϊ����ָ��ĵݹ�����
		// ������ջ�����������������ȷ���˶���������Ե�����ʽ���С�
		~BSTNode()
		{
			//std::cout << "Destructor : " << value << std::endl;

			while (left_child)
			{
				left_child = std::move(left_child->left_child);
			}

			while (right_child)
			{
				right_child = std::move(right_child->right_child);
			}
		}
	};

	using BSTNodePtr = shared_ptr<BSTNode>;

	BSTNodePtr root_ = nullptr;

	// ����Ԫ�صĵݹ�ʵ�ֺ���
	BSTNodePtr& SearchImpl(
		const T& value, 
		const BSTNodePtr& current_node, 
		size_t& compare_count) const;
	// ɾ���Ե�ǰ���Ϊ���Ķ���������������ڵ㲢������ֵ
	T DeleteLeftMostChildAndFetchValue(BSTNodePtr& current_node);

public:
	BinarySearchTree() = default;
	~BinarySearchTree() = default;

	// ��ղ�����
	void Clear();
	// ����Ԫ��
	void Insert(const T& value);
	// ����Ԫ��
	pair<BSTNodePtr&, size_t> Search(const T& value) const;
	// ɾ��Ԫ��
	void Delete(const T& value);
	// �������е��������У�����������У�
	vector<T> GetSortedList() const;
};

template<typename T>
typename BinarySearchTree<T>::BSTNodePtr& BinarySearchTree<T>::SearchImpl(
	const T& value, 
	const BSTNodePtr& current_node, 
	size_t& compare_count) const
{
	compare_count++;

	if (!current_node)
	{
		return const_cast<BSTNodePtr&>(current_node);
	}
	else if (value == current_node->value)
	{
		return const_cast<BSTNodePtr&>(current_node);
	}
	else if (value < current_node->value)
	{
		return SearchImpl(value, current_node->left_child, compare_count);
	}
	else
	{
		return SearchImpl(value, current_node->right_child, compare_count);
	}
}

template<typename T>
T BinarySearchTree<T>::DeleteLeftMostChildAndFetchValue(
	BSTNodePtr& current_node)
{
	if (!current_node->left_child)
	{
		T value = current_node->value;
		current_node = current_node->right_child;
		return value;
	}
	else
	{
		return DeleteLeftMostChildAndFetchValue(current_node->left_child);
	}
}

template<typename T>
void BinarySearchTree<T>::Clear()
{
	root_ = nullptr;
}

template<typename T>
void BinarySearchTree<T>::Insert(const T& value)
{
	//// �����Ϊ����ֱ���½����ڵ�
	//if (!root_)
	//{
	//	root_ = make_shared<BSTNode>(value);
	//	return;
	//}

	//BSTNodePtr current_node = root_;

	//while (true)
	//{
	//	if (value < current_node->value)
	//	{
	//		// ������Ϊ��ʱ���ҵ��˲���λ�ã����в���
	//		if (!current_node->left_child)
	//		{
	//			current_node->left_child = make_shared<BSTNode>(value);
	//			return;
	//		}
	//		// ��������Ϊ�գ�������һ��ѭ���д���������ʼ����
	//		current_node = current_node->left_child;
	//	}
	//	else if (value > current_node->value)
	//	{
	//		if (!current_node->right_child)
	//		{
	//			current_node->right_child = make_shared<BSTNode>(value);
	//			return;
	//		}
	//		current_node = current_node->right_child;
	//	}
	//	// ��ǰ�����ֵ�Ѵ����ڲ������У���ִ���κβ���
	//	else
	//	{
	//		return;
	//	}
	//}

	// ����Search����ʵ��
	BSTNodePtr& target_node = Search(value).first;
	if (!target_node)
	{
		target_node = make_shared<BSTNode>(value);
	}
}

template<typename T>
auto BinarySearchTree<T>::Search(const T& value) const
	->pair<typename BinarySearchTree<T>::BSTNodePtr&, size_t>
{
	size_t compare_count = 0;
	BSTNodePtr& search_result = SearchImpl(value, root_, compare_count);
	return { search_result,compare_count };
}

template<typename T>
void BinarySearchTree<T>::Delete(const T& value)
{
	BSTNodePtr& target_node = Search(value).first;

	if (target_node)
	{
		if (!target_node->left_child)
		{
			target_node = target_node->right_child;
		}
		else if (!target_node->right_child)
		{
			target_node = target_node->right_child;
		}
		else
		{
			target_node->value = 
				DeleteLeftMostChildAndFetchValue(target_node->right_child);
		}
	}
}

template<typename T>
vector<T> BinarySearchTree<T>::GetSortedList() const
{
	vector<T> sorted_list;

	stack<BSTNodePtr> inorder_stack;
	BSTNodePtr current_node = root_;

	while (current_node || !inorder_stack.empty())
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

			sorted_list.push_back(current_node->value);

			current_node = current_node->right_child;
		}
	}

	return sorted_list;
}
