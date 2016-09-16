#include "font.h"

Font::Font(const std::string& fnt_filename, const std::string& font_atlas_filename) : chars_info(MAX_ASCII, Character())
{
    std::ifstream file(fnt_filename.c_str(), std::ios::in | std::ios::binary);

    if(!file.good()){
        std::cout << "Nu am gasit fisierul " << fnt_filename << std::endl;
        std::terminate();
    }

    std::string line;

    std::getline(file, line);
    std::getline(file, line);

    char *str = strdup(line.c_str());
    char *p = strstr(str, "scaleW=");
    char *q = strstr(str, "scaleH=");

    if(p && q){
        p += 7;
        q += 7;

        char *aux = p;
        while(*aux != ' '){
            aux++;
        }
        aux = '\0';
        scaleW = atoi(p);

        aux = q;
        while(*aux != ' '){
            aux++;
        }
        aux = '\0';
        scaleH = atoi(q);
    }
    std::getline(file, line);
    std::getline(file, line);

    while(std::getline(file,line)){
        std::stringstream ss(line, std::ios::in);
        std::string token;
        ss >> token;
        ss >> token;
        int id = get_number(token);
        ss >> token;
        int x = get_number(token);
        ss >> token;
        int y = get_number(token);
        ss >> token;
        int width = get_number(token);
        ss >> token;
        int height = get_number(token);
        ss >> token;
        int xoffset = get_number(token);
        ss >> token;
        int yoffset = get_number(token);
        ss >> token;
        int xadvance = get_number(token);

        chars_info[id] = Character(id, x, y, width, height, xoffset, yoffset, xadvance);
    }

    file.close();
    delete str;

    SDL_Surface *font_atlas = SDL_LoadBMP(font_atlas_filename.c_str());

    if(!font_atlas){
        std::cerr << "Loading font atlas error!" << std::endl;
        exit(1);
    }
		//genereaza textura
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_atlas->w, font_atlas->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, font_atlas->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.7f);

    SDL_FreeSurface(font_atlas);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Font::get_texture_id(){
    return texture_id;
}

int Font::get_number(std::string str){
    char *p = strdup(str.c_str());
    char *q = strchr(p, '=');
    q += 1;
    int number = atoi(q);
    delete p;
    return number;
}

Font::~Font()
{
    //dtor
}
