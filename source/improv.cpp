#include "improv.h"

extern std::mt19937_64 gen;
extern std::uniform_real_distribution<double> dbl_ran;
extern std::uniform_int_distribution<int> dbl_int;

impr_algorithm::impr_algorithm(reservoir R) {
	this->R = *new reservoir();
	this->R.set_max_size(R.get_maximum_size());
	decltype(dbl_ran.param()) new_range(0.0, 1.0);
	dbl_ran.param(new_range); 
}

double impr_algorithm::ran_double() {
	return dbl_ran(gen);
}

int impr_algorithm::ran_int(int A, int B) {
	decltype(dbl_int.param()) new_range(A, B);
	dbl_int.param(new_range);
	return dbl_int(gen);
}

unsigned long long impr_algorithm::get_time_step() {
	return this->time_step;
}

void impr_algorithm::process_edge(int A, int B) {
	this->time_step++;
	this->update_counter(std::make_pair(A, B));
	int idx;
	if ((idx = this->sample_edge()) != -1) {
		this->R.add_edge_to_reservoir(A, B, idx);
	}
}

int impr_algorithm::sample_edge() {
	if (this->time_step <= (unsigned long long)this->R.get_maximum_size()) {
		return R.get_current_size();
	}
	else if (ran_double() * this->time_step <= this->R.get_maximum_size()) {
		int idx = this->ran_int(0, ((int)this->R.edge_pool.size()) - 1);
		R.remove_edge_from_sampled_graph(idx);
		return idx;
	}
	else
		return -1;
}

long long impr_algorithm::bfs_per_edge(int A, int B) {
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

long double impr_algorithm::my_function(int a, long long b) {
	int M = this->R.get_maximum_size();
	if (b <= this->R.get_maximum_size()) {
		return 1;
	}
	long double ret = 1;
	for (int i = 0; i < a; i++) {
		ret *= ((long double)(b - i)) / ((M - i) * 1.);
	}
	return ret;
}

long double impr_algorithm::get_global_count() {
	return this->global_counter;
}

void impr_algorithm::update_counter(std::pair <int, int> edge) {
	long long local_answer = this->bfs_per_edge(edge.first, edge.second);
	long double M = this->R.get_maximum_size();
	long double t = (long double)this->time_step;
	long double eta = mmax(1., ((t - 1.) / (M * (M - 1.) * (M - 2.))) * (t - 2.) * (t - 3.));
	this->global_counter += local_answer * eta ;
}

void impr_algorithm::process_stream(int itr, stream S) {
	this->time_secs = 0.;
	this->time_step = 0;
	this->global_counter = 0;
	this->res.clear();
	this->runtime.clear();

	for (auto edge : S.edges) {
		int A = edge.first;
		int B = edge.second;
		clock_t beg = clock();
		this->process_edge(A, B);
		clock_t end = clock();
		this->time_secs += ((double)end - beg) / CLOCKS_PER_SEC;
		if (this->time_step % S.batch_size == 0) {
			printf("%d, %d, IRes, %d, %.2f, %.2Lf\n", itr + 1, this->R.get_maximum_size(), (int)this->time_step, this->time_secs, this->get_global_count());
			fflush(stdout);
		}
	}
	return;
}

double impr_algorithm::elapsed_time() {
	return this->time_secs;
}

std::vector < double > impr_algorithm::get_runtime() {
	return this->runtime;
}

std::vector < double > impr_algorithm::get_res() {
	return this->res;
}