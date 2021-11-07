#include <iostream>
#include <format>
#include <fstream>
#include <tuple>

#include "adjacency_list_graph.h"
#include "adjacency_matrix_graph.h"

using std::cin;
using std::cout;
using std::endl;

using std::ifstream;

using std::tuple;
using std::make_tuple;

// 图算法展示类
// 模板参数TE: 边权数据类型; TV:顶点数据域数据类型
template<typename TE = int, typename TV = char>
class GraphPresenter final
{
private:
	AdjacencyListGraph<TE, TV> graph_adj_list;
	AdjacencyMatrixGraph<TE, TV> graph_adj_matrix;

	// 成员函数BuildAdjacencyListGraphFromStream:
	// 从给定输入流读取图的邻接表表示数据，
	// 并调用graph_adj_list对象的BuildGraph成员函数建图
	// 流读取到的第一个数据是表示是否是有向图的一个数字，
	// 第二个数据是表图顶点数量的一个正整数vertex_count，
	// 第三个数据是表示图边数量的正整数edge_count，
	// 后面是vertex_count个TV类型的顶点数据域数据，
	// 此后是edge_count对的(边起点下标-边终点下标-边权)数据。
	tuple<vector<TV>,
		const vector<vector<pair<size_t, TE>>>,
		size_t> GetAdjacencyListGraphDataFromStream(std::istream& input_stream);

public:
	GraphPresenter() = default;
	~GraphPresenter() = default;

	// 唯一的对外API，负责整个展示过程的主持
	void BeginPresentation();
};

template<typename TE, typename TV>
tuple<vector<TV>,
	const vector<vector<pair<size_t, TE>>>,
	size_t> GraphPresenter<TE, TV>::
	GetAdjacencyListGraphDataFromStream(std::istream& input_stream)
{
	vector<TV> vex_data;
	vector<vector<pair<size_t, TE>>> vex_adj_data;

	bool is_directed_graph = false;
	size_t vertex_count = 0;
	size_t edge_count = 0;
	input_stream >> is_directed_graph >> vertex_count >> edge_count;

	vex_adj_data.resize(vertex_count);

	for (size_t i = 0; i < vertex_count; i++)
	{
		TV current_vertex_data;
		input_stream >> current_vertex_data;

		vex_data.emplace_back(std::move(current_vertex_data));
	}

	for (size_t i = 0; i < edge_count; i++)
	{
		size_t current_vertex_begin_index = 0;
		size_t current_vertex_end_index = 0;
		TE current_cost;

		input_stream >> current_vertex_begin_index
			>> current_vertex_end_index
			>> current_cost;

		vex_adj_data[current_vertex_begin_index].emplace_back(
			current_vertex_end_index, current_cost);

		if (!is_directed_graph)
		{
			vex_adj_data[current_vertex_end_index].emplace_back(
				current_vertex_begin_index, current_cost);
		}
	}

	return make_tuple(
		std::move(vex_data),
		std::move(vex_adj_data),
		edge_count);
}

template<typename TE, typename TV>
void GraphPresenter<TE, TV>::BeginPresentation()
{
	// BuildAdjacencyListGraphFromStream(cin);

	string file_name;

	cin >> file_name;

	ifstream ifs(file_name, std::ios::in);

	if (!ifs.is_open())
	{
		cout << "DATA FILE COULDN'T BE OPENED. PRESENTATION EXIT." << endl;
		return;
	}

	auto adj_list_graph_data_from_stream =
		GetAdjacencyListGraphDataFromStream(ifs);

	ifs.close();

	graph_adj_list.BuildGraph(adj_list_graph_data_from_stream);

	if (graph_adj_list.vertex_count == 0)
	{
		cout << "EMPTY GRAPH RECEIVED. PRESENTATION EXIT." << endl;
		return;
	}

	graph_adj_list.DisplayGraph();

	graph_adj_list.DFS(false);

	graph_adj_list.DFS(true);

	graph_adj_list.BFS();

	auto adj_matrix_graph_data =
		graph_adj_list.GetAdjacencyMatrixGraphData(0x3F3F3F3F);

	graph_adj_matrix.BuildGraph(adj_matrix_graph_data);

	graph_adj_matrix.DisplayGraph();

	graph_adj_matrix.DFS(false);

	graph_adj_matrix.DFS(true);

	graph_adj_matrix.BFS();

	auto adj_list_graph_data =
		graph_adj_matrix.GetAdjacencyListGraphData();

	graph_adj_list.BuildGraph(adj_list_graph_data);

	graph_adj_list.DisplayGraph();
}

int main()
{
	GraphPresenter presenter;

	presenter.BeginPresentation();

	return 0;
}

