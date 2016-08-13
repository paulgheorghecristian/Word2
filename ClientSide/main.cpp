#include <iostream>
#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "texture.h"
#include "framebuffer.h"
#include "cubemaptexture.h"
#include "cubemapshader.h"

using namespace std;

int main()
{
		//rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;
    const int NUM_WAVES = 3;
		//date despre valuri
    Wave waves[NUM_WAVES];

    waves[0].direction = glm::vec2(0.5f, 0.5f);
    waves[0].center = glm::vec2(0.0f, 0.0f);
    waves[0].w = 20.0f;
    waves[0].f = 10.0;
    waves[0].A = 0.03f;
    waves[0].circular = true;

    waves[1].direction = glm::vec2(1.0f, 0.0f);
    waves[1].center = glm::vec2(0.5f, 0.5f);
    waves[1].w = 20.0f;
    waves[1].f = 10.0f;
    waves[1].A = 0.03f;
    waves[1].circular = true;

    waves[2].direction = glm::vec2(0.0f, 1.0f);
    waves[2].center = glm::vec2(0.0f, 0.0f);
    waves[2].w = 20.0f;
    waves[2].f = 10.0f;
    waves[2].A = 0.03f;
    waves[2].circular = false;

    Display display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Framebuffer fb1(1080, 720);
    Framebuffer fb2(1080, 720);
    Shader water_shader("res/shaders/vertex", "res/shaders/fragment");
    Shader general_shader("res/shaders/vertex2", "res/shaders/fragment2");
    Shader terrain_shader("res/shaders/v_terrain", "res/shaders/f_terrain");
    CubeMapShader *shader2 = new CubeMapShader("res/shaders/cubemap_vertex", "res/shaders/cubemap_fragment");
    Texture terrain_heightmap("res/textures/lakehm.bmp");
    Texture terrain_texture("res/textures/lakehm.bmp");
    Texture reflection(fb1.get_texture_id());
    Texture refraction(fb2.get_texture_id());
    Texture blendmap("res/textures/terrain_textures/blendmap2.bmp");
    Texture dirt("res/textures/terrain_textures/dirt.bmp");
    Texture grass("res/textures/terrain_textures/grass.bmp");
    Texture rock("res/textures/terrain_textures/rock.bmp");
    Texture soil("res/textures/terrain_textures/soil.bmp");
    Texture bamboo_texture("res/textures/bambooalpha.bmp");
    CubeMapTexture *txt = new CubeMapTexture("res/textures/CubeMap");

    Input input;
    Camera camera(glm::vec3(0.0f, HEIGHT/2.0f-200.0f, -150.0f), 0.0, 0.0, 0.0);
    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);
    Mesh *sphere = Mesh::load_object("res/models/sphere.obj");
    Mesh *bamboo_mesh = Mesh::load_object("res/models/bamboo.obj");
    Mesh *cube = Mesh::load_object("res/models/cube.obj");

    glm::vec3 light_position(0.0f, 300.0f, -500.0f);
		//entitati
    Entity *water = new Entity("water", surface, glm::vec4(0.0f, 0.0f, 0.1f, 1.0f), glm::vec3(-5.5, 12.1, -597.6), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 40.0f, 100.0f));
    Entity *light_source = new Entity("light_source", sphere, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), light_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f));
    Entity *terrain = new Entity("terrain", surface, glm::vec4(120.0f/255.0f, 50.0f/255.0f, 10.0f/255.0f, 1.0f), glm::vec3(0.0f, 0.0f, -600.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.0f, 50.0f, 200.0f));
    Entity *skybox = new Entity("skybox", cube, glm::vec4(1,0,0,1), glm::vec3(0), glm::vec3(0), glm::vec3(1000));
    Entity *bamboo = new Entity("bamboo", bamboo_mesh, glm::vec4(0.0), glm::vec3(70.4f, 40.0f, -699.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5,5,5));
    Entity *bamboo2 = new Entity("bamboo", bamboo_mesh, glm::vec4(0.0), glm::vec3(50.4f, 40.0f, -640.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5,5,5));
    Entity *bamboo3 = new Entity("bamboo", bamboo_mesh, glm::vec4(0.0), glm::vec3(-5.4f, 30.0f, -450.8f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5,5,5));

    glm::mat4 projection_matrix = glm::perspective(90.0f, WIDTH/HEIGHT, 0.1f, 2000.0f);

    water_shader.bind();
    water_shader.loadWaves(waves, NUM_WAVES);
    water_shader.loadProjectionMatrix(projection_matrix);
    water_shader.loadViewMatrix(camera.get_view_matrix());

    general_shader.bind();
    general_shader.loadProjectionMatrix(projection_matrix);
    general_shader.loadViewMatrix(camera.get_view_matrix());

    shader2->bind();
    shader2->load_projection_matrix(glm::perspective(90.0f, WIDTH/HEIGHT, 0.1f, 2000.0f));
    shader2->load_view_matrix(camera.get_view_matrix());

    bool move_cam = 1;

    while(!display.isClosed()){
        input.update();
        display.clear(0.1f, 0.1f, 0.5f, 1.0f);

        if(input.GetKeyDown(SDLK_e)){ //muta camera sau lumina
            move_cam = !move_cam;
        }
        if(input.GetKeyDown(SDLK_r)){
						//reload shaders
            water_shader.reload_shader();
            water_shader.bind();
            water_shader.loadWaves(waves, NUM_WAVES);
            water_shader.loadProjectionMatrix(glm::perspective(90.0f, WIDTH/HEIGHT, 0.1f, 1000.0f));
            water_shader.loadViewMatrix(camera.get_view_matrix());
            reflection.use(2);

            terrain_shader.reload_shader();
            terrain_shader.bind();
            terrain_shader.loadProjectionMatrix(glm::perspective(90.0f, WIDTH/HEIGHT, 0.1f, 1000.0f));
            terrain_shader.loadViewMatrix(camera.get_view_matrix());
            terrain_shader.loadLightPosition(light_position);
            terrain_shader.loadEyePosition(camera.get_position());
            terrain_heightmap.use(0);
            terrain_texture.use(1);
        }
				//miscare lumina sau camera
        if(input.GetKey(SDLK_w)){
            if(!move_cam)
            light_position.z -= 100.0f * Display::get_delta();
            else camera.move_forward(400.0f * Display::get_delta());
        }else if(input.GetKey(SDLK_s)){
            if(!move_cam)
            light_position.z += 100.0f * Display::get_delta();
            else camera.move_forward(-400.0f * Display::get_delta());
        }

        if(input.GetKey(SDLK_a)){
            if(!move_cam)
            light_position.x -= 100.0f * Display::get_delta();
            else camera.move_sideways(-400.0f * Display::get_delta());
        }else if(input.GetKey(SDLK_d)){
            if(!move_cam)light_position.x += 100.0f * Display::get_delta();
            else camera.move_sideways(400.0f * Display::get_delta());
        }

        if(input.GetKey(0)){
            camera.rotate_y(-100.0f * Display::get_delta());
        }else if(input.GetKey(1)){
            camera.rotate_y(100.0f * Display::get_delta());
        }

        if(input.GetKey(2)){
            if(!move_cam)light_position.y -= 100.0f * Display::get_delta();
            else camera.rotate_x(-100.0 * Display::get_delta());
        }else if(input.GetKey(3)){
            if(!move_cam)light_position.y += 100.0f * Display::get_delta();
            else camera.rotate_x(100.0 * Display::get_delta());
        }
        light_source->set_position(light_position.x, light_position.y, light_position.z);

        if(glm::abs(glm::length(input.GetMouseDelta())) > 0.1f){
            camera.rotate_x(input.GetMouseDelta().y * 0.05f);
            camera.rotate_y(input.GetMouseDelta().x * 0.05f);
        }
				//fb1 este folosit pentru reflexie
        fb1.bind();
				//distanta de la camera la apa
        float length = glm::abs(camera.get_position().y - water->get_position().y);
				//muta camera pe OY cu -2*length, inverseaza forward si randeaza scena din acest unghi pentru reflexie
        camera.move_up(-2*length);
        camera.invert_forward();
        shader2->bind();
        shader2->load_view_matrix(camera.get_view_matrix());
        skybox->draw(shader2);
        terrain_shader.bind();
        terrain_shader.loadProjectionMatrix(projection_matrix);
        terrain_shader.loadViewMatrix(camera.get_view_matrix());
        terrain_shader.loadLightPosition(light_position);
        terrain_shader.loadEyePosition(camera.get_position());
        terrain_shader.loadClipPlane(glm::vec4(0.0, 1.0, 0.0, -water->get_position().y));
        terrain_heightmap.use(0);
        terrain_texture.use(1);
				blendmap.use(5);
        dirt.use(6);
        grass.use(7);
        soil.use(8);
        rock.use(9);
        terrain->draw(&terrain_shader);

        general_shader.bind();
        general_shader.loadViewMatrix(camera.get_view_matrix());
        general_shader.loadHasTexture(0);
        light_source->draw(&general_shader);
        general_shader.loadHasTexture(1);
        bamboo_texture.use(10);
        bamboo->draw(&general_shader);
        bamboo2->draw(&general_shader);
        bamboo3->draw(&general_shader);
        camera.invert_forward();
        camera.move_up(2*length);
        fb1.unbind();
				//fb2 pentru refractie
        fb2.bind();
        terrain_shader.bind();
        terrain_shader.loadProjectionMatrix(projection_matrix);
        terrain_shader.loadViewMatrix(camera.get_view_matrix());
        terrain_shader.loadLightPosition(light_position);
        terrain_shader.loadEyePosition(camera.get_position());
        terrain_shader.loadClipPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
        terrain_heightmap.use(0);
        terrain_texture.use(1);
        blendmap.use(5);
        dirt.use(6);
        grass.use(7);
        soil.use(8);
        rock.use(9);
        terrain->draw(&terrain_shader);

        general_shader.bind();
        general_shader.loadViewMatrix(camera.get_view_matrix());
        bamboo_texture.use(10);
        bamboo->draw(&general_shader);
        bamboo2->draw(&general_shader);
        bamboo3->draw(&general_shader);
        fb2.unbind();

        glViewport(0, 0, WIDTH, HEIGHT);
				//deseneaza scena pe framebuffer-ul default
        shader2->bind();
        shader2->load_view_matrix(camera.get_view_matrix());
        skybox->draw(shader2);
        water_shader.bind();
        water_shader.loadLightPosition(light_position);
        water_shader.loadViewMatrix(camera.get_view_matrix());
        water_shader.loadEyePosition(camera.get_position());
        water_shader.loadTime((float)SDL_GetTicks()/1000.0f);
        water_shader.loadForward(camera.get_forward());
        reflection.use(2);
        refraction.use(3);
        water->draw(&water_shader);

        general_shader.bind();
        general_shader.loadViewMatrix(camera.get_view_matrix());
        general_shader.loadHasTexture(0);
        light_source->draw(&general_shader);
        general_shader.loadHasTexture(1);
        bamboo_texture.use(10);
        bamboo->draw(&general_shader);
        bamboo2->draw(&general_shader);
        bamboo3->draw(&general_shader);
        terrain_shader.bind();
        terrain_shader.loadProjectionMatrix(projection_matrix);
        terrain_shader.loadViewMatrix(camera.get_view_matrix());
        terrain_shader.loadLightPosition(light_position);
        terrain_shader.loadEyePosition(camera.get_position());
        terrain_shader.loadClipPlane(glm::vec4(0.0, 0.0, 0.0, 0.0));
        terrain_heightmap.use(0);
        terrain_texture.use(1);
        blendmap.use(5);
        dirt.use(6);
        grass.use(7);
        soil.use(8);
        rock.use(9);
        terrain->draw(&terrain_shader);

        display.update();
    }

    return 0;
}
