#include "text.h"

Text::Text(Font *f, const std::string& text, glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size) : mesh(0), current_string(text), m_font(f), position(position), rotation(rotation), color(color), size(size), model_matrix(1.0)
{
    update_mesh(0);
    compute_model_matrix();
}

void Text::update_mesh(bool dir){
    std::vector<Character> chars = m_font->get_chars();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::vec2 cursor(0);
    if(mesh){
        delete mesh;
    }
    for(int i = (dir == 0 ? 0 : current_string.size()); dir == 0 ? (i < current_string.size()) : (i >= 0); dir == 0 ? i++ : i--){
        char letter = current_string[i];
        int id = (int)letter;
        Character char_info = chars[id];
        int index = i*4;
        glm::vec2 font_dim = m_font->get_dimensions();

        float x, y, width, height;

        x = (float)(cursor.x + char_info.xoffset) / (float)font_dim.x;;
        y = (float)(cursor.y + char_info.yoffset) /(float)font_dim.y;
        width = (float)char_info.width /(float)font_dim.x;
        height = (float)char_info.height/(float)font_dim.y;
        cursor.x += (float)char_info.xadvance;

        float x_text = (float)char_info.x / (float)font_dim.x;
        float y_text = (float)char_info.y / (float)font_dim.y;
        float width_text = (float)char_info.width / (float)font_dim.x;
        float height_text = (float)char_info.height / (float)font_dim.y;

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

void Text::compute_model_matrix(){
    model_matrix = glm::translate(glm::mat4(1.0), position);
    model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1,0,0));
    model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0,1,0));
    model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0,0,1));
    model_matrix = glm::scale(model_matrix, glm::vec3(size*50));

    update_model_matrix = false;
}

void Text::change_size(int dsize){
    size += dsize;
    if(size < 1){
        size = 1;
    }else if(size > 20){
        size = 20;
    }
}

void Text::add_letter(char letter){
    current_string.push_back(letter);
    update_mesh(0);
}

void Text::display_number(long number){
    current_string.clear();
    while(number){
        current_string.push_back((number % 10)+'0');
        number /= 10;
    }
    update_mesh(1);
}

void Text::delete_letter(){
    current_string = current_string.substr(0, current_string.size()-1);
    update_mesh(0);
}

void Text::draw(TextShader *shader){

    if(update_model_matrix){
        compute_model_matrix();
    }

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_font->get_texture_id());

    glBindVertexArray(mesh->get_vao());
    shader->load_model_matrix(model_matrix);
    shader->load_color(color);
    glDrawElements(GL_TRIANGLES, mesh->get_no_triangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

Text::~Text()
{
    //dtor
}
