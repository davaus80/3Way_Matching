#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include "Patient.h"
#include "Matcher.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Use format: ./3Way_Matching [INPUT PATH] [OUTPUT PATH] [-v for verbose mode]" << std::endl;
        return 1;
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

    // Default threshold is 0.25
    float threshold = 0.25;
    // Check for ./threshold.in
    std::ifstream thres_stream;
    std::string thres_file = "./threshold.in";
    thres_stream.open(thres_file);
    if (!thres_stream) {
        thres_stream.close();
        std::cout << "Could not open ./threshold.in. Using default value for threshold." << std::endl;
    } else {
        std::string line;
        std::getline(thres_stream, line);
        std::istringstream in(line);
        std::string thres_str;
        in >> thres_str;
        try {
            threshold = std::stof(thres_str);
        } catch (...) {
            std::cout << "Value in ./threshold.in is not a valid float. Using default value for threshold." << std::endl;
        }
        std::cout << "Using " << threshold << " as threshold." << std::endl;
    }



    // Read input from file
    if (verbose) {
        std::cout << "Reading inputs..." << std::endl;
    }
    try {
        m->read_inputs(filename, threshold);
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
