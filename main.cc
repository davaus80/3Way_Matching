#include <iostream>
#include <exception>
#include "Patient.h"
#include "Matcher.h"

int run_matching(std::string filename) {
    Matcher *m = new Matcher();

    // Read input from file
    try {
        m->read_inputs(filename);
    } catch (...) {
        std::cerr << "An error occurred while reading from " << filename << std::endl;
        std::cerr << "Ensure that the file is a csv file formatted as ID / GROUP / PSCORE1 / PSCORE2 with types INT / INT [1-3] / DOUBLE / DOUBLE" << std::endl;
        return 1;
    }

    // Construct matches
    std::cout << "Building Initial Matches..." << std::endl;
    m->build_best_match_list();
    std::cout << "Setting Matches..." << std::endl;
    std::vector<Match> *final_match = m->match_from_list();

    // Write matches to file
    std::cout << "Writing final matches to output..." << std::endl;
    for (Match m : *final_match)
    {
        std::cout << m.p1->get_id() << ' ' << m.p2->get_id() << ' ' << m.p3->get_id() << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Use format: ./prop_match [FILENAME]" << std::endl;
    }
    return run_matching(argv[1]);
}
