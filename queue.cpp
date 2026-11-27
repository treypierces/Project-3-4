#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

#include "patient.hpp"
#include "doctor.hpp"
#include "BTree.h"
#include "BTree.cpp"

patient parse_patient(const string& line) {
    istringstream row(line);

    string birthdate;
    string firstname;
    string lastname;
    string condition_text;
    string id;
    string extra_field;

    if (!getline(row, birthdate, ',') ||
        !getline(row, firstname, ',') ||
        !getline(row, lastname, ',') ||
        !getline(row, condition_text, ',') ||
        !getline(row, id, ',') ||
        getline(row, extra_field))
    {
        throw runtime_error("Invalid patient data row");
    }

    size_t parsed_characters = 0;
    int condition = stoi(condition_text, &parsed_characters);

    if (condition_text.find_first_not_of(" \t\r", parsed_characters) !=
        string::npos)
    {
        throw runtime_error("Invalid condition in patient data row");
    }

    return patient(
        firstname,
        lastname,
        birthdate,
        condition,
        stoi(id)
    );
}

void load_patients(queue<patient>& incoming_patients) {
    ifstream input("data/patient_data.csv");

    string line;
    getline(input, line);  // skip header

    while (getline(input, line)) {
        incoming_patients.push(parse_patient(line));
    }
}

int new_patients(
    priority_queue<patient, vector<patient>, PatientPriority>& waiting_room,
    queue<patient>& incoming_patients)
{
    static mt19937 rng(random_device{}());
    if (incoming_patients.empty()) {
        return 0;
    }
    size_t max_count = min<size_t>(5, incoming_patients.size());
    size_t count =
        uniform_int_distribution<size_t>(1, max_count)(rng);

    for (size_t i = 0; i < count; ++i) {
        waiting_room.push(incoming_patients.front());
        incoming_patients.pop();
    }

    return static_cast<int>(count);
}


doctor parse_doctor(const string& line) {
    istringstream row(line);

    string birthdate;
    string firstname;
    string lastname;
    string skill_text;
    string shift_start_text;
    string shift_end_text;
    string id_text;
    string extra_field;

    if (!getline(row, birthdate, ',') ||
        !getline(row, firstname, ',') ||
        !getline(row, lastname, ',') ||
        !getline(row, skill_text, ',') ||
        !getline(row, shift_start_text, ',') ||
        !getline(row, shift_end_text, ',') ||
        !getline(row, id_text, ',') ||
        getline(row, extra_field))
    {
        throw runtime_error("Invalid doctor data row");
    }

    size_t parsed_characters = 0;
    int skill = stoi(skill_text, &parsed_characters);
    int shift_start = stoi(shift_start_text, &parsed_characters);
    int shift_end = stoi(shift_end_text, &parsed_characters);
    int id = stoi(id_text, &parsed_characters);

    if (skill_text.find_first_not_of(" \t\r", parsed_characters) !=
        string::npos)
    {
        throw runtime_error("Invalid skill in doctor data row");
    }

    return doctor(
        firstname,
        lastname,
        birthdate,
        skill,
        shift_start,
        shift_end,
        id
    );
}

void load_doctors(BTree<doctor>& doctor_tree, priority_queue<doctor, vector<doctor>, DoctorPriority>& available_doctors, const int current_time, vector<doctor>& off_duty_doctors) {
    ifstream input("data/doctor_data.csv");

    string line;
    getline(input, line);  // skip header

    while (getline(input, line)) {
        const doctor new_doctor = doctor(parse_doctor(line));
        doctor_tree.insert(new_doctor.id, new_doctor);
        if (new_doctor.is_available(current_time)) {
            available_doctors.push(new_doctor);
        }
        else  {
            off_duty_doctors.push_back(new_doctor);
        }
    }
}

void update_doctor_availability(priority_queue<doctor, vector<doctor>, DoctorPriority>& available_doctors, vector<doctor>& off_duty_doctors, const int current_time) {

    // Move doctors from off-duty to available if they are now available
    auto it = off_duty_doctors.begin();
    while (it != off_duty_doctors.end()) {
        if (it->is_available(current_time)) {
            available_doctors.push(*it);
            it = off_duty_doctors.erase(it);
        } else {
            ++it;
        }
    }
}

