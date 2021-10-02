#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
out vec3 vBC;

void main() 
{ 
    gl_Position = position; 
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 vBC;

uniform vec4 u_color;

void main() { color = u_color; };