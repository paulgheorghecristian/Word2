#include "text.h"

Text::Text(Font *f, const std::string& text, glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size) : mesh(0), currentString(text), m_font(f), position(position), rotation(rotation), color(color), size(size), modelMatrix(1.0)
{
    updateMesh(0);
    computeModelMatrix();
}

void Text::updateMesh(bool dir){
    std::vector<Character> chars = m_font->getChars();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::vec2 cursor(0);
    if(mesh){
        delete mesh;
    }
    for(int i = (dir == 0 ? 0 : currentString.size()); dir == 0 ? (i < currentString.size()) : (i >= 0); dir == 0 ? i++ : i--){
        char letter = currentString[i];
        int id = (int)letter;
        Character charInfo = chars[id];
        int index = i*4;
        glm::vec2 fontDim = m_font->getDimensions();

        float x, y, width, height;

        x = (float)(cursor.x + charInfo.xoffset) / (float)fontDim.x;;
        y = (float)(cursor.y + charInfo.yoffset) /(float)fontDim.y;
        width = (float)charInfo.width /(float)fontDim.x;
        height = (float)charInfo.height/(float)fontDim.y;
        cursor.x += (float)charInfo.xadvance;

        float x_text = (float)charInfo.x / (float)fontDim.x;
        float y_text = (float)charInfo.y / (float)fontDim.y;
        float width_text = (float)charInfo.width / (float)fontDim.x;
        float height_text = (float)charInfo.height / (float)fontDim.y;

        vertices.push_back(Vertex(glm::vec3(x, y-height, 0.0), glm::vec3(0,0,1), glm::vec2(x_text, (y_text + height_text))));
        vertices.push_back(Vertex(glm::vec3(x+width, y-height, 0.0), glm::vec3(0,0,1), glm::vec2(x_text + width_text, (y_text + height_text))));
        vertices.push_back(Vertex(glm::vec3(x, y, 0.0), glm::vec3(0,0,1), glm::vec2(x_text, y_text)));
        vertices.push_back(Vertex(glm::vec3(x+width, y, 0.0), glm::vec3(0,0,1), glm::vec2(x_text + width_text, y_text)));

        indices.push_back(index);
        indices.push_back(index+1);
        indices.push_back(index+2);
        indices.push_back(index+2);
        indices.push_back(index+1);
        indices.push_back(index+3);
    }
    mesh = new Mesh(vertices, indices);
}

void Text::computeModelMatrix(){
    modelMatrix = glm::translate(glm::mat4(1.0), position);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size*50));

    isModelMatrixModified = false;
}

void Text::changeSize(int dsize){
    size += dsize;
    if(size < 1){
        size = 1;
    }else if(size > 20){
        size = 20;
    }
}

void Text::addLetter(char letter){
    currentString.push_back(letter);
    updateMesh(0);
}

void Text::displayNumber(long number){
    currentString.clear();
    while(number){
        currentString.push_back((number % 10)+'0');
        number /= 10;
    }
    updateMesh(1);
}

void Text::deleteLetter(){
    currentString = currentString.substr(0, currentString.size()-1);
    updateMesh(0);
}

void Text::draw(TextShader* shader){

    if(isModelMatrixModified){
        computeModelMatrix();
    }

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_font->getTextureId());

    glBindVertexArray(mesh->getVao());
    shader->loadModelMatrix(modelMatrix);
    shader->loadColor(color);
    glDrawElements(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

Text::~Text()
{
    //dtor
}
