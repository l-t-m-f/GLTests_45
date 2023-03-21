#version 460 core
uniform vec4 input_color;
out vec4 frag_color;
void main() {
    frag_color = input_color;
}