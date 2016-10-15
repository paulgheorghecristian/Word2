#ifndef TEXT_H
#define TEXT_H

#include "font.h"
#include "mesh.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "textshader.h"

class Text
{
    public:
        Text(Font*, const std::string&, glm::vec3, glm::vec3, glm::vec3, float);
        Mesh* get_mesh(){
            return mesh;
        }
        void set_color(glm::vec3 color){
            this->color = color;
        }
        void set_position(glm::vec3 pos){
            position = pos;
            update_model_matrix = true;
        }
        glm::vec3 get_position(){
            return position;
        }
        void change_size(int);
        void move(glm::vec3 dr){
            position += dr;
            update_model_matrix = true;
        }
        void add_letter(char letter);
        void delete_letter();
        void display_number(long);
        void draw(TextShader *);
        virtual ~Text();
    protected:
    private:
        Font *m_font;
        Mesh *mesh;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 color;
        float size;
        std::string current_string;
        glm::mat4 model_matrix;
        bool update_model_matrix;
        void compute_model_matrix();
        void update_mesh(bool);
};

#endif // TEXT_H
