#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "reservoir_ada.h"
#include "stream.h"

class adaptive {
public:
	adaptive(reservoir_ada);
	~adaptive();
	unsigned long long get_time_step();
	std::vector < double > get_runtime();
	std::vector < double > get_res();
	long double get_global_count();
	void process_stream(int, stream);
	unsigned long long BFC_in_reservoir();
	double elapsed_time();
	std::vector<int> count_full_reservoir();
	std::vector<int> count_sampled_edges();
private:
	double time_secs;
	unsigned long long degree_sq(std::unordered_map < int, std::unordered_set<int> > &);
	void process_edge(int, int);
	double ran_double();
	long long bfc_per_edge(int, int);
	long double global_counter;
	unsigned long long time_step;
	reservoir_ada R;
	double estimated_butterfly;
	double prob;
	std::vector < double > runtime;
	std::vector < double > res;
	int n_full_reservoir;
	int n_sampled_edges;
	std::vector <int> vec_sampled_edges;
	std::vector <int> vec_full_reservoir;
};



