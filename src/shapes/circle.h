#ifndef GRAPHICS_CIRCLE_H
#define GRAPHICS_CIRCLE_H

#include "shape.h"
#include "rect.h"
#include "../shader/shader.h"
using std::vector, glm::vec2, glm::vec3, glm::normalize, glm::dot;


class Circle : public Shape {
private:

/// @brief Number of x,y points to draw the circle
    const static int segments = 100;
    /// @brief Radius of the circle (half of screen width
    float radius = 0.5f;

public:
    /// @brief Construct a new Circle object
    /// @details This is the main constructor for the Circle class.
    /// @details All other constructors call this constructor.
    Circle(Shader &shader, vec2 pos, vec2 size, vec4 color)
        : Shape(shader, pos, size, color), radius(size.x / 2) {
        initVectors();
        initVAO();
        initVBO();
    }

    Circle(Shader & shader, vec2 pos, vec2 size, struct color color)
        : Circle(shader, pos, size, vec4(color.red, color.green, color.blue, 1.0f)) {}

    Circle(Shader &shader, vec2 pos, float radius, struct color color)
        : Circle(shader, pos, vec2(radius * 2, radius * 2), vec4(color.red, color.green, color.blue, 1.0f)) {}

    Circle(Shader &shader, vec2 pos, float radius, vec4 color)
        : Circle(shader, pos, vec2(radius * 2, radius * 2), color) {}


    /// @brief Destroy the Circle object
    /// @details destroys the VAO and VBO associated with the circle
    ~Circle() override;

    /// @brief Draws the circle
    void draw() const override;

    /// @brief Computes the border of the circle, and stores the vertices in the circleVertices array.
    void initVectors();

    float getRadius() const;

    // --------------------------------------------------------
    // Overloaded functions
    // --------------------------------------------------------
    // Position/Movement Functions
    float getLeft() const override;
    float getRight() const override;
    float getTop() const override;
    float getBottom() const override;


    bool isOverlapping(const Circle &c) const;
    bool isOverlapping(const Shape& other) const;
    bool isOverlapping(const Rect& other) const;
};


#endif //GRAPHICS_CIRCLE_H
