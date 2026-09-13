#version 330 core

in vec3 ourColor;
in vec3 localPosition;

out vec4 color;

uniform vec3 objectColor;

void main()
{
    // =========================================================
    // COLOR BASE
    // =========================================================

    vec3 baseColor =
        objectColor;


    // =========================================================
    // COLOR DE LA CARA
    //
    // ourColor viene del VBO original.
    // Sirve para identificar cada cara.
    // =========================================================

    vec3 faceColor =
        mix(
            vec3(0.65, 0.65, 0.65),
            vec3(1.0, 1.0, 1.0),
            ourColor
        );


    // =========================================================
    // DEGRADADO VERTICAL
    //
    // localPosition.y va aproximadamente de:
    //
    // -0.5 -> parte inferior
    //  0.5 -> parte superior
    //
    // Lo convertimos al rango 0-1.
    // =========================================================

    float gradient =
        localPosition.y + 0.5;


    // =========================================================
    // MEZCLA DE TONALIDADES
    // =========================================================

    vec3 darkColor =
        baseColor * 0.60;

    vec3 lightColor =
        baseColor * 1.15;


    vec3 gradientColor =
        mix(
            darkColor,
            lightColor,
            gradient
        );


    // =========================================================
    // COMBINAR COLOR DEL CUBO CON COLOR DE LA CARA
    // =========================================================

    vec3 finalColor =
        gradientColor * faceColor;


    // Evitar colores superiores a 1
    finalColor =
        min(
            finalColor,
            vec3(1.0)
        );


    color =
        vec4(
            finalColor,
            1.0
        );
}
