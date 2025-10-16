# L-System Fractal Generator
An L-System fractal generator and visualizer, written in C++, using SFML
---
Generates a fractal from its L-system "blueprint", using an axiom, and rules.

L-system algorithm:
- `F` goes forward a specified length
- `+` turn right at an angle
- `-` turn left at an angle
- `X,Y...` replacement characters for rules

Rules:\
Rules can be set for (now 2) characters, which replace the corresponding character in the previus iteration with their rules.
For example, `X` has the rule `+F-F`, which means every `X` character in the previous iteration string gets replaced with `+F-F`.
This goes on for as many iterations, as your computer allows. (This varies between 7-16 iterations usually)

---

Example:\
Axiom: `FXF--FF--FF`\
Rules:
- `F`: `FF`
- `X`: `--FXF++FXF++FXF--`
- angle: 60 degrees

(This draws the Serpinski triangle)

---

Controls:\
<kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd> - Move camera\
<kbd>Space + WASD</kbd> - Speed up camera movement\
<kbd>Scroll</kbd> - Zoom camera\
<kbd>I</kbd><kbd>K</kbd> - Increase / Decrease iteration level\
<kbd>L</kbd> - Reload current iteration\
<kbd>R</kbd> - Enable randomized forward length and angles\
<kbd>O</kbd> - Disable fractal size limit 


---
Screenshots:

### Serpinski triangle:
![Serpinski triangle](/images/serpinski.png)

### Small triangles:
![Small triangles](/images/triangles.png)

### Flower:
![Flower](/images/flower.png)

