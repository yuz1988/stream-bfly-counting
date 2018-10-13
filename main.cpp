#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "stream.h"
#include "batch.h"
#include "reservoir_sampling.h"
#include "basic.h"
#include "improv.h"
#include "reservoir_ada.h"
#include "adaptive.h"

std::random_device rd;
std::mt19937_64 gen(0);
std::uniform_real_distribution<double> dbl_ran;
std::uniform_int_distribution<int> dbl_int;

int main(int argc, const char * argv[]) {
	std::ios::sync_with_stdio(false);
	int MAX_ITERATIONS, start_res_sz, end_res_sz;

	if (argc <= 1) {
		fprintf(stderr, "Error: file <name of algorithm>\n"); fflush(stderr);
		fprintf(stderr, "Options: batch | exact | Res | IRes | Ada \n"); fflush(stderr);
		exit(0);
	}

	std::string algorithm(argv[1], argv[1] + strlen(argv[1]));

	if (algorithm != "batch" && algorithm != "Res" && algorithm != "IRes" && algorithm != "Ada" && algorithm != "exact") {
		fprintf(stderr, " Algorithm not found\n"); fflush(stderr);
		exit(0);
	}

	if (algorithm != "exact" && algorithm != "batch") {
		std::cerr << " Insert the number of repeation of the algorithm" << std::endl;
		std::cerr << " >>> "; std::cin >> MAX_ITERATIONS;
		std::cerr << " Insert the minimum reservoir size" << std::endl;
		std::cerr << " >>> "; std::cin >> start_res_sz;
		std::cerr << " Insert the maximum reservoir size" << std::endl;
		std::cerr << " >>> "; std::cin >> end_res_sz;
	}

	stream S;
	S.read_filenames();
	freopen(S.output_address, "w", stdout);

	fprintf(stderr, " #E, #V(Left), #V(Right), max degree, average degree\n"); fflush(stderr);
	fprintf(stderr, " %d,%d,%d,%d,%.2f\n", S.E, S.n_L, S.n_R, S.max_degree, ((double)S.E * 2.) / (S.n_L + S.n_R));

	if (algorithm == "batch") {
		// batch exact algorithm 
		reservoir_ada* R_batch = new reservoir_ada();
		R_batch->set_max_size((int)1e9);
		batch* batch_instance = new batch(*R_batch);
		printf("algo, batch, time, bfly\n");
		batch_instance->process_stream(S);
	}
	else if (algorithm == "exact") {
		// exact algorithm is basically basic algorithm with infinity reservoir size
		reservoir* R_exact = new reservoir();
		R_exact->set_max_size((int)1e9);
		basic_algorithm *exact = new basic_algorithm(*R_exact);
		printf("algo, batch, time, bfly\n");
		exact->process_stream(-1, S);
		delete R_exact;
	} 
	else {
		if (algorithm == "Ada") {
			printf("iteration, res-sz, algo, batch, time, bfly, samplededges, full-reservoir\n");
		}
		else if (algorithm == "Res") {
			printf("iteration, res-sz, algo, batch, time, bfly, samplededges\n");
		}
		else {
			printf("iteration, res-sz, algo, batch, time, bfly\n");
		}
		for (int r_sz = start_res_sz; r_sz <= end_res_sz; r_sz *= 2) {
			for (int itr = 0; itr < MAX_ITERATIONS; itr++) {
				if (algorithm == "Res") {
					reservoir* R_basic = new reservoir();
					R_basic->set_max_size(r_sz);
					basic_algorithm* basic = new basic_algorithm(*R_basic);
					basic->process_stream(itr, S);
					delete R_basic;
					delete basic;
				}
				else if (algorithm == "IRes") {
					reservoir* R_impr = new reservoir();
					R_impr->set_max_size(r_sz);
					impr_algorithm* impr = new impr_algorithm(*R_impr);
					impr->process_stream(itr, S);
					delete R_impr;
					delete impr;
				}
				else {
					reservoir_ada* R_ada = new reservoir_ada();
					R_ada->set_max_size(r_sz);
					adaptive* ada = new adaptive(*R_ada);
					ada->process_stream(itr, S);
					delete R_ada;
					delete ada;
				}
			}
			fprintf(stderr, " reservoir size %d is finished\n", r_sz); fflush(stderr);
		}
	}

	return 0;
}