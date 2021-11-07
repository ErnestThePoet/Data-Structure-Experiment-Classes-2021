#pragma once

#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <tuple>
#include <utility>
#include <memory>

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
using std::shared_ptr;
using std::make_shared;

// 以邻接表实现的图类
template<typename TE, typename TV>
class AdjacencyListGraph final :public AbstractGraph<TE, TV>
{
private:
	// 边类
	class Edge final
	{
	public:
		TE cost;
		size_t adj_vertex_index = 0;
		shared_ptr<Edge> next_edge = nullptr;

		Edge() = default;
		Edge(const TE& cost, size_t adj_vertex_index) :
			cost(cost), adj_vertex_index(adj_vertex_index)
		{}
		Edge(TE&& cost, size_t adj_vertex_index) :
			cost(std::forward<TE>(cost)), adj_vertex_index(adj_vertex_index)
		{}

		~Edge() = default;
	};

	// 顶点类
	class Vertex final
	{
	public:
		TV data;
		shared_ptr<Edge> first_edge = nullptr;

		Vertex() = default;
		Vertex(const TV& data) :data(data) {}
		Vertex(TV&& data) :data(std::forward<TV>(data)) {}

		~Vertex() = default;
	};

	vector<Vertex> vertexes;

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

	AdjacencyListGraph() = default;
	~AdjacencyListGraph() = default;

	void DisplayGraph() const override;

	void DFS(bool is_iterative) const override;

	void BFS() const override;

	// 成员函数BuildGraph：建立图
	// 参数graph_data：用于建立邻接表存储的图的数据包
	// tuple成员1：包含各顶点数据域数据的向量；
	// tuple成员2：包含各顶点相邻边的信息的向量，其每个元素也是一个向量，包含
	// 相应顶点各相邻边的信息，元素是每条相邻边的另一个相邻顶点的下标和边权的组合;
	// tuple成员3：边的数量。
	void BuildGraph(
		const tuple<vector<TV>,
		vector<vector<pair<size_t, TE>>>,
		size_t>& graph_data);

	// 成员函数GetAdjacencyMatrixGraphData：
	// 获取以邻接矩阵形式建立当前图（调用AdjacencyMatrixGraph类型对象的
	// BuildGraph成员函数）所需的建图参数包。
	auto GetAdjacencyMatrixGraphData(const TE infinity_cost)
		->tuple<vector<TV>, vector<vector<TE>>, size_t, const TE> const;
};

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::BuildGraph(
	const tuple<vector<TV>,
	vector<vector<pair<size_t, TE>>>,
	size_t>& graph_data)
{
	auto& vex_data = get<0>(graph_data);
	auto& vex_adj_data = get<1>(graph_data);

	vertexes.clear();
	this->vertex_count = vex_data.size();
	this->edge_count = get<2>(graph_data);

	for (auto& i : vex_data)
	{
		vertexes.emplace_back(std::move(i));
	}

	for (size_t i = 0; i < vex_adj_data.size(); i++)
	{
		// 指向智能指针的指针，用以在建图时修改顶点的起始边结点指针和
		// 各边结点的下一边结点指针的指向
		shared_ptr<Edge>* current_edge_ptr = &(vertexes[i].first_edge);

		for (auto& j : vex_adj_data[i])
		{
			// 让当前要修改的边结点指针指向新建的边结点
			*current_edge_ptr =
				make_shared<Edge>(std::move(j.second), j.first);
			// 让二级指针指向当前被指边结点对象的next_edge成员
			current_edge_ptr = &((*current_edge_ptr)->next_edge);
		}
	}
}

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::DisplayGraph() const
{
	cout << "[DISPLAYING ADJ-LIST GRAPH]" << endl;

	for (size_t i = 0; i < vertexes.size(); i++)
	{
		cout << format("{0}: {1} ", i, vertexes[i].data);

		for (shared_ptr<Edge> j = vertexes[i].first_edge;
			j != nullptr;
			j = j->next_edge)
		{
			cout << format("([{0}]{1}@{2}) ",
				j->adj_vertex_index,
				vertexes[j->adj_vertex_index].data,
				j->cost);
		}

		cout << endl;
	}

	cout << endl;
}

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::DFS(bool is_iterative) const
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
void AdjacencyListGraph<TE, TV>::HostDFSRecursive() const
{
	vector<bool> is_visited(this->vertex_count);

	TraversalResults<TV> traversal_results(
		this->vertex_count, TraversalResultsType::DFS_RECURSIVE);

	int dfs_number_initial = 0;

	size_t component_count = 0;

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (!is_visited[i])
		{
			component_count++;
			DFSRecursive(i, dfs_number_initial, is_visited, traversal_results);
		}
	}

	// cout << "NUMBER OF CONNECTED COMPONENT(s):" << component_count << endl << endl;

	TraversalResults<TV>::DisplayTraversalResults(traversal_results);
}

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::DFSRecursive(
	size_t current_vertex_index,
	int& current_dfs_number,
	vector<bool>& is_visited,
	TraversalResults<TV>& traversal_results) const
{
	is_visited[current_vertex_index] = true;

	traversal_results.traversal_numbers[current_vertex_index] =
		format("([{0}]{1}:{2})",
			current_vertex_index,
			vertexes[current_vertex_index].data,
			current_dfs_number++);

	traversal_results.traversal_list.emplace_back(
		format("([{0}]{1})",
			current_vertex_index, vertexes[current_vertex_index].data));

	for (auto i = vertexes[current_vertex_index].first_edge;
		i != nullptr;
		i = i->next_edge)
	{
		if (!is_visited[i->adj_vertex_index])
		{
			traversal_results.spanning_tree_edges.emplace_back(
				format("([{0}]{1}-[{2}]{3}@{4})",
					current_vertex_index,
					vertexes[current_vertex_index].data,
					i->adj_vertex_index,
					vertexes[i->adj_vertex_index].data,
					i->cost));

			DFSRecursive(
				i->adj_vertex_index, current_dfs_number,
				is_visited, traversal_results);
		}
	}
}

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::DFSIterative() const
{
	vector<bool> is_visited(this->vertex_count);

	TraversalResults<TV> traversal_results(
		this->vertex_count, TraversalResultsType::DFS_ITERATIVE);

	stack<size_t> dfs_stack;

	size_t previous_vertex_index = 0;
	int current_dfs_number = 0;

	// 外层循环确保所有的连通分量都被访问到
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
					vertexes[current_vertex_index].data,
					current_dfs_number++);

			traversal_results.traversal_list.emplace_back(format("([{0}]{1})",
				current_vertex_index, vertexes[current_vertex_index].data));

			for (auto j = vertexes[current_vertex_index].first_edge;
				j != nullptr;
				j = j->next_edge)
			{
				if (!is_visited[j->adj_vertex_index])
				{
					traversal_results.spanning_tree_edges.emplace_back(
						format("([{0}]{1}-[{2}]{3}@{4})",
							current_vertex_index,
							vertexes[current_vertex_index].data,
							j->adj_vertex_index,
							vertexes[j->adj_vertex_index].data,
							j->cost));

					// is_visited[j->adj_vertex_index] = true;

					dfs_stack.push(j->adj_vertex_index);
				}
			}
		}
	}

	TraversalResults<TV>::DisplayTraversalResults(traversal_results);
}

template<typename TE, typename TV>
void AdjacencyListGraph<TE, TV>::BFS() const
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
void AdjacencyListGraph<TE, TV>::BFSImp(
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
	// 之所以设计为在添加树边时就标记邻接顶点为已访问，是为了解决这一问题：
	// 如果设计为在进入循环时标记当前顶点为已访问，那么考虑这一种情况：
	// 若在上一次循环中，在对某顶点邻接表的遍历时将顶点A和其他若干顶点入队，则在这一次循环中
	// 顶点A还未来得及被标记为已访问。此时，遍历当前顶点的邻接表时，有可能再次将顶点A入队，
	// 最终使得顶点A被多次访问。解决这一问题可以在循环开始时额外判断当前顶点是否已被访问，
	// 也可采取此种设计。对于深度优先遍历的迭代算法，则不能采用此方法。
	while (!bfs_queue.empty())
	{
		size_t current_vertex_index = bfs_queue.front();
		bfs_queue.pop();

		traversal_results.traversal_numbers[current_vertex_index] =
			format("([{0}]{1}:{2})",
				current_vertex_index,
				vertexes[current_vertex_index].data,
				current_bfs_number++);

		traversal_results.traversal_list.emplace_back(format("([{0}]{1})",
			current_vertex_index, vertexes[current_vertex_index].data));

		for (auto i = vertexes[current_vertex_index].first_edge;
			i != nullptr;
			i = i->next_edge)
		{
			if (!is_visited[i->adj_vertex_index])
			{
				traversal_results.spanning_tree_edges.emplace_back(
					format("([{0}]{1}-[{2}]{3}@{4})",
						current_vertex_index,
						vertexes[current_vertex_index].data,
						i->adj_vertex_index,
						vertexes[i->adj_vertex_index].data,
						i->cost));

				is_visited[i->adj_vertex_index] = true;

				bfs_queue.push(i->adj_vertex_index);
			}
		}
	}
}

template<typename TE, typename TV>
auto AdjacencyListGraph<TE, TV>::GetAdjacencyMatrixGraphData(
	const TE infinity_cost)
	->tuple<vector<TV>, vector<vector<TE>>, size_t, const TE> const
{
	vector<TV> result_vertexes;
	vector<vector<TE>> result_edges;

	for (auto& i : vertexes)
	{
		result_vertexes.push_back(i.data);

		vector<TE> current_edge_data(this->vertex_count, infinity_cost);

		for (auto j = i.first_edge; j != nullptr; j = j->next_edge)
		{
			current_edge_data[j->adj_vertex_index] = j->cost;
		}

		result_edges.emplace_back(std::move(current_edge_data));
	}

	for (size_t i = 0; i < this->vertex_count; i++)
	{
		result_edges[i][i] = 0;
	}

	cout << "<CONVERTED FROM ADJ-LIST GRAPH TO ADJ-MATRIX GRAPH.>" << endl << endl;

	return make_tuple(
		std::move(result_vertexes),
		std::move(result_edges),
		this->edge_count, infinity_cost);
}
