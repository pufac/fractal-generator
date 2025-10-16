#define SFML_STATIC
#define SFML_NO_DEPRECATED_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <chrono>
#include <map>
#include <SFML/Graphics.hpp>
#include <atomic>

// g++ fractal.cpp -o program -IC:/sfml/include -LC:/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

using namespace sf;

Vertex newpoint(Vertex origin, int angle, float length, int idx);
std::string prepare2(const std::string& axiom);
void execute(std::string instructions, std::vector<Vertex>& points, int size);
void customfractal();
void loadpresets();
std::string prepare2(const std::string& input);
double mapToHue(double value, double maxValue);
struct RGB {
    double r, g, b;
};
RGB hueToRGB(double hue);
bool createFractal();
void RefreshScreen();




std::string instructions = "";

std::string axiom;
std::map<char, std::string> rules;
float angle = 0;
float setangle = 0;
size_t iterations;

std::vector<Vertex> points;
int size = 1;

std::atomic<size_t> progress = 0;
std::atomic<bool> done = false;


float cameraSpeed = 20.f;
float defaultSpeed = 20.f;
float fastspeed = 2000.0f;


float zoomSpeed = 1.f;
float fastZoomSpeed = 0.5f;
bool bigfractal = false; 
bool randomfractal = false;

std::mt19937 rng(time(0));
RenderWindow window(VideoMode({1920,1080}), "Fractal");
View view({0.0f,0.0f},{1920.0f,1080.0f});

sf::Font font;

int main(){
/*
    font.openFromFile("arial.ttf");

    sf::Text text(font,"Iteration 1");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition({960, 10}); // képernyő bal felső sarka
*/
    window.setView(view);

    std::cout << "L-SYSTEM FRACTAL GENERATOR" << std::endl << std::endl;

    int choice = -1;
    std::cout << "1. custom fractal" << std::endl;
    std::cout << "2. presets" << std::endl;
    
    
        int input;
        std::cin >> input;

        switch (input)
        {
        case 1:
            customfractal();
            break;
        
        case 2:
            loadpresets();

            break;

        default:
            break;
        }
    
    
    std::cout << "Iterations?" << std::endl;
    size_t iterationsinput;
    std::cin >> iterationsinput;
    iterations = iterationsinput;
    
    
    createFractal();
    

    
    Clock clock;
    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent()){
            if (event -> is<Event::Closed>()){
                window.close();
            }
            if (const auto* e = event->getIf<Event::MouseWheelScrolled>())
            {
                if (e->delta > 0)
                    view.zoom(0.9f);
                    
                else
                {
                    view.zoom(1.1f);
                }
            }
        }

        float deltatime = clock.restart().asSeconds();
        
        float mult = deltatime * cameraSpeed;
        if(Keyboard::isKeyPressed(Keyboard::Key::Space)){
            cameraSpeed = fastspeed;
            
        }
        else{
            cameraSpeed = defaultSpeed;
            
        }

        if(Keyboard::isKeyPressed(Keyboard::Key::A)){
            view.move({-30.0f * mult,0.0f});
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::D)){
            view.move({30.0f * mult,0.0f});
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::W)){
            view.move({0.0f,-30.0f * mult});
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::S)){
            view.move({0.0f,30.0f * mult});
        }

        if(Keyboard::isKeyPressed(Keyboard::Key::I)){
            iterations++;
            if(!createFractal()){
                //fractal failed
                createFractal();
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::K)){
            if(iterations > 1) iterations--;
            createFractal();
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::O) && !bigfractal){
            bigfractal = true;
            std::cout << "\nFractal size limitations disabled" << std::endl;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::R) && !randomfractal){
            randomfractal = true;
            std::cout << "\nRandom fractal attributes enabled" << std::endl;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::L)){
            createFractal(); //regenerate
        }
        
        RefreshScreen();
        
        
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }


}

void RefreshScreen(){
    window.setView(view);
    window.clear();
    window.draw(points.data(),points.size(),PrimitiveType::LineStrip);
    window.display();
}

RGB hueToRGB(double hue) {
    double c = 1.0;  // Telítettség és fényerő fixen 1, ha akarod, bővíthető
    double x = 1 - fabs(fmod(hue / 60.0, 2) - 1);
    double r = 0, g = 0, b = 0;

    if (hue < 60)      { r = c; g = x; b = 0; }
    else if (hue < 120){ r = x; g = c; b = 0; }
    else if (hue < 180){ r = 0; g = c; b = x; }
    else if (hue < 240){ r = 0; g = x; b = c; }
    else if (hue < 300){ r = x; g = 0; b = c; }
    else               { r = c; g = 0; b = x; }

    return { r * 255, g * 255, b * 255 };

}

double mapToHue(double value, double maxValue) {
    return (value / maxValue) * 360.0;
}

Vertex newpoint(Vertex origin, int angle, float length, int idx)
{
    angle = angle % 360;
    float x = origin.position.x + cos(angle * M_PI / 180.0f) * length;
    float y = origin.position.y + sin(angle * M_PI / 180.0f) * length;

    
    

    return Vertex{{x,y}, Color::Red, {0.0f,0.0f}};
}

std::string prepare(std::string string)
{
    int length = string.length()/sizeof(char);
    //std::cout << "length" << length << std::endl;
    //std::cout << string << std::endl;
    for (int i = length - 1; i >= 0; i--)
    {
        
        switch (string[i])
        {
        case 'F':
            string.erase(i,1);
            string.insert(i,rules['F']);
            break;
        case 'X':
            string.erase(i,1);
            string.insert(i,rules['X']);
            break;
        case 'Y':
            string.erase(i,1);
            string.insert(i,rules['Y']);
            break;
        default:
            break;
        }
        //std::cout << string << std::endl;
    }
    return string;
}

std::string prepare2(const std::string& axiom) {
    std::string current = axiom;
    std::string next;

    for (size_t i = 0; i < iterations; i++)
    {
        next.clear();
        next.reserve(current.size() * 2);

        for (char c : current) {
            auto it = rules.find(c);
            if (it != rules.end())
                next.append(it->second);
            else
                next.push_back(c);
        }
        current.swap(next);
        progress = i;
    }
    done = true;
    return current;
}

void execute(std::string instructions, std::vector<Vertex>& points, int size)
{

    int length = instructions.length()/sizeof(char);
    float angle = 0;
    

    for (int i = 0; i < length; i++)
    {
        

        switch (instructions[i])
        {
        case 'F':
            {
                Vertex p;
                if(randomfractal){
                    std::uniform_int_distribution<int> dist(-10, 10);
                    std::uniform_int_distribution<int> randomlength(-2, 2);
                    p = newpoint(points[size-1],angle + dist(rng),10 + randomlength(rng),i);
                }
                else{
                    p = newpoint(points[size-1],angle,10,i);
                }
                
                //p.color = c;
                //p.color = Color(100,100,100);

                //std::cout << "r: " << c.r << " - g: " << c.g << " - b: " << c.b << std::endl;

                points.push_back(p);
                size++;
                break;
            }
        
        case '+':
            angle += setangle;
            break;
        case '-':
            angle -= setangle;
            break;
        default:
            break;
        }
        if(i % 100 == 0)
        {
            progress = i;
        }
        
    }
    done = true;
    

}


void customfractal()
{
    std::cout << "Axiom?:" << std::endl;
    std::string axiominput;
    std::cin >> axiominput;

    std::cout << "F: ";
    std::string Finput;
    std::cin >> Finput;

    std::cout << "X: (random key if unused) ";
    std::string Xinput;
    std::cin >> Xinput;

    std::cout << "Y: (random key if unused) ";
    std::string Yinput;
    std::cin >> Yinput;

    std::cout << "Angle? ";
    float angleinput;
    std::cin >> angleinput;

    

    std::cout << std::endl;

    axiom = axiominput;
    setangle = angleinput;
    //iterations = iterationsinput;
    rules['F'] = Finput;
    rules['X'] = Xinput;
    rules['Y'] = Yinput;
}

void loadpresets()
{
    std::cout << "1. Serpinski triangle [8]" <<std::endl;
    std::cout << "2. kis haromszogek [10]" <<std::endl;
    std::cout << "3. oneline [5]" <<std::endl;
    std::cout << "4. csiga [5]" <<std::endl;
    std::cout << "5. virag1 [5]" <<std::endl;
    std::cout << "6. virag2 [5]" <<std::endl;
    std::cout << "7. bug negyzet" <<std::endl;
    std::cout << "8. gilbert" <<std::endl;
    std::cout << "9. Board" <<std::endl;
    
    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        axiom = "FXF--FF--FF";
        rules['F'] = "FF";
        rules['X'] = "--FXF++FXF++FXF--";
        setangle = 60;
        break;
    
    case 2:
        axiom = "F";
        rules['F'] = "F+F-F";
        setangle = 120;
        break;

    case 3:
        axiom = "X";
        rules['F'] = "";
        rules['X'] = "-F++F-X-F--F+Y---F--F+Y+F++F-X+++F++F-X-F++F-X+++F--F+Y--";
        rules['Y'] = "+F++F-X-F--F+Y+F--F+Y---F--F+Y---F++F-X+++F++F-X+++F--F+Y";
        setangle = 30;
        break;

    case 4:
        axiom = "X";
        rules['F'] = "";
        rules['X'] = "FX+FX+FXFYFX+FXFY-FY-FY-";
        rules['Y'] = "+FX+FX+FXFY-FYFXFY-FY-FY";
        setangle = 90;
        break;

    case 5:
        axiom = "X-X-X-X-X";
        rules['F'] = "";
        rules['X'] = "FX-FX-FX+FY+FY+FX-FX";
        rules['Y'] = "FY+FY-FX-FX-FY+FY+FY";
        setangle = 72;
        break;

    case 6:
        axiom = "F-F-F-F-F";
        rules['F'] = "F-F-F++F+F-F";
        setangle = 72;
        break;

    case 7:
        axiom = "F+F+F+F";
        rules['F'] = "FF+F++F+F";
        setangle = 90;
        break;

    case 8:
        axiom = "X";
        rules['F'] = "F";
        rules['X'] = "-YF+XFX+FY-";
        rules['Y'] = "+XF-YFY-FX+";
        setangle = 90;
        break;
    case 9:
        axiom = "F+F+F+F";
        rules['F'] = "FF+F+F+F+FF";
        setangle = 90;
        break;    
    default:
        break;
    }
    
}

bool createFractal(){
    points.clear();
    Vertex origin = Vertex{{0,0},Color::White,{0.0f,0.0f}};
    points.push_back(origin);
    instructions = axiom;
    auto start = std::chrono::high_resolution_clock::now();
    
    //std::cout << "time taken: " << end - start << "s" << std::endl;

    //std::thread p(prepare2, std::ref(axiom));
    std::cout << "\nITERATION " << iterations << std::endl;
    std::thread p([] {
        instructions = prepare2(axiom);
    });

    while (!done)
    {
        float percent = (float)progress / iterations * 100.0f;
        std::cout << "\rString Progress: " << std::setprecision(4) << percent << "%" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    p.join();
    
    std::cout << "\rString Progress: 100%   " << std::flush;

    if(instructions.length() > 300000000 && !bigfractal){
        std::cout << "\nERROR: Fractal too large" << std::endl;
        iterations--;
        return false;
    }

    done = false;

    if(instructions.length() > 2000)
    {
        std::cout << "\nInstruction too long (" << instructions.length() << "), skipping..." << std::endl;
    }else{
        std::cout << instructions << std::endl;
    }

    std::thread e(execute, std::ref(instructions), std::ref(points), size);

    while(!done)
    {
        float percent = (float)progress / instructions.size() * 100.0f;
        std::cout << "\rExecute Progress: " << percent << "%              " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    e.join();
    
    std::cout << "\rExecute Progress: 100%   " << std::flush;
    std::cout << "\nDrawing..." << std::endl;

    //execute(instructions, points, size);
    std::cout << "\nVertex count: " << points.size() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken: " << elapsed.count() << "s" << std::endl;

    
    


    //coloring
    for (size_t i = 0; i < points.size(); i++)
    {
        double ratio = mapToHue(i,points.size());
        RGB color = hueToRGB(ratio);

        Color c = Color(color.r,color.g,color.b);
        points[i].color = c;
    }
    return true;
}