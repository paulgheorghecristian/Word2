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
    lightPosition_location = glGetUniformLocation(program, "light_position");
    eyePosition_location = glGetUniformLocation(program, "eye_position");
    time_location = glGetUniformLocation(program, "T");
    glUniform1i(glGetUniformLocation(program, "height_sampler"), 0);
    glUniform1i(glGetUniformLocation(program, "height_texture"), 1);
    glUniform1i(glGetUniformLocation(program, "reflection_texture"), 2);
    glUniform1i(glGetUniformLocation(program, "refraction_texture"), 3);
    glUniform1i(glGetUniformLocation(program, "cut_texture"), 4);
    glUniform1i(glGetUniformLocation(program, "blendmap"), 5);
    glUniform1i(glGetUniformLocation(program, "dirt"), 6);
    glUniform1i(glGetUniformLocation(program, "grass"), 7);
    glUniform1i(glGetUniformLocation(program, "soil"), 8);
    glUniform1i(glGetUniformLocation(program, "rock"), 9);
    glUniform1i(glGetUniformLocation(program, "txtr"), 10);
    has_texture_location = glGetUniformLocation(program, "has_texture");
    clip_plane_location = glGetUniformLocation(program, "clip_plane");
    forward_location = glGetUniformLocation(program, "forward");
}

void Shader::loadHasTexture(int has){
    glUniform1i(has_texture_location, has);
}

void Shader::loadWaves(Wave *waves, int NUM_WAVES){
    for(int i = 0; i < NUM_WAVES; i++){
        std::string dir = "";
        dir = "direction[";
        dir += (char)(i+'0');
        dir.append(std::string("]"));

        std::string center = "";
        center = "center[";
        center += (char)(i+'0');
        center.append(std::string("]"));

        std::string w = "";
        w = "w[";
        w += (char)(i+'0');
        w.append(std::string("]"));

        std::string f = "";
        f = "f[";
        f += (char)(i+'0');
        f.append(std::string("]"));

        std::string A = "";
        A = "A[";
        A += (char)(i+'0');
        A.append(std::string("]"));

        std::string circular = "";
        circular = "circular[";
        circular += (char)(i+'0');
        circular.append(std::string("]"));

        glUniform2f(glGetUniformLocation(program, dir.c_str()), waves[i].direction.x, waves[i].direction.y);
        glUniform2f(glGetUniformLocation(program, center.c_str()), waves[i].center.x, waves[i].center.y);
        glUniform1f(glGetUniformLocation(program, w.c_str()), waves[i].w);
        glUniform1f(glGetUniformLocation(program, f.c_str()), waves[i].f);
        glUniform1f(glGetUniformLocation(program, A.c_str()), waves[i].A);
        glUniform1i(glGetUniformLocation(program, circular.c_str()), (int)waves[i].circular);
    }
}

void Shader::loadForward(glm::vec3 f){
    loadVector3(forward_location, f);
}

void Shader::loadTime(float t){
    loadFloat(time_location, t);
}

void Shader::loadFloat(GLuint location, float f){
    glUniform1f(location, f);
}

void Shader::loadViewMatrix(glm::mat4 matrix){
    loadMatrix(viewMat_location, matrix);
}

void Shader::loadClipPlane(glm::vec4 plane){
    loadVector4(clip_plane_location, plane);
}

void Shader::loadEyePosition(glm::vec3 eye){
    loadVector3(eyePosition_location, eye);
}

void Shader::loadColor(glm::vec4 color){
    loadVector4(colorVector_location, color);
}

void Shader::loadLightPosition(glm::vec3 vect){
    loadVector3(lightPosition_location, vect);
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
