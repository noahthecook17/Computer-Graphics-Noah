#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform float time;

void build_house(vec4 position)
{    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
    EmitVertex();
    EndPrimitive();
}

layout (std140) uniform Matrices
{
						// base alignment	// aligned offset
	mat4 view;			// 16				// 0
						// 16				// 16
						// 16				// 32
						// 16				// 48
	mat4 projection;	// 16				// 64
						// 16				// 80
						// 16				// 96
						// 16				// 112
};

vec3 getNormal(){
    vec3 line1 = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 line2 = vec3(gl_in[1].gl_Position) - vec3(gl_in[2].gl_Position);
    return normalize(cross(line1, line2));
}

void explode(vec4 position, vec3 normal){
    if(time > 5.0){
        if(time > 5.8){
            EndPrimitive();
        }
        gl_Position = position + vec4(normal * (time - 5.0) * 30, 1.0f);
    }
    else
        gl_Position = position;

    EmitVertex();
}

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + 
                                vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main(){
	//build_house(gl_in[0].gl_Position);
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
};