#ifndef _RANDDISTRO_H
#define _RANDDISTRO_H

#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <map>

class RandomNodes
{
	public:
		static uint16_t getRandomNeighbor(uint16_t previous, std::vector<uint16_t> nbors, std::default_random_engine &gen);
		// static uint16_t getRandomNeighbor(std::vector<uint16_t> nbors, std::default_random_engine &gen);
		static uint16_t getPheromoneNeighbor(uint16_t previos, std::map<uint16_t, double> edges, double powCoeff, std::default_random_engine &gen);
		static double rand_exponential(double mean, std::default_random_engine &gen);
		static uint16_t rand_uniform(uint16_t min, uint16_t max, std::default_random_engine &gen);
		static double rand_discrete(std::vector<double> edges, std::default_random_engine &gen);
		static std::default_random_engine _generator;
};


#endif
