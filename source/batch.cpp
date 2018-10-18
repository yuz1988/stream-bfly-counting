#include "batch.h"

extern std::mt19937_64 gen;
extern std::uniform_real_distribution<double> dbl_ran;

batch::batch(reservoir_ada R) {
	this->R = *new reservoir_ada();
	this->R.set_max_size(R.get_maximum_size());
	decltype(dbl_ran.param()) new_range(0.0, 1.0);
	dbl_ran.param(new_range);
}

batch::~batch() {}

unsigned long long batch::get_time_step() {
	return this->time_step;
}

unsigned long long batch::degree_sq(std::unordered_map < int, std::unordered_set<int> > &partition) {
	unsigned long long res = 0;
	for (auto p : partition) {
		res += ((unsigned long long)p.second.size()) * ((unsigned long long)p.second.size());
	}
	return res;
}

//	Count the number of butterflies inside the reservoir
unsigned long long batch::BFC_in_reservoir() {
	std::unordered_map < int, std::unordered_set<int> > left_par = this->R.left_partition();
	std::unordered_map < int, std::unordered_set<int> > right_par = this->R.right_partition();
	unsigned long long d2_l = degree_sq(left_par);
	unsigned long long d2_r = degree_sq(right_par);
	if (d2_l < d2_r) {
		std::swap(left_par, right_par);
	}
	unsigned long long total_count = 0;
	for (auto p : left_par) {
		int l = p.first;
		std::unordered_map <int, unsigned long long> counter;
		for (auto r : left_par[l]) {
			for (auto l2 : right_par[r]) if (l2 > l) {
				if (counter.find(l2) != counter.end()) {
					total_count += counter[l2];
					counter[l2] ++;
				}
				else {
					counter[l2] = 1;
				}
			}
		}
	}
	return total_count;
}

void batch::process_edge(int A, int B) {
	this->time_step++;
	this->R.add_edge_to_reservoir(A, B);
}

void batch::process_stream(stream S) {
	this->time_secs = 0.;
	this->time_step = 0;
	this->global_counter = 0;

	for (auto edge : S.edges) {
		int A = edge.first;
		int B = edge.second;
		this->process_edge(A, B);
		if (this->time_step % S.batch_size == 0) {
			clock_t beg = clock();
			this->global_counter = (long double)this->BFC_in_reservoir();
			clock_t end = clock();
			this->time_secs = ((double)end - beg) / CLOCKS_PER_SEC;
			printf("batch, %d, %.2f, %.2Lf\n", (int)this->time_step, this->time_secs, this->global_counter); fflush(stdout);
		}
	}
	return;
}

double batch::elapsed_time() {
	return this->time_secs;
}
