#include "doctor.hpp"
#include <utility>
#include <string>

using namespace std;

doctor::doctor(string firstname,
               string lastname,
               string birthdate,
               int skill,
               int shift_start,
               int shift_end,
               int id)
{
    this->firstname = firstname;
    this->lastname = lastname;
    this->birthdate = birthdate;
    this->skill = skill;
    this->shift_start = shift_start;
    this->shift_end = shift_end;
    this->id = id;
}

std::string doctor::print() {
    return "Doctor Name: " + firstname + " " + lastname + "\nBirthdate: " + birthdate + "\nSkill: " + std::to_string(skill) + "\nShift: " + std::to_string(shift_start) + " - " + std::to_string(shift_end);
}

bool DoctorPriority::operator()(
    const doctor& left,
    const doctor& right) const
{
    return left.skill < right.skill;
}

