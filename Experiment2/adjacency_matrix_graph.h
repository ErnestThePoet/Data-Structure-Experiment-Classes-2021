#pragma once

#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <tuple>
#include <utility>

#include "abstract_graph.h"
#include "traversal_results.h"

using std::cin;
using std::cout;
using std::endl;

using std::format;

using std::string;
using std::vector;
using std::stack;
using std::queue;
using std::pair;
using std::tuple;
using std::get;
using std::make_tuple;

// 以邻接矩阵实现的图类
template<typename TE, typename TV>
class AdjacencyMatrixGraph final :public AbstractGraph<TE, TV>
{
private:
	vector<TV> vertexes;
	vector<vector<TE>> edges;
	TE infinity_cost_ = 0x3F3F3F3F;

	void HostDFSRecursive() const override;

	void DFSRecursive(
		size_t current_vertex_index,
		int& current_dfs_number,
		vector<bool>& is_visited,
		TraversalResults<TV>& traversal_results) const override;

	void DFSIterative() const override;

	void BFSImp(
		size_t current_vertex_index,
		int& current_bfs_number,
		vector<bool>& is_visited,
		TraversalResults<TV>& traversal_results) const override;

public:
	AdjacencyMatrixGraph() = default;
	~AdjacencyMatrixGraph() = default;

	void DisplayGraph()  const override;

	void DFS(bool is_iterative) const override;

	void BFS() const override;

	// 成员函数BuildGraph：建立图
	// 参数graph_data：用于建立邻接表存储的图的数据包
	// tuple成员1：包含各顶点数据域数据的向量；
	// tuple成员2：邻接矩阵二维向量，以tuple成员3表示无路；
	// tuple成员3：边的数量；
	// tuple成员4：表示无穷大（无路）的TE类型常量。
	void BuildGraph(
		const tuple<vector<TV>,
		vector<vector<TE>>,
		size_t,
		const TE>& graph_data);

	// 成员函数GetAdjacencyListGraphData：
	// 获取以邻接表形式建立当前图（调用AdjacencyListGraph类型对象的
	// BuildGraph成员函数）所需的建图参数包。
	auto GetAdjacencyListGraphData() const
		->tuple<vector<TV>,
		const vector<vector<pair<size_t, TE>>>,
		size_t>;
};

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::HostDFSRecursive() const
{
	vector<bool> is_visited(this->vertex_count);

	TraversalResults<TV> traversal_results(
		this->vertex_count, TraversalResultsType::DFS_RECURSIVE);

	int dfs_number_initial = 0;

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (!is_visited[i])
		{
			DFSRecursive(i, dfs_number_initial, is_visited, traversal_results);
		}
	}

	TraversalResults<TV>::DisplayTraversalResults(traversal_results);
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::DFSRecursive(
	size_t current_vertex_index,
	int& current_dfs_number,
	vector<bool>& is_visited,
	TraversalResults<TV>& traversal_results) const
{
	is_visited[current_vertex_index] = true;

	traversal_results.traversal_numbers[current_vertex_index] =
		format("([{0}]{1}:{2})",
			current_vertex_index,
			vertexes[current_vertex_index],
			current_dfs_number++);

	traversal_results.traversal_list.emplace_back(
		format("([{0}]{1})",
			current_vertex_index, vertexes[current_vertex_index]));

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (edges[current_vertex_index][i] != 0
			&& edges[current_vertex_index][i] < infinity_cost_
			&& !is_visited[i])
		{
			traversal_results.spanning_tree_edges.emplace_back(
				format("([{0}]{1}-[{2}]{3}@{4})",
					current_vertex_index,
					vertexes[current_vertex_index],
					i,
					vertexes[i],
					edges[current_vertex_index][i]));

			DFSRecursive(
				i, current_dfs_number,
				is_visited, traversal_results);
		}
	}
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::DFSIterative() const
{
	vector<bool> is_visited(this->vertex_count);

	TraversalResults<TV> traversal_results(
		this->vertex_count, TraversalResultsType::DFS_ITERATIVE);

	stack<size_t> dfs_stack;

	size_t previous_vertex_index = 0;
	int current_dfs_number = 0;

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (is_visited[i])
		{
			continue;
		}

		dfs_stack.push(i);

		// 每次进入循环，栈顶元素对应顶点未被标记访问过，
		// 遍历编号、遍历序列、以其为起点的生成树边集待记录。
		while (!dfs_stack.empty())
		{
			size_t current_vertex_index = dfs_stack.top();
			dfs_stack.pop();

			if (is_visited[current_vertex_index])
			{
				continue;
			}

			is_visited[current_vertex_index] = true;

			traversal_results.traversal_numbers[current_vertex_index] =
				format("([{0}]{1}:{2})",
					current_vertex_index,
					vertexes[current_vertex_index],
					current_dfs_number++);

			traversal_results.traversal_list.emplace_back(format("([{0}]{1})",
				current_vertex_index, vertexes[current_vertex_index]));

			for (size_t j = 0; j < this->vertex_count; j++)
			{
				if (edges[current_vertex_index][j] != 0
					&& edges[current_vertex_index][j] < infinity_cost_
					&& !is_visited[j])
				{
					traversal_results.spanning_tree_edges.emplace_back(
						format("([{0}]{1}-[{2}]{3}@{4})",
							current_vertex_index,
							vertexes[current_vertex_index],
							j,
							vertexes[j],
							edges[current_vertex_index][j]));

					// is_visited[j] = true;

					dfs_stack.push(j);
				}
			}
		}
	}

	TraversalResults<TV>::DisplayTraversalResults(traversal_results);
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::BFSImp(
	size_t current_vertex_index,
	int& current_bfs_number,
	vector<bool>& is_visited,
	TraversalResults<TV>& traversal_results) const
{
	queue<size_t> bfs_queue;

	bfs_queue.push(current_vertex_index);
	is_visited[current_vertex_index] = true;

	// 每次进入循环，队首元素对应顶点已被标记访问过，
	// 而遍历编号、遍历序列、以其为起点的生成树边集待记录。
	// 设计为在添加树边时就标记邻接顶点为已访问。
	while (!bfs_queue.empty())
	{
		size_t current_vertex_index = bfs_queue.front();
		bfs_queue.pop();

		traversal_results.traversal_numbers[current_vertex_index] =
			format("([{0}]{1}:{2})",
				current_vertex_index,
				vertexes[current_vertex_index],
				current_bfs_number++);

		traversal_results.traversal_list.emplace_back(format("([{0}]{1})",
			current_vertex_index, vertexes[current_vertex_index]));

		for (size_t i = 0; i < this->vertex_count; i++)
		{
			if (edges[current_vertex_index][i] != 0
				&& edges[current_vertex_index][i] < infinity_cost_
				&& !is_visited[i])
			{
				traversal_results.spanning_tree_edges.emplace_back(
					format("([{0}]{1}-[{2}]{3}@{4})",
						current_vertex_index,
						vertexes[current_vertex_index],
						i,
						vertexes[i],
						edges[current_vertex_index][i]));

				is_visited[i] = true;

				bfs_queue.push(i);
			}
		}
	}
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::BuildGraph(
	const tuple<vector<TV>,
	vector<vector<TE>>,
	size_t,
	const TE>& graph_data)
{
	vertexes = get<0>(graph_data);
	edges = get<1>(graph_data);
	this->vertex_count = get<0>(graph_data).size();
	this->edge_count = get<2>(graph_data);
	infinity_cost_ = get<3>(graph_data);
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::DisplayGraph() const
{
	cout << "[DISPLAYING ADJ-MATRIX GRAPH]" << endl;
	cout << "   |";
	for (size_t i = 0; i < this->vertex_count; i++)
	{
		cout << format("{0:>3d}", i);
	}

	cout << endl;

	cout << string(3 + 1, '-');
	cout << string(3 * this->vertex_count, '-') << endl;

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		cout << format("{0:-3d}|", i);

		for (auto& j : edges[i])
		{
			if (j == infinity_cost_)
			{
				cout << format("{0:>3c}", '.');
			}
			else
			{
				cout << format("{0:>3d}", j);
			}
		}

		cout << endl;
	}

	cout << endl;
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::DFS(bool is_iterative) const
{
	if (is_iterative)
	{
		DFSIterative();
	}
	else
	{
		HostDFSRecursive();
	}
}

template<typename TE, typename TV>
void AdjacencyMatrixGraph<TE, TV>::BFS() const
{
	vector<bool> is_visited(this->vertex_count);

	TraversalResults<TV> traversal_results(
		this->vertex_count, TraversalResultsType::BFS);

	int bfs_number_initial = 0;

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (!is_visited[i])
		{
			BFSImp(i, bfs_number_initial, is_visited, traversal_results);
		}
	}

	TraversalResults<TV>::DisplayTraversalResults(traversal_results);
}

template<typename TE, typename TV>
auto AdjacencyMatrixGraph<TE, TV>::GetAdjacencyListGraphData() const
->tuple<vector<TV>,
	const vector<vector<pair<size_t, TE>>>,
	size_t>
{
	vector<vector<pair<size_t, TE>>> vex_adj_data;

	for (auto& i : edges)
	{
		vector<pair<size_t, TE>> current_vex_adj_data;

		for (size_t j = 0; j < this->vertex_count; j++)
		{
			if (i[j] != 0 && i[j] < infinity_cost_)
			{
				current_vex_adj_data.emplace_back(j, i[j]);
			}
		}

		vex_adj_data.emplace_back(std::move(current_vex_adj_data));
	}

	cout << "<CONVERTED FROM ADJ-MATRIX GRAPH TO ADJ-LIST GRAPH.>"
		<< endl << endl;

	return make_tuple(vertexes, std::move(vex_adj_data), this->edge_count);
}