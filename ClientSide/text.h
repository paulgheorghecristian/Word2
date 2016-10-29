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
        Mesh* getMesh(){
            return mesh;
        }
        void setColor(glm::vec3 color){
            this->color = color;
        }
        void setPosition(glm::vec3 pos){
            position = pos;
            isModelMatrixModified = true;
        }
        glm::vec3 getPosition(){
            return position;
        }
        void changeSize(int);
        void move(glm::vec3 dr){
            position += dr;
            isModelMatrixModified = true;
        }
        void addLetter(char);
        void deleteLetter();
        void displayNumber(long);
        void draw(TextShader*);
        virtual ~Text();
    protected:
    private:
        Font* m_font;
        Mesh* mesh;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 color;
        float size;
        std::string currentString;
        glm::mat4 modelMatrix;
        bool isModelMatrixModified;
        void computeModelMatrix();
        void updateMesh(bool);
};

#endif // TEXT_H
