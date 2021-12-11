#version 130
// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

in vec4 fragment_color;
in vec4 out_position;

out vec4 color;

uniform vec2 c;
uniform float m;

void main(void)
{
	//color = fragment_color * 0.5 + vec4(0.5);
    //color = fragment_color;
	 /**** Begin of tasks ***
	 - 2.2.5
	 Implement the pseudo-code for calculating the julia fractal at a point.
	 To get the point you can declare a new "in" variable which contains the
	 position and just use the X- and Y- value. 

	 *** End of tasks ***/
	 //vec2 c = vec2(0.45, -0.3);
	 //float m = 1;
	 vec4 p = out_position;
	 vec4 z = p * m;
	 int imax = 200;

	 float x = 0;
	 float y = 0;
     int i = 1;

	 for(i = 1; i < imax; i++){
	    x = (z.x * z.x - z.y * z.y) + c.x;
	    y = (z.y * z.x + z.x * z.y) + c.y;
	    if((x*x + y*y) > 4){
	        break;
	    }
	    z.x = x;
	    z.y = y;
	 }
	 float alpha = 0;
	 if(i < imax){
	    alpha = float(i)/float(imax);
	 }
	 vec4 a = vec4(alpha, alpha, alpha, alpha);
	 color = a * 10 * fragment_color;
	
}