#include "model.h"
#include <stdexcept>
#include <iostream>
#include <fstream>


/**
 In constructor, it initializes the model by loading it from an OFF file and setting up its rendering properties.
 */
Model::Model(const std::string& filePath) {
    loadOFF(filePath);
    setupModel();
}

/**
 This function renders the model by drawing triangles.
 Also, it binds the model's VAO and issues a draw call using the indices stored in the EBO.
 */
void Model::render() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/**
 This function loads the model data from an OFF file.
 It reads vertex and face data from the OFF file and stores it in the vertices and indices vectors.
 The parameter is filepath which we get the path of the bunny.off (for this homework).
 */
void Model::loadOFF(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filePath);
    }

    std::getline(file, line);
    if (line != "OFF") {
        throw std::runtime_error("File is not in OFF format: " + filePath);
    }

    size_t vertexCount, faceCount, edgeCount;
    file >> vertexCount >> faceCount >> edgeCount;

    vertices.reserve(vertexCount * 3);
    indices.reserve(faceCount * 3);

    for (size_t i = 0; i < vertexCount; ++i) {
        float x, y, z;
        file >> x >> y >> z;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    for (size_t i = 0; i < faceCount; ++i) {
        int verticesInFace, v1, v2, v3;
        file >> verticesInFace >> v1 >> v2 >> v3;
        if (verticesInFace == 3) {
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }
}

/**
 This function repares the model for rendering by setting up vertex array, buffer objects, and vertex attributes.
 */
void Model::setupModel() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
