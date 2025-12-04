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

static bool emissionEnabled = false;

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

        unsigned int diffuseMap;
        unsigned int specularMap;
        unsigned int emissionMap;

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
                diffuseMap = loadTexture("resources/textures/container2.png");
                specularMap = loadTexture("resources/textures/container2_specular.png");
                emissionMap = loadTexture("resources/textures/matrix.jpg");
        }
        
        unsigned int loadTexture(char const* path) {
                stbi_set_flip_vertically_on_load(true);

                int width, height, nrComponents;
                unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

                if (!data) {
                        std::string msg = std::string("Failed to load image: ") + path;
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

                glGenVertexArrays(1, &cubeVAO);
                glGenBuffers(1, &VBO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindVertexArray(cubeVAO);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // texcoords vectices
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glGenVertexArrays(1, &lightVAO);
                glBindVertexArray(lightVAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                                                                /** stride */ 
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
        }

        void wireframeMode() {
                if (wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
        }

        void processInput()
        {
                // toggle for enabling and desabling emmision map
                if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                        emissionEnabled = !emissionEnabled;
                        if (shader != nullptr) {
                                shader->use();
                                shader->setBool("useEmission", emissionEnabled);
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

        void render() {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                // actual rendering and uniform  buffers
                shader->use();
                shader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
                shader->setVec3("light.position", lightPos);
                shader->setVec3("viewPos", camera.Position);


                // light properties
                shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
                shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
                shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

                shader->setFloat("light.constant",  1.0f);
                shader->setFloat("light.linear",    0.09f);
                shader->setFloat("light.quadratic", 0.032f);	

                // material properties
                shader->setFloat("material.shininess", 64.0f);
                shader->setFloat("time", glfwGetTime());


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

                  // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, emissionMap);

                glBindVertexArray(cubeVAO);
                for(unsigned int i = 0; i < 10; i++)
                {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, cubePositions[i]);
                        float angle = 20.0f * i;
                        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                        shader->setMat4("model", model);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                }

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
                loadAssets();
                
                wireframe = false;
                wireframeMode();
                createShader();

                shader->use();
                shader->setInt("material.diffuse", 0);
                shader->setInt("material.specular", 1);
                shader->setInt("material.emission", 2);

                shader->setBool("useEmission", emissionEnabled);
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