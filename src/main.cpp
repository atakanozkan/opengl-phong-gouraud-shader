#include<filesystem>
namespace fs = std::filesystem;
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "sphere.h"
#include "cube.h"
#include "model.h"
#include "room.h"
#include "texture.h"

int currentObjectType = 1;
bool wireframeMode = false;
bool setObjectLeftTop = false;
bool set3DmodeOn = false;
bool usePhong = true;

float fov = glm::radians(45.0f);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/**
    The user needs to set the paths for the shaders and bunny model correctly.
 */
const char* textureVertexShaderPath = "/comp410-hw1/src/texture_vertex.glsl";
const char* textureFragmentShaderPath ="/comp410-hw1/src/texture_fragment.glsl";
const char* phongVertexShaderPath = "/comp410-hw1/src/phong_vertex.glsl";
const char* phongFragmentShaderPath ="/comp410-hw1/src/phong_fragment.glsl";
const char* gouraudVertexShaderPath = "/comp410-hw1/src/gouraud_vertex.glsl";
const char* gouraudFragmentShaderPath = "/comp410-hw1/src/gouraud_fragment.glsl";
const char* roomVertexShaderPath = "/comp410-hw1/src/room_vertex.glsl";
const char* roomFragmentShaderPath ="/comp410-hw1/src/room_fragment.glsl";
const char* basketballTexturePath = "/comp410-hw1/texture/basketball.ppm";
const char* earthTexturePath = "/comp410-hw1/texture/earth.ppm";
const char* bunnyModelPath = "/comp410-hw1/model/bunny.off";

std::vector<glm::vec4> colors = {
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), // Cyan
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Magenta
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
};

int currentColorIndex = 0;
glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 11.0f);
glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 objectVelocity = glm::vec3(0.12f, 0.08f, 0.0f);
float objectVelocityZ = -0.1f;
bool isMetallic = false;
bool isTextureUse = false;
bool useAmbient = true;
bool useDiffuse = true;
bool useSpecular = true;
bool lightFixed = true;
bool inspectMode = false;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
const float rotationSpeed = glm::radians(10.0f);

struct Material {
    float specularStrength;
    float shininess;
};

Material plastic = {0.05, 16.0};
Material metallic = {1.0, 1024.0};

int activeTextureIndex = 0;  // 0 for basketball, 1 for earth
std::vector<Texture*> textures;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void updateObjectPosition(glm::vec3 min, glm::vec3 max, float objectSize);
void moveObjectToLeftTop(glm::vec3 min, glm::vec3 max, float objectSize);

/**
    This function is the main entry point of the application.
    Initializes GLFW and configures it for OpenGL.
    It creates the window and sets up GLAD to handle OpenGL function pointers.
    It creates the depth testing for 3D rendering and sets the functionalities of receving input in window.
    Before main loop, it creates shader programs and set models for rendering. Also it sets up the camera and projection matrices.
    In the main loop, we handle the movement of object and current type of the object (sphere,cube and bunny).
    Also, we set the toggling wireframe mode according to user input and change the color of the object.
    After the mechanics, user interaction and rendering the objects, it swaps the buffers.
    In the last part, it terminates the program.
 */
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "COMP410 HW3", NULL, NULL);
    
    float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    
    Texture basketballTexture((parentDir+basketballTexturePath).c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture earthTexture((parentDir+earthTexturePath).c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    textures.push_back(&basketballTexture);
    textures.push_back(&earthTexture);
    
    Shader textureShader((parentDir+textureVertexShaderPath).c_str(), (parentDir+textureFragmentShaderPath).c_str());
    
    GLuint uniID = glGetUniformLocation(textureShader.ID, "scale");
    
    
    Shader phongShader((parentDir+phongVertexShaderPath).c_str(), (parentDir+phongFragmentShaderPath).c_str());
    
    Shader gouraudShader((parentDir+gouraudVertexShaderPath).c_str(), (parentDir+gouraudFragmentShaderPath).c_str());
    
    Shader roomShader((parentDir+roomVertexShaderPath).c_str(), (parentDir+roomFragmentShaderPath).c_str());
    
    Sphere sphere;
    Cube cube;
    Model bunnyModel((parentDir+bunnyModelPath).c_str());
    Room room;
    
    glm::mat4 projection = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        cameraPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    moveObjectToLeftTop(room.getMinBounds(),room.getMaxBounds(),1.0f);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        updateObjectPosition(room.getMinBounds(),room.getMaxBounds(),1.0f);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (inspectMode) {
            glm::vec3 inspectCamPos = glm::vec3(
                10.0f * cos(cameraAngleX) * sin(cameraAngleY),
                10.0f * sin(cameraAngleX),
                10.0f * cos(cameraAngleX) * cos(cameraAngleY)
            );
            view = glm::lookAt(inspectCamPos, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        } 
        else {
            view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        
        glm::vec3 lightPos;
        if(inspectMode){
            lightPos = lightFixed ? lightPosition : glm::vec3(0.0f, 0.0f, 5.0f);
        }
        else{
            lightPos = lightFixed ? lightPosition : objectPosition;
        }
        glm::mat4 roomModel = glm::mat4(1.0f);
        roomShader.use();
        roomShader.setMat4("view", view);
        roomShader.setMat4("projection", projection);
        roomShader.setMat4("model", roomModel);
        roomShader.setVec3("lightPos", lightPos);
        roomShader.setVec3("viewPos", cameraPos);
        roomShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
        roomShader.setBool("useAmbient", true);
        roomShader.setBool("useDiffuse", true);
        roomShader.setBool("useSpecular", true);
        room.render();
       
       Shader objectShader = usePhong ? phongShader : gouraudShader;

        if (isTextureUse && currentObjectType == 1) {
            textureShader.use();
            textureShader.setMat4("view", view);
            textureShader.setMat4("projection", projection);
            glUniform1f(uniID, 0.5f);
        } else {
            objectShader.use();
            objectShader.setMat4("view", view);
            objectShader.setMat4("projection", projection);
            objectShader.setVec3("lightPos", lightPos);
            objectShader.setVec3("viewPos", cameraPos);
            objectShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
            objectShader.setFloat("specularStrength", isMetallic ? metallic.specularStrength : plastic.specularStrength);
            objectShader.setFloat("shininess", isMetallic ? metallic.shininess : plastic.shininess);
            objectShader.setBool("useAmbient", useAmbient);
            objectShader.setBool("useDiffuse", useDiffuse);
            objectShader.setBool("useSpecular", useSpecular);
        }
        
        if(setObjectLeftTop){
            moveObjectToLeftTop(room.getMinBounds(),room.getMaxBounds(),1.0f);
            setObjectLeftTop = false;
        }
        
       if (wireframeMode) {
           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       } else {
           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       }
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), inspectMode ? glm::vec3(0.0f, 0.0f, 5.0f) : objectPosition);
        model = glm::scale(model, inspectMode ? glm::vec3(3.0f) : glm::vec3(1.0f));
        if(currentObjectType == 2){
            if(isTextureUse){
                std::cout << "Texture Mode OFF\n";
                isTextureUse = false;
            }
            model = glm::rotate(model, 40.02f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.03f));
            objectShader.setMat4("model", model);
            objectShader.setInt("useTexture", 0);
            objectShader.setVec4("chosenColor", colors[currentColorIndex]);
            objectShader.setInt("useChosenColor", GL_TRUE);
            bunnyModel.render();
        }
       else if (currentObjectType == 1) {
           model = glm::scale(model, glm::vec3(0.5f));
            if (isTextureUse) {
                glActiveTexture(GL_TEXTURE0);
                textures[activeTextureIndex]->Bind();
                textureShader.setInt("texture1", 0);
                textureShader.setMat4("model", model);
                sphere.render();
                textures[activeTextureIndex]->Unbind();
            } else {
                objectShader.setMat4("model", model);
                objectShader.setInt("useTexture", 0);
                objectShader.setVec4("chosenColor", colors[currentColorIndex]);
                objectShader.setInt("useChosenColor", GL_TRUE);
                sphere.render();
            }
       }
       else {
           if(isTextureUse){
               std::cout << "Texture Mode OFF\n";
               isTextureUse = false;
           }
           model = glm::scale(model, glm::vec3(0.5f));
           objectShader.setMat4("model", model);
           objectShader.setInt("useTexture", 0);
           objectShader.setVec4("chosenColor", colors[currentColorIndex]);
           objectShader.setInt("useChosenColor", GL_TRUE);
           cube.render();
       }
       glfwSwapBuffers(window);
       glfwPollEvents();
    }

    basketballTexture.Delete();
    glfwTerminate();
    return 0;
}

/**
    This function sets the viewport size and handles window resizing
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
    This function process the input from user for keyboard and mouse inputs
 */
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/**
    This funciton handles the keyboard inputs to handle the events.
    Key Events:
    - When user presses key "C", it changes the color index by increasing according to size of color vector.
    - When user presses key "H", it prints the control instructions in the terminal.
    - When user presses key "I", it sets the boolean value "setObjectLeftTop" to true for noticing the main loop to trigger the function "moveObjectToLeftTop"
    - When user presses the key "M", it activates the wire frame mode.
    - When user presses the key "D", the program sets the value "set3DmodeOn" to true, then assigns the velocity of z axis to handle 3D movement.
    - When user presses the key "Q", the program terminates.
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_C:
                currentColorIndex = (currentColorIndex + 1) % colors.size();
                break;
            case GLFW_KEY_H:
                std::cout << "CONTROLS :\n"
                          << "c - Change the color of current object\n"
                          << "w - Enable wireframe (i.e., as lines) or solid mode\n"
                          << "Left Mouse Click - Change the type of the object (Sphere, Cube or Bunny)\n"
                          << "o - Initialize object position to top-left corner\n"
                          << "ı - Open Inspect Mode => USE ARROW KEYS TO ROTATE AROUND (On Turkish keyboard press 'ı')\n"
                          << "p - Switch between textures\n"
                          << "d - Set 3D Movement (ON / OFF)\n"
                          << "s - Toggle between Phong and Gouraud shading (only when textures are off)\n"
                          << "m - Toggle between metallic and plastic materials\n"
                          << "1 - Toggle ambient component\n"
                          << "2 - Toggle diffuse component\n"
                          << "3 - Toggle specular component\n"
                          << "k - Toogle Ambient, Diffuse and Specular ON\n"
                          << "l - Toggle light source fixed/moving\n"
                          << "h - Display this help message\n"
                          << "q - Quit the program\n";
                break;
            case GLFW_KEY_I:
                inspectMode = !inspectMode;
                std::cout << "Inspect Mode " << (inspectMode ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_O:
                setObjectLeftTop = true;
                std::cout << "Object moved to left-top corner.\n";
                break;
            case GLFW_KEY_W:
                wireframeMode = !wireframeMode;
                std::cout << "Wireframe Mode " << (wireframeMode ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_T:
                isTextureUse = !isTextureUse;
                usePhong = true;
                useAmbient = true;
                useDiffuse = true;
                useSpecular = true;
                lightFixed = true;
                std::cout << "Texture Mode " << (isTextureUse ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_M:
                isMetallic = !isMetallic;
                std::cout << "Material: " << (isMetallic ? "Metallic" : "Plastic") << std::endl;
                break;
            case GLFW_KEY_S:
                if (!isTextureUse) {
                    usePhong = !usePhong;
                    if (usePhong) {
                        std::cout << "Shading Mode ON: Current Shading => Phong (Ambient: "
                                  << (useAmbient ? "ON" : "OFF") << ", Diffuse: "
                                  << (useDiffuse ? "ON" : "OFF") << ", Specular: "
                                  << (useSpecular ? "ON" : "OFF") << ")\n";
                    } else {
                        std::cout << "Shading Mode ON: Current Shading => Gouraud (Ambient: "
                                  << (useAmbient ? "ON" : "OFF") << ", Diffuse: "
                                  << (useDiffuse ? "ON" : "OFF") << ", Specular: "
                                  << (useSpecular ? "ON" : "OFF") << ")\n";
                    }
                } else {
                    std::cout << "Turn off texture mode (Press T) to change shading mode.\n";
                }
                break;
            case GLFW_KEY_P:
                if (isTextureUse) {
                    activeTextureIndex = (activeTextureIndex + 1) % textures.size();
                    std::cout << "Active Texture Index: " << activeTextureIndex << std::endl;
                }
                break;
            case GLFW_KEY_D:
                set3DmodeOn = !set3DmodeOn;
                if (set3DmodeOn) {
                    objectVelocity.z = objectVelocityZ;
                } else {
                    objectVelocity.z = 0.0f;
                }
                std::cout << "3D Movement " << (set3DmodeOn ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_1:
                useAmbient = !useAmbient;
                std::cout << "Ambient Component " << (useAmbient ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_2:
                useDiffuse = !useDiffuse;
                std::cout << "Diffuse Component " << (useDiffuse ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_3:
                useSpecular = !useSpecular;
                std::cout << "Specular Component " << (useSpecular ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_L:
                lightFixed = !lightFixed;
                std::cout << "Light Source " << (lightFixed ? "Fixed" : "Moving with Object") << std::endl;
                break;
            case GLFW_KEY_K:
                useAmbient = true;
                useDiffuse = true;
                useSpecular = true;
                std::cout << "Ambient, Diffuseand Specular Components : ON " << std::endl;
                break;
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, true);
                std::cout << "Program Terminated.\n";
                break;
            case GLFW_KEY_LEFT:
                if (inspectMode) {
                    cameraAngleY -= rotationSpeed;
                }
                break;
            case GLFW_KEY_RIGHT:
                if (inspectMode) {
                    cameraAngleY += rotationSpeed;
                }
                break;
            case GLFW_KEY_UP:
                if (inspectMode) {
                    cameraAngleX -= rotationSpeed;
                }
                break;
            case GLFW_KEY_DOWN:
                if (inspectMode) {
                    cameraAngleX += rotationSpeed;
                }
                break;
        }
    }
}


/**
 This function handles mouse button inputs to switch between object types (sphere, bunny, cube)
 */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        currentObjectType = (currentObjectType + 1) % 3;
    }
}

/**
    This function updates the position of the object based on velocity and collision with room boundaries
    In main loop, it calls the function with min and max parameter which are called from room's constant min and max boundaries.
    Also, objectSize parameter decides the offset value to set the collision boundaries for better viual experience.
    If there is a collision, function change the velocity on the specific axis to opposite value.
 */
void updateObjectPosition(glm::vec3 min, glm::vec3 max, float objectSize) {
    float offset = objectSize * (0.75f);

    glm::vec3 minBounds = min + glm::vec3(offset);
    glm::vec3 maxBounds = max - glm::vec3(offset);
    
    objectPosition += objectVelocity;

    if (objectPosition.x <= minBounds.x || objectPosition.x >= maxBounds.x) {
        objectVelocity.x = -objectVelocity.x;
        objectPosition.x = glm::clamp(objectPosition.x, minBounds.x, maxBounds.x);
    }

    if (objectPosition.y <= minBounds.y || objectPosition.y >= maxBounds.y) {
        objectVelocity.y = -objectVelocity.y;
        objectPosition.y = glm::clamp(objectPosition.y, minBounds.y, maxBounds.y);
    }

    if (objectPosition.z <= minBounds.z || objectPosition.z >= maxBounds.z) {
        objectVelocity.z = -objectVelocity.z;
        objectPosition.z = glm::clamp(objectPosition.z, minBounds.z, maxBounds.z);
    }
}

/**
    This function resets the object's position to the top-left corner of the room by calculating the position with min and max bounds.
    In main we assign the paramaters min and max according to room's constant max and min boundaries.
 */
void moveObjectToLeftTop(glm::vec3 min, glm::vec3 max, float objectSize) {
    glm::vec3 minBounds = min;
    glm::vec3 maxBounds = max;

    glm::vec3 initialPosition = glm::vec3(minBounds.x, maxBounds.y - objectSize, minBounds.z);

    objectPosition = initialPosition;
}

