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

patient parse_patient(const string& line) {
    istringstream row(line);

    string birthdate;
    string firstname;
    string lastname;
    string condition_text;
    string extra_field;

    if (!getline(row, birthdate, ',') ||
        !getline(row, firstname, ',') ||
        !getline(row, lastname, ',') ||
        !getline(row, condition_text, ',') ||
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
        condition);
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

