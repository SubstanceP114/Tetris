#shader vert
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_MVP;

out vec4 v_Color;

void main(){
	gl_Position = u_MVP * position;
	v_Color = color;
}

#shader frag
#version 330 core

out vec4 o;

in vec4 v_Color;

void main(){
	o = v_Color;
}