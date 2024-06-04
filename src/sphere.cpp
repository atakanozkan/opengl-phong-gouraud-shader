#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "sphere.h"
#include <glad/glad.h>

/**
   Sphere constructor that initializes and generates the sphere mesh.
 */
Sphere::Sphere() {
    generateSphere(1.0f, 20, 20);
    setupMesh();
}

/**
 This function generates vertices, normals, texture coordinates, and indices to create a sphere mesh.
 */
void Sphere::generateSphere(float radius, unsigned int longDiv, unsigned int latDiv) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int y = 0; y <= latDiv; ++y) {
        for (unsigned int x = 0; x <= longDiv; ++x) {
            float xSegment = (float)x / longDiv;
            float ySegment = (float)y / latDiv;
            float xPos = radius * std::cos(xSegment * 2.0 * M_PI) * std::sin(ySegment * M_PI);
            float yPos = radius * std::cos(ySegment * M_PI);
            float zPos = radius * std::sin(xSegment * 2.0 * M_PI) * std::sin(ySegment * M_PI);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            vertices.push_back(xSegment);
            vertices.push_back(ySegment);
        }
    }

    for (unsigned int y = 0; y < latDiv; ++y) {
        for (unsigned int x = 0; x < longDiv; ++x) {
            indices.push_back((y + 1) * (longDiv + 1) + x);
            indices.push_back(y * (longDiv + 1) + x);
            indices.push_back(y * (longDiv + 1) + x + 1);

            indices.push_back((y + 1) * (longDiv + 1) + x);
            indices.push_back(y * (longDiv + 1) + x + 1);
            indices.push_back((y + 1) * (longDiv + 1) + x + 1);
        }
    }

    this->vertices = vertices;
    this->indices = indices;
}

/**
 This function sets up the mesh for the sphere by creating buffers and configuring vertex attributes.
 */
void Sphere::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/**
 Renders the sphere by binding the VAO and drawing its triangles.
 */
void Sphere::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
