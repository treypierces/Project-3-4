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

#include "queue.hpp"
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
    queue<patient>& incoming_patients,
    vector<int>& new_patient_ids)
{
    static mt19937 rng(random_device{}());
    if (incoming_patients.empty()) {
        return 0;
    }
    size_t max_count = min<size_t>(5, incoming_patients.size());
    size_t count =
        uniform_int_distribution<size_t>(1, max_count)(rng);

    for (size_t i = 0; i < count; ++i) {
        new_patient_ids.push_back(incoming_patients.front().id);
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

void initialize_doctor_availability(priority_queue<doctor, vector<doctor>, DoctorPriority>& available_doctors, vector<doctor>& off_duty_doctors, const int current_time) {

    // Move doctors from off-duty to available if they are now available
    auto it = off_duty_doctors.begin();
    while (it != off_duty_doctors.end()) {
        if (it->is_available(current_time)) {
            available_doctors.push(*it);
            ++it;
        } else {
            ++it;
        }
    }
}

void update_doctor_availability(priority_queue<doctor, vector<doctor>, DoctorPriority>& available_doctors, 
    vector<doctor>& off_duty_doctors, BTree<doctor>& doctor_tree, 
    priority_queue<patient, vector<patient>, PatientPriority>& waiting_room, BTree<patient>& patient_tree,
    const int current_time, TimestepEvents& events) {

    std::optional<doctor> doc;
    std::optional<patient> pat;

    doctor_tree.forEach([&](doctor& current_doctor) {
        if (current_doctor.shift_start == current_time) {
            events.arriving_doctor_ids.push_back(current_doctor.id);
        }
        if (current_doctor.shift_end == current_time) {
            events.departing_doctor_ids.push_back(current_doctor.id);
        }
    });
    
    // Move doctors from off-duty to available if they are now available
    auto it = off_duty_doctors.begin();
    while (it != off_duty_doctors.end()) {
        if (it->shift_start == current_time) { // doctor is coming on duty
            available_doctors.push(*it);
            
        } else if (it->shift_end == current_time) { // Doctor is going off duty
            doc = doctor_tree.search(it->id); 
            if (doc) {
                pat = patient_tree.search(doc->patID);
            
                //doc = doctor_tree.search(std::stoi(userinput));
                if (pat) {
                    pat->docID = -1; // Unassign the patient from the doctor
                    waiting_room.push(*pat); // Add the patient back to the waiting room
                    doc->patID = -1; // Unassign the doctor from the patient
                }
            }
            
        }
        ++it;
    }
}

void update_patients_in_treatment(
    BTree<patient>& patient_tree,
    BTree<doctor>& doctor_tree,
    priority_queue<doctor, vector<doctor>, DoctorPriority>& available_doctors,
    TimestepEvents& events)
{
    patient_tree.forEach([&](patient& treated_patient) {
        const optional<doctor> assigned_doctor = doctor_tree.search(treated_patient.docID);
        if (!assigned_doctor) {
            if (treated_patient.docID != -1) {
                cout << "Error: cannot find doctor with ID " << treated_patient.docID << " for patient " << treated_patient.id << endl;
            }
            return;
        }

        doctor updated_doctor = *assigned_doctor;
        treated_patient.condition -= updated_doctor.skill;
        if (treated_patient.condition <= 0) {
            //cout << "Patient " << treated_patient.id << " has been successfully treated and discharged." << endl;
            events.discharged_patient_ids.push_back(treated_patient.id);
            treated_patient.docID = -1;
            updated_doctor.patID = -1;
            available_doctors.push(updated_doctor);
        }
    });
}

