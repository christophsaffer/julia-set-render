#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;


int main()
{

    using color = array<float, 4>;
    int screen_width = 600;
    int screen_height = 800;
    vector<color> pixel_buffer(screen_width * screen_height);

    for(int i=0; i < screen_width; ++i){
      for(int j=0; j < screen_height; ++j){
        auto index = i + j * screen_width;
        auto scale = static_cast<float>(j) / screen_height;
        pixel_buffer[index] = {scale, scale, scale, 1};
      }
    }
    // create the window
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        // window.clear(sf::Color::Red);

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...
        glDrawPixels(screen_width, screen_height, GL_RGBA, GL_FLOAT, pixel_buffer.data());

        // end the current frame
        window.display();
    }

    return 0;
}
