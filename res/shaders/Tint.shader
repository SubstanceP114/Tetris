#shader vert
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;

uniform mat4 u_MVP;

out vec4 v_Color;
out vec2 v_Texcoord;

void main(){
	gl_Position = u_MVP * position;
	v_Color = color;
	v_Texcoord = texcoord;
}

#shader frag
#version 330 core

out vec4 o;

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_Texcoord;

void main(){
	o = v_Color * texture(u_Texture, v_Texcoord);
}