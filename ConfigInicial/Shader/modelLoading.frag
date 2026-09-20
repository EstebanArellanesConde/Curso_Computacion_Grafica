#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

/*
Indicates whether the current mesh has
a diffuse texture.
*/
uniform bool hasTexture;

/*
Diffuse material color obtained from
the Kd value in the .mtl file.
*/
uniform vec3 materialDiffuse;

void main()
{
    if (hasTexture)
    {
        /*
        Textured model.
        Example:
        RedDog.obj + Texture_albedo.jpg
        */
        FragColor =
            texture(
                texture_diffuse1,
                TexCoords
            );
    }
    else
    {
        /*
        Material-color model.
        Example:
        car_project.obj + car_project.mtl
        */
        FragColor =
            vec4(
                materialDiffuse,
                1.0
            );
    }
}