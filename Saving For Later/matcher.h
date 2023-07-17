#ifndef _DAVAUS_MATCHER_H_
#define _DAVAUS_MATCHER_H_

#include <queue>
#include <tuple>
#include "patient.h"
#include "KDTree/KDTree.hpp"

//  #############################################################################
//  Matcher is the main class of the three-way matching algorithm
//  #############################################################################

class Matcher {
    protected:
      std::queue<std::tuple<Patient*, Patient*, Patient*>> match_queue; // Stores a queue of potential best matches, closest first
      KDTree* green_kd_tree;
      KDTree* blue_kd_tree;

    public:
      Matcher(); // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields
      ~Matcher(); // Maybe we need custom destructor?
      void build_kd_tree(); // Build KD tree from green and blue groups
      std::tuple<Patient*, Patient*, Patient*> get_best_match_for_red(Patient*); // Given a red patient, return best available match
      void build_best_match_queue(); // Construct best match queue 
      void match_from_queue(); // Find best match for each red patient
};


#endif