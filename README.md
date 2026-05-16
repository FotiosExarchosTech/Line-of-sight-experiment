# 2D Line of Sight & Visibility Prototype

A 2D visibility and dynamic lighting prototype built in C++ using the **SFML (Simple and Fast Multimedia Library)**. The system calculates real-time field of view (FOV) and shadows from a player-controlled source by casting rays at the corners of obstacles and rendering a dynamic visibility polygon.

## Features

* **Dynamic Raycasting:** Generates rays targeting wall endpoints with fine-tuned angular offsets to simulate perfect edge shadows.
* **Real-time 2D Line-Line Intersection:** Custom geometric intersection math determines exactly where light rays hit solid objects.
* **Optimized Visibility Mesh:** Sorts rays by angle and renders a mesh using `sf::TriangleFan` to create a continuous fluid light field.
* **Debug Overlay:** Toggleable visual indicators to display underlying collision walls and ray segments.
* **Camera Tracking:** A smooth camera system that centers around the player’s movement.

---

## The Math: Line-Line Intersection

To determine where a light ray hits an obstacle, the program checks for an intersection between a ray segment (from the player's position to a distant point) and a wall segment. 

Based on the geometric principles outlined in [Wikipedia's Line-Line Intersection](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection), given a ray segment from $(x_1, y_1)$ to $(x_2, y_2)$ and a wall segment from $(x_3, y_3)$ to $(x_4, y_4)$, the intersection point can be solved using Bézier parameters $t$ and $u$.

The denominator $D$ represents the cross-product of the direction vectors:
$$D = (x_1 - x_2)(y_3 - y_4) - (y_1 - y_2)(x_3 - x_4)$$

If $D = 0$, the lines are parallel or collinear, meaning there is no single intersection point. Otherwise, the intersection coefficients are calculated as follows:

$$t = \frac{(x_1 - x_3)(y_3 - y_4) - (y_1 - y_3)(x_3 - x_4)}{D}$$

$$u = \frac{(x_1 - x_3)(y_1 - y_2) - (y_1 - y_3)(x_1 - x_2)}{D}$$

### Intersection Criteria
An intersection occurs explicitly on both segments if and only if:
$$0 < t < 1 \quad \text{and} \quad 0 < u < 1$$

When this condition is true, the ray's endpoint is updated to the exact point of intersection $(x, y)$:
$$x = x_1 + t(x_2 - x_1)$$
$$y = y_1 + t(y_2 - y_1)$$

---

## How It Works

1. **Ray Generation Strategy:** Instead of casting 360 rays uniformly, the engine casts rays directly at the corners (`b_1` and `b_2`) of every wall in the scene. For each corner, it casts three rays: one directly at the corner, one at $\text{angle} + 0.0001$, and one at $\text{angle} - 0.0001$. This allows the light to "peek" past corners and cast accurate shadows behind walls.
2. **Ray Clipping:** Every ray is evaluated against every wall using the intersection math. If a ray intersects a wall, its length is shortened to that exact point.
3. **Angular Sorting:** The resulting endpoints are saved alongside their absolute angles. The program sorts these rays sequentially by their angles to prevent the rendering mesh from twisting.
4. **Visibility Mesh Rendering:** An `sf::TriangleFan` is anchored at the player's position, stitching together the sorted ray endpoints to construct a seamless, lit environment mask.

---

## Controls

| Key | Action |
| :--- | :--- |
| **Up / Down / Left / Right Arrows** | Move the player (Blue Circle) |
| **S (Hold)** | Show Debug Mode (Draws red walls and raw ray lines) |
| **Escape** | Close the application |

---

## Prerequisites & Installation

### Dependencies
* **C++11** or higher compiler
* **SFML 2.5+** (Graphics, Window, System modules)

### Building the Project
If you are using `g++` via the command line, you can compile the code using:

```bash
g++ -c line_of_sight_prototype.cpp
g++ line_of_sight_prototype.o -o line_of_sight_app -lsfml-graphics -lsfml-window -lsfml-system
./line_of_sight_app
