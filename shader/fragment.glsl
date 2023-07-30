#version 150 core

in vec2 Texcoord;

uniform sampler2D tex;
uniform float in_Time;
uniform float in_Amplitude;
uniform vec2  in_Resolution;

out vec4 out_Color;

float wave(float x, float amplitude)
{
    return amplitude * sin(50.0 * x);
    // return amplitude * (sin(50.0 * x) > 0 ? 1.0 : -1.0);
    // return amplitude * asin(sin(50.0 * x)) * (2.0 / 3.14);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / in_Resolution.y;
    uv -= 0.5 * in_Resolution / in_Resolution.y;
    // uv.x *= 5.0;
    uv.y *= 2.0;
    
    out_Color = vec4(0.88, 0.88, 0.88, 1.0);

    // if (uv.x * uv.x + uv.y * uv.t < in_Amplitude * in_Amplitude)
    // {
    //     vec3 c = vec3(0.55, 0.3, smoothstep(0.88, 0.15, abs(uv.y - in_Amplitude)));
    //     out_Color = vec4(c, 1.0);
    // }

    if (abs(wave(uv.x, in_Amplitude) - uv.y) < 0.0)
        out_Color = vec4(in_Amplitude, 0.0, 0.0, 1.0);
}