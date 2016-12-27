#include "general_shader.h"

GeneralShader::GeneralShader(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath) : vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath), geometryShaderPath(geometryShaderPath)
{
    GLint compileResult = 0, linkResult = 0;
    char infoLogMessage[1024];

    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    std::string geometryShaderSource;

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
        std::cout << "EROARE COMPILARE vertex shader[" << vertexShaderPath << "]"  << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

	fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentHandle, 1, fragmentShaderSourceP, fragmentLength);
    glCompileShader(fragmentHandle);

    glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(fragmentHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE fragment shader[" << fragmentShaderPath << "]" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    if(geometryShaderPath.size() > 0){
        geometryShaderSource = loadShader(geometryShaderPath);
        const GLchar* geometryShaderSourceP[1];
        GLint geometryLength[1];
        geometryShaderSourceP[0] = geometryShaderSource.c_str();
        geometryLength[0] = strlen(geometryShaderSourceP[0]);

        geometryHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryHandle, 1, geometryShaderSourceP, geometryLength);
        glCompileShader(geometryHandle);

        glGetShaderiv(geometryHandle, GL_COMPILE_STATUS, &compileResult);
        if(compileResult == GL_FALSE) {
            glGetShaderInfoLog(geometryHandle, 1024, NULL, infoLogMessage);
            std::cout << "EROARE COMPILARE geometry shader[" << geometryShaderPath << "]" << std::endl << "LOG=" << infoLogMessage << std::endl;
            exit(-1);
        }
	}

    program = glCreateProgram();

    glAttachShader(program, vertexHandle);
    glAttachShader(program, fragmentHandle);

    if(geometryShaderPath.size() > 0){
        glAttachShader(program, geometryHandle);
    }
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if(linkResult == GL_FALSE) {
		glGetProgramInfoLog(program, 1024, NULL, infoLogMessage);
        std::cout << "EROARE LINK program shader[" << vertexShaderPath << "]" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

    glUseProgram(program);

    getAllUniformLocations();

    glDetachShader(program, vertexHandle);
    glDetachShader(program, fragmentHandle);
    if(geometryShaderPath.size() > 0){
        glDetachShader(program, geometryHandle);
    }

    glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
	if(geometryShaderPath.size() > 0){
        glDeleteShader(geometryHandle);
	}
}

GeneralShader::GeneralShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : GeneralShader(vertexShaderPath, "", fragmentShaderPath){}


std::string GeneralShader::loadShader(const std::string& filename){
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

void GeneralShader::reloadShader(){
    glUseProgram(0);
    glDeleteProgram(program);

    std::string vertexShaderSource = loadShader(vertexShaderPath);
    std::string fragmentShaderSource = loadShader(fragmentShaderPath);

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

    GLuint vertexHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexHandle, 1, vertexShaderSourceP, vertexLength);
    glCompileShader(vertexHandle);

    glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &compileResult);
	if(compileResult == GL_FALSE) {
        glGetShaderInfoLog(vertexHandle, 1024, NULL, infoLogMessage);
        std::cout << "EROARE COMPILARE vertex shader" << std::endl << "LOG=" << infoLogMessage << std::endl;
        exit(-1);
	}

	GLuint fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
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

void GeneralShader::bind(){
    glUseProgram(program);
}

void GeneralShader::getAllUniformLocations(){
    projMatLocation = glGetUniformLocation(program, "projectionMatrix");
    modelMatLocation = glGetUniformLocation(program, "modelMatrix");
    viewMatLocation = glGetUniformLocation(program, "viewMatrix");
    colorVectorLocation = glGetUniformLocation(program, "colorVector");
}

void GeneralShader::loadViewMatrix(const glm::mat4& matrix){
    loadMatrix(viewMatLocation, matrix);
}

void GeneralShader::loadColor(glm::vec4 color){
    loadVector4(colorVectorLocation, color);
}

void GeneralShader::loadProjectionMatrix(const glm::mat4& matrix){
    loadMatrix(projMatLocation, matrix);
}

void GeneralShader::loadModelMatrix(const glm::mat4& matrix){
    loadMatrix(modelMatLocation, matrix);
}

void GeneralShader::loadMatrix(GLuint location, const glm::mat4& matrix){
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

void GeneralShader::loadVector4(GLuint location, glm::vec4 vect){
    glUniform4f(location, vect.x, vect.y, vect.z, vect.w);
}

void GeneralShader::loadVector2(GLuint location, glm::vec2 vect){
    glUniform2f(location, vect.x, vect.y);
}

void GeneralShader::loadInt(GLuint location, int x){
    glUniform1i(location, x);
}

void GeneralShader::loadFloat(GLuint location, float x){
    glUniform1f(location, x);
}

GLuint GeneralShader::getProgram(){
    return program;
}

GeneralShader::~GeneralShader()
{
    glUseProgram(0);
    glDeleteProgram(program);
}
