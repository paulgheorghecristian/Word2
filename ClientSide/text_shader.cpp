#include "text_shader.h"

TextShader::TextShader(const std::string& vs_filename, const std::string& fs_filename) : GeneralShader(vs_filename, fs_filename)
{
    bind();
    getAllUniformLocations();
}

void TextShader::getAllUniformLocations(){
    GeneralShader::getAllUniformLocations();
    glUniform1i(glGetUniformLocation(program, "fontAtlas"), 0);
}

TextShader::~TextShader()
{
    //dtor
}
