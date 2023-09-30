#ifndef _DAVAUS_MATCHER_H_
#define _DAVAUS_MATCHER_H_

#include <set>
#include <string>
#include <tuple>
#include "Patient.h"
#include "Patient_KDTree.h"

//  #############################################################################
//  Matcher is the main class of the three-way matching algorithm
//  #############################################################################

// input: Four columns: Patient ID, treatment group, two columns of propensity scores
// output: dataset with one row per match patient with ID of matched patient and new ID of matched triplet they belong to

// Match contains the three matched patients and the overall distance value
struct Match {
    double dist;
    Patient* p1;
    Patient* p2;
    Patient* p3;
}; 

struct match_cmp { // Objects with larger distances are "smaller" so that a set has the smallest distance at the end
  inline bool operator() (const Match& struct1, const Match& struct2) {
    return (struct1.dist > struct2.dist);
  }
};

inline bool operator<(const Match& lhs, const Match& rhs) { 
    return lhs.dist < rhs.dist;
}


class Matcher {
    protected:
      std::multiset<Match> *match_list; // A set of potential best matches, from smallest distance to largest
      std::vector<Patient*> *red_vec; // A vector of red Patients still to be matched
      std::set<point> *blue_set; // A set of blue Patients still to be matched
      std::set<point> *green_set; // A set of green Patients still to be matched
      kdtree* green_kd_tree;
      kdtree* blue_kd_tree;
      float caliper;

    public:
      Matcher(); // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields
      ~Matcher();
      void read_inputs(std::string file_name, float caliper_coeff); // read inputs, create patients, and sort into respective slots
      Match get_best_match_for_red(Patient* red_p); // Given a red patient, return best available match
      void build_best_match_list(bool verbose); // Construct best match queue 
      std::vector<Match> *match_from_list(); // Find best match for each red patient
      void print_unmatched() const; // Print all unmatched patients
      void print_match_list() const; // Print all match in the match list
};


#endif