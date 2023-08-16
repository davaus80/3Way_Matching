#ifndef _PATIENT_H_
#define _PATIENT_H_

//  #############################################################################
//  Patient is used to store information about a patient
//  #############################################################################

class Patient {
    protected:
      int id;
      int group;
      float p_score1;
      float p_score2;
      bool is_matched;
      Patient* match1;
      Patient* match2;

    public:
      Patient(int the_id, int the_group, float the_ps1, float the_ps2); // In the constructor, we divide the inputs into red, blue, and green groups and store in class fields
      ~Patient(); // Maybe we need custom destructor?
      void set_is_matched(bool new_val);
      bool get_is_matched() const;
      void set_match(Patient* p1, Patient* p2); // Set the match for this Patient to the arguments
      Patient *get_match1() const;
      Patient *get_match2() const;
      float get_p_score1() const;
      float get_p_score2() const;
      int get_id() const;
      int get_group() const;
      void print_match() const;
};

#endif