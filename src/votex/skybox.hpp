#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "shader.hpp"
#include "handle_data.hpp"
#include "stb_image.h"
#include "camera.hpp"
#include "const.hpp"

struct Skybox{
    ShaderProgram* skybox_shader;
    Buffer_data* skybox_buffer_data;
    unsigned int skybox_tex;

    Skybox(const char* vshaderPath, const char* fshaderPath, std::vector<std::string> image_paths);
    unsigned int load_cubemap(std::vector<std::string> faces);

    void render_skybox(Camera game_view);
};

#endif 