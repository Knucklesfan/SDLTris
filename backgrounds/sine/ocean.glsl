// From https://www.shadertoy.com/view/XdtfRB
// Credits to sangwhan
// Modified by @y4my4my4m
//TODO: will make this customizable via GUI

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 vUV;

uniform float time;
uniform sampler2D texture0;

vec3 C = vec3(0.12, 0.11, 0.37);
float GWM = 1.15;
float TM = 0.25;

float getAmp(float frequency) {
    return 0.0;
}

float getWeight(float f) {
    return (getAmp(f - 2.0) + getAmp(f - 1.0) + \
            getAmp(f + 2.0) + getAmp(f + 13.0) + \
            getAmp(f)) / 5.0;
}

void main(  )
{
      //vec3 backdrop = mix(C, C, C);
      vec3 backdrop;
  	  vec2 uvTrue = vUV;
      vec2 uv = 2.5 * uvTrue - 1.33;

  	  float li;
      float gw;
      float ts;
      float tsr;
      float tsg;
      float tsb;

      float cr;
      float cg;
      float cb;
      vec3 color = vec3(0.0);

	for(float i = 0.0; i < 5.0; i++) {
		  uv.y += (0.2 * sin(uv.x + i / 7.0 - time * 0.4));
      float Y = uv.y + getWeight(pow(i, 2.0) * 20.0) * (0 - 0.5);
      li = 0.4 + pow(1.2 * abs(mod(uvTrue.x + i / 1.1 + time, 2.0) - 1.0), 2.0);
	    gw = abs(li / (150.0 * Y));

      ts  = gw * (GWM + sin(time * TM));
      tsr = gw * (GWM + sin(time * TM * 1.10));
      tsg = gw * (GWM + sin(time * TM * 1.20));
      tsb = gw * (GWM + sin(time * TM * 1.50));
	    color += vec3(tsr, tsg, tsb);

      backdrop = mix(C * normalize(color), C * normalize(color), C * normalize(color));
	}

	FragColor = vec4(color + backdrop, 0.5);
}