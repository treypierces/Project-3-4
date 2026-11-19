#include <iostream>
#include <string>
#include "doctor.hpp"

using namespace std;


doctor::doctor(std::string name, int id){
    this->name = name;
    this->id = id;
}

void doctor::print(){
    cout << "Name: " << name << ", ID: " << id << endl;
}
