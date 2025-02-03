#shader vert
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main(){
	gl_Position = u_MVP * position;
	v_TexCoord = texcoord;
}

#shader frag
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 o;

void main(){
	o = texture(u_Texture, v_TexCoord);
}