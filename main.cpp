#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <format>
#include "doctor.hpp"
#include "B_Tree.hpp"

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

//function used to draw an array to the screen using the rectangles that contain the text
void draw_array(vector<shared_ptr<RectangleShape>>& visual_array, 
                vector<shared_ptr<Text>>& text_array, 
                double key, 
                RectangleShape& key_rect, 
                double A[], 
                Font& font);



//array size
const int N = 10;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main()
{
    //create an array to display to the screen
    //double A[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    //double A[] = {3, 7, 8, 2, 10, 5, 1, 6, 4, 9}; //test array

    //insertion sort init -> see else branch in sort
    // int index = 0;
    // int j = 1;
    // int temp;
    // int key = 0;
    string userinput = "A";

    //720p
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "DCH Hospital");
    window.setPosition({1200, 50});

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

    
    
    //store rectangle objects for each visual element of the array
    // vector<shared_ptr<RectangleShape>> visual_array;
    
    //store text objects for each textual element of the array
    // vector<shared_ptr<Text>> text_array;

    //load font from a file
    Font font;
    if (!font.openFromFile("DejaVuSans.ttf")) {
        cerr << "Error opening DejaVu Sans font" << endl;
        return 1;
    }

    sf::Text text(font, userinput);
    text.setPosition({20.f, 50.f});
    text.setFillColor(Color::Black);

    //draw blank squares where array text will go
    // for(int i = 0; i < N; ++i){
    //     auto rect = make_shared<RectangleShape>();
        
    //     //position with spacing
    //     rect->setSize({80.f, 80.f});
    //     rect->setPosition({50.f + (i * 120.f), 250.f});
    //     rect->setFillColor(Color::White);
    //     visual_array.push_back(rect);
    // }

    //rectangle to contain the key in insertion sort
    // RectangleShape key_rect; 
    // key_rect.setSize({80.f, 80.f});
    // key_rect.setPosition({50.f, 150.f});
    // key_rect.setFillColor(Color::White);
    
    // //draw the inital array
    // draw_array(visual_array, text_array, key, key_rect, A, font);

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
                    userinput.append("B");
                    text.setString(userinput);
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
                    default:
                        break;                        
                }

                text.setString(userinput); 
            }
        }

        //reset the window
        window.clear(Color::White);

        //draw buttons
        window.draw(play_button);
        window.draw(play_button_triangle);
        // window.draw(key_rect);
        window.draw(text);

        //draw array and text
        // for(int i = 0; i < N; ++i){
        //     window.draw(*visual_array[i]);
        //     window.draw(*text_array[i]);
        // }
        // window.draw(*text_array[N]);    

        //display all drawn objects
        window.display();
    }
}

//input: rectangle array, text_array, key, the key rectangle object, array to display, font object
//output: modified text array
//return: N/A
void draw_array(vector<shared_ptr<RectangleShape>>& visual_array, 
                vector<shared_ptr<Text>>& text_array, 
                double key, RectangleShape& key_rect, 
                double A[], Font& font){
    text_array.clear();
    
    //add key text first
    auto text = make_shared<Text>( font , format("{:.2f}", key), 24);
    
    Vector2f shapeCenter = key_rect.getPosition() + (key_rect.getSize() / 2.0f);

    //adjust
    shapeCenter.x -= 20.f;
    shapeCenter.y -= 10.f;

    //set text
    text->setPosition(shapeCenter);
    text->setFillColor(Color::Black);

    //update text array
    text_array.push_back(text);

    for(int i = 0; i < N; ++i){
        //same process as above
        auto rect = visual_array[i];
        auto text = make_shared<Text>(font, format("{:.2f}", A[i]), 24);
        
        Vector2f shapeCenter = rect->getPosition() + (rect->getSize() / 2.0f);
        shapeCenter.x -= 20.f;        
        shapeCenter.y -= 10.f;        

        text->setPosition(shapeCenter);
        text->setFillColor(Color::Black);

        //append new shape and text to vectors
        text_array.push_back(text);
    }
}