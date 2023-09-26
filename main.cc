#include <iostream>
#include <fstream>
#include <exception>
#include "Patient.h"
#include "Matcher.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Use format: ./prop_match [INPUT PATH] [OUTPUT PATH] [-v for verbose mode]" << std::endl;
    }

    Matcher *m = new Matcher();

    // Check if using verbose mode
    std::string filename = argv[1];
    std::string output_file = argv[2];
    std::string verb_option = "-v";
    bool verbose = false;
    if ((argc > 3)){
        std::string option = argv[3];
        if (option.compare(verb_option) == 0) {
            std::cout << "Using verbose mode..." << std::endl;
            verbose = true;
        }
    }

    // Read input from file
    if (verbose) {
        std::cout << "Reading inputs..." << std::endl;
    }
    try {
        m->read_inputs(filename);
    } catch (...) {
        std::cerr << "An error occurred while reading from " << filename << std::endl;
        std::cerr << "Ensure that the file is a csv file formatted as ID / GROUP / PSCORE1 / PSCORE2 with types INT / INT [1-3] / DOUBLE / DOUBLE" << std::endl;
        return 1;
    }

    // Construct matches
    if (verbose) {
        std::cout << "Building Initial Matches..." << std::endl;
    }
    m->build_best_match_list(verbose);
    if (verbose) {
        std::cout << "Setting Matches..." << std::endl;
    }
    std::vector<Match> *final_match = m->match_from_list();

    // Write matches to file
    if (verbose) {
        std::cout << "Writing final matches to output..." << std::endl;
    }

    // Try to open output file. If failure, write to prop_score_output.out
    std::ofstream out_stream;
    out_stream.open(output_file);
    if (!out_stream) {
        out_stream.close();
        std::cout << "Could not open " << output_file << " Printing to ./prop_score_output.out" << std::endl;
        out_stream.open("./prop_score_output.out");
    }

    for (Match m : *final_match)
    {
        out_stream << m.p1->get_id() << ',' << m.p2->get_id() << ',' << m.p3->get_id() << std::endl;
    }

    return 0;
}
