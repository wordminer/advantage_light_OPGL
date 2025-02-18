#include "camera.hpp"
#include "const.hpp"
#include "window_driver.hpp"

#include <iostream>
#include "skybox.hpp"


int main(int argv, char *argc[]){
    Window screen("shadow_map", WIDTH_WIN, HIGHT_WIN, SDL_INIT_VIDEO, SDL_WINDOW_OPENGL);
    
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

    ShaderProgram deep_map("resources/shader_source/deep_map_vshader.vert", 
                           "resources/shader_source/deep_map_fshader.frag");
    ShaderProgram shadow_map("resources/shader_source/simple_shadow_map_vshader.vert", 
                             "resources/shader_source/simple_shadow_map_fshader.frag");

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shadow_map.activate();
    shadow_map.uniformInt("deepMap", 0);

    float Verticese[] = {  
        // ground faces  
        -10.0f, 0.0f, -10.0f, 0.0f, 1.0f,
         10.0f, 0.0f, -10.0f, 0.0f, 0.0f,
         10.0f, 0.0f,  10.0f, 1.0f, 1.0f,
        -10.0f, 0.0f, -10.0f, 0.0f, 1.0f,  
         10.0f, 0.0f,  10.0f, 1.0f, 1.0f,
        -10.0f, 0.0f,  10.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, 0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f, 1.0f, 0.0f
    };

    Buffer_data render_data;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticese), &Verticese, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
    glBindVertexArray(0);

    float Vertices_quad[] = { 
        // positions        // texture Coords
        -1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f,  1.0f, 1.0f, 1.0f,
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    };
    Buffer_data quad_data;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_quad), &Vertices_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
    glBindVertexArray(0);
    
    
    bool running = true;
    SDL_Event event;
    float move = 0;
    while (running){
        //move -= 0.001;
        Main_view.control_mouse(screen, WIDTH_WIN, HIGHT_WIN);
        Main_view.control_moving();
        screen.clear_color(SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z, SKY_COLOR.a);
    
        deep_map.activate();
        glm::mat4 view, projection, model;
        float near = 0.7f, far = 10.0f;
        glm::vec3 LightPos(5.0f);  
  
        // projection =  glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
        projection = glm::perspective(glm::radians(45.0f), SHADOW_WIDTH / static_cast<float>(SHADOW_HEIGHT), near, far);
        view = glm::lookAt(LightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
        model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f));
        //model = glm::scale(model, glm::vec3(0.0f));
        glm::mat4 lightMatrix = projection * view;

        deep_map.uniformMat4f("Light_matrix", 1, false, lightMatrix);
        deep_map.uniformMat4f("model", 1, false, model);

        //render_deep_map
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glBindVertexArray(render_data.VAO);
            glDrawArrays(GL_TRIANGLES, 0, 12);
            glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
 
        glViewport(0, 0, WIDTH_WIN, HIGHT_WIN);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shadow_map.activate();
        shadow_map.uniformMat4f("Light_matrix", 1, false, lightMatrix);
        shadow_map.uniformMat4f("projection", 1, false, Main_view.getProjection_matrix(WIDTH_WIN, HIGHT_WIN));
        shadow_map.uniformMat4f("view", 1, false, Main_view.getViewMatrix());
        shadow_map.uniformMat4f("model", 1, false, Main_view.getModelMatrix(glm::vec3(0.0f), Angle_rotate, VECTOR_ROTATE));
        
        shadow_map.uniformFloat("near_plane", near);
        shadow_map.uniformFloat("far_plane", far);

        glActiveTexture(GL_TEXTURE0);  
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(render_data.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
 
        //skybox_game.render_skybox(Main_view);
        
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
    deep_map.purge();
    shadow_map.purge();
    return 0;

}
