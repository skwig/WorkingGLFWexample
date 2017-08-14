#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_tex_coord;
                      out vec3 color;
                      out vec2 tex_coord;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(in_position, 1.0);
    color = in_color;
    tex_coord = vec2(in_tex_coord.x, 1.0 - in_tex_coord.y);
}