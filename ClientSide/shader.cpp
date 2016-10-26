#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath)
{
    GLint compileResult = 0, linkResult = 0;
    char infoLogMessage[1024];

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    vertexShaderSource = loadShader(vertexShaderPath);
    fragmentShaderSource = loadShader(fragmentShaderPath);

    const GLchar* vertexShaderSourceP[1];
    const GLchar* fragmentShaderSourceP[1];
    GLint vertexLength[1];
    GLint fragmentLength[1];

    vertexShaderSourceP[0] = vertexShaderSource.c_str();
    fragmentShaderSourceP[0] = fragmentShaderSource.c_str();

    vertexLength[0] = strlen(vertexShaderSourceP[0]);
    fragmentLength[0] = strlen(fragmentShaderSourceP[0]);

    vertexHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexHandle, 1, vertexShaderSourceP, vertexLength);
    glCompileShader(vertexHandle);

    glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(vertexHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE vertex shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

	fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentHandle, 1, fragmentShaderSourceP, fragmentLength);
    glCompileShader(fragmentHandle);

    glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(fragmentHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE fragment shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    program = glCreateProgram();

    glAttachShader(program, vertexHandle);
    glAttachShader(program, fragmentHandle);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if(linkResult == GL_FALSE) {
		glGetProgramInfoLog(program, 1024, NULL, infoLogMessage);
        std::cout << "EROARE LINK program shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    glUseProgram(program);

    getAllUniformLocations();

    glDetachShader(program, vertexHandle);
    glDetachShader(program, fragmentHandle);

    glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);

}

void Shader::getAllUniformLocations(){
    //caut in shader-e uniformele respective
    projMatLocation = glGetUniformLocation(program, "projectionMatrix");
    modelMatLocation = glGetUniformLocation(program, "modelMatrix");
    viewMatLocation = glGetUniformLocation(program, "viewMatrix");
    colorVectorLocation = glGetUniformLocation(program, "colorVector");
    hasTextureLocation = glGetUniformLocation(program, "hasTexture");
    glUniform1i(glGetUniformLocation(program, "textureSampler"), 0);
}

void Shader::loadViewMatrix(glm::mat4 matrix){
    loadMatrix(viewMatLocation, matrix);
}

void Shader::loadColor(glm::vec4 color){
    loadVector4(colorVectorLocation, color);
}

void Shader::loadInt(GLuint location, int var){
    glUniform1i(location, var);
}

void Shader::loadHasTexture(int has_texture){
    loadInt(hasTextureLocation, has_texture);
}

void Shader::loadVector4(GLuint location, glm::vec4 vect){
    glUniform4f(location, vect.x, vect.y, vect.z, vect.w);
}

void Shader::loadVector3(GLuint location, glm::vec3 vect){
    glUniform3f(location, vect.x, vect.y, vect.z);
}

void Shader::loadProjectionMatrix(glm::mat4 matrix){
    loadMatrix(projMatLocation, matrix);
}

void Shader::loadModelMatrix(glm::mat4 matrix){
    loadMatrix(modelMatLocation, matrix);
}

void Shader::loadMatrix(GLuint location, glm::mat4 matrix){
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

std::string Shader::loadShader(const std::string& filename){
    std::ifstream file;
    file.open(filename.c_str());

    std::string output;
    std::string line;

    if(file.is_open()){
        while(file.good()){
            getline(file, line);
            output.append(line + "\n");
        }

    }else{
        std::cerr << "Unable to load shader: " << filename << std::endl;

    }
    file.close();

    return output;
}

GLuint Shader::getProgram(){
    return program;
}

void Shader::reloadShader(){
    glUseProgram(0);
    glDeleteProgram(program);

    std::string vertexShaderSource = loadShader(vertexShaderPath);
    std::string fragmentShaderSource = loadShader(f_path);

    GLint compileResult = 0, linkResult = 0;
    char infoLogMessage[1024];

    const GLchar* vertexShaderSourceP[1];
    const GLchar* fragmentShaderSourceP[1];
    GLint vertexLength[1];
    GLint fragmentLength[1];

    vertexShaderSourceP[0] = vertexShaderSource.c_str();
    fragmentShaderSourceP[0] = fragmentShaderSource.c_str();

    vertexLength[0] = strlen(vertexShaderSourceP[0]);
    fragmentLength[0] = strlen(fragmentShaderSourceP[0]);

    vertexHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexHandle, 1, vertexShaderSourceP, vertexLength);
    glCompileShader(vertexHandle);

    glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(vertexHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE vertex shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

	fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentHandle, 1, fragmentShaderSourceP, fragmentLength);
    glCompileShader(fragmentHandle);

    glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(fragmentHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE fragment shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    program = glCreateProgram();

    glAttachShader(program, vertexHandle);
    glAttachShader(program, fragmentHandle);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if(linkResult == GL_FALSE) {
		glGetProgramInfoLog(program, 1024, NULL, infoLogMessage);
        std::cout << "EROARE LINK program shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    glUseProgram(program);

    getAllUniformLocations();

    glDetachShader(program, vertexHandle);
    glDetachShader(program, fragmentHandle);

    glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);

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
