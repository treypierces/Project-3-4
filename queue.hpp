#ifndef PATIENT_QUEUE_HPP
#define PATIENT_QUEUE_HPP

#include <queue>
#include <vector>
#include <string>
#include <utility>
#include "patient.hpp"
#include "doctor.hpp"
#include "BTree.h"

struct TimestepEvents {
    std::vector<int> new_patient_ids;
    std::vector<int> arriving_doctor_ids;
    std::vector<int> departing_doctor_ids;
    std::vector<int> waiting_patient_ids;
    std::vector<std::pair<int, int>> doctor_patient_assignments;
    std::vector<int> discharged_patient_ids;
    std::vector<int> critical_patient_ids;
    std::vector<int> deceased_patient_ids;
};

patient parse_patient(const std::string& line);


// Reads patient_data.csv and loads all patients into the
// incoming-patients queue. Intended to be called once at
// the start of the simulation.
void load_patients(std::queue<patient>& incoming_patients);


// Simulates the passage of time by moving a random number
// of patients from the incoming-patients queue into the
// waiting-room priority queue.
int new_patients(
    std::priority_queue<
        patient,
        std::vector<patient>,
        PatientPriority>& waiting_room,
    std::queue<patient>& incoming_patients,
    std::vector<int>& new_patient_ids,
    bool too_many_patients_mode);

doctor parse_doctor(const std::string& line);

void load_doctors(BTree<doctor>& doctor_tree,    std::priority_queue<doctor, std::vector<doctor>, DoctorPriority>& available_doctors,
    const int current_time, std::vector<doctor>& off_duty_doctors);

void initialize_doctor_availability(std::priority_queue<doctor, std::vector<doctor>, DoctorPriority>& available_doctors, std::vector<doctor>& off_duty_doctors, const int current_time);
void update_doctor_availability(std::priority_queue<doctor, std::vector<doctor>, DoctorPriority>& available_doctors, 
    std::vector<doctor>& off_duty_doctors, BTree<doctor>& doctor_tree, 
    std::priority_queue<patient, std::vector<patient>, PatientPriority>& waiting_room, 
    BTree<patient>& patient_tree, const int current_time, TimestepEvents& events);

void update_patients_in_treatment(
    BTree<patient>& patient_tree,
    BTree<doctor>& doctor_tree,
    std::priority_queue<doctor, std::vector<doctor>, DoctorPriority>& available_doctors,
    TimestepEvents& events);

#endif