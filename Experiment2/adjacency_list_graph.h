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

// ���ڽӱ�ʵ�ֵ�ͼ��
template<typename TE, typename TV>
class AdjacencyListGraph final :public AbstractGraph<TE, TV>
{
private:
	// ����
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

	// ������
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

	// ��Ա����BuildGraph������ͼ
	// ����graph_data�����ڽ����ڽӱ�洢��ͼ�����ݰ�
	// tuple��Ա1���������������������ݵ�������
	// tuple��Ա2���������������ڱߵ���Ϣ����������ÿ��Ԫ��Ҳ��һ������������
	// ��Ӧ��������ڱߵ���Ϣ��Ԫ����ÿ�����ڱߵ���һ�����ڶ�����±�ͱ�Ȩ�����;
	// tuple��Ա3���ߵ�������
	void BuildGraph(
		const tuple<vector<TV>,
		vector<vector<pair<size_t, TE>>>,
		size_t>& graph_data);

	// ��Ա����GetAdjacencyMatrixGraphData��
	// ��ȡ���ڽӾ�����ʽ������ǰͼ������AdjacencyMatrixGraph���Ͷ����
	// BuildGraph��Ա����������Ľ�ͼ��������
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
		// ָ������ָ���ָ�룬�����ڽ�ͼʱ�޸Ķ������ʼ�߽��ָ���
		// ���߽�����һ�߽��ָ���ָ��
		shared_ptr<Edge>* current_edge_ptr = &(vertexes[i].first_edge);

		for (auto& j : vex_adj_data[i])
		{
			// �õ�ǰҪ�޸ĵı߽��ָ��ָ���½��ı߽��
			*current_edge_ptr =
				make_shared<Edge>(std::move(j.second), j.first);
			// �ö���ָ��ָ��ǰ��ָ�߽������next_edge��Ա
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

	// ���ѭ��ȷ�����е���ͨ�����������ʵ�
	for (size_t i = 0; i < this->vertex_count; i++)
	{
		if (is_visited[i])
		{
			continue;
		}

		dfs_stack.push(i);

		// ÿ�ν���ѭ����ջ��Ԫ�ض�Ӧ����δ����Ƿ��ʹ���
		// ������š��������С�����Ϊ�����������߼�����¼��
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

	// ÿ�ν���ѭ��������Ԫ�ض�Ӧ�����ѱ���Ƿ��ʹ���
	// ��������š��������С�����Ϊ�����������߼�����¼��
	// ֮�������Ϊ���������ʱ�ͱ���ڽӶ���Ϊ�ѷ��ʣ���Ϊ�˽����һ���⣺
	// ������Ϊ�ڽ���ѭ��ʱ��ǵ�ǰ����Ϊ�ѷ��ʣ���ô������һ�������
	// ������һ��ѭ���У��ڶ�ĳ�����ڽӱ�ı���ʱ������A���������ɶ�����ӣ�������һ��ѭ����
	// ����A��δ���ü������Ϊ�ѷ��ʡ���ʱ��������ǰ������ڽӱ�ʱ���п����ٴν�����A��ӣ�
	// ����ʹ�ö���A����η��ʡ������һ���������ѭ����ʼʱ�����жϵ�ǰ�����Ƿ��ѱ����ʣ�
	// Ҳ�ɲ�ȡ������ơ�����������ȱ����ĵ����㷨�����ܲ��ô˷�����
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
