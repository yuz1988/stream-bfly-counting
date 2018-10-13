#include "basic.h"

extern std::mt19937_64 gen;
extern std::uniform_real_distribution<double> dbl_ran;
extern std::uniform_int_distribution<int> dbl_int;

basic_algorithm::basic_algorithm(reservoir R) {
	this->R = *new reservoir();
	this->R.set_max_size(R.get_maximum_size());
	decltype(dbl_ran.param()) new_range(0.0, 1.0);
	dbl_ran.param(new_range);
}

basic_algorithm::~basic_algorithm() {}

double basic_algorithm::ran_double() {
	double ret = dbl_ran(gen);
	return ret;
}

int basic_algorithm::ran_int(int A, int B) {
	decltype(dbl_int.param()) new_range(A, B);
	dbl_int.param(new_range);
	return dbl_int(gen);
}

unsigned long long basic_algorithm::get_time_step() {
	return this->time_step;
}

void basic_algorithm::process_edge(int A, int B) {
	this->time_step++;
	int idx;
	if ((idx = this->sample_edge()) != -1) {
		this->n_sampled_edges++;
		this->R.add_edge_to_reservoir(A, B, idx);
		this->update_counter(+1, std::make_pair(A, B));
	}
}

int basic_algorithm::sample_edge() {
	if (this->time_step <= (unsigned long long)this->R.get_maximum_size()) {
		return R.get_current_size();
	}
	else if (ran_double() * this->time_step <= this->R.get_maximum_size()) {
		int idx = this->ran_int(0, ((int)this->R.edge_pool.size()) - 1);
		R.remove_edge_from_sampled_graph(idx);
		this->update_counter(-1, this->R.edge_pool[idx]);
		return idx;
	}
	else
		return -1;
}

long long basic_algorithm::bfs_per_edge(int A, int B) {
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

long double basic_algorithm::my_function(int a, long long b) {
	int M = R.get_maximum_size();
	if (b <= M) {
		return 1;
	}
	long double ret = 1;
	for (int i = 0; i < a; i++) {
		ret *= ((long double)(b - i)) / ((M - i) * 1.);
	}
	return ret;
}

long double basic_algorithm::get_global_count() {
	return this->global_counter * this->my_function(4, this->time_step);
}

void basic_algorithm::update_counter(int op, std::pair <int, int> edge) {
	long long local_answer = this->bfs_per_edge(edge.first, edge.second);
	this->global_counter += local_answer * op;
}

void basic_algorithm::process_stream(int itr, stream S) {
	this->time_secs = 0.;
	this->time_step = 0;
	this->global_counter = 0;
	this->n_sampled_edges = 0;
	this->res.clear();
	this->runtime.clear();
	this->vec_sampled_edges.clear();
	for (auto edge : S.edges) {
		int A = edge.first;
		int B = edge.second;
		clock_t beg = clock();
		this->process_edge(A, B);
		clock_t end = clock();
		this->time_secs += ((double)end - beg) / CLOCKS_PER_SEC ;
		if (this->time_step % S.batch_size == 0) {
			if (itr == -1) { // the algorithm should be exact
				printf("exact, %d, %.2f, %.2Lf\n", (int)this->time_step, this->time_secs, this->get_global_count()); 
				fflush(stdout);
			}
			else {
				printf("%d, %d, Res, %d, %.2f, %.2Lf, %d\n", itr + 1, this->R.get_maximum_size(), (int)this->time_step, this->time_secs, this->get_global_count(), this->n_sampled_edges);  
				fflush(stdout);
			}
		}
	}
	return;
}

double basic_algorithm::elapsed_time() {
	return this->time_secs;
}

std::vector < double > basic_algorithm::get_runtime() {
	return this->runtime;
}

std::vector < double > basic_algorithm::get_res() {
	return this->res;
}

std::vector<int> basic_algorithm::count_sampled_edges() {
	return this->vec_sampled_edges;
}