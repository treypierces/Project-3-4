#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <format>
#include <utility>

#include "doctor.hpp"
#include "BTree.h"
#include "BTree.cpp"
#include "graphics.hpp"
#include <queue>
#include "patient.hpp"
#include "queue.hpp"

//std namespaces
using namespace std;
using std::cerr;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::format;
using std::optional;

//sf namespaces
using sf::RenderWindow;
using sf::CircleShape;
using sf::VideoMode;
using sf::RectangleShape;
using sf::Color;
using sf::degrees;
using sf::Text;
using sf::Vector2i;
using sf::Vector2f;
using sf::Font;
using sf::Event;

//array size
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main()
{
    // initialize variables
    string userinput = "";
    bool doctor_lookup = true;
    bool user_in_changed = true; // see if user input has changed to see if lookup needs to be performed
    int time = 7; // hr of the day

    BTree<doctor> doctor_tree{10};
    BTree<patient> patient_tree{10};

    queue<patient> incoming_patients;
    vector<doctor> off_duty_doctors;
    vector<patient> worsening_patients;
    
    priority_queue< patient,vector<patient>,PatientPriority> waiting_room;
    priority_queue< doctor,vector<doctor>,DoctorPriority> available_doctors;
    
    
    load_patients(incoming_patients);
    load_doctors(doctor_tree, available_doctors, time, off_duty_doctors);

    initialize_doctor_availability(available_doctors, off_duty_doctors, time);
    
    //720p
    RenderWindow window(VideoMode({HospitalUI::WINDOW_WIDTH, HospitalUI::WINDOW_HEIGHT}), "DCH Hospital");
    window.setPosition({1200, 50});

    HospitalUI ui("DejaVuSans.ttf", time);

    
    // ========================= EVENT LOOP =========================
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()){
                window.close();
                break;
            }
            
            //next iteration of insertion sort -> click the play button
            if (const auto* mouseClick = event->getIf<Event::MouseButtonPressed>()){ // mouse clicked
            if (mouseClick->button == sf::Mouse::Button::Left){
                Vector2i pixelPos(mouseClick->position.x, mouseClick->position.y);
                Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            
                if (ui.playButtonClicked(worldPos)) { // play button clicked
                    TimestepEvents events;

                    // TIMESTEP
                    time = (time + 1) % 24;
                    ui.setTime(time);

                    new_patients(waiting_room, incoming_patients, events.new_patient_ids);
                    update_doctor_availability(available_doctors, off_duty_doctors, doctor_tree, waiting_room, patient_tree, time, events);

                    patient newPat = patient("","","",0,0); // assign doctors to patients
                    doctor newDoc = doctor("","","",0,0,0,0);
                    while (waiting_room.size() > 0 && available_doctors.size() > 0) { 
                        newPat = waiting_room.top();
                        newDoc = available_doctors.top();
                        newPat.docID = newDoc.id;
                        newDoc.patID = newPat.id;
                        events.doctor_patient_assignments.emplace_back(newDoc.id, newPat.id);
                        patient_tree.insert(newPat.id, newPat);
                        waiting_room.pop();
                        doctor_tree.insert(newDoc.id, newDoc);
                        available_doctors.pop();
                    }

                    while (waiting_room.size() > 0) { // patients remaining in waiting room have conditions worsen
                        newPat = waiting_room.top();
                        waiting_room.pop();
                        events.waiting_patient_ids.push_back(newPat.id);

                        newPat.condition += 1;
                        if(newPat.condition == 11){
                            events.critical_patient_ids.push_back(newPat.id);
                            //cout << "Patient " << newPat.id << " has worsened to critical condition." << endl;
                        }
                        if(newPat.condition == 12){
                            events.deceased_patient_ids.push_back(newPat.id);
                            //cout << "Patient " << newPat.id << " has died due to worsening condition." << endl;
                        } else {
                            worsening_patients.push_back(newPat);
                        }
                    }
                    while (worsening_patients.size() > 0) { // add worsening patients back to waiting room
                        newPat = worsening_patients.back();
                        worsening_patients.pop_back();
                        waiting_room.push(newPat);
                    }

                    // update patients in treatment
                    update_patients_in_treatment(patient_tree, doctor_tree, available_doctors, events);

                    const auto formatIds = [](const vector<int>& ids) {
                        if (ids.empty()) {
                            return string("None");
                        }

                        string result;
                        for (const int id : ids) {
                            if (!result.empty()) {
                                result += ", ";
                            }
                            result += to_string(id);
                        }
                        return result;
                    };
                    const auto formatAssignments = [](const vector<pair<int, int>>& assignments) {
                        if (assignments.empty()) {
                            return string("None");
                        }

                        string result;
                        for (const auto& [doctor_id, patient_id] : assignments) {
                            if (!result.empty()) {
                                result += ", ";
                            }
                            result += "D" + to_string(doctor_id) + "-P" + to_string(patient_id);
                        }
                        return result;
                    };

                    ui.setTimestepEventText(0, formatIds(events.new_patient_ids));
                    ui.setTimestepEventText(1, formatIds(events.arriving_doctor_ids));
                    ui.setTimestepEventText(2, formatIds(events.departing_doctor_ids));
                    ui.setTimestepEventText(3, formatIds(events.waiting_patient_ids));
                    ui.setTimestepEventText(4, formatAssignments(events.doctor_patient_assignments));
                    ui.setTimestepEventText(5, formatIds(events.discharged_patient_ids));
                    ui.setTimestepEventText(6, formatIds(events.critical_patient_ids));
                    ui.setTimestepEventText(7, formatIds(events.deceased_patient_ids));

                }
            }}

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                ui.buttonTyped(keyPressed->code, userinput, doctor_lookup);

                ui.setUserInput(userinput);
                user_in_changed = true;
            }
        }


        // Perform lookup
        if(user_in_changed){
            if (doctor_lookup) {
                std::optional<doctor> found_doctor;
                if (!userinput.empty()) {
                    found_doctor = doctor_tree.search(std::stoi(userinput));
                }
                if (found_doctor) {
                    ui.setLookupResult(found_doctor ? "Doctor Found" : "Doctor Not Found");
                    ui.setLookupInfo(found_doctor ? found_doctor->print() : "");
                } else {
                    ui.setLookupResult("Doctor Not Found");
                    ui.setLookupInfo("");
                }
            } else { // patient lookup
                std::optional<patient> found_patient;
                if (!userinput.empty()) {
                    found_patient = patient_tree.search(std::stoi(userinput));
                }
                if (found_patient) {
                    ui.setLookupResult(found_patient ? "Patient Found" : "Patient Not Found");
                    ui.setLookupInfo(found_patient ? found_patient->print() : "");
                } else {
                    ui.setLookupResult("Patient Not Found");
                    ui.setLookupInfo("");
                }
            }

            user_in_changed = false;
        }

        // draw the UI
        ui.draw(window);
    }
}
