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

// ��������࣬������Ȼ������ȱ������������߼����������С�������������
// ģ�����TV:������������������
template<typename TV>
class TraversalResults final
{
public:
	TraversalResultsType results_type;
	// �������߼���ÿ��Ԫ�ض�����
	// ([vertex_index1]vertex_data1-[vertex_index2]vertex_data2@cost)����ʽ
	vector<string> spanning_tree_edges;
	// �������У�ÿ��Ԫ�ض�����([vertex_index]vertex_data)����ʽ
	vector<string> traversal_list;
	// ������������
	vector<string> traversal_numbers;

	// ����ʱ�贫�붥��������Ԥ��Ϊ�����������Ŵ洢���������ռ�
	TraversalResults(size_t vertex_count,TraversalResultsType results_type):
		results_type(results_type)
	{
		traversal_numbers.resize(vertex_count);
	}

	~TraversalResults() = default;

	// ��Ա������DisplayTraversalResults��չʾ�����ı������
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