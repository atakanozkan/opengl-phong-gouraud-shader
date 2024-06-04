#include "Room.h"
#include <vector>
#include <glm/glm.hpp>

Room::Room() : VAO(0), VBO(0) {
    this->init();
}

Room::~Room() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

glm::vec3 backColor = glm::vec3(1.0f, 1.0f, 1.0f);   // White
glm::vec3 rightColor = glm::vec3(0.7f, 0.7f, 0.7f);  // Light Gray
glm::vec3 leftColor = glm::vec3(0.5f, 0.5f, 0.5f);   // Dark Gray
glm::vec3 bottomColor = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow
glm::vec3 topColor = glm::vec3(0.0f, 1.0f, 1.0f);    // Cyan

void Room::init() {
    this->vertices= {
        -4.0f, -4.0f, -4.0f, backColor.r, backColor.g, backColor.b, 0.0f, 0.0f, 1.0f,
         4.0f, -4.0f, -4.0f, backColor.r, backColor.g, backColor.b, 0.0f, 0.0f, 1.0f,
         4.0f,  4.0f, -4.0f, backColor.r, backColor.g, backColor.b, 0.0f, 0.0f, 1.0f,
        -4.0f,  4.0f, -4.0f, backColor.r, backColor.g, backColor.b, 0.0f, 0.0f, 1.0f,
        
        // Right
         4.0f, -4.0f, -4.0f, rightColor.r, rightColor.g, rightColor.b, -1.0f, 0.0f, 0.0f,
         4.0f, -4.0f,  4.0f, rightColor.r, rightColor.g, rightColor.b, -1.0f, 0.0f, 0.0f,
         4.0f,  4.0f,  4.0f, rightColor.r, rightColor.g, rightColor.b, -1.0f, 0.0f, 0.0f,
         4.0f,  4.0f, -4.0f, rightColor.r, rightColor.g, rightColor.b, -1.0f, 0.0f, 0.0f,

        // Left
        -4.0f, -4.0f, -4.0f, leftColor.r, leftColor.g, leftColor.b, 1.0f, 0.0f, 0.0f,
        -4.0f,  4.0f, -4.0f, leftColor.r, leftColor.g, leftColor.b, 1.0f, 0.0f, 0.0f,
        -4.0f,  4.0f,  4.0f, leftColor.r, leftColor.g, leftColor.b, 1.0f, 0.0f, 0.0f,
        -4.0f, -4.0f,  4.0f, leftColor.r, leftColor.g, leftColor.b, 1.0f, 0.0f, 0.0f,
        
        // Bottom
        -4.0f, -4.0f, -4.0f, bottomColor.r, bottomColor.g, bottomColor.b, 0.0f, 1.0f, 0.0f,
         4.0f, -4.0f, -4.0f, bottomColor.r, bottomColor.g, bottomColor.b, 0.0f, 1.0f, 0.0f,
         4.0f, -4.0f,  4.0f, bottomColor.r, bottomColor.g, bottomColor.b, 0.0f, 1.0f, 0.0f,
        -4.0f, -4.0f,  4.0f, bottomColor.r, bottomColor.g, bottomColor.b, 0.0f, 1.0f, 0.0f,

        // Top
        -4.0f,  4.0f, -4.0f, topColor.r, topColor.g, topColor.b, 0.0f, -1.0f, 0.0f,
         4.0f,  4.0f, -4.0f, topColor.r, topColor.g, topColor.b, 0.0f, -1.0f, 0.0f,
         4.0f,  4.0f,  4.0f, topColor.r, topColor.g, topColor.b, 0.0f, -1.0f, 0.0f,
        -4.0f,  4.0f,  4.0f, topColor.r, topColor.g, topColor.b, 0.0f, -1.0f, 0.0f,
    };

    this->indices = {
        0, 1, 2, 0, 2, 3,       // Back
        4, 5, 6, 4, 6, 7,       // Right
        8, 9, 10, 8, 10, 11,    // Left
        12, 13, 14, 12, 14, 15, // Bottom
        16, 17, 18, 16, 18, 19  // Top
    };
    
    
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec3 Room::getMinBounds() const {
    return glm::vec3(-4.0f, -4.0f, -4.0f);
}

glm::vec3 Room::getMaxBounds() const {
    return glm::vec3(4.0f, 4.0f, 4.0f);
}

void Room::render() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
