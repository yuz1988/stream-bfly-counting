#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <string>
#include <string.h>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>
#include <math.h>
#include <random>
#include <time.h>

#define mmin(x, y) ((x)<(y)?(x):(y))
#define mmax(x, y) ((x)>(y)?(x):(y))


class stream {
public:
	stream();
	~stream();
	std::vector < std::pair <int, int> > edges;
public:
	void read_filenames();
	char input_address[2000], output_address[2000];
	int batch_size;
	void preprocessing();
	int E, n_L, n_R;
	int max_degree;
private:
	int to_s(std::string &);
	bool all_num(std::string &);
};
