#ifndef _PATIENT_H_
#define _PATIENT_H_

//  #############################################################################
//  Patient is used to store information about a patient
//  #############################################################################

class Patient {
    protected:
      int id; // Unique ID
      int group; // red = 1, blue = 2, green = 3
      float p_score1; // propensity score
      float p_score2;
      bool is_matched; // indicates if this patient has been matched yet
      Patient* match1; // First associated patient in match
      Patient* match2; // Second associated patient in match -> TODO: Maybe could replace w pointer to match?

    public:
      Patient(int the_id, int the_group, float the_ps1, float the_ps2); // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields
      ~Patient();
      void set_is_matched(bool new_val); // mutator
      bool get_is_matched() const; // accessor
      void set_match(Patient* p1, Patient* p2); // Set the match for this Patient to the arguments
      Patient *get_match1() const; // accessor
      Patient *get_match2() const; // accessor
      float get_p_score1() const; // accessor
      float get_p_score2() const; // accessor
      int get_id() const; // accessor
      int get_group() const; // accessor
      void print_match() const; // prints the match
};

#endif