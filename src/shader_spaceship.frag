#version 330

uniform vec3 u_color;
out vec4 fragColor;

in vec3 v_color;

void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	fragColor =  vec4(u_color,1.0);
}
