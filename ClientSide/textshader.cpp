#include "textshader.h"

TextShader::TextShader(const std::string& vs_filename, const std::string& fs_filename) : Shader(vs_filename, fs_filename)
{
    getAllUniformLocations();
}

void TextShader::getAllUniformLocations(){
    modelMatrix_location = glGetUniformLocation(program, "model_matrix");
    viewMatrix_location = glGetUniformLocation(program, "view_matrix");
    projectionMatrix_location = glGetUniformLocation(program, "projection_matrix");
    color_location = glGetUniformLocation(program, "color");
    glUniform1i(glGetUniformLocation(program, "font_atlas"), 0);
}

void TextShader::load_model_matrix(glm::mat4 matrix){
    glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void TextShader::load_view_matrix(glm::mat4 matrix){
    glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void TextShader::load_projection_matrix(glm::mat4 matrix){
    glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void TextShader::load_color(glm::vec3 color){
    glUniform3fv(color_location, 1, glm::value_ptr(color));
}

TextShader::~TextShader()
{
    //dtor
}
