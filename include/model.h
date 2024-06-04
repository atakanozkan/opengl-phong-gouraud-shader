#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <glad/glad.h>

class Model {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
    
    Model(const std::string& filePath);

    void render() const;

private:
    void loadOFF(const std::string& filePath);

    void setupModel();
};

#endif // MODEL_H
