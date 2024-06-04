#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <glad/glad.h>

class Cube {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    unsigned int VAO, VBO, EBO;

    Cube();
    void setupMesh();
    void render();
};

#endif // CUBE_H

