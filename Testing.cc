#include <iostream>
#include "Patient.h"
#include "Matcher.h"

void test_patients() {
    Patient *p1 = new Patient(1, 1, 0, 0);
    Patient *p2 = new Patient(2, 1, 0, 0);
    Patient *p3 = new Patient(3, 2, 0, 0);
    Patient *p4 = new Patient(4, 2, 0, 0);
    Patient *p5 = new Patient(5, 3, 0, 0);
    Patient *p6 = new Patient(6, 3, 0, 0);

    std::cout << "Match Statuses:" << p1->get_is_matched() << " "
                                   << p2->get_is_matched() << " "
                                   << p3->get_is_matched() << " "
                                   << p4->get_is_matched() << " "
                                   << p5->get_is_matched() << " "
                                   << p6->get_is_matched() << std::endl;
    
    p1->set_match(p2, p3);
    p2->set_match(p1, p3);
    p3->set_match(p1, p2);
    p4->set_match(p5, p6);
    p5->set_match(p4, p6);
    p6->set_match(p4, p5);

    std::cout << "Match Statuses:" << p1->get_is_matched() << " "
                                   << p2->get_is_matched() << " "
                                   << p3->get_is_matched() << " "
                                   << p4->get_is_matched() << " "
                                   << p5->get_is_matched() << " "
                                   << p6->get_is_matched() << std::endl;

    p1->print_match();
    p2->print_match();
    p3->print_match();
    p4->print_match();
    p5->print_match();
    p6->print_match();
}

void test_reading() {
    Matcher *m = new Matcher();
    std::string filename = "./data/sample.txt";
    m->read_inputs(filename, 0.6);
    m->print_unmatched();
}

void test_matching() {
    Matcher *m = new Matcher();
    std::string filename = "./tests/big_test.txt";
    m->read_inputs(filename, 0.6);
    m->print_unmatched();
    std::cout << "Building Initial Matches..." << std::endl;
    m->build_best_match_list(true);
    std::cout << "Setting Matches..." << std::endl;
    std::vector<Match> *final_match = m->match_from_list();

    std::cout << "Final Matches:" << std::endl;
    for (Match m : *final_match)
    {
        std::cout << m.p1->get_id() << ' ' << m.p2->get_id() << ' ' << m.p3->get_id() << std::endl;
    }
}

// int main() {
//     //test_patients();
//     //test_reading();
//     test_matching();
//     return 0;
// }

