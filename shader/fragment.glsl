#version 150 core

in vec2 Texcoord;

uniform sampler2D tex;
uniform float in_Time;
uniform float in_Amplitude;
uniform vec2  in_Resolution;

uniform sampler2D in_AudioData;

out vec4 out_Color;

void main()
{
    vec2 uv = gl_FragCoord.xy / in_Resolution.y;
    vec2 uv_size = in_Resolution / in_Resolution.y;
    uv -= 0.5 * uv_size;

    out_Color = vec4(0.88, 0.88, 0.88, 1.0);

    int tx = int(((uv.x + 0.5 * uv_size.x) / uv_size.x) * 512.0);
    float wave = texelFetch(in_AudioData, ivec2(tx, 0), 0).x;

    if (abs(uv.y) < in_Amplitude)
        out_Color = vec4(1.0, 0.0, 0.0, 1.0);
}