#pragma once
#include <ctime>
#include <iostream>
#include <random>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <format>

#include "exp3bst.h"

using std::cin;
using std::cout;
using std::endl;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::pair;
using std::shuffle;
using std::format;

template<typename T=int>
class SearchPresenter final
{
private:
	// pimpl
	unique_ptr<BinarySearchTree<T>> binary_search_tree_;

	const string line_message_format_ = "{0:*^70s}";

	// 折半查找算法，返回{查找结果下标,查找比较次数}
	pair<int, size_t> BinarySearch(const vector<T>& data,const T& value) const;

	using CompareCount = unsigned long long;

	void PrintAndClearTestResults(
		CompareCount& total_successful_compare_count,
		size_t& total_successful_count,
		CompareCount& total_failure_compare_count,
		size_t& total_failure_count) const;

public:
	SearchPresenter() :
		binary_search_tree_(make_unique<BinarySearchTree<T>>()){}
	void BeginPresentation();
};

template<typename T>
pair<int, size_t> SearchPresenter<T>::BinarySearch(
	const vector<T>& data, const T& value) const
{
	int low = 0, high = data.size() - 1;

	size_t compare_count = 0;

	while (low <= high)
	{
		int mid = (low + high) / 2;

		compare_count++;

		if (value == data[mid])
		{
			return { mid,compare_count };
		}
		else if(value<data[mid])
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	return{ -1,compare_count };
}

template<typename T>
void SearchPresenter<T>::PrintAndClearTestResults(
	CompareCount& total_successful_compare_count, 
	size_t& total_successful_count, 
	CompareCount& total_failure_compare_count, 
	size_t& total_failure_count) const
{
	cout << "TEST RESULTS:" << endl;
	cout << "AVERAGE COMPARES FOR SUCCESSFUL SEARCHES: "
		<< total_successful_compare_count
		<< '/'
		<< total_successful_count
		<< '='
		<< (static_cast<double>(total_successful_compare_count)
			/ total_successful_count) << endl;
	cout << "AVERAGE COMPARES FOR FAILED SEARCHES: "
		<< total_failure_compare_count
		<< '/'
		<< total_failure_count
		<< '='
		<< (static_cast<double>(total_failure_compare_count)
			/ total_failure_count) << endl;
	cout << format(line_message_format_, " TEST ENDS ") << endl << endl << endl;

	total_successful_compare_count = 0;
	total_successful_count = 0;
	total_failure_compare_count = 0;
	total_failure_count = 0;
}

template<typename T>
void SearchPresenter<T>::BeginPresentation()
{
	constexpr size_t kNumberCount = 1024;
	const string kLongDashLine = string(25, '-');

	vector<int> sorted_data(kNumberCount, 1);
	vector<int> unsorted_data(kNumberCount, INT_MIN);

	for (int i = 0; i < kNumberCount; i++)
	{
		sorted_data[i] = unsorted_data[i] = 2 * i + 1;
	}

	shuffle(
		unsorted_data.begin(), 
		unsorted_data.end(), 
		default_random_engine(time(nullptr)));

	cout 
		<< format(line_message_format_,
			" Test data successfully generated for today's experiment ") 
		<< endl << endl << endl;

	// BST Sorted test
	cout 
		<< format(line_message_format_, " TEST BEGINS : BST, 1024 SORTED INTs ") 
		<< endl;
	cout << kLongDashLine << endl << "Inserting test data..." << endl;

	for (int i : sorted_data)
	{
		binary_search_tree_->Insert(i);
	}

	cout << "Successful." << endl << kLongDashLine << endl;

	CompareCount total_successful_compare_count = 0;
	size_t total_successful_count = 0;
	CompareCount total_failure_compare_count = 0;
	size_t total_failure_count = 0;

	for (int i = 1; i <= 2048; i++)
	{
		auto search_results = binary_search_tree_->Search(i);

		if (search_results.first)
		{
			total_successful_compare_count += search_results.second;
			total_successful_count++;
		}
		else
		{
			total_failure_compare_count += search_results.second;
			total_failure_count++;
		}
	}

	PrintAndClearTestResults(total_successful_compare_count,
		total_successful_count,
		total_failure_compare_count,
		total_failure_count);

	binary_search_tree_->Clear();

	// BST Random test
	cout 
		<< format(line_message_format_," TEST BEGINS : BST, 1024 RANDOM INTs ") 
		<< endl;
	cout << kLongDashLine << endl << "Inserting test data..." << endl;

	for (int i : unsorted_data)
	{
		binary_search_tree_->Insert(i);
	}

	cout << "Successful." << endl << kLongDashLine << endl;

	for (int i = 1; i <= 2048; i++)
	{
		auto search_results = binary_search_tree_->Search(i);

		if (search_results.first)
		{
			total_successful_compare_count += search_results.second;
			total_successful_count++;
		}
		else
		{
			total_failure_compare_count += search_results.second;
			total_failure_count++;
		}
	}

	PrintAndClearTestResults(total_successful_compare_count,
		total_successful_count,
		total_failure_compare_count,
		total_failure_count);

	auto bst_sorted_list = binary_search_tree_->GetSortedList();

	binary_search_tree_->Clear();

	// Binary Search Test
	cout 
		<< format(line_message_format_,
			" TEST BEGINS : BINARY SEARCH, 1024 SORTED INTs ")
		<< endl;

	for (int i = 1; i <= 2048; i++)
	{
		auto search_results = BinarySearch(bst_sorted_list, i);

		if (search_results.first!=-1)
		{
			total_successful_compare_count += search_results.second;
			total_successful_count++;
		}
		else
		{
			total_failure_compare_count += search_results.second;
			total_failure_count++;
		}
	}

	PrintAndClearTestResults(total_successful_compare_count,
		total_successful_count,
		total_failure_compare_count,
		total_failure_count);
}
