#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <format>
#include <utility>
#include "doctor.hpp"
#include "B_Tree.hpp"

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
    bool lookup_success = false;

    B_Tree<doctor> doctor_tree;
    doctor* doc1 = new doctor("Dr. Smith", 123);
    doctor* doc2 = new doctor("Dr. Johnson", 456);
    doctor_tree.insert(doc1);
    doctor_tree.insert(doc2);

    queue<patient> incoming_patients;

    priority_queue<
        patient,
        vector<patient>,
        PatientPriority> waiting_room;
    
    load_patients(incoming_patients);

    
    //720p
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "DCH Hospital");
    window.setPosition({1200, 50});

    //load font from a file
    Font font;
    if (!font.openFromFile("DejaVuSans.ttf")) {
        cerr << "Error opening DejaVu Sans font" << endl;
        return 1;
    }

    // ========================= GRAPHICS =========================
    //create play button triangle
    CircleShape play_button_triangle(10, 3);          //radius size, sides
    play_button_triangle.rotate(degrees(90));  
    play_button_triangle.setPosition({WINDOW_WIDTH / 2.f + 10.f, WINDOW_HEIGHT * .95f});       //absolute position
    play_button_triangle.setFillColor(Color(0, 255, 0));

    //create play button
    RectangleShape play_button({30.f, 30.f});
    play_button.setPosition({WINDOW_WIDTH / 2.f - 15.f, WINDOW_HEIGHT *.95f - 5.f});
    play_button.setFillColor(Color::White);
    play_button.setOutlineColor(Color::Black);
    play_button.setOutlineThickness(2.f);

    //create user text input box
    RectangleShape text_box({200.f, 50.f});
    text_box.setPosition({20, 20});
    text_box.setFillColor(Color::White);
    text_box.setOutlineColor(Color::Black);
    text_box.setOutlineThickness(2.f);  

    // user input text
    sf::Text sf_userin(font, userinput);
    sf_userin.setPosition({20.f, 20.f});
    sf_userin.setFillColor(Color::Black);

    // lookup result text
    sf::Text sf_lookup_result(font, "");
    sf_lookup_result.setPosition({20.f, 80.f});
    sf_lookup_result.setFillColor(Color::Black);
    
    sf::Text sf_lookup_info(font, "");
    sf_lookup_info.setPosition({20.f, 110.f});
    sf_lookup_info.setFillColor(Color::Black);

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
            
                if (play_button.getGlobalBounds().contains(worldPos)) { // play button clicked
                    // FIXME
                    // TIMESTEP
                    new_patients(waiting_room, incoming_patients);
                    userinput.append("B");
                    sf_userin.setString(userinput);
                }
            }}

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                //get shifts for our "player" when we press a key

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

                if(userinput.length() > 10){
                    userinput = userinput.substr(0, 10);
                }

                sf_userin.setString(userinput); 
            }
        }


        // Perform lookup
        if (doctor_lookup) {
            int lookup_id = (userinput.empty() ? 0 : std::stoi(userinput));
            doctor* found_doctor = doctor_tree.find(lookup_id);
            if (found_doctor) {
                lookup_success = true;
                sf_lookup_result.setString(format("Doctor Found: {}", found_doctor->name));
                sf_lookup_info.setString(format("ID: {}", found_doctor->id));
            } else {
                lookup_success = false;
                sf_lookup_result.setString("Doctor Not Found");
                sf_lookup_info.setString("");
            }
        } else {
            sf_lookup_result.setString("");
            sf_lookup_info.setString("");
        }

        //reset the window
        window.clear(Color::White);

        //draw buttons
        window.draw(play_button);
        window.draw(play_button_triangle);

        // draw search box
        window.draw(text_box);
        window.draw(sf_userin);

        // draw search results
        window.draw(sf_lookup_result);
        window.draw(sf_lookup_info);

        //display all drawn objects
        window.display();
    }
}
