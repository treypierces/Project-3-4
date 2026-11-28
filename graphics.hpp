#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

// Owns every shape, text object and the font used by the hospital UI,
// so main() only has to update state and call draw().
class HospitalUI
{
public:
    static constexpr int WINDOW_WIDTH  = 1280;
    static constexpr int WINDOW_HEIGHT = 720;

    // Throws std::runtime_error if the font cannot be loaded.
    explicit HospitalUI(const std::string& fontPath = "DejaVuSans.ttf", int startTime = 7)
        : font(loadFont(fontPath)),
          play_button_triangle(10.f, 3),
          play_button({30.f, 30.f}),
          text_box({200.f, 50.f}),
          look_up_box({500.f, 300.f}),
          sf_userin(font, ""),
          sf_lookup_result(font, ""),
          sf_lookup_info(font, ""),
          sf_time(font, ""),
          sf_info(font,
                  "Press 'D' to search for a doctor by ID\n"
                  "Press 'P' to search for a patient by ID\n"
                  "Click the play button to simulate time passing",
                  20),
          event_texts{
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12),
              sf::Text(font, "", 12)}
    {
        // play button triangle
        play_button_triangle.rotate(sf::degrees(90));
        play_button_triangle.setPosition({WINDOW_WIDTH / 2.f + 10.f, WINDOW_HEIGHT * .95f});
        play_button_triangle.setFillColor(sf::Color(0, 255, 0));

        // play button
        play_button.setPosition({WINDOW_WIDTH / 2.f - 15.f, WINDOW_HEIGHT * .95f - 5.f});
        play_button.setFillColor(sf::Color::White);
        play_button.setOutlineColor(sf::Color::Black);
        play_button.setOutlineThickness(2.f);

        // user text input box
        text_box.setPosition({20.f, 20.f});
        text_box.setFillColor(sf::Color::White);
        text_box.setOutlineColor(sf::Color::Black);
        text_box.setOutlineThickness(2.f);

        // lookup result box
        look_up_box.setPosition({10.f, 10.f});
        look_up_box.setFillColor(sf::Color::White);
        look_up_box.setOutlineColor(sf::Color::Black);
        look_up_box.setOutlineThickness(2.f);

        // texts
        sf_userin.setPosition({20.f, 20.f});
        sf_userin.setFillColor(sf::Color::Black);

        sf_lookup_result.setPosition({20.f, 80.f});
        sf_lookup_result.setFillColor(sf::Color::Black);

        sf_lookup_info.setPosition({20.f, 110.f});
        sf_lookup_info.setFillColor(sf::Color::Black);

        sf_time.setPosition({WINDOW_WIDTH / 2.f - 75.f, WINDOW_HEIGHT * .90f - 25.f});
        sf_time.setFillColor(sf::Color::Black);
        setTime(startTime);

        sf_info.setPosition({20.f, 350.f});
        sf_info.setFillColor(sf::Color::Black);

        for (std::size_t i = 0; i < event_boxes.size(); ++i) {
            const float x = 620.f + static_cast<float>(i % 2) * 320.f;
            const float y = 12.f + static_cast<float>(i / 2) * 155.f;
            event_boxes[i].setSize({306.f, 145.f});
            event_boxes[i].setPosition({x, y});
            event_boxes[i].setFillColor(sf::Color::White);
            event_boxes[i].setOutlineColor(sf::Color::Black);
            event_boxes[i].setOutlineThickness(1.f);
            event_texts[i].setPosition({x + 8.f, y + 6.f});
            event_texts[i].setFillColor(sf::Color::Black);
            setTimestepEventText(i, "None");
        }
    }

    // sf::Text keeps a reference to the font, so this object must not be copied or moved.
    HospitalUI(const HospitalUI&) = delete;
    HospitalUI& operator=(const HospitalUI&) = delete;

    // ---------- state updates ----------
    void setTime(int hour)                       { sf_time.setString("Time: " + std::to_string(hour) + ":00"); }
    void setUserInput(const std::string& s)      { sf_userin.setString(s); }
    void setLookupResult(const std::string& s)   { sf_lookup_result.setString(s); }
    void setLookupInfo(const std::string& s)     { sf_lookup_info.setString(s); }
    void setTimestepEventText(std::size_t index, const std::string& text)
    {
        static constexpr std::array<const char*, 8> titles{
            "New patients",
            "Doctors starting shifts",
            "Doctors ending shifts",
            "Patients waiting after assignments",
            "Doctor-patient assignments",
            "Healed and discharged",
            "Critical patients",
            "Patients who died"};
        if (index >= event_texts.size()) {
            throw std::out_of_range("Invalid timestep event panel index");
        }
        event_texts[index].setString(std::string(titles[index]) + ":\n" + wrapText(text));
    }

    // ---------- input helpers ----------
    bool playButtonClicked(sf::Vector2f worldPos) const
    {
        return play_button.getGlobalBounds().contains(worldPos);
    }

    // ---------- drawing ----------
    void draw(sf::RenderWindow& window)
    {
        window.clear(sf::Color::White);

        // buttons
        window.draw(play_button);
        window.draw(play_button_triangle);

        // search results
        window.draw(look_up_box);
        window.draw(sf_lookup_result);
        window.draw(sf_lookup_info);

        // search box
        window.draw(text_box);
        window.draw(sf_userin);

        // time + info text
        window.draw(sf_time);
        window.draw(sf_info);

        for (std::size_t i = 0; i < event_boxes.size(); ++i) {
            window.draw(event_boxes[i]);
            window.draw(event_texts[i]);
        }

        window.display();
    }

    void buttonTyped(sf::Keyboard::Key key, std::string& userinput, bool& doctor_lookup)
    {
        switch(key){
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
    }

private:
    static std::string wrapText(const std::string& text)
    {
        constexpr std::size_t maxLineLength = 39;
        std::string wrapped;
        std::string line;
        std::size_t wordStart = 0;
        while (wordStart < text.size()) {
            while (wordStart < text.size() && text[wordStart] == ' ') {
                ++wordStart;
            }
            if (wordStart == text.size()) {
                break;
            }
            const std::size_t wordEnd = text.find(' ', wordStart);
            const std::string word = text.substr(
                wordStart,
                wordEnd == std::string::npos ? std::string::npos : wordEnd - wordStart);
            if (!line.empty() && line.size() + 1 + word.size() > maxLineLength) {
                if (!wrapped.empty()) {
                    wrapped += '\n';
                }
                wrapped += line;
                line.clear();
            }
            if (!line.empty()) {
                line += ' ';
            }
            line += word;
            if (wordEnd == std::string::npos) {
                break;
            }
            wordStart = wordEnd + 1;
        }
        if (!line.empty()) {
            if (!wrapped.empty()) {
                wrapped += '\n';
            }
            wrapped += line;
        }
        return wrapped;
    }

    static sf::Font loadFont(const std::string& path)
    {
        sf::Font f;
        if (!f.openFromFile(path))
            throw std::runtime_error("Error opening font: " + path);
        return f;
    }

    // NOTE: font must be declared first so it is constructed before the texts.
    sf::Font font;

    sf::CircleShape    play_button_triangle;
    sf::RectangleShape play_button;
    sf::RectangleShape text_box;
    sf::RectangleShape look_up_box;

    sf::Text sf_userin;
    sf::Text sf_lookup_result;
    sf::Text sf_lookup_info;
    sf::Text sf_time;
    sf::Text sf_info;
    std::array<sf::RectangleShape, 8> event_boxes;
    std::array<sf::Text, 8> event_texts;
};

#endif // GRAPHICS_HPP
