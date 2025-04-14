#include "circle.h"
#include "rect.h"


Circle::~Circle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Circle::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2); // +2 for center and last vertex
    glBindVertexArray(0);
}

void Circle::initVectors() {
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        vertices.push_back(radius * cosf(theta)); // x = r*cos(theta)
        vertices.push_back(radius * sinf(theta)); // y = r*sin(theta)
    }
}

float Circle::getRadius() const { return radius; }

float Circle::getLeft() const   { return pos.x - radius; }
float Circle::getRight() const  { return pos.x + radius; }
float Circle::getTop() const    { return pos.y + radius; }
float Circle::getBottom() const { return pos.y - radius; }

bool Circle::isOverlapping(const Circle &c) const {
    // Check if the distance between the centers of the circles is less than the sum of their radii
    // distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
    float dist = distance(pos, c.getPos());
    float radiusSum = radius + c.getRadius();
    return dist < radiusSum;
}

bool Circle::isOverlapping(const Shape& other) const {
    return false; // placeholder value
}

bool Circle::isOverlapping(const Rect& other) const {
    float r = (radius * 5) + 5;
    //detect upper left corner
    float A = abs(other.getLeft() - pos.x);
    float B = abs(other.getTop() - pos.y);
    float C = sqrt(pow(A, 2) + pow(B, 2));
    if (C <= r) {
        //true if overlapping
        return true;
    }

    //detect upper right corner
    float D = abs(other.getRight() - pos.x);
    float E = abs(other.getTop() - pos.y);
    float F = sqrt(pow(D, 2) + pow(E, 2));
    if (F <= r) {
        //true if overlapping
        return true;
    }

    //detect lower left corner
    float G = abs(other.getLeft() - pos.x);
    float H = abs(other.getBottom() - pos.y);
    float I = sqrt(pow(G, 2) + pow(H, 2));
    if (I <= r) {
        //true if overlapping
        return true;
    }

    //detect lower right corner
    float J = abs(other.getRight() - pos.x);
    float K = abs(other.getBottom() - pos.y);
    float L = sqrt(pow(J, 2) + pow(K, 2));
    if (L <= r) {
        //true if overlapping
        return true;
    }
    return false;
}