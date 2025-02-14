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

    ShaderProgram floor_shader("resources/shader_source/floor_vshader.vert", "resources/shader_source/floor_fshader.frag");

    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("resources/image/floor.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    floor_shader.activate();
    floor_shader.uniformInt("texture_floor", 0);

    int radius_load = 300;
    std::vector<glm::vec3> offset;
    
    float x_pos_min = std::floor(Main_view.Camera_pos.x) - radius_load;
    float z_pos_min = std::floor(Main_view.Camera_pos.z) - radius_load;

    for (int x = 0; x <= radius_load*2; x++){
    for (int z = 0; z <= radius_load*2; z++){
        offset.insert(offset.end(), glm::vec3(x_pos_min + x, 0, z_pos_min + z));
    }}     

    // store instance data in an array buffer
    // --------------------------------------
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offset.size(), offset.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


 
    float block_float_width = 1;
    float quad_floor[] = {
        -block_float_width/2, 0.0f, -block_float_width/2, 0.0f, 1.0f,
         block_float_width/2, 0.0f, -block_float_width/2, 1.0f, 1.0f,
         block_float_width/2, 0.0f,  block_float_width/2, 1.0f, 0.0f,
        -block_float_width/2, 0.0f, -block_float_width/2, 0.0f, 1.0f,
         block_float_width/2, 0.0f,  block_float_width/2, 1.0f, 0.0f,
        -block_float_width/2, 0.0f,  block_float_width/2, 0.0f, 0.0f
    }; 
    

    Buffer_data quad_data;
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_floor), quad_floor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    
    
    bool running = true;
    SDL_Event event;
    while (running){
        Main_view.control_mouse(screen, WIDTH_WIN, HIGHT_WIN);
        Main_view.control_moving();
        screen.clear_color(SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z, SKY_COLOR.a);
   
        floor_shader.activate();
        floor_shader.uniformMat4f("projection", 1, false, Main_view.getProjection_matrix(WIDTH_WIN, HIGHT_WIN)); ;
        floor_shader.uniformMat4f("model",1, false, 
                Main_view.getModelMatrix(glm::vec3(std::floor(Main_view.Camera_pos.x),0,std::floor(Main_view.Camera_pos.z)), Angle_rotate, VECTOR_ROTATE));
        floor_shader.uniformMat4f("view",1 , false, Main_view.getViewMatrix());
    
 
        glBindVertexArray(quad_data.VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (radius_load*2 + 1)*(radius_load*2 + 1));// 100 triangles of 6 vertices each
        glBindVertexArray(0);

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
    floor_shader.purge();
    skybox_game.skybox_shader->purge();
    return 0;

}

// ShaderProgram floor_shader("resources/shader_source/floor_vshader.vert", "resources/shader_source/floor_fshader.frag");

//     unsigned int texture1;
//     // texture 1
//     // ---------
//     glGenTextures(1, &texture1);
//     glBindTexture(GL_TEXTURE_2D, texture1);
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // set texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     // load image, create texture and generate mipmaps
//     int width, height, nrChannels;
//     stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//     unsigned char* data = stbi_load("resources/image/floor.jpg", &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture" << std::endl;
//     }
//     stbi_image_free(data);
//     floor_shader.activate();
//     floor_shader.uniformInt("texture_floor", 0);

//     int radius_load = 300;
//     std::vector<glm::vec3> offset;
    
//     float x_pos_min = std::floor(Main_view.Camera_pos.x) - radius_load;
//     float z_pos_min = std::floor(Main_view.Camera_pos.z) - radius_load;

//     for (int x = 0; x <= radius_load*2; x++){
//     for (int z = 0; z <= radius_load*2; z++){
//         offset.insert(offset.end(), glm::vec3(x_pos_min + x, 0, z_pos_min + z));
//     }}     

//     // store instance data in an array buffer
//     // --------------------------------------
//     unsigned int instanceVBO;
//     glGenBuffers(1, &instanceVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offset.size(), offset.data(), GL_STATIC_DRAW);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);


 
//     float block_float_width = 1;
//     float quad_floor[] = {
//         -block_float_width/2, 0.0f, -block_float_width/2, 0.0f, 1.0f,
//          block_float_width/2, 0.0f, -block_float_width/2, 1.0f, 1.0f,
//          block_float_width/2, 0.0f,  block_float_width/2, 1.0f, 0.0f,
//         -block_float_width/2, 0.0f, -block_float_width/2, 0.0f, 1.0f,
//          block_float_width/2, 0.0f,  block_float_width/2, 1.0f, 0.0f,
//         -block_float_width/2, 0.0f,  block_float_width/2, 0.0f, 0.0f
//     }; 
    

//     Buffer_data quad_data;
//     glBufferData(GL_ARRAY_BUFFER, sizeof(quad_floor), quad_floor, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//     glBindBuffer(GL_ARRAY_BUFFER, 0);

//     glEnableVertexAttribArray(2);
//     glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
//     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    