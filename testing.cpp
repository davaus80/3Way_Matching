#include <iostream>
#include "Patient.h"

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

int main() {
    test_patients();
    return 0;
}