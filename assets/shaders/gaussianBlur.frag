#version 330 core

out vec4 fragColor;

in vec2 vTextureCoord;

uniform sampler2D image;
uniform bool horizontal;
//uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
uniform float weight[9] = float[] (0.000229,0.005977,0.060598,0.241732,0.382928,0.241732,0.060598,0.005977, 0.000229);


void main() {
    vec2 tex_offset = 1.0 / textureSize(image, 0);
    vec3 result = texture(image, vTextureCoord).rgb * weight[0];
    if(horizontal)
    {
        for(int i = 1; i < 9; ++i)
        {
            result += texture(image, vTextureCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, vTextureCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 9; ++i)
        {
            result += texture(image, vTextureCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, vTextureCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    fragColor = vec4(result, 1.0);
}
