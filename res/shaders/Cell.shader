#shader vert
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 offset;

uniform mat4 u_MVP;

out vec4 v_Color;

void main(){
	gl_Position = u_MVP * vec4(position + offset, 0.0, 1.0);
	v_Color = color;
}

#shader frag
#version 330 core

in vec4 v_Color;

out vec4 o;

void main(){
	o = v_Color;
}