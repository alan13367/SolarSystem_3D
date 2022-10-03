#version 330
 
in vec3 a_vertex;
in vec3 a_color;
in vec3 a_normal; 
uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat3 u_normal;
out vec3 v_normal; 
out vec3 v_vertex;

// STEP 2: add a attribute: a_uv and varying v_uv here
in vec2 a_uv;  
out vec2 v_uv; 

out vec3 v_color;

void main()
{
	//gl_Position = vec4( a_vertex , 1.0 );
	gl_Position = u_projection * u_view * u_model * vec4( a_vertex , 1.0 );

	// pass the colour to the fragment shader
	v_color = a_color;

	v_vertex = a_vertex;

	v_normal = u_normal * a_normal;

    // STEP 2: pass the uv coordinates to the fragment shader
	v_uv = a_uv;
}

