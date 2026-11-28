#include "doctor.hpp"
#include "patient.hpp"
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
    this->patID = -1; // Initialize patID to -1 to indicate no patient assigned
}

bool doctor::is_available(int time) const
{
    time = time%24; // Ensure time is within 0-23 range
    if (time < 0 || time >= 24) {
        return false;
    }

    if (shift_start <= shift_end) {
        return time >= shift_start && time < shift_end;
    }

    return time >= shift_start || time < shift_end;
}

std::string doctor::print() {
    return "Doctor Name: " + firstname + " " + lastname + 
    "\nBirthdate: " + birthdate + 
    "\nSkill: " + std::to_string(skill) + 
    "\nShift: " + std::to_string(shift_start) + " - " + std::to_string(shift_end) +
    "\nPatient ID: " + (patID != -1 ? std::to_string(patID) : "None");
}



bool DoctorPriority::operator()(
    const doctor& left,
    const doctor& right) const
{
    return left.skill < right.skill;
}

