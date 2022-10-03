#version 330

out vec4 fragColor;
in vec3 v_vertex;
in vec3 v_normal; 
in vec3 v_color;


// STEP 3: get the uv coordinates form the vertex shader
in vec2 v_uv;

uniform sampler2D u_texture;
uniform vec3 u_light_dir;

uniform vec3 u_light_color;
uniform vec3 u_cam_pos;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;

void main(void)
{

	vec3 texture_color = texture(u_texture, v_uv).xyz;
	
	vec3 N = normalize(v_normal); 
	vec3 L = normalize(u_light_dir - v_vertex); 

	vec3 R = normalize(-reflect(L, N) ); 
	vec3 E = normalize(u_cam_pos - v_vertex); 
	float RdotE = max( dot(R,E) , 0.0 );
	float NdotL = max ( dot (N,L) , 0.0 );

	vec3 ambient_color = texture_color * u_ambient;
	vec3 diffuse_color = texture_color * NdotL;
	vec3 spec_color = texture_color * pow (RdotE, u_shininess);
	
	vec3 final_color = ambient_color + diffuse_color + spec_color;
	fragColor =  vec4(final_color, 1.0);
}
