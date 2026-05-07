#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    vec3 coords = TexCoords;

    if (coords.y < 0.0 && abs(coords.y) >= abs(coords.x) && abs(coords.y) >= abs(coords.z))
    {
        float tilingFactor = 15; 

        vec2 uv = coords.xz / abs(coords.y);
        
        uv = uv * 0.5 + 0.5;
        
        uv = fract(uv * tilingFactor);
        
        uv = uv * 2.0 - 1.0;
      
        coords.xz = uv * abs(coords.y);
    }

    color = texture(skybox, coords);
}

