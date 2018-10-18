#include "adaptive.h"

extern std::mt19937_64 gen;
extern std::uniform_real_distribution<double> dbl_ran;

adaptive::adaptive(reservoir_ada R) {
	this->R = *new reservoir_ada();
	this->R.set_max_size(R.get_maximum_size());
	decltype(dbl_ran.param()) new_range(0.0, 1.0);
	dbl_ran.param(new_range);
}

adaptive::~adaptive() {}

double adaptive::ran_double() {
	double ret = dbl_ran(gen);
	return ret;
}

unsigned long long adaptive::get_time_step() {
	return this->time_step;
}

unsigned long long adaptive::degree_sq(std::unordered_map < int, std::unordered_set<int> > &partition) {
	unsigned long long res = 0;
	for (auto p : partition) {
		res += ((unsigned long long)p.second.size()) * ((unsigned long long)p.second.size());
	}
	return res;
}

//	Count the number of butterflies inside the reservoir
unsigned long long adaptive::BFC_in_reservoir() { 
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

void adaptive::process_edge(int A, int B) {
	this->time_step++;
	if ((this->R.get_current_size() == this->R.get_maximum_size())) {
		// TODO: we can do another method to sample exactly 50% of reservoir edges. Use vector for edge_pool.
		// With this method we do not need to have new_R. 
		while (this->R.get_current_size() == this->R.get_maximum_size()) {
			reservoir_ada *new_R = new reservoir_ada();
			new_R->set_max_size(this->R.get_maximum_size());
			for (auto e : this->R.edge_pool) {
				std::pair<int, int> p = R.to_edge(e);
				int x = p.first, y = p.second;
				if (ran_double() <= 0.5) {
					new_R->add_edge_to_reservoir(x, y);
					// one optimization is that we can memoize the value of local BFC for each edge.
				}
			}
			this->R = *new_R;
			delete new_R;
		}
		this->n_full_reservoir++;
		this->prob /= 2.;
		this->global_counter = (long double)this->BFC_in_reservoir();
	}
	if (ran_double() <= this->prob) {
		this->n_sampled_edges++;
		this->R.add_edge_to_reservoir(A, B);
		this->global_counter += bfc_per_edge(A, B);
	}
}

long long adaptive::bfc_per_edge(int A, int B) {
	std::unordered_set <int> adj_A = this->R.get_adj_left(A);
	std::unordered_set <int> adj_B = this->R.get_adj_right(B);
	std::unordered_map <int, long long> counter;
	for (auto neighbor_a : adj_A) {
		if (neighbor_a == B) continue;
		std::unordered_set <int> two_hop_adj = this->R.get_adj_right(neighbor_a);
		for (auto two_neighbor_a : two_hop_adj) {
			counter[two_neighbor_a] ++;
		}
	}
	long long local_answer = 0;
	for (auto neighbor_b : adj_B) {
		if (neighbor_b != A && counter.find(neighbor_b) != counter.end()) {
			local_answer += counter[neighbor_b];
		}
	}
	return local_answer;
}


long double adaptive::get_global_count() {
	double inv_p = 1. / this->prob;
	return this->global_counter * inv_p * inv_p * inv_p * inv_p ;
}

void adaptive::process_stream(int itr, stream S) {
	this->prob = 1.;
	this->time_secs = 0.;
	this->time_step = 0;
	this->global_counter = 0;
	this->n_full_reservoir = 0;
	this->n_sampled_edges = 0;
	this->res.clear();
	this->runtime.clear();
	this->vec_full_reservoir.clear();
	this->vec_sampled_edges.clear();

	for (auto edge : S.edges) {
		int A = edge.first;
		int B = edge.second;
		clock_t beg = clock();
		this->process_edge(A, B);
		clock_t end = clock();
		this->time_secs += ((double)end - beg) / CLOCKS_PER_SEC;
		if (this->time_step % S.batch_size == 0) {
			printf("%d, %d, Ada, %d, %.2f, %.2Lf, %d, %d\n", itr + 1, this->R.get_maximum_size(), (int)this->time_step, 
				this->time_secs, this->get_global_count(), this->n_sampled_edges, this->n_full_reservoir);
			fflush(stdout);
		}
	}
	return;
}

double adaptive::elapsed_time() {
	return this->time_secs;
}

std::vector < double > adaptive::get_runtime() {
	return this->runtime;
}

std::vector < double > adaptive::get_res() {
	return this->res;
}

std::vector<int> adaptive::count_full_reservoir() {
	return this->vec_full_reservoir;
}

std::vector<int> adaptive::count_sampled_edges() {
	return this->vec_sampled_edges;
}