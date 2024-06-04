#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h>

class Sphere {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    unsigned int VAO, VBO, EBO;

    Sphere();
    void generateSphere(float radius, unsigned int longDiv, unsigned int latDiv);
    void setupMesh();
    void render();
};

#endif // SPHERE_H
