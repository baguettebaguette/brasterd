#include "Window.h"


const char *vertex_shader_src = R"glsl(
    #version 330 core

    layout (location = 0) in vec3 aPos;

    out vec2 uv;

    void main() {
        uv = vec2(aPos.xy * 0.5 + 0.5);
        gl_Position = vec4(aPos, 1.0);
    }
)glsl";

const char *fragment_shader_src = R"glsl(
    #version 330 core

    in vec2 uv;

    uniform sampler2D tex;

    out vec4 color;

    void main() {
        color = texture(tex, uv);
    }
)glsl";

brasterd::Window::Window(std::string title, glm::ivec2 resolution) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(resolution.x, resolution.y, title.c_str(), nullptr, nullptr);
    window_size = resolution;
    glfwMakeContextCurrent(window);
    gladLoadGL();

    display_buffer = nullptr;

    float data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &rect_vao);
    glBindVertexArray(rect_vao);
    glGenBuffers(1, &rect_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    GLuint vertex_shader = compile(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_shader_src);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    tex_location = glGetUniformLocation(program, "tex");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // No AA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void brasterd::Window::set_display_buffer(Buffer2D<3, unsigned char> &buffer) {
    display_buffer = &buffer;
}

brasterd::Window::~Window() {
    glDeleteBuffers(1, &rect_vbo);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &rect_vao);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
}

bool brasterd::Window::should_close() {
    return glfwWindowShouldClose(window);
}

void brasterd::Window::update() {
    if (display_buffer == nullptr) {
        return;
    }

    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
        display_buffer->size().x,
        display_buffer->size().y,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        display_buffer->buffer);
    glUniform1i(tex_location, 0);
    glBindVertexArray(rect_vao);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
}

void brasterd::Window::poll_events() {
    glfwPollEvents();
}

glm::ivec2 brasterd::Window::size() {
    return window_size;
}

float brasterd::Window::clock() {
    return (float) glfwGetTime();
}

GLuint brasterd::Window::compile(GLuint shader_type, const char* src) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    char log[512] = { 0 };
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    std::cout << "Shader info log: " << log << std::endl;
    return shader;
}
