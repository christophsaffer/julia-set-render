#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <array>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

using color = array<float, 4>;
int screen_width = 600;
int screen_height = 300;
float x_min = -2.0f;
float x_max = 2.0f;
float y_max = 1.0f;
float y_min = -1.0f;

float stepsize_x = (x_max - x_min) / screen_width;
float stepsize_y = (y_max - y_min) / screen_height;

const int max_iterations = 2 << 4;
complex<float> coeff = {-0.835f, -0.2321f};

vector<color> pixel_buffer(screen_width *screen_height);

void update_viewport() {
  stepsize_x = (x_max - x_min) / screen_width;
  stepsize_y = (y_max - y_min) / screen_height;
}

void draw_shadow() {
  for (int i = 0; i < screen_width; ++i) {
    for (int j = 0; j < screen_height; ++j) {
      auto index = i + j * screen_width;
      auto scale = static_cast<float>(j) / screen_height;
      pixel_buffer[index] = {scale, scale, scale, 1};
    }
  }
}

void draw_julia_set() {
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < screen_width; ++i) {
    for (int j = 0; j < screen_height; ++j) {
      auto index = i + j * screen_width;

      float real = i * stepsize_x + x_min;
      float imag = j * stepsize_y + y_min;

      complex<float> z = {real, imag};

      int it = 0;
      for (; (norm(z) < 4) && (it < max_iterations); ++it) {
        z = z * z + coeff;
      }
      auto scale = log(1.f + it) / log(1.f + max_iterations);
      pixel_buffer[index] = {scale, scale, 0.5, 1};
    }
  }
}

int main() {

  // create object window from sfml
  // width, height - name - window style, 32 bit colors
  sf::RenderWindow window(sf::VideoMode(screen_width, screen_height),
                          "Julia Sets", sf::Style::Default,
                          sf::ContextSettings{32});

  // turn on vertical synchronisation for constant 60 FPS on monitor.
  window.setVerticalSyncEnabled(true);
  // activate the window. Change OpenGL context to use this window.
  window.setActive(true);

  // run the program as long as the window is open
  while (window.isOpen()) {
    auto mouse_position = sf::Mouse::getPosition(window);
    int mouse_x = mouse_position.x;
    int mouse_y = mouse_position.y;

    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::Resized) {
        screen_width = event.size.width;
        screen_height = event.size.height;
        update_viewport();
        glViewport(0, 0, screen_width, screen_height);
        pixel_buffer.resize(screen_width * screen_height);
      }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      coeff = {mouse_x * stepsize_x + x_min, mouse_y * stepsize_y + y_min};
      draw_julia_set();
    }

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // fill pixel buffer
    draw_julia_set();

    // draw from pixel_buffer
    glDrawPixels(screen_width, screen_height, GL_RGBA, GL_FLOAT,
                 pixel_buffer.data());

    // end the current frame
    window.display();
  }

  return 0;
}
