#include "reservoir_sampling.h"

reservoir::reservoir() {
	this->clear();
}

reservoir::~reservoir() {}

void reservoir::add_edge_to_reservoir(int A, int B, int idx) {
	if ((int)this->edge_pool.size() == idx) {
		edge_pool.push_back(std::make_pair(A, B));
	}
	else {
		edge_pool[idx] = std::make_pair(A, B);
	}
	this->adj_left[A].insert(B);
	this->adj_right[B].insert(A);
	this->current_reservoir_size++;
}

void reservoir::remove_edge_from_sampled_graph(int idx) {
	int A = this->edge_pool[idx].first;
	int B = this->edge_pool[idx].second;
	this->adj_left[A].erase(B);
	this->adj_right[B].erase(A);
	this->current_reservoir_size--;
}

void reservoir::set_max_size(int sz) {
	this->max_size_of_reservoir = sz;
	return;
}

int reservoir::get_maximum_size() {
	return this->max_size_of_reservoir;
}

int reservoir::get_current_size() {
	return this->current_reservoir_size;
}

std::unordered_set <int> reservoir::get_adj_left(int vertex) {
	return this->adj_left[vertex];
}

std::unordered_set <int> reservoir::get_adj_right(int vertex) {
	return this->adj_right[vertex];
}

void reservoir::clear() {
	this->adj_left.clear();
	this->adj_right.clear();
	this->edge_pool.clear();
	this->current_reservoir_size = 0;
}
