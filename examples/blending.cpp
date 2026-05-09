/*
 * File: blending.cpp
 * Created: 2026-05-09
 * Description: Blending implementation in opengl for windows texture with alpha channel
* 
* MIT License
* 
* Copyright (c) 2026 Aditya Yadav
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*/




#include <iostream>
#include <exception>
#include <vector>
#include <chrono>
#include <memory>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "logger.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static bool emissionEnabled = false;
#define NR_POINT_LIGHTS 4

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};  

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float planeVertices[] = {
    // positions            // normals       // texcoords
     5.0f, -0.5f,  5.0f,    0,1,0,          2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,    0,1,0,          0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,    0,1,0,          0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,    0,1,0,          2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,    0,1,0,          0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,    0,1,0,          2.0f, 2.0f
};

float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

// transparent window locations
// --------------------------------
std::vector<glm::vec3> windows
{
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3( 0.5f, 0.0f, -0.6f)
};


// unsigned int indices[] = {  
//         0, 1, 3, // first triangle
//         1, 2, 3  // second triangle
// };

float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };



Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

class Timer
{
public:
        using clock = std::chrono::steady_clock;

        void BeginFrame()
        {
                auto now = clock::now();

                m_DeltaTime =
                std::chrono::duration<float>(now - m_LastFrame).count();

                m_LastFrame = now;
        }

        float GetDeltaTime() const
        {
                return m_DeltaTime;
        }

        float GetFPS() const
        {
                return 1.0f / m_DeltaTime;
        }

private:
        std::chrono::time_point<clock> m_LastFrame = clock::now();
        float m_DeltaTime = 0.0f;
};

// Passes and Render Draw Calls
struct OutlineDrawCall {
    uint32_t    vao;
    int         vertexCount;
    glm::mat4   transform;
    glm::vec3   color;       // outline tint, passed to stencilShader
};


class App
{
private:
        GLFWwindow* window;
        Timer timer;

        uint32_t VBO, VAO, EBO, cubeVAO;
        uint32_t lightVAO;
        uint32_t skyboxVAO, skyboxVBO;
        uint32_t planeVAO, planeVBO;
        uint32_t transparentVAO, transparentVBO;
        
        uint32_t texture;
        uint32_t cubeMapTexture;
        uint32_t floorTexture;
        uint32_t transparentTexture;

        std::unique_ptr<Shader> modelShader;
        std::unique_ptr<Shader> stencilShader;
        std::unique_ptr<Shader> cubeShader;
        std::unique_ptr<Shader> lightShader;
        std::unique_ptr<Shader> cubeMapShader;
        std::unique_ptr<Shader> blendShader;

        std::unique_ptr<Model> backpackScene;

        // std::vector<PointLight> pointLights;

        /// Uniform Declarations
        unsigned int diffuseMap;
        unsigned int specularMap;
        unsigned int emissionMap;
        glm::mat4 projection;
        glm::mat4 view;

        // RenderPass queues
        std::vector<OutlineDrawCall> outlineQueue;
        std::map<float, glm::vec3> sorted;

        glm::vec3 lightPos{1.2f, 1.0f, 2.0f};

        int texwidth, texheight, texnrChannels;

        bool wireframe = false;

        void inititializeOpenGL() {
                glfwInit();
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        }

        bool initWindow() {
                window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
                if (window == NULL)
                {
                        Logger::Fatal("Failed to create GLFW window");
                        glfwTerminate();
                        return false;
                }
                glfwMakeContextCurrent(window);
                // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
                glfwSetCursorPosCallback(window, mouse_callback);
                glfwSetScrollCallback(window, scroll_callback);

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                        Logger::Fatal("Failed to initialize GLAD");
                        return false;
                }
                
                return true;
        }

        void enableFeatures() {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);

                /** Formula for blending is (C = Csrc * Fsrc + Cdest * Fdest) where is Csrc and Cdest is color of source and destination
                 * Fsrc and Fdest are the impact values of alpha channel on the color vectors.
                 * Can be simplified to C = Csrc * alpha + Cdest * (1 - alpha)
                 */
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // setting the state of blending
                // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); // setting the state for RGB channels and alpha seperately
                // glBlendEquation(GL_FUNC_ADD); // selecting equation: GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX, default is ADD
                /** Stencil test is turned off until I develop a renderpass structure */
                // glEnable(GL_STENCIL_TEST);
                // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }

        void createShader() {
                // modelShader = std::make_unique<Shader>("shaders/modelShader.vs", "shaders/modelShader.fs");
                cubeShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
                lightShader = std::make_unique<Shader>("shaders/lightShader.vs", "shaders/lightShader.fs");
                cubeMapShader = std::make_unique<Shader>("shaders/cubemap.vs", "shaders/cubemap.fs");
                stencilShader = std::make_unique<Shader>("shaders/stencilShader.vs", "shaders/stencilShader.fs");
                blendShader = std::make_unique<Shader>("shaders/blending.vs", "shaders/blending.fs");
        }


        void loadAssets() {
                stbi_set_flip_vertically_on_load(true);
                // backpackScene = std::make_unique<Model>("resources/models/backpack/backpack.obj");
                transparentTexture = loadTexture("resources/textures/window.png");
                floorTexture = loadTexture("resources/textures/metal.png");
                
                diffuseMap = loadTexture("resources/textures/container2.png");
                specularMap = loadTexture("resources/textures/container2_specular.png");
                emissionMap = loadTexture("resources/textures/matrix.jpg");
                std::vector<std::string> faces
                {
                        "resources/textures/skybox/right.jpg",
                        "resources/textures/skybox/left.jpg",
                        "resources/textures/skybox/top.jpg",
                        "resources/textures/skybox/bottom.jpg",
                        "resources/textures/skybox/front.jpg",
                        "resources/textures/skybox/back.jpg"
                };

                cubeMapTexture = loadCubeMap(faces);
        }

        unsigned int loadCubeMap(std::vector<std::string> faces) {
                unsigned int textureID;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

                int width, height, nrChannels;
                stbi_set_flip_vertically_on_load(false);
                for (unsigned int i = 0; i < faces.size(); i++)
                {
                        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                        if (data)
                        {
                                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                                stbi_image_free(data);
                        }
                        else
                        {
                                Logger::Error("Cubemap tex failed to load at path: {}", faces[i]);
                                stbi_image_free(data);
                        }
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                return textureID;
        }
        
        unsigned int loadTexture(char const* path) {
                stbi_set_flip_vertically_on_load(true);

                int width, height, nrComponents;
                unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

                if (!data) {
                        std::string msg = std::string("Failed to load image: ") + path;
                        Logger::Error(msg);
                        throw std::runtime_error(msg);
                }

                unsigned int textureID = 0;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                GLenum format = GL_RGB;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(data);
                return textureID;
        }

        // split the buffer attributes later.
        void setupBuffers() {

                /** CUBE BUFFER */
                {
                        glGenVertexArrays(1, &cubeVAO);
                        glGenBuffers(1, &VBO);

                        glBindVertexArray(cubeVAO);

                        glBindBuffer(GL_ARRAY_BUFFER, VBO);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                        // position attribute
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);
                        // normal attribute
                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                        glEnableVertexAttribArray(1);
                        // texcoords vectices
                        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                        glEnableVertexAttribArray(2);

                        glBindVertexArray(0);
                }

                /** LIGHT CUBE BUFFER */
                {

                        glGenVertexArrays(1, &lightVAO);
                        glBindVertexArray(lightVAO);
                        
                        glBindBuffer(GL_ARRAY_BUFFER, VBO);
                        /** stride */ 
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);
                        
                        glBindVertexArray(0);
                }

                /** FLOOR BUFFER */
                {
                        glGenVertexArrays(1, &planeVAO);
                        glGenBuffers(1, &planeVBO);
                        glBindVertexArray(planeVAO);

                        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
                        
                        glEnableVertexAttribArray(0);
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                        
                        glEnableVertexAttribArray(1);
                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
                        
                        glEnableVertexAttribArray(2);
                        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

                        glBindVertexArray(0);
                }

                /** WINDOW BUFFER */
                {
                        glGenVertexArrays(1, &transparentVAO);
                        glGenBuffers(1, &transparentVBO);
                        glBindVertexArray(transparentVAO);

                        glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);

                        // positions (location = 0)
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);

                        // texcoords (location = 1)
                        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
                        glEnableVertexAttribArray(1);

                        glBindVertexArray(0);
                }

                /** SKYBOX BUFFER */
                {

                        glGenVertexArrays(1, &skyboxVAO);
                        glGenBuffers(1, &skyboxVBO);
                        glBindVertexArray(skyboxVAO);

                        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);
                        
                        glBindVertexArray(0);
                }

        }

        void wireframeMode() {
                if (wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
        }

        void processInput(float deltaTime)
        {
                // toggle for enabling and desabling emmision map
                if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                        emissionEnabled = !emissionEnabled;
                        if (cubeShader != nullptr) {
                                cubeShader->use();
                                cubeShader->setBool("useEmission", emissionEnabled);
                        }
                }
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, true);

                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                        camera.ProcessKeyboard(FORWARD, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                        camera.ProcessKeyboard(BACKWARD, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                        camera.ProcessKeyboard(LEFT, deltaTime);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                        camera.ProcessKeyboard(RIGHT, deltaTime);
        }

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
                glViewport(0, 0, width, height);
        }

        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
                
                float xpos = static_cast<float>(xposIn);
                float ypos = static_cast<float>(yposIn);

                if (firstMouse)
                {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                }

                float xoffset = xpos - lastX;
                float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                lastX = xpos;
                lastY = ypos;

                camera.ProcessMouseMovement(xoffset, yoffset);
        }

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
                camera.ProcessMouseScroll(static_cast<float>(yoffset));
        }

        /** @brief Function for holding lighting uniforms */
        void drawLightUniforms(Shader* shader) {
                shader->use();
                shader->setVec3("viewPos", camera.Position);
                shader->setMat4("projection", projection);
                shader->setMat4("view", view);
                shader->setFloat("material.shininess", 64.0f);
                shader->setFloat("time", (float)glfwGetTime());

                shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                shader->setVec3("dirLight.ambient",    0.05f, 0.05f, 0.05f);
                shader->setVec3("dirLight.diffuse",    0.4f,  0.4f,  0.4f);
                shader->setVec3("dirLight.specular",   0.5f,  0.5f,  0.5f);


                for (int i = 0; i < 4; i++) {
                        std::string base = "pointLights[" + std::to_string(i) + "].";
                        shader->setVec3((base + "position").c_str(),  pointLightPositions[i]);
                        shader->setVec3((base + "ambient").c_str(),   0.05f, 0.05f, 0.05f);
                        shader->setVec3((base + "diffuse").c_str(),   0.8f,  0.8f,  0.8f);
                        shader->setVec3((base + "specular").c_str(),  1.0f,  1.0f,  1.0f);
                        shader->setFloat((base + "constant").c_str(),  1.0f);
                        shader->setFloat((base + "linear").c_str(),    0.09f);
                        shader->setFloat((base + "quadratic").c_str(), 0.032f);
                }

                shader->setVec3("spotLight.position",   camera.Position);
                shader->setVec3("spotLight.direction",  camera.Front);
                shader->setVec3("spotLight.ambient",    0.0f, 0.0f, 0.0f);
                shader->setVec3("spotLight.diffuse",    1.0f, 1.0f, 1.0f);
                shader->setVec3("spotLight.specular",   1.0f, 1.0f, 1.0f);
                shader->setFloat("spotLight.constant",  1.0f);
                shader->setFloat("spotLight.linear",    0.09f);
                shader->setFloat("spotLight.quadratic", 0.032f);
                shader->setFloat("spotLight.cutOff",    glm::cos(glm::radians(12.5f)));
                shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        }

        /** @brief Draw the cubes example from learnopengl.com */
        void drawCubes(Shader* shader) {
                // material properties
                drawLightUniforms(shader);

                // bind diffuse map /// TODO: these need to be set from the runtime or something
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, emissionMap);

                glBindVertexArray(cubeVAO);

                // Drawing only two cubes for now
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
                shader->setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                shader->setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                // for(unsigned int i = 0; i < 10; i++)
                // {
                //         glm::mat4 model = glm::mat4(1.0f);
                //         model = glm::translate(model, cubePositions[i]);
                //         float angle = 20.0f * i;
                //         model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                //         shader->setMat4("model", model);

                //         glDrawArrays(GL_TRIANGLES, 0, 36);
                // }
        }

        /** @brief Draw floor for testing */
        void drawFloor(Shader* shader) {
                shader->use();
                glBindVertexArray(planeVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, floorTexture);
                shader->setMat4("model", glm::mat4(1.0f));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
        }

        /** @brief Draw point lights for testing lighting */
        void drawPointLights() {

                lightShader->use();
                lightShader->setMat4("projection", projection);
                lightShader->setMat4("view", view);

                glBindVertexArray(lightVAO);
                for (size_t i = 0; i < 4; i++) {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, pointLightPositions[i]);
                        model = glm::scale(model, glm::vec3(0.2f));

                        lightShader->setMat4("model", model);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                glBindVertexArray(0);
        }

        void drawModel(Model* scene) {
                modelShader->use();
                modelShader->setMat4("view", view);
                modelShader->setMat4("projection", projection);
                modelShader->setVec3("viewPos", camera.Position);

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
                modelShader->setMat4("model", model);
                scene->Draw(*modelShader);
        }

        void drawCubeMap() {
                glDepthFunc(GL_LEQUAL);
                cubeMapShader->use();
                cubeMapShader->setMat4("view", glm::mat4(glm::mat3(view)));
                cubeMapShader->setMat4("projection", projection);
                
                // skybox cube
                glBindVertexArray(skyboxVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);

                // Restore depth state for the next frame
                glDepthFunc(GL_LESS); // GL_LESS
        }
        
        /** draw call for outlines */
        void drawWithOutline(
                Shader*           shader,
                uint32_t          vao,
                int               vertexCount,
                const glm::mat4&  transform,
                const glm::vec3&  outlineColor = glm::vec3(1.0f)  // 0.04, 0.28, 0.26
        ) {
                // Pass 1: normal draw into stencil
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);

                shader->use();
                shader->setMat4("model", transform);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, vertexCount);

                // Enqueue pass 2
                outlineQueue.push_back({ vao, vertexCount, transform, outlineColor });
        }

        // flushes outline draw calls (call this at the end)
        void flushOutlinePass(float scale = 1.05f) {
                if (outlineQueue.empty()) return;

                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                // glDisable(GL_DEPTH_TEST);

                stencilShader->use();
                stencilShader->setMat4("view", view);
                stencilShader->setMat4("projection", projection);

                for (const auto& call : outlineQueue) {
                        glm::mat4 scaledModel = glm::scale(call.transform, glm::vec3(scale));

                        stencilShader->setMat4("model", scaledModel);
                        stencilShader->setVec3("outlineColor", call.color);  // wire this up in the fs

                        glBindVertexArray(call.vao);
                        glDrawArrays(GL_TRIANGLES, 0, call.vertexCount);
                }

                outlineQueue.clear();

                // Restore state
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
                // glEnable(GL_DEPTH_TEST);
        }

        void render() {
                for (uint8_t i = 0; i < windows.size(); i++) {
                        float distance = glm::length(camera.Position - windows[i]);
                        sorted[distance] = windows[i];
                }


                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                projection = glm::perspective(
                        glm::radians(camera.Zoom),
                        (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                        0.1f,
                        100.0f
                );
                
                view = camera.GetViewMatrix();
                glm::mat4 model = glm::mat4(1.0f);

                cubeShader->use();
                cubeShader->setVec3("viewPos", camera.Position);
                cubeShader->setMat4("projection", projection);
                cubeShader->setMat4("view", view);
                // draw normal stencil test
                drawFloor(cubeShader.get());
                drawPointLights();
                // drawModel(backpackScene.get());
                
                drawLightUniforms(cubeShader.get());
                drawCubes(cubeShader.get());

                blendShader->use();
                blendShader->setMat4("projection", projection);
                blendShader->setMat4("view", view);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, transparentTexture);
                
                glBindVertexArray(transparentVAO);
                for (auto it = sorted.rbegin(); it != sorted.rend(); it++) {
                        model = glm::mat4(1.0f);
                        model = glm::translate(model, it->second);
                        blendShader->setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 6);
                }

                drawCubeMap();
        }
public:

        void init() {
                Logger::Init();
                inititializeOpenGL();
                if (!initWindow()) {
                        throw std::runtime_error("Window creation Failed!\n");
                }

                enableFeatures();
                setupBuffers();
                loadAssets();
                
                wireframe = false;
                // wireframeMode();
                createShader();

                cubeShader->use();
                cubeShader->setInt("material.diffuse", 0);
                cubeShader->setInt("material.specular", 1);
                cubeShader->setInt("material.emission", 2);
                cubeShader->setBool("useEmission", emissionEnabled);

                cubeMapShader->use();
                cubeMapShader->setInt("skybox", 0);
                
                blendShader->use();
                blendShader->setInt("texture1", 0);


        }

        void clear() {
                glDeleteVertexArrays(1, &cubeVAO);
                glDeleteVertexArrays(1, &skyboxVAO);
                glDeleteVertexArrays(1, &lightVAO);
                glDeleteVertexArrays(1, &planeVAO);
                glDeleteVertexArrays(1, &transparentVAO);
                glDeleteBuffers(1, &planeVBO);
                glDeleteBuffers(1, &transparentVBO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &skyboxVBO);
        }

        void run() {
                while(!glfwWindowShouldClose(window))
                {
                        timer.BeginFrame();
                        float dt = timer.GetDeltaTime();
                        float fps = timer.GetFPS();

                        processInput(dt);

                        Logger::Info("dt: {:.4f} ms | fps: {:.2f}", dt * 1000.0f, fps);

                        render();

                        glfwSwapBuffers(window);
                        glfwPollEvents();    
                }

                glfwTerminate();
        }
};

int main()
{
        App app;
        app.init();
        app.run();
        app.clear();
        return 0;
}