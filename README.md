# Project 3/4

Code for the hospital simulation project in CS 201.

## Building and running the SFML visualizer

The visualizer uses the SFML 3 API. In Ubuntu on WSL, install the development
package and build with:

```sh
sudo apt update
sudo apt install libsfml-dev
make clean
make
./bin/main
```

SFML doesn't bundle a font; the visualizer loads the system's DejaVu Sans font
from `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`. On Windows, run it from
WSL with WSLg enabled (available in current Windows 11 WSL installations) so
the SFML window can be displayed.

## Division of labor

### Trey

- B Tree implementation
- Time logic
  - Doctor movement
  - Doctor shifts
  - Patient conditions

### Victoria

- Priority queue
- Data sets
- UML Class Diagram

### Daniel

- Visual representation of simulation with SFML
  - Patient/doctor lookup
