#include "Ground_face.hpp"

Floor::Floor(const char* floor_image_path, Camera View, float radius_load){
    this->shader_floor = new ShaderProgram("resources/shader_source/floor_vshader.vert", "resources/shader_source/floor_fshader.frag");
    this->texture = loadTexture(floor_image_path);
   
    this->shader_floor->activate();
    this->shader_floor->uniformInt("texture_floor", 0);

    this->radius_load = radius_load;
    std::vector<glm::vec3> offset;
    
    float x_pos_min = std::floor(View.Camera_pos.x) - radius_load;
    float z_pos_min = std::floor(View.Camera_pos.z) - radius_load;

    for (int x = 0; x <= radius_load*2; x++){
    for (int z = 0; z <= radius_load*2; z++){
        offset.insert(offset.end(), glm::vec3(x_pos_min + x, 0, z_pos_min + z));
    }}     

    // store instance data in an array buffer
    // --------------------------------------
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
    
    this->quad_data = new Buffer_data();
    
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
}

void Floor::render_floor(Camera View){
    shader_floor->activate();
    shader_floor->uniformMat4f("projection", 1, false, View.getProjection_matrix(WIDTH_WIN, HIGHT_WIN)); ;
    shader_floor->uniformMat4f("model",1, false, 
            View.getModelMatrix(glm::vec3(std::floor(View.Camera_pos.x),0,std::floor(View.Camera_pos.z)), Angle_rotate, VECTOR_ROTATE));
    shader_floor->uniformMat4f("view",1 , false, View.getViewMatrix());


    // draw 100 instanced quads
    glBindVertexArray(quad_data->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (radius_load*2 + 1)*(radius_load*2 + 1));// 100 triangles of 6 vertices each
    glBindVertexArray(0);
}