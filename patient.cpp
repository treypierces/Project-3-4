#include "patient.hpp"
#include <utility>
#include <string>

using namespace std;

patient::patient(string firstname,
                 string lastname,
                 string birthdate,
                 int condition,
                 int id)
{
    this->firstname = firstname;
    this->lastname = lastname;
    this->birthdate = birthdate;
    this->condition = condition;
    this->id = id;
}

std::string patient::print() {
    return "Patient Name: " + firstname + " " + lastname + "\nBirthdate: " + birthdate + "\nCondition: " + std::to_string(condition);
}

bool PatientPriority::operator()(
    const patient& left,
    const patient& right) const
{
    return left.condition < right.condition;
}

