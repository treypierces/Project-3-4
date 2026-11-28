#include "patient.hpp"
#include "doctor.hpp"
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
    this->docID = -1; // Initialize docID to -1 to indicate no doctor assigned
}

std::string patient::print() {
    return "Patient Name: " + firstname + " " + lastname + 
    "\nBirthdate: " + birthdate + 
    "\nCondition: " + std::to_string(condition) +
    "\nDoctor ID: " + (docID != -1 ? std::to_string(docID) : "None");
}

bool PatientPriority::operator()(
    const patient& left,
    const patient& right) const
{
    return left.condition < right.condition;
}

