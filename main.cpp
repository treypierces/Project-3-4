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
    
    priority_queue< patient,vector<patient>,PatientPriority> waiting_room;
    priority_queue< doctor,vector<doctor>,DoctorPriority> available_doctors;
    vector<doctor> off_duty_doctors;
    
    load_patients(incoming_patients);
    load_doctors(doctor_tree, available_doctors, time, off_duty_doctors);
    
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
                    // FIXME
                    // TIMESTEP
                    time++;
                    ui.setTime(time);
                    new_patients(waiting_room, incoming_patients);

                    update_doctor_availability(available_doctors, off_duty_doctors, time);

                    patient a = patient("","","",0,0);
                    while (waiting_room.size() > 0) {
                        a = waiting_room.top();
                        cout << a.print() << "\nID: " << a.id << endl;
                        patient_tree.insert(a.id, a);
                        waiting_room.pop();

                    }
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
