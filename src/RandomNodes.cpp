#include "RandomNodes.h"


// uint16_t RandomNodes::getRandomNeighbor(std::vector<uint16_t> nbors, std::default_random_engine &gen)
// {
//     // get a random number from a uniform distribution in the range
//     // of 0 and the number of neighbors this will be the neighbor's index
//     uint16_t nborIndex = rand_uniform(0, nbors.size() - 1, gen);
//     uint16_t nbor;

//     nbor = nbors.at(nborIndex);

//     return nbor;
// }


uint16_t RandomNodes::getRandomNeighbor(uint16_t prevSender, std::vector<uint16_t> nbors, std::default_random_engine &gen)
{
    // get a random number from a uniform distribution in the range
    // of 0 and the number of neighbors that to this Node
    // this will be the neighbor's index
    uint16_t nborIndex = rand_uniform(0, nbors.size() - 1, gen);
    uint16_t nbor;

    // if the neighbor at index "nborIndex" was the last sender of the the
    // message then get a neighbor "nbor"
    if(nbors.at(nborIndex) == prevSender){
        if(nbors.size() != 1)
            nbor = getRandomNeighbor(prevSender, nbors, gen);
    }
    else{
        // if not assign neighbor
        nbor = nbors.at(nborIndex);
    }

    return nbor;
}

uint16_t RandomNodes::getPheromoneNeighbor(uint16_t prev, const std::map<uint16_t, double> edges, double powCoef, std::default_random_engine &gen)
{
    /* Store values and keys in parallel vectors */
    std::vector<uint16_t> keys;
    std::vector<double> discretValues;
    std::map<uint16_t, double>::const_iterator iter;
    uint16_t nborIndex = 0;

    if(edges.size() != 1){
        for(iter = edges.begin(); iter != edges.end(); iter++){
            if(iter->first != prev){
                keys.push_back(iter->first);
                discretValues.push_back(pow(iter->second, powCoef));
            }
        }
    
        nborIndex = rand_discrete(discretValues, gen);
    }
    else{
        keys[nborIndex] = prev;
    }

    return keys.at(nborIndex);
}

double RandomNodes::rand_exponential(double mean, std::default_random_engine &gen)
{
    // printTestInfo(_ID, "rand_exp");
    std::exponential_distribution<double> expDistro(mean);
    // create an exponinetial distribution around the mean

    // get a random number fron that distribution based on
    // the static generator
    double randNumber = expDistro(gen);

    return randNumber;
}

uint16_t RandomNodes::rand_uniform(uint16_t min, uint16_t max, std::default_random_engine &gen)
{
    std::uniform_int_distribution<uint16_t> uniDistro(min, max);
    // create a uniform distribution between the min and max

    // get a random number from that distribution based on
    // the static generator
    uint16_t randNumber = uniDistro(gen);

    return randNumber;
}

double RandomNodes::rand_discrete(std::vector<double> edges, std::default_random_engine &gen)
{
    std::discrete_distribution<int> discDistro(edges.begin(), edges.end());

    return discDistro(gen);
}
