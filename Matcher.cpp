#include "Matcher.h"


Matcher::Matcher() {

} // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields


Matcher::~Matcher() {

} // Maybe we need custom destructor?


void Matcher::read_inputs() {

}


KDTree *Matcher::build_kd_tree(std::vector<Patient *> patient_list) {

} // Build KD tree from green and blue groups


std::tuple<Patient*, Patient*, Patient*> Matcher::get_best_match_for_red(Patient*) {

} // Given a red patient, return best available match


void Matcher::build_best_match_queue() {

} // Construct best match queue 


void Matcher::match_from_queue() {

} // Find best match for each red patient