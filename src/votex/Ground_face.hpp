#ifndef GROUND_FACE_HPP
#define GROUND_FACE_HPP

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "handle_data.hpp"
#include "glm.hpp"
#include <vector>

struct Floor{
    ShaderProgram *shader_floor;
    Buffer_data* quad_data;
    float Angle_rotate[3] = {0,0,0};
    float radius_load;

    unsigned int texture;
    unsigned int instanceVBO;
    Floor(const char* floor_image_path, Camera View, float radius_load);
    void render_floor(Camera View);
};

#endif