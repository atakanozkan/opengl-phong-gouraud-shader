#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Room {
public:
    Room();
    ~Room();
    
    void init();
    void render();
    glm::vec3 getMinBounds() const;
    glm::vec3 getMaxBounds() const;
private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    unsigned int VAO, VBO, EBO;
};

#endif // ROOM_H
