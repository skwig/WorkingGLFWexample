#version 330 core
out vec4 frag_color;

in vec3 color;
in vec2 tex_coord;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main(){
//    frag_color = texture(texture_0, tex_coord) * vec4(color, 1.0);
    frag_color = mix(texture(texture_0, tex_coord), texture(texture_1, tex_coord), 0.2);
}