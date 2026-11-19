#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <string>
#include <iostream>

class doctor {
    public: 
        doctor* left;
        doctor* right;

        int id;
        std::string name;

    
        doctor(std::string name, int id);
        ~doctor();

        std::ostream& print(std::ostream& os) const;

};

#endif