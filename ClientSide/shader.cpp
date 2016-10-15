#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path) : v_path(vertex_shader_path), f_path(fragment_shader_path)
{
    GLint compile_result = 0, link_result = 0;
    char info_log_message[1024];

    std::string vertex_shader_source;
    std::string fragment_shader_source;

    vertex_shader_source = load_shader(vertex_shader_path);
    fragment_shader_source = load_shader(fragment_shader_path);

    const GLchar* vertexSS[1];
    const GLchar* fragmentSS[1];
    GLint vertex_length[1];
    GLint fragment_length[1];

    vertexSS[0] = vertex_shader_source.c_str();
    fragmentSS[0] = fragment_shader_source.c_str();

    vertex_length[0] = strlen(vertexSS[0]);
    fragment_length[0] = strlen(fragmentSS[0]);

    vertex_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_handle, 1, vertexSS, vertex_length);
    glCompileShader(vertex_handle);

    glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &compile_result);
	if(compile_result == GL_FALSE) {
        glGetShaderInfoLog(vertex_handle, 1024, NULL, info_log_message);
        std::cout << "EROARE COMPILARE vertex shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-1);
	}

	fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_handle, 1, fragmentSS, fragment_length);
    glCompileShader(fragment_handle);

    glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &compile_result);
	if(compile_result == GL_FALSE) {
        glGetShaderInfoLog(fragment_handle, 1024, NULL, info_log_message);
        std::cout << "EROARE COMPILARE fragment shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-1);
	}

    program = glCreateProgram();

    glAttachShader(program, vertex_handle);
    glAttachShader(program, fragment_handle);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if(link_result == GL_FALSE) {
		glGetProgramInfoLog(program, 1024, NULL, info_log_message);
        std::cout << "EROARE LINK program shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-2);
	}

    glUseProgram(program);

    getAllUniformLocations();

    glDetachShader(program, vertex_handle);
    glDetachShader(program, fragment_handle);

    glDeleteShader(vertex_handle);
	glDeleteShader(fragment_handle);

}

void Shader::getAllUniformLocations(){
    //caut in shader-e uniformele respective
    projMat_location = glGetUniformLocation(program, "projection_matrix");
    modelMat_location = glGetUniformLocation(program, "model_matrix");
    viewMat_location = glGetUniformLocation(program, "view_matrix");
    colorVector_location = glGetUniformLocation(program, "color_vector");
    has_texture_location = glGetUniformLocation(program, "has_texture");
    glUniform1i(glGetUniformLocation(program, "texture_sampler"), 0);
}

void Shader::loadViewMatrix(glm::mat4 matrix){
    loadMatrix(viewMat_location, matrix);
}

void Shader::loadColor(glm::vec4 color){
    loadVector4(colorVector_location, color);
}

void Shader::loadInt(GLuint location, int var){
    glUniform1i(location, var);
}

void Shader::load_has_texture(int has_texture){
    loadInt(has_texture_location, has_texture);
}

void Shader::loadVector4(GLuint location, glm::vec4 vect){
    glUniform4f(location, vect.x, vect.y, vect.z, vect.w);
}

void Shader::loadVector3(GLuint location, glm::vec3 vect){
    glUniform3f(location, vect.x, vect.y, vect.z);
}

void Shader::loadProjectionMatrix(glm::mat4 matrix){
    loadMatrix(projMat_location, matrix);
}

void Shader::loadModelMatrix(glm::mat4 matrix){
    loadMatrix(modelMat_location, matrix);
}

void Shader::loadMatrix(GLuint location, glm::mat4 matrix){
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

std::string Shader::load_shader(const std::string& fileName){
    std::ifstream file;
    file.open(fileName.c_str());

    std::string output;
    std::string line;

    if(file.is_open()){
        while(file.good()){
            getline(file, line);
            output.append(line + "\n");
        }

    }else{
        std::cerr << "Unable to load shader: " << fileName << std::endl;

    }
    file.close();

    return output;
}

GLuint Shader::get_program(){
    return program;
}

void Shader::reload_shader(){
    glUseProgram(0);
    glDeleteProgram(program);

    std::string vertex_shader_source = load_shader(v_path);
    std::string fragment_shader_source = load_shader(f_path);

    GLint compile_result = 0, link_result = 0;
    char info_log_message[1024];

    const GLchar* vertexSS[1];
    const GLchar* fragmentSS[1];
    GLint vertex_length[1];
    GLint fragment_length[1];

    vertexSS[0] = vertex_shader_source.c_str();
    fragmentSS[0] = fragment_shader_source.c_str();

    vertex_length[0] = strlen(vertexSS[0]);
    fragment_length[0] = strlen(fragmentSS[0]);

    vertex_handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_handle, 1, vertexSS, vertex_length);
    glCompileShader(vertex_handle);

    glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &compile_result);
	if(compile_result == GL_FALSE) {
        glGetShaderInfoLog(vertex_handle, 1024, NULL, info_log_message);
        std::cout << "EROARE COMPILARE vertex shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-1);
	}

	fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_handle, 1, fragmentSS, fragment_length);
    glCompileShader(fragment_handle);

    glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &compile_result);
	if(compile_result == GL_FALSE) {
        glGetShaderInfoLog(fragment_handle, 1024, NULL, info_log_message);
        std::cout << "EROARE COMPILARE fragment shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-1);
	}

    program = glCreateProgram();

    glAttachShader(program, vertex_handle);
    glAttachShader(program, fragment_handle);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if(link_result == GL_FALSE) {
		glGetProgramInfoLog(program, 1024, NULL, info_log_message);
        std::cout << "EROARE LINK program shader" << std::endl << "LOG=" << info_log_message << std::endl;
        exit(-2);
	}

    glUseProgram(program);

    getAllUniformLocations();

    glDetachShader(program, vertex_handle);
    glDetachShader(program, fragment_handle);

    glDeleteShader(vertex_handle);
	glDeleteShader(fragment_handle);

	glUseProgram(program);
}

Shader::~Shader()
{
    glUseProgram(0);
    glDeleteProgram(program);
}

void Shader::bind(){
    glUseProgram(program);
}

void Shader::unbind(){
    glUseProgram(0);
}
