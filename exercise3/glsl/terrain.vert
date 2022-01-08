#version 330
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

in vec4 position;

out vec3 normal;


uniform mat4 mvp;

//Returns the height of the procedural terrain at a given xz position
float getTerrainHeight(vec2 p);



void main()
{
    vec4 temp_position = position;
    vec2 p = vec2(position.x, position.z);
    temp_position.y = getTerrainHeight(p);
	gl_Position = mvp * temp_position;

	vec3 x0 = vec3(temp_position.x-1, temp_position.y, temp_position.z);
	vec3 x2 = vec3(temp_position.x+1, temp_position.y, temp_position.z);

	vec3 z0 = vec3(temp_position.x, temp_position.y, temp_position.z-1);
	vec3 z2 = vec3(temp_position.x, temp_position.y, temp_position.z+1);

	x0.y = getTerrainHeight(vec2(x0.x,x0.z));
    x2.y = getTerrainHeight(vec2(x2.x,x2.z));
    z0.y = getTerrainHeight(vec2(z0.x,z0.z));
    z2.y = getTerrainHeight(vec2(z2.x,z2.z));

    vec3 xx = vec3(x0.x - x2.x, x0.y - x2.y, x0.z - x2.z);
    vec3 zz = vec3(z0.x - z2.x, z0.y - z2.y, z0.z - z2.z);

    normal = normalize(cross(xx,zz));

}

//source: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 c)
{
	return 2 * fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453) - 1;
}

float perlinNoise(vec2 p )
{
	vec2 ij = floor(p);
	vec2 xy = p - ij;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(3.1415926 * xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

//based on https://www.seedofandromeda.com/blogs/58-procedural-heightmap-terrain-generation
float getTerrainHeight(vec2 p)
{
	float total = 0.0;
	float maxAmplitude = 0.0;
	float amplitude = 1.0;
	float frequency = 0.02;
	for (int i = 0; i < 11; i++) 
	{
		total +=  ((1.0 - abs(perlinNoise(p * frequency))) * 2.0 - 1.0) * amplitude;
		frequency *= 2.0;
		maxAmplitude += amplitude;
		amplitude *= 0.45;
	}
	return 15 * total / maxAmplitude;
}
