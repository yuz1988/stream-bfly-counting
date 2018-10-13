#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "reservoir_sampling.h"
#include "stream.h"

class basic_algorithm {
public:
	basic_algorithm(reservoir);
	~basic_algorithm();
	unsigned long long get_time_step();
	std::vector < double > get_runtime();
	std::vector < double > get_res();
	long double get_global_count();
	void process_stream(int, stream);
	double elapsed_time();
	std::vector<int> count_sampled_edges();
private:
	double time_secs;
	void process_edge(int, int);
	int sample_edge();
	double ran_double();
	int ran_int(int, int);
	long long bfs_per_edge(int, int);
	void update_counter(int, std::pair<int, int>);
	long double my_function(int, long long);
	long double global_counter; 
	unsigned long long time_step;
	reservoir R;
	double estimated_butterfly;
	std::vector < double > runtime;
	std::vector < double > res;
	int n_sampled_edges;
	std::vector <int> vec_sampled_edges;
};



