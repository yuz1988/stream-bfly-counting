#include "reservoir_ada.h"

const long long shift = 1000 * 1000 * 1000;

reservoir_ada::reservoir_ada() {
	this->clear();
}

reservoir_ada::~reservoir_ada() {}

long long reservoir_ada::to_hash(int A, int B) {
	return A * shift + B;
}

std::pair <int, int> reservoir_ada::to_edge(long long &hash) {
	return std::make_pair(hash / shift, hash % shift);
}

void reservoir_ada::add_edge_to_reservoir(int A, int B) {
	this->edge_pool.insert(this->to_hash(A, B));
	this->adj_left[A].insert(B);
	this->adj_right[B].insert(A);
	this->current_reservoir_size++;
}

void reservoir_ada::remove_edge_from_sampled_graph(int A, int B) {
	this->adj_left[A].erase(B);
	this->adj_right[B].erase(A);
	this->edge_pool.erase(this->to_hash(A, B));
	this->current_reservoir_size--;
}

void reservoir_ada::set_max_size(int sz) {
	this->max_size_of_reservoir = sz;
	return;
}

int reservoir_ada::get_maximum_size() {
	return this->max_size_of_reservoir;
}

int reservoir_ada::get_current_size() {
	return this->current_reservoir_size;
}

std::unordered_set <int> reservoir_ada::get_adj_left(int vertex) {
	return this->adj_left[vertex];
}

std::unordered_set <int> reservoir_ada::get_adj_right(int vertex) {
	return this->adj_right[vertex];
}

void reservoir_ada::clear() {
	this->adj_left.clear();
	this->adj_right.clear();
	this->edge_pool.clear();
	this->current_reservoir_size = 0;
}

std::unordered_map < int, std::unordered_set<int> > reservoir_ada::left_partition() {
	return this->adj_left;
}
std::unordered_map < int, std::unordered_set<int> > reservoir_ada::right_partition() {
	return this->adj_right;
}
