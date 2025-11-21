#include <iostream>
#include <exception>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// unsigned int indices[] = {  
//         0, 1, 3, // first triangle
//         1, 2, 3  // second triangle
// };

Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

class App
{
private:
        GLFWwindow* window;

        uint32_t VBO, VAO, EBO, cubeVAO;
        uint32_t texture;
        uint32_t lightVAO;
        std::unique_ptr<Shader> shader;
        std::unique_ptr<Shader> lightCubeShader;

        glm::vec3 lightPos{1.2f, 1.0f, 2.0f};

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        

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
                        std::cout << "Failed to create GLFW window" << std::endl;
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
                        std::cout << "Failed to initialize GLAD" << std::endl;
                        return false;
                }
                
                glEnable(GL_DEPTH_TEST);
                return true;
        }

        void createShader() {
                shader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
                lightCubeShader = std::make_unique<Shader>("shaders/lightShader.vs", "shaders/lightShader.fs");
        }

        void loadAssets() {
        }
        
        void createTexture() {
                unsigned char *data = stbi_load("resources/textures/wall.jpg", &texwidth, &texheight, &texnrChannels, 0);
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                if (!data) {
                        throw std::runtime_error("Failed to load image");
                }
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(data);
        }

        // split the buffer attributes later.
        void setupBuffers() {

                glGenVertexArrays(1, &cubeVAO);
                glGenBuffers(1, &VBO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindVertexArray(cubeVAO);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glGenVertexArrays(1, &lightVAO);
                glBindVertexArray(lightVAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                                                                /** stride */ 
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
        }

        void wireframeMode() {
                if (wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
        }

        void processInput()
        {
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

        void render() {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // start rendering here
                shader->use();
                shader->setVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
                shader->setVec3("material.diffuse", 0.61424f, 0.04136 ,0.04136f);
                shader->setVec3("material.specular", 0.727811f, 0.626959f, 0.626959f);
                shader->setFloat("material.shininess", 0.6f * 128.0f);

                shader->setVec3("light.ambient",  glm::vec3(1.0f));
                shader->setVec3("light.diffuse",  glm::vec3(1.0f)); // darken diffuse light a bit
                shader->setVec3("light.specular", glm::vec3(1.0f)); 

                shader->setVec3("lightPos", lightPos);

                glm::mat4 projection = glm::perspective(
                        glm::radians(camera.Zoom),
                        (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                        0.1f,
                        100.0f
                );

                glm::mat4 view = camera.GetViewMatrix();

                shader->setMat4("projection", projection);
                shader->setMat4("view", view);

                glm::mat4 model = glm::mat4(1.0f);
                shader->setMat4("model", model);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                lightCubeShader->use();
                lightCubeShader->setMat4("projection", projection);
                lightCubeShader->setMat4("view", view);

                model = glm::mat4(1.0f);
                model = glm::translate(model, lightPos);
                model = glm::scale(model, glm::vec3(0.2f));

                lightCubeShader->setMat4("model", model);

                glBindVertexArray(lightVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
        }
public:

        void init() {
                inititializeOpenGL();
                if (!initWindow()) {
                        throw std::runtime_error("Window creation Failed!\n");
                }

                setupBuffers();
                // createTexture();
                wireframe = false;
                wireframeMode();
                createShader();
        }

        void clear() {
                glDeleteVertexArrays(1, &cubeVAO);
                glDeleteVertexArrays(1, &lightVAO);
                glDeleteBuffers(1, &VBO);
        }

        void run() {
                while(!glfwWindowShouldClose(window))
                {
                        float currentFrame = static_cast<float>(glfwGetTime());
                        deltaTime = currentFrame - lastFrame;
                        lastFrame = currentFrame;

                        processInput();

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