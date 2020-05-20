#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>

std::string get_shader_string(const char *shader_file_name) {
    std::string shader_path;
    shader_path += "../";
    shader_path += shader_file_name;
    std::ifstream stream(shader_path);
    if (!stream.is_open())
        std::cerr << "Failed to open shader file: " << shader_path;
    std::string shader_source;
    std::string tp;
    while (std::getline(stream, tp)) {
        shader_source += tp + '\n';
    }
    stream.close();
    return shader_source;
}

int main() {

    // Boring initialization stuff //
    GLFWwindow *window = nullptr;
    if (!glfwInit())
        std::cerr << "Error initializing glfw";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    window = glfwCreateWindow(800, 600, "Path Tracer", NULL, NULL);

    if (window == nullptr)
        std::cerr << "Error creating glfw window";

    glfwMakeContextCurrent(window);
    if (GLEW_OK != glewInit())
        std::cerr << "Error initializing glew";
    // Okay "boring" stuff is over with, now we do shader stuff //

    // Lets do shader stuff shall we? (somewhat less boring, but still the same thing over and over again bleh) //
    std::string string_vert_shader_source = get_shader_string("shader.vert");
    const char *vertex_shader_source = string_vert_shader_source.c_str();
    unsigned int vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_handle, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader_handle);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader_handle, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    std::string string_frag_shader_source = get_shader_string("shader.frag");
    const char *fragment_shader_source = string_frag_shader_source.c_str();
    unsigned int fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_handle, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader_handle);

    glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader_handle, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader_handle);
    glAttachShader(shader_program, fragment_shader_handle);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader_handle);
    glDeleteShader(fragment_shader_handle);
    // Phew, shader stuff is done and over with (for now, until I add a compute shader to the mix) And now to work on Vertex buffers
    // going to change sooner or later so I have custom buffer objects for scene information and such, but that's for future me to figure out

    // boring vertex stuff //
    float vertices[] =
            {
                    -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    0.0f, 0.5f, 0.0f
            };


    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glUseProgram(shader_program);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // end of boring vertex stuff //


    // Main rendering loop //
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        // Main rendering loop
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
