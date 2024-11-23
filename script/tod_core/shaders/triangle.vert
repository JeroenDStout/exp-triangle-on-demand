#version 450

layout (set = 1, binding = 0) uniform unf_transform {
  mat4x4 tr_object;
  mat4x4 tr_view;
  mat4x4 tr_proj;
};

layout (location = 0) in vec3  in_position;
layout (location = 1) in vec4  in_colour;

layout (location = 0) out vec4 out_colour;

void main()
{
    out_colour  = in_colour;
    
    vec4 out_pos = vec4(in_position, 1);

    out_pos = tr_object * out_pos;
    out_pos = tr_view   * out_pos;
    out_pos = tr_proj   * out_pos;

	gl_Position = out_pos;
}
