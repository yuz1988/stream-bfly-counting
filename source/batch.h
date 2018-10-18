#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "reservoir_ada.h"
#include "stream.h"

class batch {
public:
	batch(reservoir_ada);
	~batch();
	unsigned long long get_time_step();
	void process_stream(stream);
	unsigned long long BFC_in_reservoir();
	double elapsed_time();
private:
	double time_secs;
	unsigned long long degree_sq(std::unordered_map < int, std::unordered_set<int> > &);
	void process_edge(int, int);
	long double global_counter;
	unsigned long long time_step;
	reservoir_ada R;
};
