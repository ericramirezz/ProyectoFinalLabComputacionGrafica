#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float brightness; // > 1.0 para efecto de brillo extra

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    color = vec4(texColor.rgb * brightness, texColor.a);
}
