// ============================================================
// BRAZO ROBOTICO - MODELO JERARQUICO
// Computacion Grafica / CGeIHC
//
// CORRECCION:
// - Los 4 dedos principales cierran como GARARRA.
// - La flexion de los 4 dedos se realiza sobre el eje Y.
// - El pulgar tiene movimiento independiente.
// - Las mismas teclas controlan las falanges equivalentes
//   de los 4 dedos principales.
// ============================================================

#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

// ============================================================
// PROTOTIPOS
// ============================================================

void processInput(GLFWwindow* window);

void DrawCube(
    Shader& shader,
    const glm::mat4& parentMatrix,
    const glm::vec3& translation,
    const glm::vec3& scale,
    const glm::vec3& color
);

void DrawFinger(
    Shader& shader,
    const glm::mat4& palmMatrix,
    float baseY,
    float baseZ,
    float angle1,
    float angle2,
    float angle3,
    const glm::vec3& color1,
    const glm::vec3& color2,
    const glm::vec3& color3
);

void DrawThumb(
    Shader& shader,
    const glm::mat4& palmMatrix,
    float angle1,
    float angle2,
    float angle3
);

float ClampAngle(float value, float minValue, float maxValue);

// ============================================================
// CONSTANTES
// ============================================================

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// ============================================================
// GEOMETRIA DEL CUBO
// ============================================================

float vertices[] = {

    // Cara trasera
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,

     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // Cara frontal
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // Cara izquierda
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // Cara derecha
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,

     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

     // Cara inferior
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,

      0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f, -0.5f,

     // Cara superior
     -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,

      0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f
};

// ============================================================
// MOVIMIENTO DEL BRAZO
// ============================================================

float movX = 0.0f;
float movY = 0.0f;
float movZ = -11.0f;

float rot = 0.0f;

// ============================================================
// ARTICULACIONES DEL BRAZO
// ============================================================

float hombro = 0.0f;
float codo = 35.0f;
float muneca = 0.0f;

// ============================================================
// ARTICULACIONES DE LOS CUATRO DEDOS PRINCIPALES
//
// Una variable representa la misma articulacion para:
// indice + medio + anular + menique.
//
// falange1 -> articulacion base
// falange2 -> articulacion media
// falange3 -> articulacion distal
// ============================================================

float falange1 = 0.0f;
float falange2 = 0.0f;
float falange3 = 0.0f;

// ============================================================
// ARTICULACIONES DEL PULGAR
//
// Son independientes de los otros cuatro dedos.
// ============================================================

float pulgar1 = 0.0f;
float pulgar2 = 0.0f;
float pulgar3 = 0.0f;

// ============================================================
// LIMITES DEL BRAZO
// ============================================================

const float HOMBRO_MIN = -90.0f;
const float HOMBRO_MAX = 90.0f;

const float CODO_MIN = 0.0f;
const float CODO_MAX = 135.0f;

const float MUNECA_MIN = -70.0f;
const float MUNECA_MAX = 70.0f;

// ============================================================
// LIMITES DE LOS CUATRO DEDOS
//
// Se reducen los limites para evitar que atraviesen la palma.
// ============================================================

const float FALANGE1_MIN = 0.0f;
const float FALANGE1_MAX = 75.0f;

const float FALANGE2_MIN = 0.0f;
const float FALANGE2_MAX = 85.0f;

const float FALANGE3_MIN = 0.0f;
const float FALANGE3_MAX = 70.0f;

// ============================================================
// LIMITES DEL PULGAR
// ============================================================

const float PULGAR1_MIN = -20.0f;
const float PULGAR1_MAX = 65.0f;

const float PULGAR2_MIN = 0.0f;
const float PULGAR2_MAX = 75.0f;

const float PULGAR3_MIN = 0.0f;
const float PULGAR3_MAX = 70.0f;

// ============================================================
// FACTORES MECANICOS
//
// La articulacion siguiente se mueve ligeramente menos.
// Esto hace que la mano tenga una forma mas natural de garra.
// ============================================================

const float FALANGE1_FACTOR = 1.00f;
const float FALANGE2_FACTOR = 0.80f;
const float FALANGE3_FACTOR = 0.60f;

// ============================================================
// FUNCION PARA LIMITAR ANGULOS
// ============================================================

float ClampAngle(float value, float minValue, float maxValue)
{
    return glm::clamp(value, minValue, maxValue);
}

// ============================================================
// DIBUJAR CUBO
// ============================================================

void DrawCube(
    Shader& shader,
    const glm::mat4& parentMatrix,
    const glm::vec3& translation,
    const glm::vec3& scale,
    const glm::vec3& color
)
{
    glm::mat4 model = parentMatrix;

    model = glm::translate(
        model,
        translation
    );

    model = glm::scale(
        model,
        scale
    );

    GLint modelLoc =
        glGetUniformLocation(
            shader.Program,
            "model"
        );

    GLint colorLoc =
        glGetUniformLocation(
            shader.Program,
            "objectColor"
        );

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glUniform3fv(
        colorLoc,
        1,
        glm::value_ptr(color)
    );

    glDrawArrays(
        GL_TRIANGLES,
        0,
        36
    );
}

// ============================================================
// DIBUJAR UNO DE LOS CUATRO DEDOS PRINCIPALES
//
// IMPORTANTE:
//
// Los segmentos de los dedos avanzan inicialmente sobre +X.
//
// Para cerrar como GARRA:
//
//       dedo
//        |
//        |------ F1
//        |       \
//        |        \ F2
//        |         \
//        |          \ F3
//        |
//      PALMA
//
// La rotacion de cada falange se realiza sobre Y.
//
// Esto hace que el dedo se curve hacia la palma en profundidad,
// en lugar de abrirse lateralmente como unas tijeras.
// ============================================================

void DrawFinger(
    Shader& shader,
    const glm::mat4& palmMatrix,
    float baseY,
    float baseZ,
    float angle1,
    float angle2,
    float angle3,
    const glm::vec3& color1,
    const glm::vec3& color2,
    const glm::vec3& color3
)
{
    // --------------------------------------------------------
    // Dimensiones
    // --------------------------------------------------------

    const float L1 = 0.85f;
    const float L2 = 0.72f;
    const float L3 = 0.58f;

    const float W1 = 0.34f;
    const float W2 = 0.30f;
    const float W3 = 0.27f;

    const float D = 0.42f;

    // --------------------------------------------------------
    // MATRIZ TEMPORAL DE LA BASE DEL DEDO
    // --------------------------------------------------------

    glm::mat4 fingerBase = palmMatrix;

    fingerBase = glm::translate(
        fingerBase,
        glm::vec3(
            0.78f,
            baseY,
            baseZ
        )
    );

    // ========================================================
    // FALANGE 1
    // ========================================================

    glm::mat4 falange1Matrix = fingerBase;

    // Rotacion sobre Y:
    // Esto produce la flexion hacia la palma.
    falange1Matrix = glm::rotate(
        falange1Matrix,
        glm::radians(-angle1),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Dibujar segmento centrado en su propio eje
    DrawCube(
        shader,
        falange1Matrix,
        glm::vec3(L1 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L1, W1, D),
        color1
    );

    // ========================================================
    // ARTICULACION 2
    // ========================================================

    glm::mat4 falange2Matrix = falange1Matrix;

    // Avanzar hasta el final de la falange 1
    falange2Matrix = glm::translate(
        falange2Matrix,
        glm::vec3(L1, 0.0f, 0.0f)
    );

    // Rotacion LOCAL sobre Y
    falange2Matrix = glm::rotate(
        falange2Matrix,
        glm::radians(
            -angle2 * FALANGE2_FACTOR
        ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // ========================================================
    // FALANGE 2
    // ========================================================

    DrawCube(
        shader,
        falange2Matrix,
        glm::vec3(L2 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L2, W2, D * 0.92f),
        color2
    );

    // ========================================================
    // ARTICULACION 3
    // ========================================================

    glm::mat4 falange3Matrix = falange2Matrix;

    falange3Matrix = glm::translate(
        falange3Matrix,
        glm::vec3(L2, 0.0f, 0.0f)
    );

    // Segunda flexion LOCAL sobre Y
    falange3Matrix = glm::rotate(
        falange3Matrix,
        glm::radians(
            -angle3 * FALANGE3_FACTOR
        ),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // ========================================================
    // FALANGE 3
    // ========================================================

    DrawCube(
        shader,
        falange3Matrix,
        glm::vec3(L3 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L3, W3, D * 0.84f),
        color3
    );
}

// ============================================================
// PULGAR
//
// El pulgar permanece separado de las articulaciones
// sincronizadas de los otros cuatro dedos.
//
// Su orientacion inicial sale lateralmente de la palma.
// ============================================================

void DrawThumb(
    Shader& shader,
    const glm::mat4& palmMatrix,
    float angle1,
    float angle2,
    float angle3
)
{
    const float L1 = 0.70f;
    const float L2 = 0.58f;
    const float L3 = 0.48f;

    const float W1 = 0.42f;
    const float W2 = 0.36f;
    const float W3 = 0.32f;

    const float D = 0.48f;

    // ========================================================
    // BASE DEL PULGAR
    // ========================================================

    glm::mat4 thumbBase = palmMatrix;

    thumbBase = glm::translate(
        thumbBase,
        glm::vec3(
            0.55f,
            -1.55f,
            0.0f
        )
    );

    // Orientacion inicial del pulgar.
    //
    // A diferencia de los cuatro dedos principales,
    // el pulgar comienza apuntando lateralmente.
    thumbBase = glm::rotate(
        thumbBase,
        glm::radians(-35.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    // ========================================================
    // FALANGE 1 DEL PULGAR
    // ========================================================

    glm::mat4 thumb1Matrix = thumbBase;

    thumb1Matrix = glm::rotate(
        thumb1Matrix,
        glm::radians(-angle1),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    DrawCube(
        shader,
        thumb1Matrix,
        glm::vec3(L1 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L1, W1, D),
        glm::vec3(0.05f, 0.45f, 0.85f)
    );

    // ========================================================
    // FALANGE 2 DEL PULGAR
    // ========================================================

    glm::mat4 thumb2Matrix = thumb1Matrix;

    thumb2Matrix = glm::translate(
        thumb2Matrix,
        glm::vec3(L1, 0.0f, 0.0f)
    );

    thumb2Matrix = glm::rotate(
        thumb2Matrix,
        glm::radians(-angle2),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    DrawCube(
        shader,
        thumb2Matrix,
        glm::vec3(L2 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L2, W2, D * 0.92f),
        glm::vec3(0.03f, 0.35f, 0.75f)
    );

    // ========================================================
    // FALANGE 3 DEL PULGAR
    // ========================================================

    glm::mat4 thumb3Matrix = thumb2Matrix;

    thumb3Matrix = glm::translate(
        thumb3Matrix,
        glm::vec3(L2, 0.0f, 0.0f)
    );

    thumb3Matrix = glm::rotate(
        thumb3Matrix,
        glm::radians(-angle3),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    DrawCube(
        shader,
        thumb3Matrix,
        glm::vec3(L3 * 0.5f, 0.0f, 0.0f),
        glm::vec3(L3, W3, D * 0.84f),
        glm::vec3(0.08f, 0.25f, 0.65f)
    );
}

// ============================================================
// ENTRADA DE TECLADO
// ============================================================

void processInput(GLFWwindow* window)
{
    // ========================================================
    // SALIR
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            true
        );
    }

    // ========================================================
    // MOVIMIENTO X
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movX -= 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movX += 0.05f;
    }

    // ========================================================
    // MOVIMIENTO Y
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        movY += 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        movY -= 0.05f;
    }

    // ========================================================
    // MOVIMIENTO Z
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movZ += 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movZ -= 0.05f;
    }

    // ========================================================
    // ROTACION GENERAL
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rot -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rot += 1.0f;
    }

    // ========================================================
    // HOMBRO
    // R / F
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        hombro += 1.0f;

        hombro = ClampAngle(
            hombro,
            HOMBRO_MIN,
            HOMBRO_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        hombro -= 1.0f;

        hombro = ClampAngle(
            hombro,
            HOMBRO_MIN,
            HOMBRO_MAX
        );
    }

    // ========================================================
    // CODO
    // G / T
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        codo += 1.0f;

        codo = ClampAngle(
            codo,
            CODO_MIN,
            CODO_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        codo -= 1.0f;

        codo = ClampAngle(
            codo,
            CODO_MIN,
            CODO_MAX
        );
    }

    // ========================================================
    // MUÑECA
    // H / Y
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        muneca += 1.0f;

        muneca = ClampAngle(
            muneca,
            MUNECA_MIN,
            MUNECA_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        muneca -= 1.0f;

        muneca = ClampAngle(
            muneca,
            MUNECA_MIN,
            MUNECA_MAX
        );
    }

    // ========================================================
    // FALANGE 1 DE LOS 4 DEDOS
    //
    // J = cerrar
    // U = abrir
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        falange1 += 1.0f;

        falange1 = ClampAngle(
            falange1,
            FALANGE1_MIN,
            FALANGE1_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        falange1 -= 1.0f;

        falange1 = ClampAngle(
            falange1,
            FALANGE1_MIN,
            FALANGE1_MAX
        );
    }

    // ========================================================
    // FALANGE 2 DE LOS 4 DEDOS
    //
    // O = cerrar
    // L = abrir
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        falange2 += 1.0f;

        falange2 = ClampAngle(
            falange2,
            FALANGE2_MIN,
            FALANGE2_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        falange2 -= 1.0f;

        falange2 = ClampAngle(
            falange2,
            FALANGE2_MIN,
            FALANGE2_MAX
        );
    }

    // ========================================================
    // FALANGE 3 DE LOS 4 DEDOS
    //
    // P = cerrar
    // ; = abrir
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        falange3 += 1.0f;

        falange3 = ClampAngle(
            falange3,
            FALANGE3_MIN,
            FALANGE3_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
    {
        falange3 -= 1.0f;

        falange3 = ClampAngle(
            falange3,
            FALANGE3_MIN,
            FALANGE3_MAX
        );
    }

    // ========================================================
    // PULGAR
    //
    // Z / X -> primera falange
    // C / V -> segunda falange
    // B / N -> tercera falange
    //
    // Estas teclas NO afectan los otros cuatro dedos.
    // ========================================================

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        pulgar1 += 1.0f;

        pulgar1 = ClampAngle(
            pulgar1,
            PULGAR1_MIN,
            PULGAR1_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        pulgar1 -= 1.0f;

        pulgar1 = ClampAngle(
            pulgar1,
            PULGAR1_MIN,
            PULGAR1_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        pulgar2 += 1.0f;

        pulgar2 = ClampAngle(
            pulgar2,
            PULGAR2_MIN,
            PULGAR2_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        pulgar2 -= 1.0f;

        pulgar2 = ClampAngle(
            pulgar2,
            PULGAR2_MIN,
            PULGAR2_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pulgar3 += 1.0f;

        pulgar3 = ClampAngle(
            pulgar3,
            PULGAR3_MIN,
            PULGAR3_MAX
        );
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        pulgar3 -= 1.0f;

        pulgar3 = ClampAngle(
            pulgar3,
            PULGAR3_MIN,
            PULGAR3_MAX
        );
    }
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // INICIALIZAR GLFW
    // ========================================================

    glfwInit();

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

    // ========================================================
    // CREAR VENTANA
    // ========================================================

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Brazo Robotico - Modelo Jerarquico",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout
            << "Error al crear la ventana"
            << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    // ========================================================
    // INICIALIZAR GLEW
    // ========================================================

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout
            << "Error al inicializar GLEW"
            << std::endl;

        return -1;
    }

    // ========================================================
    // VIEWPORT
    // ========================================================

    glViewport(
        0,
        0,
        SCR_WIDTH,
        SCR_HEIGHT
    );

    // ========================================================
    // DEPTH TEST
    // ========================================================

    glEnable(GL_DEPTH_TEST);

    // ========================================================
    // SHADER
    // ========================================================

    Shader ourShader(
        "Shader/core.vs",
        "Shader/core.frag"
    );

    // ========================================================
    // VAO / VBO
    // ========================================================

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(
        1,
        &VAO
    );

    glGenBuffers(
        1,
        &VBO
    );

    glBindVertexArray(VAO);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Posiciones
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // ========================================================
    // BUCLE PRINCIPAL
    // ========================================================

    while (!glfwWindowShouldClose(window))
    {
        // ----------------------------------------------------
        // INPUT
        // ----------------------------------------------------

        processInput(window);

        // ----------------------------------------------------
        // LIMPIAR
        // ----------------------------------------------------

        glClearColor(
            0.02f,
            0.02f,
            0.02f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        // ----------------------------------------------------
        // SHADER
        // ----------------------------------------------------

        ourShader.Use();

        // ====================================================
        // MATRIZ DE VISTA
        // ====================================================

        glm::mat4 view = glm::mat4(1.0f);

        view = glm::lookAt(
            glm::vec3(
                8.0f,
                6.0f,
                12.0f
            ),

            glm::vec3(
                2.0f,
                0.0f,
                0.0f
            ),

            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            )
        );

        // ====================================================
        // PROYECCION
        // ====================================================

        glm::mat4 projection =
            glm::perspective(
                glm::radians(45.0f),
                (float)SCR_WIDTH /
                (float)SCR_HEIGHT,
                0.1f,
                100.0f
            );

        GLint viewLoc = glGetUniformLocation(
            ourShader.Program,
            "view"
        );

        GLint projectionLoc = glGetUniformLocation(
            ourShader.Program,
            "projection"
        );

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        // ====================================================
        // MATRIZ PRINCIPAL DEL BRAZO
        // ====================================================

        glm::mat4 model =
            glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                movX,
                movY,
                movZ
            )
        );

        model = glm::rotate(
            model,
            glm::radians(rot),
            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            )
        );

        // ====================================================
        // HOMBRO
        // ====================================================

        glm::mat4 hombroMatrix = model;

        hombroMatrix = glm::rotate(
            hombroMatrix,
            glm::radians(hombro),
            glm::vec3(
                0.0f,
                0.0f,
                1.0f
            )
        );

        // ====================================================
        // BICEPS
        // ====================================================

        DrawCube(
            ourShader,
            hombroMatrix,
            glm::vec3(
                1.5f,
                0.0f,
                0.0f
            ),
            glm::vec3(
                3.0f,
                0.9f,
                0.9f
            ),
            glm::vec3(
                0.05f,
                0.80f,
                0.20f
            )
        );

        // ====================================================
        // CODO
        // ====================================================

        glm::mat4 codoMatrix =
            hombroMatrix;

        codoMatrix = glm::translate(
            codoMatrix,
            glm::vec3(
                3.0f,
                0.0f,
                0.0f
            )
        );

        codoMatrix = glm::rotate(
            codoMatrix,
            glm::radians(codo),
            glm::vec3(
                0.0f,
                0.0f,
                1.0f
            )
        );

        // ====================================================
        // ANTEBRAZO
        // ====================================================

        DrawCube(
            ourShader,
            codoMatrix,
            glm::vec3(
                1.0f,
                0.0f,
                0.0f
            ),
            glm::vec3(
                2.0f,
                0.8f,
                0.8f
            ),
            glm::vec3(
                0.75f,
                0.75f,
                0.80f
            )
        );

        // ====================================================
        // MUÑECA
        // ====================================================

        glm::mat4 munecaMatrix =
            codoMatrix;

        munecaMatrix = glm::translate(
            munecaMatrix,
            glm::vec3(
                2.0f,
                0.0f,
                0.0f
            )
        );

        munecaMatrix = glm::rotate(
            munecaMatrix,
            glm::radians(muneca),
            glm::vec3(
                0.0f,
                0.0f,
                1.0f
            )
        );

        // ====================================================
        // PALMA
        // ====================================================

        DrawCube(
            ourShader,
            munecaMatrix,
            glm::vec3(
                0.40f,
                0.0f,
                0.0f
            ),
            glm::vec3(
                0.80f,
                2.20f,
                1.0f
            ),
            glm::vec3(
                0.70f,
                0.70f,
                0.75f
            )
        );

        // ====================================================
        // MATRIZ TEMPORAL DE LA PALMA
        //
        // Todas las falanges parten de esta matriz.
        // ====================================================

        glm::mat4 palmaMatrix =
            munecaMatrix;

        palmaMatrix = glm::translate(
            palmaMatrix,
            glm::vec3(
                0.80f,
                0.0f,
                0.0f
            )
        );

        // ====================================================
        // LOS CUATRO DEDOS PRINCIPALES
        //
        // IMPORTANTE:
        //
        // Todos reciben:
        //
        // falange1
        // falange2
        // falange3
        //
        // Por lo tanto:
        //
        // J/U -> F1 de los cuatro
        // O/L -> F2 de los cuatro
        // P/; -> F3 de los cuatro
        //
        // Pero cada dedo tiene su propia posicion Y.
        // ====================================================

        // ----------------------------------------------------
        // INDICE
        // ----------------------------------------------------

        DrawFinger(
            ourShader,
            palmaMatrix,
            1.35f,
            0.0f,

            falange1 * FALANGE1_FACTOR,
            falange2,
            falange3,

            glm::vec3(
                0.05f,
                0.65f,
                0.90f
            ),

            glm::vec3(
                0.02f,
                0.55f,
                0.80f
            ),

            glm::vec3(
                0.02f,
                0.45f,
                0.70f
            )
        );

        // ----------------------------------------------------
        // MEDIO
        // ----------------------------------------------------

        DrawFinger(
            ourShader,
            palmaMatrix,
            0.45f,
            0.0f,

            falange1 * FALANGE1_FACTOR,
            falange2,
            falange3,

            glm::vec3(
                0.10f,
                0.75f,
                0.95f
            ),

            glm::vec3(
                0.05f,
                0.65f,
                0.90f
            ),

            glm::vec3(
                0.02f,
                0.55f,
                0.80f
            )
        );

        // ----------------------------------------------------
        // ANULAR
        // ----------------------------------------------------

        DrawFinger(
            ourShader,
            palmaMatrix,
            -0.45f,
            0.0f,

            falange1 * FALANGE1_FACTOR,
            falange2,
            falange3,

            glm::vec3(
                0.85f,
                0.10f,
                0.70f
            ),

            glm::vec3(
                0.75f,
                0.05f,
                0.60f
            ),

            glm::vec3(
                0.65f,
                0.02f,
                0.50f
            )
        );

        // ----------------------------------------------------
        // MEÑIQUE
        // ----------------------------------------------------

        DrawFinger(
            ourShader,
            palmaMatrix,
            -1.35f,
            0.0f,

            falange1 * FALANGE1_FACTOR,
            falange2,
            falange3,

            glm::vec3(
                0.90f,
                0.10f,
                0.50f
            ),

            glm::vec3(
                0.80f,
                0.05f,
                0.45f
            ),

            glm::vec3(
                0.70f,
                0.02f,
                0.40f
            )
        );

        // ====================================================
        // PULGAR
        //
        // SE DIBUJA APARTE.
        //
        // NO UTILIZA:
        //
        // falange1
        // falange2
        // falange3
        //
        // Utiliza:
        //
        // pulgar1
        // pulgar2
        // pulgar3
        // ====================================================

        DrawThumb(
            ourShader,
            munecaMatrix,
            pulgar1,
            pulgar2,
            pulgar3
        );

        // ====================================================
        // FINAL DEL FRAME
        // ====================================================

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // ========================================================
    // LIBERAR RECURSOS
    // ========================================================

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glfwTerminate();

    return 0;
}