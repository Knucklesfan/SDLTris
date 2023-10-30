/**
 * @author jonobr1 / http://jonobr1.com/
 */
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 vUV;

uniform float time;
uniform sampler2D texture0;

// void main() {
//     vec2 st = vUV.xy/vec2(640,480);
//     float pct = 0.0;

//     // a. The DISTANCE from the pixel to the center
//     pct = distance(vUV,vec2(0.5));

//     // b. The LENGTH of the vector
//     //    from the pixel to the center
//     // vec2 toCenter = vec2(0.5)-st;
//     // pct = length(toCenter);

//     // c. The SQUARE ROOT of the vector
//     //    from the pixel to the center
//     // vec2 tC = vec2(0.5)-st;
//     // pct = sqrt(tC.x*tC.x+tC.y*tC.y);

//     vec3 color = vec3(pct);


// 	gl_FragColor = vec4( color, 1.0 );

// }

// From https://www.shadertoy.com/view/wlsczH
// Credits to thefox231
// Modified by @y4my4my4m
const float thickness = 0.02;
const vec3 mainColor = vec3(0.32, .51, .75);

bool equals(float a, float b) {
    return abs(a - b) < thickness;
}

float sum(vec2 a) {
    return a.x + a.y;
}
float sum(vec3 a) {
    return a.x + a.y + a.z;
}

mat2 rotate2d(float angle) {
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}

void main() {
    vec2 uv = vUV;

    vec3 color = vec3(.0);

    // fix aspect ratio
    uv.x *= 640 / 480;

    // pixelate
    float pxAmt = 256.0;

    uv.x = floor(uv.x * pxAmt) / pxAmt;
    uv.y = floor(uv.y * pxAmt) / pxAmt;

    vec2 uvOrig = uv;

    // unfix aspect ratio
    uvOrig.x /= 640 / 480;

    // interlacing .
    float pixAmt = 2.;
    if (mod(vUV.y*480, pixAmt) < pixAmt * 0.5) {
        uv += 0.1 + tan(time * 0.2 + uv.y * 8.) * 0.05;
    } else {
        uv -= 0.1 + tan(time * 0.2 + uv.y * 8. + .5) * 0.05;
    }

    // move ing...
    uv.x += sin(time * 1.6) * 0.3;
    uv.y += cos(time * .2) * 2.5;

    // rotate......
    uv.xy *= rotate2d(time * 0.15);

    // bg waves
    if (equals(sin(uvOrig.x * 90. + time) * 0.1 + 0.2, mod(uvOrig.y + sin(time * 0.4) * 0.2 + sin(uv.x) * 0.3, 0.3146))) {
        color += mainColor * 0.2;
    }
    if (equals(sin(uvOrig.y * 50. + time) * 0.1 + 0.2, mod(uvOrig.x + sin(time * 0.7) * 0.2 + sin(uv.y) * 0.3, 0.3146))) {
        color += mainColor * 0.2;
    }

    // rotating squares
    // repeat them
    float repeatNum = 4.3;
   	uv = mod(uv, 1. / repeatNum) * repeatNum;

    vec2 seed = floor(uv);

    float brightness = 1.0;

    vec3 squareColor = vec3(.0);
    vec2 squarePos = vec2(.5, .5);
    squareColor = mainColor * brightness * sum(abs(uv - squarePos));
    if (sum(abs(uv - squarePos)) < .6) {
        squareColor -= 0.2;
    }

    // glowy......
    squareColor += sin(time) * 0.1;

    if (sum(squareColor) > .1) {
    	color = squareColor;
    }

    // also add some circles, for good measure
    uv = uvOrig;

    // move ing...
    uv.x += sin(time * .6) * 0.7;
    uv.y += cos(time * .9) * 1.5;

    // rotate......
    uv.xy *= rotate2d(time * 0.18);

    // fix aspect ratio
    uv.x *= 640 / 480;

    // interlacing .
    if (mod(vUV.y*480, pixAmt) < pixAmt * 0.5) {
        uv += 0.2 + sin(time * 0.2 + uv.y * 8.) * 0.05;
    } else {
        uv -= 0.2 + cos(time * 0.5 + uv.y * 8. + .5) * 0.05;
    }

    repeatNum = 1.3;
   	uv = mod(uv, 1. / repeatNum) * repeatNum;

    vec2 circlePosition = vec2(.5, .5);
    color += mainColor * 1.0 - length(circlePosition - uv);

    // color shortening
    // gives it a kind of like snes-like palette
    float shortAmt = 14.0;
    color = ceil(color * shortAmt) / shortAmt;

    FragColor = vec4(color, 1.0);
}