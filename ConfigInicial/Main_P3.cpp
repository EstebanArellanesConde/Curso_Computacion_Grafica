/* 
   Arellanes Conde Esteban
   Práctica #03 - Pila vertical de cubos
   CGeIHC (L+) - Grupo: 05
   30/08/2027-1
   Nombre: ESTEBAN
*/

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"


const GLint WIDTH = 800;
const GLint HEIGHT = 600;


int main()
{
    // =========================================================
    // INICIALIZACIÓN DE GLFW
    // =========================================================

    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "ESTEBAN ARELLANES CONDE",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }


    int screenWidth;
    int screenHeight;

    glfwGetFramebufferSize(
        window,
        &screenWidth,
        &screenHeight
    );


    glfwMakeContextCurrent(window);


    // =========================================================
    // INICIALIZACIÓN DE GLEW
    // =========================================================

    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;

        glfwTerminate();

        return EXIT_FAILURE;
    }


    // =========================================================
    // VIEWPORT
    // =========================================================

    glViewport(
        0,
        0,
        screenWidth,
        screenHeight
    );


    // =========================================================
    // CONFIGURACIÓN DE OPENGL
    // =========================================================

    // Profundidad
    glEnable(GL_DEPTH_TEST);

    // Transparencia
    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );


    // =========================================================
    // SHADER
    // =========================================================

    Shader ourShader(
        "Shader/core.vs",
        "Shader/core.frag"
    );


    // =========================================================
    // VÉRTICES DEL CUBO
    //
    // Cada vértice:
    //
    // X Y Z R G B
    //
    // 36 vértices = 12 triángulos = 6 caras
    // =========================================================

    float vertices[] =
    {
        // =====================================================
        // FRENTE
        // =====================================================

        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,


        // =====================================================
        // ATRÁS
        // =====================================================

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,


        // =====================================================
        // DERECHA
        // =====================================================

         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,


         // =====================================================
         // IZQUIERDA
         // =====================================================

         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,


         // =====================================================
         // ABAJO
         // =====================================================

         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,


         // =====================================================
         // ARRIBA
         // =====================================================

         -0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f,
          0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f,
          0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,

          0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f
    };


    // =========================================================
    // VAO Y VBO
    // =========================================================

    GLuint VBO;
    GLuint VAO;

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


    // =========================================================
    // ATRIBUTO DE POSICIÓN
    // =========================================================

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);


    // =========================================================
    // ATRIBUTO DE COLOR
    // =========================================================

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(0);


    // =========================================================
    // PROYECCIÓN
    // =========================================================

    glm::mat4 projection =
        glm::perspective(
            glm::radians(45.0f),
            (GLfloat)screenWidth /
            (GLfloat)screenHeight,
            0.1f,
            100.0f
        );


    // =========================================================
    // UBICACIONES DE UNIFORMS
    // =========================================================

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

    GLint objectColorLoc =
        glGetUniformLocation(
            ourShader.Program,
            "objectColor"
        );


    // =========================================================
    // CONFIGURACIÓN DE LOS 7 CUBOS
    //
    // ESTEBAN = 7 LETRAS
    // =========================================================

    const int numCubos = 7;


    // ---------------------------------------------------------
    // LETRAS
    // ---------------------------------------------------------

    char letras[numCubos] =
    {
        'E',
        'S',
        'T',
        'E',
        'B',
        'A',
        'N'
    };


    // ---------------------------------------------------------
    // ESCALAS
    //
    // X = ancho
    // Y = altura
    // Z = profundidad
    // ---------------------------------------------------------

    glm::vec3 escalas[numCubos] =
    {
        glm::vec3(1.10f, 0.55f, 0.85f),  // E
        glm::vec3(0.40f, 0.60f, 1.00f),  // S
        glm::vec3(1.00f, 0.50f, 0.80f),  // T
        glm::vec3(0.90f, 1.65f, 0.90f),  // E
        glm::vec3(1.05f, 0.25f, 0.70f),  // B
        glm::vec3(0.85f, 0.60f, 0.95f),  // A
        glm::vec3(1.00f, 0.55f, 0.85f)   // N
    };


    // ---------------------------------------------------------
    // ROTACIÓN SOBRE Y
    //
    // ES LA ROTACIÓN PRINCIPAL.
    //
    // Al rotar sobre Y no inclinamos la pila verticalmente.
    // ---------------------------------------------------------

    float rotacionY[numCubos] =
    {
        -18.0f,  // E
         12.0f,  // S
         -8.0f,  // T
         20.0f,  // E
        -15.0f,  // B
         10.0f,  // A
         -5.0f   // N
    };


    // ---------------------------------------------------------
    // PEQUEÑAS VARIACIONES SOBRE X
    //
    // Son deliberadamente pequeñas.
    // ---------------------------------------------------------

    float rotacionX[numCubos] =
    {
         2.0f,
        -1.0f,
         2.5f,
        -2.0f,
         1.5f,
        -2.0f,
         1.5f
    };


    // ---------------------------------------------------------
    // PEQUEÑAS VARIACIONES SOBRE Z
    // ---------------------------------------------------------

    float rotacionZ[numCubos] =
    {
         1.5f,
        -1.5f,
         2.0f,
        -1.5f,
         1.5f,
        -2.0f,
         1.0f
    };


    // ---------------------------------------------------------
    // DESPLAZAMIENTOS HORIZONTALES
    //
    // Muy pequeños para conservar la pila.
    // ---------------------------------------------------------

    float posicionesX[numCubos] =
    {
         0.00f,
         0.04f,
        -0.03f,
         0.03f,
        -0.04f,
         0.03f,
        -0.02f
    };


    float posicionesZ[numCubos] =
    {
         0.00f,
         0.02f,
        -0.02f,
         0.02f,
        -0.02f,
         0.03f,
         0.00f
    };


    // =========================================================
    // COLOR BASE DE CADA CUBO
    // =========================================================

    glm::vec3 colores[numCubos] =
    {
        glm::vec3(0.90f, 0.12f, 0.12f),  // E - Rojo
        glm::vec3(0.10f, 0.35f, 0.90f),  // S - Azul
        glm::vec3(0.95f, 0.75f, 0.05f),  // T - Amarillo
        glm::vec3(0.10f, 0.70f, 0.25f),  // E - Verde
        glm::vec3(0.60f, 0.15f, 0.80f),  // B - Morado
        glm::vec3(0.95f, 0.40f, 0.05f),  // A - Naranja
        glm::vec3(0.05f, 0.70f, 0.80f)   // N - Cian
    };


    // =========================================================
    // ALTURA TOTAL DE LA PILA
    //
    // Cada cubo toca al siguiente.
    // =========================================================

    float alturaTotal = 0.0f;

    for (int i = 0; i < numCubos; i++)
    {
        alturaTotal += escalas[i].y;
    }


    // Centrar la pila verticalmente
    float posicionBaseY =
        -(alturaTotal / 2.0f);


    // =========================================================
    // BUCLE PRINCIPAL
    // =========================================================

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        // =====================================================
        // LIMPIAR PANTALLA
        // =====================================================

        glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        // =====================================================
        // ACTIVAR SHADER
        // =====================================================

        ourShader.Use();


        // =====================================================
        // MATRIZ VIEW
        // =====================================================

        glm::mat4 view =
            glm::mat4(1.0f);


        view =
            glm::translate(
                view,
                glm::vec3(
                    0.0f,
                    0.0f,
                    -7.0f
                )
            );


        // =====================================================
        // ENVIAR VIEW Y PROJECTION
        // =====================================================

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


        // =====================================================
        // ACTIVAR VAO
        // =====================================================

        glBindVertexArray(VAO);


        // =====================================================
        // POSICIÓN VERTICAL ACTUAL
        // =====================================================

        float alturaActual = 0.0f;


        // =====================================================
        // DIBUJAR LOS 7 CUBOS
        // =====================================================

        for (int i = 0; i < numCubos; i++)
        {
            glm::mat4 model =
                glm::mat4(1.0f);


            // -------------------------------------------------
            // POSICIÓN Y
            //
            // El centro del cubo se coloca a la mitad de
            // su propia altura.
            //
            // De esta forma:
            //
            // CUBO 2
            // --------
            // CUBO 1
            // --------
            //
            // Las caras quedan juntas.
            // -------------------------------------------------

            float posY =
                posicionBaseY +
                alturaActual +
                (escalas[i].y / 2.0f);


            model =
                glm::translate(
                    model,
                    glm::vec3(
                        posicionesX[i],
                        posY,
                        posicionesZ[i]
                    )
                );


            // -------------------------------------------------
            // ROTACIÓN Y
            // -------------------------------------------------

            model =
                glm::rotate(
                    model,
                    glm::radians(
                        rotacionY[i]
                    ),
                    glm::vec3(
                        0.0f,
                        1.0f,
                        0.0f
                    )
                );


            // -------------------------------------------------
            // PEQUEÑA ROTACIÓN X
            // -------------------------------------------------

            model =
                glm::rotate(
                    model,
                    glm::radians(
                        rotacionX[i]
                    ),
                    glm::vec3(
                        1.0f,
                        0.0f,
                        0.0f
                    )
                );


            // -------------------------------------------------
            // PEQUEÑA ROTACIÓN Z
            // -------------------------------------------------

            model =
                glm::rotate(
                    model,
                    glm::radians(
                        rotacionZ[i]
                    ),
                    glm::vec3(
                        0.0f,
                        0.0f,
                        1.0f
                    )
                );


            // -------------------------------------------------
            // ESCALA
            // -------------------------------------------------

            model =
                glm::scale(
                    model,
                    escalas[i]
                );


            // -------------------------------------------------
            // ENVIAR MODEL
            // -------------------------------------------------

            glUniformMatrix4fv(
                modelLoc,
                1,
                GL_FALSE,
                glm::value_ptr(model)
            );


            // -------------------------------------------------
            // ENVIAR COLOR
            // -------------------------------------------------

            glUniform3fv(
                objectColorLoc,
                1,
                glm::value_ptr(
                    colores[i]
                )
            );


            // -------------------------------------------------
            // DIBUJAR CUBO
            // -------------------------------------------------

            glDrawArrays(
                GL_TRIANGLES,
                0,
                36
            );


            // -------------------------------------------------
            // SIGUIENTE CUBO
            // -------------------------------------------------

            alturaActual += escalas[i].y;
        }


        // =====================================================
        // DESACTIVAR VAO
        // =====================================================

        glBindVertexArray(0);


        // =====================================================
        // ACTUALIZAR PANTALLA
        // =====================================================

        glfwSwapBuffers(window);
    }


    // =========================================================
    // LIMPIEZA
    // =========================================================

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glfwTerminate();


    return EXIT_SUCCESS;
}