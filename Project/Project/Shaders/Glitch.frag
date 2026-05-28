#version 460 core
in vec2 v_UV;
out vec4 FragColor;

uniform sampler2D u_Scene;
uniform float u_Time;
uniform float u_Amount;
uniform vec2 u_Resolution; 

float hash(vec2 p)
{
    p = fract(p*vec2(123.34, 456.21));
    p += dot(p, p+45.32);
    return fract(p.x*p.y);
}

void main()
{
    float line = floor(v_UV.y * u_Resolution.y);
    float tear = (hash(vec2(line, floor(u_Time*10.0))) - 0.5) * 0.02 * u_Amount;

    vec2 blockId = floor(v_UV * vec2(32.0, 18.0));
    float blockNoise = hash(blockId + floor(u_Time*10.0));
    vec2 blockShift = vec2((blockNoise - 0.5) * 0.01 * u_Amount);

    vec2 uvR = v_UV + vec2( 0.002*u_Amount, tear) + blockShift;
    vec2 uvG = v_UV + vec2(-0.002*u_Amount, tear) + blockShift;
    vec2 uvB = v_UV + vec2( 0.000*u_Amount, tear) + blockShift;

    vec3 col;
    col.r = texture(u_Scene, uvR).r;
    col.g = texture(u_Scene, uvG).g;
    col.b = texture(u_Scene, uvB).b;

    float band = sin(v_UV.y*400.0 + u_Time*15.0)*0.02*u_Amount;
    col += band;

    FragColor = vec4(col, 1.0);
}
