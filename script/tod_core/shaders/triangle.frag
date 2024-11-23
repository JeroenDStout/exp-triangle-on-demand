#version 450

layout (set = 3, binding = 0) uniform unf_colours_ {
  vec4 colours[3];
} unf_colours;

layout (location = 0) in vec4  in_colour;

layout (location = 0) out vec4 frag_colour;

float colour_get_saturation(vec3 c)
{
    c.gb = vec2(max(c.g, c.b), min(c.g, c.b));
	c.rg = vec2(max(c.r, c.g), min(c.r, c.g));   
	return (c.r - min(c.g, c.b)) / (c.r + 1e-7);
}

vec3 colour_saturate_to(vec3 colour, float from_sat, float to_sat)
{
	float diff = to_sat - from_sat;
    vec3  dir  = normalize(vec3(0.0001 + 2.*colour.x - colour.y - colour.z, 2.*colour.y - colour.x - colour.z, 2.*colour.z - colour.y - colour.x));
    float lgt  = dot(vec3(1.0, 1.0, 1.0), colour);
    float ff   = dot(dir, normalize(colour));
	return colour + dir * diff * ff * lgt;
}

vec3 colour_saturate_to(vec3 c, float to_sat)
{
	return colour_saturate_to(c, colour_get_saturation(c), to_sat);    
}

void main()
{
	float saturation = colour_get_saturation(unf_colours.colours[0].xyz) * in_colour.x
					 + colour_get_saturation(unf_colours.colours[1].xyz) * in_colour.y
				 	 + colour_get_saturation(unf_colours.colours[2].xyz) * in_colour.z;

	frag_colour = unf_colours.colours[0] * in_colour.x
			    + unf_colours.colours[1] * in_colour.y
				+ unf_colours.colours[2] * in_colour.z;
	frag_colour.xyz = colour_saturate_to(frag_colour.xyz, colour_get_saturation(frag_colour.xyz), saturation);
}
