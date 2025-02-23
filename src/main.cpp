#include "camera.hpp"
#include "const.hpp"
#include "window_driver.hpp"

#include <iostream>
#include <string>
#include "skybox.hpp"
#include "shader.h"
#include "Ground_face.hpp"



void render_sence(Shader shader, Buffer_data *render_data, Buffer_data *flat_render_data, unsigned int Texture[]){
    shader.setBool("Is_sun", false);
    
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(5.0f));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[0]);

    glBindVertexArray(flat_render_data->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindVertexArray(0);
    
    glBindVertexArray(render_data->VAO); 
    
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.3f));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.5f, 2.0f, -0.5));
    model = glm::scale(model, glm::vec3(0.6f));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.3f, 0.5f, -1.2));
    model = glm::scale(model, glm::vec3(0.4f));
    model = glm::rotate(model, glm::radians(30.0f), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.7f, 2.5f, 0.4));
    model = glm::scale(model, glm::vec3(0.6f));
    model = glm::rotate(model, glm::radians(45.0f), glm::normalize(glm::vec3(0.0, 1.0, 1.0)));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.5f, -2.0f, 0.5));
    model = glm::scale(model, glm::vec3(0.3f));
    model = glm::rotate(model, glm::radians(30.0f), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, -2.0f, 0.5));
    model = glm::scale(model, glm::vec3(0.5f));
    model = glm::rotate(model, glm::radians(30.0f), glm::normalize(glm::vec3(0.5, 1.0, 1.0)));
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[2]);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    glBindVertexArray(0);
}

void render_light_point(float light_power, Buffer_data *render_data, glm::vec3 light_pos, Shader shader, glm::vec3 color_light){
    glBindVertexArray(render_data->VAO); 
    shader.setBool("Is_sun", true);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(light_power));
    shader.setMat4("model", model);
    shader.setVec3("sun_color", color_light);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


int main(int argv, char *argc[]){
    Window screen("shader_map", WIDTH_WIN, HIGHT_WIN, SDL_INIT_VIDEO, SDL_WINDOW_OPENGL);
    
    glEnable(GL_DEPTH_TEST);

    glm::vec3 Camera_pos = { 0.0f, 1.0f, 5.0f };
    float Angle_rotate[] = {0,0,0};
    Camera Main_view(Camera_pos, 45, 0.03, 0.1, 1, 0.1f, 3000.0f);
    screen.swap_mouse(WIDTH_WIN / 2, HIGHT_WIN / 2);
    
    std::vector<std::string> SkyBox_image_path
    {
        "resources/image/skybox/right.jpg",
        "resources/image/skybox/left.jpg",
        "resources/image/skybox/top.jpg",
        "resources/image/skybox/bottom.jpg",
        "resources/image/skybox/front.jpg",
        "resources/image/skybox/back.jpg"
    };
    Skybox skybox_game("resources/shader_source/cubemap_vshader.vert", "resources/shader_source/cubemap_fshader.frag", SkyBox_image_path);

    // ShaderProgram deep_map("resources/shader_source/deep_map_vshader.vert", 
    //                        "resources/shader_source/deep_map_fshader.frag",
    //                        "resources/shader_source/deep_map_gshader.gs");
    // ShaderProgram shader_map("resources/shader_source/shadow_map_vshader.vert", 
    //                          "resources/shader_source/shadow_map_fshader.frag");

    Shader deep_map("resources/shader_source/deep_map_vshader.vert", 
                    "resources/shader_source/deep_map_fshader.frag",
                    "resources/shader_source/deep_map_gshader.gs");
    Shader shader_map("resources/shader_source/shadow_map_vshader.vert", 
                             "resources/shader_source/shadow_map_fshader.frag");

    unsigned int textureFloor = loadTexture("resources/image/floor.jpg");
    unsigned int textureSand = loadTexture("resources/image/sand.png");
    unsigned int textureDirt = loadTexture("resources/image/dirt.jfif");
    unsigned int textureBedrock = loadTexture("resources/image/bedrock.jpg");
    unsigned int textureDiamond = loadTexture("resources/image/diamond_ore.jfif");

    unsigned int Texture[] = {textureFloor, textureDiamond, textureDirt, textureDirt, textureDiamond};

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBOs[3];
    unsigned int depthMaps[3];
    for (int i = 0; i < 3; i ++){    
        glGenFramebuffers(1, &depthMapFBOs[i]);
        // create depth texture
        
        glGenTextures(1, &depthMaps[i]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMaps[i]);

        for (int i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

        // create depth texture
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[i]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMaps[i], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // texture 1
    // ---------
    
    shader_map.use();
    shader_map.setInt("depthMap[0]", 1);
    shader_map.setInt("depthMap[1]", 2);
    shader_map.setInt("depthMap[2]", 3);
    shader_map.setInt("diffuseTexture", 0);

    float Verticese[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    Buffer_data render_data;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticese), &Verticese, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*5));
    glBindVertexArray(0);

    float flat_Verticese[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  10.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  10.0f, 10.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  10.0f, 10.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  10.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  10.0f, 10.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 10.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 10.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  10.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  10.0f, 10.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 10.0f, 0.0f, 1.0f, 0.0f,

    };

    Buffer_data flat_data;
    glBufferData(GL_ARRAY_BUFFER, sizeof(flat_Verticese), &flat_Verticese, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*5));
    glBindVertexArray(0);


    Floor ground_game("resources/image/floor.jpg", Main_view, 100);
    
       
    bool running = true;
    SDL_Event event;
    float move = 0;
    glm::vec3 lightPos(3.0f);

    struct Light_info{
        glm::vec3 lightPos;
        float far_plane;
        glm::vec3 color_ambient;
    };
    Light_info red_light{
        glm::vec3(1.0f, 3.0f, 0.5f),
        125.0f,
        glm::vec3(0.3f, 0.3f, 0.3f)
    };
    Light_info green_light{
        glm::vec3(3.0f, -0.5f, 0.5f),
        125.0f,
        glm::vec3(0.3f, 0.3f, 0.3f)
    };
    Light_info blue_light{
        glm::vec3(1.0f, 2.0f, 1.5f),
        125.0f,
        glm::vec3(0.3f, 0.3f, 0.3f)
    };
    Light_info Lights[] = {red_light, green_light, blue_light};

    while (running){
        move -= 0.0001;
        lightPos = glm::vec3( 3 + move, 3, 0);

        Main_view.control_mouse(screen, WIDTH_WIN, HIGHT_WIN);
        Main_view.control_moving();
        screen.clear_color(SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z, SKY_COLOR.a);
     
        for (int light = 0; light < 3; light ++)    
        {   
            float near_plane = 1.0f;
            float far_plane  = Lights[light].far_plane;
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(Lights[light].lightPos, Lights[light].lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[light]);
                glClear(GL_DEPTH_BUFFER_BIT);
                deep_map.use();
                for (int i = 0; i < 6; i ++){
                    deep_map.setMat4(("PointMatrix[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);}
                deep_map.setVec3("LightPos",Lights[light].lightPos);
                deep_map.setFloat("far_plane", far_plane);
                render_sence(deep_map, &render_data, &flat_data, Texture);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glViewport(0, 0, WIDTH_WIN, HIGHT_WIN);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_map.use();
        shader_map.setMat4("projection", Main_view.getProjection_matrix(WIDTH_WIN, HIGHT_WIN));
        shader_map.setMat4("view", Main_view.getViewMatrix());
         
        shader_map.setVec3("viewPos", Main_view.Camera_pos);
        shader_map.setInt("light_source_number", 3);

        for (int i = 0; i < 3; i++){
            shader_map.setVec3("light_sources[" + std::to_string(i) + "].lightPos", Lights[i].lightPos);
            shader_map.setFloat("light_sources[" + std::to_string(i) + "].far_plane", Lights[i].far_plane);
            shader_map.setVec3("light_sources[" + std::to_string(i) + "].color_ambient", Lights[i].color_ambient);
        }
        for (int i = 0; i < 3; i ++){
            glActiveTexture(GL_TEXTURE1 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthMaps[i]);
        }
        render_sence(shader_map, &render_data, &flat_data, Texture);
        for (int i = 0; i < 3; i++){
            render_light_point(0.2, &render_data, Lights[i].lightPos, shader_map, Lights[i].color_ambient);
        }
        //ground_game.render_floor(Main_view);
 
        skybox_game.render_skybox(Main_view);
        
        while (SDL_PollEvent(&event)) {
            if (SDL_QUIT == event.type) {
                running = false;
                SDL_Quit();
            }
            if (SDL_KEYDOWN == event.type) {
                if (SDL_SCANCODE_TAB == event.key.keysym.scancode) {
                    if (SDL_GetRelativeMouseMode()) {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
                    else {
                        screen.swap_mouse(WIDTH_WIN / 2, HIGHT_WIN / 2);
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                    }
                }
            } 
        }        
        screen.swap_buffer();
    }
    skybox_game.skybox_shader->purge();
    
    glDeleteProgram(shader_map.ID);
    glDeleteProgram(deep_map.ID);
    return 0;

}
