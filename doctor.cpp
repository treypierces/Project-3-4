#include <iostream>
#include <string>
#include "doctor.hpp"

using namespace std;


doctor::doctor(std::string name, int id){
    this->name = name;
    this->id = id;
}

doctor::~doctor() {
    free(left);
    free(right);
}

ostream& doctor::print(ostream& os) const {
    os << "Doctor Name: " << name << ", ID: " << id;
    return os;
}

