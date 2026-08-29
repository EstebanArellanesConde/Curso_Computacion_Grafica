/* Arellanes Conde Esteban
CGeIHC - Grupo: 05
Práctica 2 - Dibujo de primitivas 2D
28/08/2027-1
Papel-Avión - image_0.png */

#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Modificación para habilitar el uso de la GPU dedicada
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Shaders
#include "Shader.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Práctica 2 - Dibujo de Primitivas 2D (Avión de Papel)",
        nullptr,
        nullptr
    );

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    // ---------------------------------------------------------
    // CONFIGURACIÓN PARA DIBUJO 2D
    // ---------------------------------------------------------

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    // Líneas más gruesas para parecerse al dibujo original
    glLineWidth(3.0f);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    /*
        ==========================================================
        AVIÓN DE PAPEL
        ==========================================================

        La imagen original tiene aproximadamente esta geometría:

                  P1 -------- P2
                  |             \
                  |              \
                  P3--------------\---- P8
                 / \               \
                /   \               \
              P4     \              P8
                \     \            /
                 \     \          /
                  P5----P6------P7

        Coordenadas originales aproximadas de la imagen:

        P1 = (28,10)
        P2 = (67,14)
        P3 = (29,30)
        P4 = (6,43)
        P5 = (30,57)
        P6 = (31,67)
        P7 = (110,72)
        P8 = (150,78)

        Se escalan y trasladan para ocupar aproximadamente
        la misma posición dentro de una ventana de 800x600.

        IMPORTANTE:
        OpenGL utiliza el origen abajo-izquierda, mientras que
        la imagen de referencia utiliza origen arriba-izquierda.
    */

    // ---------------------------------------------------------
    // COORDENADAS
    // ---------------------------------------------------------

    // Punto superior izquierdo
    const GLfloat P1[] = {
        203.0f, 456.0f
    };

    // Punto superior central
    const GLfloat P2[] = {
        375.0f, 438.0f
    };

    // Unión central superior
    const GLfloat P3[] = {
        208.0f, 368.0f
    };

    // Punta izquierda
    const GLfloat P4[] = {
        106.0f, 311.0f
    };

    // Unión inferior izquierda
    const GLfloat P5[] = {
        212.0f, 249.0f
    };

    // Parte inferior izquierda
    const GLfloat P6[] = {
        216.0f, 205.0f
    };

    // Quiebre inferior derecho
    const GLfloat P7[] = {
        564.0f, 183.0f
    };

    // Quiebre inferior final
    const GLfloat P8[] = {
        564.0f, 152.0f
    };

    // Punta del avión
    const GLfloat P9[] = {
        740.0f, 157.0f
    };

    /*
        ==========================================================
        VÉRTICES DE LOS RELLENOS
        ==========================================================

        Se utilizan triángulos para construir las diferentes
        superficies del avión.
    */

    GLfloat vertices[] =
    {
        // =====================================================
        // 1. PARTE SUPERIOR DEL AVIÓN
        // P1 -> P2 -> P9 -> P3
        // =====================================================

        // Triángulo 1
        P1[0], P1[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P2[0], P2[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f,

        // Triángulo 2
        P1[0], P1[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P3[0], P3[1], 0.0f,   1.0f, 1.0f, 1.0f,


        // =====================================================
        // 2. ALA PRINCIPAL
        // P3 -> P4 -> P5 -> P9
        // =====================================================

        // Triángulo 3
        P3[0], P3[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P4[0], P4[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f,

        // Triángulo 4
        P4[0], P4[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P5[0], P5[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f,


        // =====================================================
        // 3. PARTE INFERIOR
        // P5 -> P6 -> P7 -> P8 -> P9
        // =====================================================

        // Triángulo 5
        P5[0], P5[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P6[0], P6[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P7[0], P7[1], 0.0f,   1.0f, 1.0f, 1.0f,

        // Triángulo 6
        P5[0], P5[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P7[0], P7[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f,

        // Triángulo 7
        P7[0], P7[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P8[0], P8[1], 0.0f,   1.0f, 1.0f, 1.0f,
        P9[0], P9[1], 0.0f,   1.0f, 1.0f, 1.0f
    };


    // ---------------------------------------------------------
    // VAO / VBO
    // ---------------------------------------------------------

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Posición
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // ---------------------------------------------------------
    // PROYECCIÓN ORTOGRÁFICA
    // ---------------------------------------------------------

    glm::mat4 projection =
        glm::ortho(
            0.0f,
            (GLfloat)screenWidth,
            0.0f,
            (GLfloat)screenHeight,
            -1.0f,
            1.0f
        );


    // =========================================================
    // BUCLE PRINCIPAL
    // =========================================================

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Fondo BLANCO como en la imagen original
        glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        GLint modelLoc =
            glGetUniformLocation(
                ourShader.Program,
                "model"
            );

        GLint viewLoc =
            glGetUniformLocation(
                ourShader.Program,
                "view"
            );

        GLint projectionLoc =
            glGetUniformLocation(
                ourShader.Program,
                "projection"
            );

        glUniformMatrix4fv(
            projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );


        // -----------------------------------------------------
        // RELLENO DEL AVIÓN
        // -----------------------------------------------------

        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            21
        );

        glBindVertexArray(0);


        // =====================================================
        // CONTORNOS DEL AVIÓN
        // =====================================================

        /*
            Para que la figura sea visualmente igual a la
            referencia, dibujamos las líneas negras por separado.
        */

        GLfloat lineVertices[] =
        {
            // =================================================
            // CONTORNO EXTERIOR
            // =================================================

            // P1 -> P2
            P1[0], P1[1], 0.1f,

            P2[0], P2[1], 0.1f,

            // P2 -> P9
            P2[0], P2[1], 0.1f,

            P9[0], P9[1], 0.1f,

            // P9 -> P8
            P9[0], P9[1], 0.1f,

            P8[0], P8[1], 0.1f,

            // P8 -> P7
            P8[0], P8[1], 0.1f,

            P7[0], P7[1], 0.1f,

            // P7 -> P6
            P7[0], P7[1], 0.1f,

            P6[0], P6[1], 0.1f,

            // P6 -> P5
            P6[0], P6[1], 0.1f,

            P5[0], P5[1], 0.1f,

            // P5 -> P4
            P5[0], P5[1], 0.1f,

            P4[0], P4[1], 0.1f,

            // P4 -> P3
            P4[0], P4[1], 0.1f,

            P3[0], P3[1], 0.1f,

            // P3 -> P1
            P3[0], P3[1], 0.1f,

            P1[0], P1[1], 0.1f,


            // =================================================
            // PLIEGUE PRINCIPAL
            // P3 -> P9
            // =================================================

            P3[0], P3[1], 0.1f,

            P9[0], P9[1], 0.1f,


            // =================================================
            // PLIEGUE INFERIOR
            // P5 -> P9
            // =================================================

            P5[0], P5[1], 0.1f,

            P9[0], P9[1], 0.1f,


            // =================================================
            // LÍNEA INFERIOR
            // P6 -> P7
            // =================================================

            P6[0], P6[1], 0.1f,

            P7[0], P7[1], 0.1f,


            // =================================================
            // PEQUEÑO PLIEGUE FINAL
            // P7 -> P8
            // =================================================

            P7[0], P7[1], 0.1f,

            P8[0], P8[1], 0.1f
        };


        GLuint lineVAO, lineVBO;

        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);

        glBindVertexArray(lineVAO);

        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(lineVertices),
            lineVertices,
            GL_STATIC_DRAW
        );

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(GLfloat),
            (GLvoid*)0
        );

        glEnableVertexAttribArray(0);


        /*
            Para las líneas necesitamos que el fragment shader
            utilice un color negro.

            Como tu shader probablemente espera el atributo
            de color, es mejor utilizar glVertexAttrib3f().
        */

        glVertexAttrib3f(
            1,
            0.0f,
            0.0f,
            0.0f
        );


        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // -----------------------------------------------------
        // Dibujar cada segmento
        // -----------------------------------------------------

        glBindVertexArray(lineVAO);

        glDrawArrays(
            GL_LINES,
            0,
            sizeof(lineVertices) / (3 * sizeof(GLfloat))
        );

        glBindVertexArray(0);


        glDeleteVertexArrays(1, &lineVAO);
        glDeleteBuffers(1, &lineVBO);


        glfwSwapBuffers(window);
    }


    // ---------------------------------------------------------
    // LIMPIEZA
    // ---------------------------------------------------------

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return EXIT_SUCCESS;

}