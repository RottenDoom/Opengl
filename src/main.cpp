#include <iostream>
#include <exception>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class App
{
private:
        GLFWwindow* window;
        std::vector<float> vertices = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };
        uint32_t VBO, VAO;
        uint32_t vertexShader;
        uint32_t fragmentShader;
        uint32_t shaderProgram;

        const char *vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";

        const char *fragmentShaderSource = "#version 330 core\n"
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                "}\0";

        void inititializeOpenGL() {
                glfwInit();
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        }

        bool initWindow() {
                window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
                if (window == NULL)
                {
                        std::cout << "Failed to create GLFW window" << std::endl;
                        glfwTerminate();
                        return false;
                }
                glfwMakeContextCurrent(window);

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                        std::cout << "Failed to initialize GLAD" << std::endl;
                        return false;
                }
                
                glViewport(0, 0, 800, 600);
                glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
                return true;
        }

        void createVertexShader() {
                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
                glCompileShader(vertexShader);

                int success;
                char infoLog[512];
                glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

                if (!success) {
                        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                }
        }

        void createFragmentShader() {
                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
                glCompileShader(fragmentShader);

                int success;
                char infoLog[512];
                glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

                if (!success) {
                        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                }
        }

        void createShader() {
                shaderProgram = glCreateProgram();
                glAttachShader(shaderProgram, vertexShader);
                glAttachShader(shaderProgram, fragmentShader);
                glLinkProgram(shaderProgram);

                int success;
                char infoLog[512];
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                if(!success) {
                        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                }
                
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
        }

        // split the buffer attributes later.
        void setupBuffers() {
                // Create and bind VAO
                glGenVertexArrays(1, &VAO);
                glBindVertexArray(VAO);

                // Create and bind VBO
                glGenBuffers(1, &VBO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

                // Vertex attribute (position, location = 0)
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                // Optional: unbind to avoid accidental modification
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
        }

        void processInput()
        {
                if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, true);
        }

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
                glViewport(0, 0, width, height);
        }

        void render() {

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glUseProgram(shaderProgram);
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
        }
public:

        void init() {
                inititializeOpenGL();
                if (!initWindow()) {
                        throw std::runtime_error("Window creation Failed!\n");
                }

                createVertexShader();
                createFragmentShader();
                createShader();
                setupBuffers();
        }

        void clear() {
                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteProgram(shaderProgram);
        }

        void run() {
                while(!glfwWindowShouldClose(window))
                {
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