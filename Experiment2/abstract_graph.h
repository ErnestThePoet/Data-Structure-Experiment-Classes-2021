#pragma once

#include <vector>

#include "traversal_results.h"

using std::vector;

// AbstractGraph：图接口类
// 模板参数TE: 边权数据类型; TV:顶点数据域数据类型
// 此接口定义了图的两种存储形式的派生类的通用操作，
// 但建图函数、两种子类之间的相互转换（生成用于构造另一种子类的参数包）需要在子类中独立声明。
template<typename TE, typename TV>
class AbstractGraph
{
protected:
	// 成员函数HostDFSRecursive：递归版本深度优先遍历的外部主调函数，
	// 负责检查图中剩余的未访问顶点并开始递归，以及最后遍历结果的输出
	virtual void HostDFSRecursive() const = 0;
	// 成员函数DFSRecursive：递归版本深度优先遍历的实际实现函数
	// 第二个参数为各个顶点的附加信息：(是否已被访问过-深度优先编号)
	virtual void DFSRecursive(
		size_t current_vertex_index,
		int& current_dfs_number,
		vector<bool>& is_visited,
		TraversalResults<TV>& traversal_results) const = 0;
	// 成员函数DFSIterative：迭代版本深度优先遍历的实际实现函数
	virtual void DFSIterative() const = 0;

	// 成员函数BFSImp：广度优先遍历的实际实现函数
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

	// 成员函数DisplayGraph：显示图的顶点及其邻接表
	virtual void DisplayGraph() const = 0;

	// 成员函数DFS：对图进行深度优先遍历。布尔参数指定是否使用迭代实现。
	virtual void DFS(bool is_iterative) const = 0;

	// 成员函数BFS：对图进行广度优先遍历
	virtual void BFS() const = 0;
};