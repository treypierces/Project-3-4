#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <string>

class patient;

class doctor {
public:
    std::string firstname;
    std::string lastname;
    std::string birthdate;
    int skill;
    int shift_start;
    int shift_end;
    int id;
    int patID;

    doctor(std::string firstname,
            std::string lastname,
            std::string birthdate,
            int skill,
            int shift_start,
            int shift_end,
            int id);

    bool is_available(int time) const;
    std::string print(); 
};

struct DoctorPriority {
    bool operator()(const doctor& left,
                    const doctor& right) const;
};

#endif
