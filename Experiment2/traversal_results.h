#pragma once

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum class TraversalResultsType
{
	DFS_RECURSIVE,
	DFS_ITERATIVE,
	BFS
};

// 遍历结果类，包含深度或广度优先遍历的生成树边集、遍历序列、各顶点遍历编号
// 模板参数TV:顶点数据域数据类型
template<typename TV>
class TraversalResults final
{
public:
	TraversalResultsType results_type;
	// 生成树边集，每个元素都具有
	// ([vertex_index1]vertex_data1-[vertex_index2]vertex_data2@cost)的形式
	vector<string> spanning_tree_edges;
	// 遍历序列，每个元素都具有([vertex_index]vertex_data)的形式
	vector<string> traversal_list;
	// 各顶点遍历编号
	vector<string> traversal_numbers;

	// 构造时需传入顶点数量，预先为各顶点遍历编号存储容器保留空间
	TraversalResults(size_t vertex_count,TraversalResultsType results_type):
		results_type(results_type)
	{
		traversal_numbers.resize(vertex_count);
	}

	~TraversalResults() = default;

	// 成员函数：DisplayTraversalResults，展示给定的遍历结果
	static void DisplayTraversalResults(
		const TraversalResults<TV>& traversal_results)
	{
		string hint;

		switch (traversal_results.results_type)
		{
			case TraversalResultsType::DFS_RECURSIVE:
				hint = "[DFS(RECURSIVE) RESULTS]";
				break;
			case TraversalResultsType::DFS_ITERATIVE:
				hint = "[DFS(ITERATIVE) RESULTS]";
				break;
			case TraversalResultsType::BFS:
				hint = "[BFS RESULTS]";
				break;
		}

		cout << hint << endl;
		cout << "TRAVERSAL LIST: ";

		for (auto& i : traversal_results.traversal_list)
		{
			cout << i << ' ';
		}

		cout << endl;
		cout << "TRAVERSAL NUMBERS: ";

		for (auto& i : traversal_results.traversal_numbers)
		{
			cout << i << ' ';
		}

		cout << endl;
		cout << "SPANNING TREE EDGES: ";

		for (auto& i : traversal_results.spanning_tree_edges)
		{
			cout << i << ' ';
		}

		cout << endl << endl;
	}
};