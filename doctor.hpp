#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <string>

class doctor {
    public: 
        doctor* left;
        doctor* right;

        int id;
        std::string name;

    
        doctor(std::string name, int id);

        void print();

};

#endif