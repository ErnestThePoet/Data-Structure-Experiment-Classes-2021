#pragma once

#include <vector>

#include "traversal_results.h"

using std::vector;

// AbstractGraph��ͼ�ӿ���
// ģ�����TE: ��Ȩ��������; TV:������������������
// �˽ӿڶ�����ͼ�����ִ洢��ʽ���������ͨ�ò�����
// ����ͼ��������������֮����໥ת�����������ڹ�����һ������Ĳ���������Ҫ�������ж���������
template<typename TE, typename TV>
class AbstractGraph
{
protected:
	// ��Ա����HostDFSRecursive���ݹ�汾������ȱ������ⲿ����������
	// ������ͼ��ʣ���δ���ʶ��㲢��ʼ�ݹ飬�Լ���������������
	virtual void HostDFSRecursive() const = 0;
	// ��Ա����DFSRecursive���ݹ�汾������ȱ�����ʵ��ʵ�ֺ���
	// �ڶ�������Ϊ��������ĸ�����Ϣ��(�Ƿ��ѱ����ʹ�-������ȱ��)
	virtual void DFSRecursive(
		size_t current_vertex_index,
		int& current_dfs_number,
		vector<bool>& is_visited,
		TraversalResults<TV>& traversal_results) const = 0;
	// ��Ա����DFSIterative�������汾������ȱ�����ʵ��ʵ�ֺ���
	virtual void DFSIterative() const = 0;

	// ��Ա����BFSImp��������ȱ�����ʵ��ʵ�ֺ���
	virtual void BFSImp(
		size_t current_vertex_index,
		int& current_bfs_number,
		vector<bool>& is_visited,
		TraversalResults<TV>& traversal_results) const = 0;

public:
	AbstractGraph() = default;
	virtual ~AbstractGraph() = default;

	size_t vertex_count = 0;
	size_t edge_count = 0;

	// ��Ա����DisplayGraph����ʾͼ�Ķ��㼰���ڽӱ�
	virtual void DisplayGraph() const = 0;

	// ��Ա����DFS����ͼ����������ȱ�������������ָ���Ƿ�ʹ�õ���ʵ�֡�
	virtual void DFS(bool is_iterative) const = 0;

	// ��Ա����BFS����ͼ���й�����ȱ���
	virtual void BFS() const = 0;
};