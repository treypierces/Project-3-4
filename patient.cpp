#include "patient.hpp"
#include <string>

using namespace std;

patient::patient(string firstname,
                 string lastname,
                 string birthdate,
                 int condition)
{
    this->firstname = firstname;
    this->lastname = lastname;
    this->birthdate = birthdate;
    this->condition = condition;
}

bool PatientPriority::operator()(
    const patient& left,
    const patient& right) const
{
    return left.condition < right.condition;
}

