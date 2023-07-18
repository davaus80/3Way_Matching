#include <iostream>
#include "Patient.h"

Patient::Patient(int the_id, int the_group, float the_ps1, float the_ps2) : id{the_id}, 
            group{the_group}, p_score1{the_ps1}, p_score2{the_ps2}, is_matched{false},
            match1{nullptr}, match2{nullptr} {}


Patient::~Patient() {
    this->match1 = nullptr;
    this->match2 = nullptr;
}


void Patient::set_is_matched(bool new_val) {
    this->is_matched = new_val;
}


bool Patient::get_is_matched(){
    return this->is_matched;
}


void Patient::set_match(Patient* p1, Patient* p2) {
    if ((p1 == nullptr) || (p2 == nullptr)) {
        return;
    }
    this->is_matched = true;
    this->match1 = p1;
    this->match2 = p2;
    p1 = nullptr;
    p2 = nullptr;
}


Patient* Patient::get_match1() {
    return this->match1;
}

Patient* Patient::get_match2() {
    return this->match2;
}


void Patient::print_match() {
    if ((this->match1 == nullptr) or (this->match2 == nullptr)) {
        std::cout << "No match for " << this->id << std::endl;
    } else {
        std::cout << this->id << " " << this->match1->id << " " << this->match2->id << std::endl;
    }
}