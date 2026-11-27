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
            if (event->is<Event::Closed>())
                window.close();
            
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
                }
            }}

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch(keyPressed->code){
                    case sf::Keyboard::Key::Num1:
                        userinput.append("1");
                        break;
                    case sf::Keyboard::Key::Num2:
                        userinput.append("2");
                        break;
                    case sf::Keyboard::Key::Num3:
                        userinput.append("3");
                        break;
                    case sf::Keyboard::Key::Num4:
                        userinput.append("4");
                        break;
                    case sf::Keyboard::Key::Num5:
                        userinput.append("5");
                        break;
                    case sf::Keyboard::Key::Num6:
                        userinput.append("6");
                        break;
                    case sf::Keyboard::Key::Num7:
                        userinput.append("7");
                        break;
                    case sf::Keyboard::Key::Num8:
                        userinput.append("8");
                        break;
                    case sf::Keyboard::Key::Num9:
                        userinput.append("9");
                        break;
                    case sf::Keyboard::Key::Num0:
                        userinput.append("0");
                        break;
                    case sf::Keyboard::Key::Backspace:
                        if (!userinput.empty()) {
                            userinput.pop_back();
                        }
                        break;
                    case sf::Keyboard::Key::D:
                        doctor_lookup = true;
                        break;
                    case sf::Keyboard::Key::P:
                        doctor_lookup = false;
                        break;
                    default:
                        break;                        
                }

                if(userinput.length() > 3){
                    userinput = userinput.substr(0, 3);
                }

                ui.setUserInput(userinput);
            }
        }


        // Perform lookup
        if (doctor_lookup) {
            std::optional<doctor> found_doctor;
            if (!userinput.empty()) {
                found_doctor = off_duty_doctors[1];
            }
            if (found_doctor) {
                //sf_lookup_result.setString("Doctor Found");
                //ui.setLookupResult(d ? "Doctor Found" : "Doctor Not Found");
                //ui.setLookupInfo(d ? d->print() : "");
            } else {
                //sf_lookup_result.setString("Doctor Not Found");
                //ui.setLookupResult(p ? "Patient Found" : "Patient Not Found");
                //ui.setLookupInfo(p ? p->print() : "");
            }
        } else { // patient lookup
            std::optional<patient> found_patient;
            if (!userinput.empty()) {
                found_patient = patient_tree.search(std::stoi(userinput));
            }
            if (found_patient) {
                //sf_lookup_result.setString("Patient Found");
                //ui.setLookupResult(p ? "Patient Found" : "Patient Not Found");
                //ui.setLookupInfo(p ? p->print() : "");
            } else {
                //sf_lookup_result.setString("Patient Not Found");
                //sf_lookup_info.setString("");
            }
        }

        // //reset the window
        // window.clear(Color::White);

        // //draw buttons
        // window.draw(play_button);
        // window.draw(play_button_triangle);

        // // draw search results
        // window.draw(look_up_box);
        // window.draw(sf_lookup_result);
        // window.draw(sf_lookup_info);

        // // draw search box
        // window.draw(text_box);
        // window.draw(sf_userin);

        // // draw time text
        // window.draw(sf_time);

        // // draw information text
        // window.draw(sf_info);

        // //display all drawn objects
        // window.display();
        ui.draw(window);
    }
}
