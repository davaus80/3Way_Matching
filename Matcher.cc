#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Patient_KDTree.h"
#include "Matcher.h"


Matcher::Matcher() {
    match_list = nullptr;
    red_vec = nullptr;
    blue_set = nullptr;
    green_set = nullptr;
    green_kd_tree = nullptr;
    blue_kd_tree = nullptr;
} // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields


Matcher::~Matcher() {
    if (match_list) {
        delete match_list;
    }
    if (red_vec) {
        delete red_vec;
    }
    if (green_kd_tree) {
        delete green_kd_tree;
    }
    if (blue_kd_tree) {
        delete blue_kd_tree;
    }

} // Maybe we need custom destructor?


void Matcher::read_inputs(std::string file_name) {
    // Read patients from file. File is expected to have rows ordered as:
    // ID / group / p score 1 / p score 2
    std::ifstream file;
    file.open(file_name); 
    std::string word;
    std::vector<int> group_counter = {0, 0, 0};

    // Load all patients into vectors for blue and green or list for red
    int red = 1;
    int blue = 2;
    blue_set = new std::set<point>();
    green_set = new std::set<point>();
    red_vec = new std::vector<Patient *>();

    while (file >> word) { // There should be four entries per row
        int id = std::stoi(word);
        file >> word;
        int group = std::stoi(word);
        group_counter[group - 1] += 1;
        file >> word;
        float p_score1 = std::stof(word);
        file >> word;
        float p_score2 = std::stof(word);
        Patient *p = new Patient(id, group, p_score1, p_score2);
        point the_point = point(p->get_p_score1(), p->get_p_score2(), p);
        if (p->get_group() == red) {
            red_vec->push_back(p);
        } else if (p->get_group() == blue) {
            blue_set->insert(the_point);
        } else {
            green_set->insert(the_point);
        }
        p = nullptr;
    }

    // Create kd trees from vectors
    blue_kd_tree = new kdtree(blue_set);
    green_kd_tree = new kdtree(green_set);
}


Match *Matcher::get_best_match_for_red(Patient* red_p) {
    point blue_p = blue_kd_tree->nearest({red_p->get_p_score1(), red_p->get_p_score2(), red_p});
    point green_p = green_kd_tree->nearest({red_p->get_p_score1(), red_p->get_p_score2(), red_p});
    Match *result = new Match(); 
    result->p1 = red_p; 
    result->p2 = blue_p.get_patient(); 
    result->p3 = green_p.get_patient();
    double blue_dist = blue_kd_tree->distance();
    double green_dist = green_kd_tree->distance();
    double other_dist = blue_p.distance(green_p);
    result->dist = blue_dist + green_dist + other_dist;

    return result;
} // Given a red patient, return best available match


void Matcher::build_best_match_list() {
    /*  go through red_vec, adding the best match tuple for each into a vector with an associated
        distance value for each (new data type?)
    */
   int n = red_vec->size();
   match_list = new std::set<Match *>();
   // Go through match list
   for (int i = 0; i < n; i++) {
     Match *result = get_best_match_for_red(red_vec->at(i));
     std::cout << result->dist << ' ' << result->p1->get_id() << ' ' << result->p2->get_id() << ' ' << result->p3->get_id() << std::endl;
     match_list->insert(result);
     std::cout << "Finished match list" << std::endl;
   }
   // Sort from largest distance to smallest
   // std::sort(match_list->begin(), match_list->end(), match_cmp());
   return;
} // Construct best match list


std::vector<Match *> *Matcher::match_from_list() {
    // go through the list, matching if they're available, and finding a new match otherwise 
    // Should new match be matched or added to a place in the list based on distance?

    // final match list to return
    std::vector<Match *> *final_match_list = new std::vector<Match *>();

    // loop through queue until it's empty
    while (!match_list->empty()) {
        // Get last (smallest) element and pop it from vector
        Match *smallest_match = *(match_list->begin());
        match_list->erase(match_list->begin());
        // Check if all of its patients are unmatched
        if ((!smallest_match->p1->get_is_matched()) && 
            (!smallest_match->p2->get_is_matched()) && 
            (!smallest_match->p3->get_is_matched())) {
            // If yes set patients as matched
            smallest_match->p1->set_is_matched(true);
            smallest_match->p2->set_is_matched(true);
            smallest_match->p3->set_is_matched(true);

            // TODO: Find blue and green points in blue and green vecs, and remove them (maybe sort to speed this up?)
            for (auto f : *blue_set) {
                if (f.get_patient()->get_id() == smallest_match->p2->get_id()) {
                    blue_set->erase(f);
                    break;
                }
            }
            for (auto f : *green_set) {
                if (f.get_patient()->get_id() == smallest_match->p3->get_id()) {
                    green_set->erase(f);
                    break;
                }
            }

            // Add to final match list
            final_match_list->push_back(smallest_match);
        } else {
            // If not, get best match from kd_tree 

            // TODO: NEED TO FIX KDTREE SO THAT IT DOESNT USE ALREADY MATCHED POINTS
            // Possible solution: update the green and blue vecs with each match, then
            // when a new best match has an already matched point, rebuild the kdtrees
            // using only the remaining points

            // TODO: Replace blue and green vec with sets

            // Rebuild KD Trees with only unmatched nodes
            delete blue_kd_tree;
            delete green_kd_tree;
            blue_kd_tree = new kdtree(blue_set);
            green_kd_tree = new kdtree(green_set);

            // Get new match
            Match *result = get_best_match_for_red(smallest_match->p1);
            
            // Add match to the appropriate spot in the vector for that match distance
            match_list->insert(result);
        }
        
    }

    return final_match_list;
} // Find best match for each red patient

void Matcher::print_unmatched() const {
    std::cout << "All reds:" << std::endl;
    for (Patient * p : *red_vec)
    {
        std::cout << p->get_id() << std::endl;
    }

    std::cout << "Unmatched Blues:" << std::endl;
    for (point p : *blue_set)
    {
        std::cout << p.get_patient()->get_id() << std::endl;
    }

    std::cout << "Unmatched Greens:" << std::endl;
    for (point p : *green_set)
    {
        std::cout << p.get_patient()->get_id() << std::endl;
    }
}

void Matcher::print_match_list() const {
    std::cout << "Matches in Match List:" << std::endl;
    for (Match *m : *match_list)
    {
        std::cout << m->p1->get_id() << ' ' << m->p2->get_id() << ' ' << m->p3->get_id() << std::endl;
    }
}