#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#define MAX_ASCII 300

class Character{
    public:
        Character(char id, int x, int y, int width, int height, int xoffset, int yoffset, int xadvance) : id(id),x(x),y(y),width(width),height(height),xoffset(xoffset),yoffset(yoffset),xadvance(xadvance){}
        Character() : Character(-1,0,0,0,0,0,0,0){}
        char id;
        int x, y;
        int width, height;
        int xoffset, yoffset;
        int xadvance;
};

class Font
{
    public:
        Font(const std::string&, const std::string&);
        std::vector<Character>& get_chars(){
            return chars_info;
        }
        glm::vec2 get_dimensions(){
            return glm::vec2(scaleW, scaleH);
        }
        GLuint get_texture_id();
        virtual ~Font();
    protected:
    private:
        int get_number(std::string);
        int scaleW, scaleH;
        std::vector<Character> chars_info;
        GLuint texture_id;
};

#endif // FONT_H
