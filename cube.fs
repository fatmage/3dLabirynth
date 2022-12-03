#version 330 core

// Ouput data

precision highp float;
varying vec3 worldPos;
uniform float N;

float pulse(float val, float dst, float dist) {
  return floor((mod(val*dst,1.0)+.5) * dist);
}

void main()
{
  vec3 dir = vec3(0,1,0); // high noon
  
  vec3 cpos = worldPos;
  
  const float d = 5.0;
  float dist = (worldPos.x + worldPos.y + worldPos.z)/(N/3);

  // note: I don't do Z since it leads to weird patterns
  float bright = pulse(cpos.x,d,dist) + pulse(cpos.y,d,dist) + pulse(cpos.z,d,dist);

  vec3 color = mod(bright,2.0) > .5 ? vec3(0.1,0.1,0.1): vec3(0.8,0.8,0.8); 


  gl_FragColor = vec4(color, 1.0);
}

