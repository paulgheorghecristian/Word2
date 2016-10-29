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
        std::vector<Character>& getChars(){
            return charsInfo;
        }
        glm::vec2 getDimensions(){
            return glm::vec2(scaleW, scaleH);
        }
        GLuint getTextureId();
        virtual ~Font();
    protected:
    private:
        int getNumber(std::string);
        int scaleW, scaleH;
        std::vector<Character> charsInfo;
        GLuint textureId;
};

#endif // FONT_H
