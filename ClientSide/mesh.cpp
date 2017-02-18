#include "mesh.h"
#include <iostream>
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    //vao care retine starea meshei
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glGenBuffers(NUM_VBOS, vboHandles);

    //trimit GPU-ului vertecsii
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    //trimit GPU-ului indecsii
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    numberOfTriangles = indices.size();

    //ii descriu shader-ului datele trimise

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
}

Mesh::~Mesh()
{
    glDeleteBuffers(NUM_VBOS, vboHandles);
    glDeleteVertexArrays(1, &vaoHandle);
}

GLuint Mesh::getVao(){
    return vaoHandle;
}

GLsizei Mesh::getNumberOfTriangles(){
    return numberOfTriangles;
}

Mesh* Mesh::getRectangle(){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);

    return new Mesh(vertices, indices);
}

void Mesh::draw(){
    glBindVertexArray(getVao());
    glDrawElements(GL_TRIANGLES, numberOfTriangles, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

Mesh* Mesh::getSurface(int width, int height){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float dw = 2.0f / (float)width;
    float dh = 2.0f / (float)height;
		//creeaza suprafata pentru apa
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int first_index = 4*(j + i*width);
            vertices.push_back(Vertex(glm::vec3(-1.0f + j * dw, 0.0f, 1.0f - i * dh), glm::vec3(0, 1, 0)));
            vertices.push_back(Vertex(glm::vec3(-1.0f + j * dw, 0.0f, 1.0f - (i+1) * dh), glm::vec3(0, 1, 0)));
            vertices.push_back(Vertex(glm::vec3(-1.0f + (j+1) * dw, 0.0f, 1.0f - (i+1) * dh), glm::vec3(0, 1, 0)));
            vertices.push_back(Vertex(glm::vec3(-1.0f + (j+1) * dw, 0.0f, 1.0f - i * dh), glm::vec3(0, 1, 0)));

            indices.push_back(first_index+2);
            indices.push_back(first_index+1);
            indices.push_back(first_index+0);

            indices.push_back(first_index);
            indices.push_back(first_index+3);
            indices.push_back(first_index+2);

        }
    }
    return new Mesh(vertices, indices);
}

float Mesh::_stringToFloat(const std::string &source){
		std::stringstream ss(source.c_str());
		float result;
		ss>>result;
		return result;
}
//transforms a string to an int
unsigned int Mesh::_stringToUint(const std::string &source){
    std::stringstream ss(source.c_str());
    unsigned int result;
    ss>>result;
    return result;
}
//transforms a string to an int
int Mesh::_stringToInt(const std::string &source){
    std::stringstream ss(source.c_str());
    int result;
    ss>>result;
    return result;
}
//writes the tokens of the source string into tokens
void Mesh::_stringTokenize(const std::string &source, std::vector<std::string> &tokens){
    tokens.clear();
    std::string aux=source;
    for(unsigned int i=0;i<aux.size();i++) if(aux[i]=='\t' || aux[i]=='\n') aux[i]=' ';
    std::stringstream ss(aux,std::ios::in);
    while(ss.good()){
        std::string s;
        ss>>s;
        if(s.size()>0) tokens.push_back(s);
    }
}
//variant for faces
void Mesh::_faceTokenize(const std::string &source, std::vector<std::string> &tokens){
    std::string aux=source;
    for(unsigned int i=0;i<aux.size();i++) if(aux[i]=='\\' || aux[i]=='/') aux[i]=' ';
    _stringTokenize(aux,tokens);
}

Mesh* Mesh::loadObject(std::string filename){

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
		if(!file.good()){
			std::cout<<"Mesh Loader: Nu am gasit fisierul obj "<<filename<<" sau nu am drepturile sa il deschid!"<<std::endl;
			std::terminate();
		}

		std::string line;
		std::vector<std::string> tokens, facetokens;
		std::vector<glm::vec3> positions;		positions.reserve(1000);
		std::vector<glm::vec3> normals;		normals.reserve(1000);
		std::vector<glm::vec2> texcoords;		texcoords.reserve(1000);
		while(std::getline(file,line)){
			//tokenizeaza linie citita
			_stringTokenize(line,tokens);

			//daca nu am nimic merg mai departe
			if(tokens.size()==0) continue;

			//daca am un comentariu merg mai departe
			if(tokens.size()>0 && tokens[0].at(0)=='#') continue;

			//daca am un vertex
			if(tokens.size()>3 && tokens[0]=="v") positions.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3]) ));

			//daca am o normala
			if(tokens.size()>3 && tokens[0]=="vn") normals.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3]) ));

			//daca am un texcoord
			if(tokens.size()>2 && tokens[0]=="vt") texcoords.push_back(glm::vec2(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]) ));

			//daca am o fata (f+ minim 3 indecsi)
			if(tokens.size()>=4 && tokens[0]=="f"){

				//foloseste primul vertex al fetei pentru a determina formatul fetei (v v/t v//n v/t/n) = (1 2 3 4)
				unsigned int face_format = 0;
				if(tokens[1].find("//")!=std::string::npos) face_format = 3;
				_faceTokenize(tokens[1],facetokens);
				if(facetokens.size()==3) face_format =4; // vertecsi/texcoords/normale
				else{
					if(facetokens.size()==2){
						if(face_format !=3) face_format=2;	//daca nu am vertecsi/normale am vertecsi/texcoords
					}else{
						face_format =1; //doar vertecsi
					}
				}

				//primul index din acest poligon
				unsigned int index_of_first_vertex_of_face = -1;


				for(unsigned int num_token =1; num_token<tokens.size();num_token++){
					if(tokens[num_token].at(0)=='#') break;					//comment dupa fata
					_faceTokenize(tokens[num_token],facetokens);
					if(face_format==1){
						//doar pozitie
						int p_index = _stringToInt(facetokens[0]);
						if(p_index>0) p_index -=1;								//obj has 1...n indices
						else p_index = positions.size()+p_index;				//index negativ

						vertices.push_back(Vertex(glm::vec3(positions[p_index].x, positions[p_index].y, positions[p_index].z)));
					}else if(face_format==2){
						// pozitie si texcoord
						int p_index = _stringToInt(facetokens[0]);
						if(p_index>0) p_index -=1;								//obj has 1...n indices
						else p_index = positions.size()+p_index;				//index negativ

						int t_index = _stringToInt(facetokens[1]);
						if(t_index>0) t_index -=1;								//obj has 1...n indices
						else t_index = texcoords.size()+t_index;				//index negativ

						vertices.push_back(Vertex(glm::vec3(positions[p_index].x, positions[p_index].y, positions[p_index].z),glm::vec2(texcoords[t_index].x, texcoords[t_index].y)));
					}else if(face_format==3){
						//pozitie si normala
						int p_index = _stringToInt(facetokens[0]);
						if(p_index>0) p_index -=1;								//obj has 1...n indices
						else p_index = positions.size()+p_index;				//index negativ

						int n_index = _stringToInt(facetokens[1]);
						if(n_index>0) n_index -=1;								//obj has 1...n indices
						else n_index = normals.size()+n_index;					//index negativ

						vertices.push_back(Vertex(glm::vec3(positions[p_index].x, positions[p_index].y, positions[p_index].z), glm::vec3(normals[n_index].x, normals[n_index].y, normals[n_index].z)));
					}else{
						//pozitie normala si texcoord
						int p_index = _stringToInt(facetokens[0]);
						if(p_index>0) p_index -=1;								//obj has 1...n indices
						else p_index = positions.size()+p_index;				//index negativ

						int t_index = _stringToInt(facetokens[1]);
						if(t_index>0) t_index -=1;								//obj has 1...n indices
						else t_index = normals.size()+t_index;					//index negativ

						int n_index = _stringToInt(facetokens[2]);
						if(n_index>0) n_index -=1;								//obj has 1...n indices
						else n_index = normals.size()+n_index;					//index negativ

						vertices.push_back(Vertex(glm::vec3(positions[p_index].x, positions[p_index].y, positions[p_index].z),glm::vec3(normals[n_index].x, normals[n_index].y, normals[n_index].z), glm::vec2(texcoords[t_index].x, texcoords[t_index].y)));
					}

					//adauga si indecsii
					if(num_token<4){
						if(num_token == 1) index_of_first_vertex_of_face = vertices.size()-1;
						//doar triunghiuri f 0 1 2 3 (4 indecsi, primul e ocupat de f)
						indices.push_back(vertices.size()-1);
					}else{
						//polygon => triunghi cu ultimul predecesor vertexului nou adaugat si 0 relativ la vertecsi poligon(independent clockwise)
						indices.push_back(index_of_first_vertex_of_face);
						indices.push_back(vertices.size()-2);
						indices.push_back(vertices.size()-1);
					}
				}//end for
			}//end face

		}//end while
		return new Mesh(vertices, indices);
}
