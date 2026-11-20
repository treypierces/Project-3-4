#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <string>

class patient {
public:
    std::string firstname;
    std::string lastname;
    std::string birthdate;
    int condition;

    patient(std::string firstname,
            std::string lastname,
            std::string birthdate,
            int condition);
};

struct PatientPriority {
    bool operator()(const patient& left,
                    const patient& right) const;
};

#endif