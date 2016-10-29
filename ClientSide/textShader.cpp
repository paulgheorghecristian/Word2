#include "textshader.h"

TextShader::TextShader(const std::string& vs_filename, const std::string& fs_filename) : Shader(vs_filename, fs_filename)
{
    getAllUniformLocations();
}

void TextShader::getAllUniformLocations(){
    modelMatLocation = glGetUniformLocation(program, "modelMatrix");
    projMatLocation = glGetUniformLocation(program, "projectionMatrix");
    colorVectorLocation = glGetUniformLocation(program, "color");
    glUniform1i(glGetUniformLocation(program, "fontAtlas"), 0);
}

TextShader::~TextShader()
{
    //dtor
}
