#ifndef PATIENT_QUEUE_HPP
#define PATIENT_QUEUE_HPP

#include <queue>
#include <vector>
#include <string>
#include "patient.hpp"
#include "doctor.hpp"
#include "BTree.h"

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
    std::queue<patient>& incoming_patients);

doctor parse_doctor(const std::string& line);

void load_doctors(BTree<doctor>& doctor_tree);

#endif