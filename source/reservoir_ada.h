#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>



class reservoir_ada {
public:
	reservoir_ada();
	~reservoir_ada();
	void add_edge_to_reservoir(int, int);
	void remove_edge_from_sampled_graph(int, int);
	void set_max_size(int);
	int get_maximum_size();
	int get_current_size();
	std::unordered_set <int> get_adj_left(int);
	std::unordered_set <int> get_adj_right(int);
	std::unordered_map < int, std::unordered_set<int> > left_partition();
	std::unordered_map < int, std::unordered_set<int> > right_partition();
	std::unordered_set < long long > edge_pool;
	std::pair <int,int> to_edge(long long &);
	long long to_hash(int, int);
	void clear();
private:
	std::unordered_map< int, std::unordered_set<int> > adj_left;
	std::unordered_map< int, std::unordered_set<int> > adj_right;
	int max_size_of_reservoir;
	int current_reservoir_size;
};