//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
//Abdallah Said - 40099027

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include "shaderloader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace glm;
using namespace std;

GLuint loadTexture(const char *filename);

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
    : position(_position), color(_color), uv(_uv) {}

    vec3 position;
    vec3 color;
    vec2 uv;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 0.0f)), //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,  0.0f, -1.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f,  0.0f,  0.0f), vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f,  0.0f,  1.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f,  0.0f), vec2(0.0f, 1.0f)),

    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f,  1.0f,  0.0f), vec2(0.0f, 1.0f))
};



int createTexturedCubeVertexArrayObject();
int createlightCubeVertexArrayObject();

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projection_matrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "view_matrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "world_matrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setModelMatrix(int shaderProgram, mat4 modelMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "model_matrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

// shader variable setters
void SetUniformMat4(GLuint shader_id, const char* uniform_name, mat4 uniform_value)
{
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
}

void SetUniformVec3(GLuint shader_id, const char* uniform_name, vec3 uniform_value)
{
    glUseProgram(shader_id);
    glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, value_ptr(uniform_value));
}

template <class T>
void SetUniform1Value(GLuint shader_id, const char* uniform_name, T uniform_value)
{
    glUseProgram(shader_id);
    glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Lab 04", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }


    // Load Textures
    GLuint brickTextureID = loadTexture("assets/textures/brick.jpg");
    GLuint cementTextureID = loadTexture("assets/textures/cement.jpg");
    GLuint tileTextureID = loadTexture("assets/textures/tile.jpg");
    GLuint clothTextureID = loadTexture("assets/textures/cloth.jpg");
    GLuint metalTextureID = loadTexture("assets/textures/metal.jpg");
    GLuint cardboardTextureID = loadTexture("assets/textures/cardboard.jpg");
    GLuint scrsht1TextureID = loadTexture("assets/textures/scrsht1.jpg");
    GLuint scrsht2TextureID = loadTexture("assets/textures/srcsht2.jpg");
    GLuint redTextureID = loadTexture("assets/textures/red.jpg");
    GLuint blueTextureID = loadTexture("assets/textures/blue.jpg");
    GLuint greenTextureID = loadTexture("assets/textures/green.png");

    std::string shaderPathPrefix = "assets/shaders/";

    GLuint shaderScene = loadSHADER(shaderPathPrefix + "scene_vertex.glsl", shaderPathPrefix + "scene_fragment.glsl");
    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");


    // Camera parameters for view transform
    vec3 cameraPosition(0.6f,1.0f,10.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    int  cameraFirstPerson = 1; // press 1 or 2 to toggle this variable


    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
                                             1024.0f / 768.0f,  // aspect ratio
                                             0.01f, 800.0f);   // near and far (near > 0)

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp ); // up

    // Set projection matrix on both shaders
    SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);

    // Set view matrix on both shaders
    SetUniformMat4(shaderScene, "view_matrix", viewMatrix);

    //boolean for light on or off
    bool lightOn = true;

    //circle camera x, z pos
    float cirCamPos = 1.0f;

    // Define and upload geometry to the GPU here ...
    int texturedCubeVAO = createTexturedCubeVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //glBindVertexArray(createlightCubeVertexArrayObject());

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;
        // cout << dt << "\t" << lastFrameTime << endl;

        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(shaderScene, "textureSampler");
        glUniform1i(textureLocation, 0);

        glBindBuffer(GL_ARRAY_BUFFER, createTexturedCubeVertexArrayObject());
        GLuint worldMatrixLocation = glGetUniformLocation(shaderScene, "worldMatrix");

        //changing circle camera pos with respect to the z and x axis

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            cirCamPos += 0.1f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            cirCamPos -= 0.1f;
        }

        // Position of the camera is on the sphere looking at the point of interest (cameraPosition)
        float radius = 15.0f;
        float camX = sin(cirCamPos) * radius;
        float camZ = cos(cirCamPos) * radius;

        float ambient_strength;
        float specular_strength;
        float diffuse_strength;

        //failed attempt at turning the lights on and off

        if ((lightOn == true) && (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
            lightOn = false;
        else if ((lightOn == false) && (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
            lightOn = true;

        if (lightOn == true) {
            ambient_strength = 0.2f;
            specular_strength = 0.6f;
            diffuse_strength = 0.2f;
        }
        else if (lightOn == false) {
            ambient_strength = 1.0;
            specular_strength = 0.0;
            diffuse_strength = 0.0;
        }

        SetUniform1Value(shaderScene, "shading_ambient_strength", ambient_strength);
        SetUniform1Value(shaderScene, "shading_specular_strength", specular_strength);
        SetUniform1Value(shaderScene, "shading_diffuse_strength", diffuse_strength);

        //Failed attempt at making shadows

        // Setup texture and framebuffer for creating shadow map

        // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be oversized and waste resources
        const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

        // Variable storing index to texture used for shadow mapping
        GLuint depth_map_texture;
        // Get the texture
        glGenTextures(1, &depth_map_texture);
        // Bind the texture so the next glTex calls affect it
        glBindTexture(GL_TEXTURE_2D, depth_map_texture);
        // Create the texture and specify it's attributes, including widthn height, components (only depth is stored, no color information)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
            NULL);
        // Set texture sampler parameters.
        // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture. Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        

        glUseProgram(shaderScene);

        float lightAngleOuter = 30.0;
        float lightAngleInner = 20.0;
        // Set light cutoff angles on scene shader
        SetUniform1Value(shaderScene, "light_cutoff_inner", cos(radians(lightAngleInner)));
        SetUniform1Value(shaderScene, "light_cutoff_outer", cos(radians(lightAngleOuter)));

        // Set light color on scene shader
        SetUniformVec3(shaderScene, "light_color", vec3(1.0, 1.0, 1.0));

        // light parameters
        vec3 lightPosition = vec3(0.0f, -50.0f, -4.0f); // the location of the light in 3D space
        vec3 lightFocus(0.0f, -40.0f, -3.0f);      // the point in 3D space the light "looks" at
        vec3 lightDirection = normalize(lightFocus - lightPosition);

        //vec3 lightPosition2 = vec3(0.0f, 0.0f, 0.0f); // the location of the light in 3D space
        //vec3 lightFocus2(0.0f, 0.0f, 0.0f);      // the point in 3D space the light "looks" at
        //vec3 lightDirection2 = normalize(lightFocus2 - lightPosition2);

        float lightNearPlane = 1.0f;
        float lightFarPlane = 70.0f;

        mat4 lightProjectionMatrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
        mat4 lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shaderScene, "light_view_proj_matrix", lightSpaceMatrix);

        // Set light far and near planes on scene shader
        SetUniform1Value(shaderScene, "light_near_plane", lightNearPlane);
        SetUniform1Value(shaderScene, "light_far_plane", lightFarPlane);

        // Set light position on scene shader
        SetUniformVec3(shaderScene, "light_position", lightFocus);

        // Set light direction on scene shader
        SetUniformVec3(shaderScene, "light_direction", lightDirection);
        //SetUniformVec3(shaderScene, "light_direction2", lightDirection2);

        // Set view position on scene shader
        SetUniformVec3(shaderScene, "view_position", lightPosition);
        //SetUniformVec3(shaderScene, "view_position2", lightPosition2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        //Draw three axis

        glBindTexture(GL_TEXTURE_2D, redTextureID);

        mat4 xAxisMatrix = translate(mat4(1.0f), vec3(1.75f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(3.5f, 0.35f, 0.35f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xAxisMatrix[0][0]);
        SetUniformMat4(shaderShadow, "model_matrix", xAxisMatrix);
        SetUniformMat4(shaderScene, "model_matrix", xAxisMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, greenTextureID);

        mat4 yAxisMatrix = translate(mat4(1.0f), vec3(0.0f, 1.75f, 0.0f)) * scale(mat4(1.0f), vec3(0.35f, 3.5f, 0.35f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yAxisMatrix[0][0]);
        SetUniformMat4(shaderShadow, "model_matrix", yAxisMatrix);
        SetUniformMat4(shaderScene, "model_matrix", yAxisMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, blueTextureID);

        mat4 zAxisMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.75f)) * scale(mat4(1.0f), vec3(0.35f, 0.35f, 3.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &zAxisMatrix[0][0]);
        SetUniformMat4(shaderShadow, "model_matrix", zAxisMatrix);
        SetUniformMat4(shaderScene, "model_matrix", zAxisMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, cardboardTextureID);

        // Draw letter A

        mat4 aTopPillar = translate(mat4(1.0f), vec3(0.0f, 20.25f, -12.5f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

        mat4 letterAMatrix = /*rotationMatrix **/ aTopPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterAMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterAMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 aRightPillar = translate(mat4(1.0f), vec3(-0.75f, 20.5f, -12.5f)) * rotate(mat4(1.0f), radians(-10.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 4.5f, 1.0f));

        letterAMatrix = /*rotationMatrix **/ aRightPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterAMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterAMatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 aLeftPillar = translate(mat4(1.0f), vec3(0.75f, 20.5f, -12.5f)) * rotate(mat4(1.0f), radians(10.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 4.5f, 1.0f));

        letterAMatrix = /*rotationMatrix **/ aLeftPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterAMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterAMatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));



        // Draw letter S

        mat4 sTopPillar = translate(mat4(1.0f), vec3(0.0f, 28.0f, -12.5f)) * scale(mat4(1.0f), vec3(2.5f, 1.0f, 1.0f));

        mat4 letterSMatrix = /*rotationMatrix **/ sTopPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterSMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterSMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 sMidPillar = translate(mat4(1.0f), vec3(0.0f, 26.0f, -12.5f)) * scale(mat4(1.0f), vec3(2.5f, 1.0f, 1.0f));

        letterSMatrix = /*rotationMatrix **/ sMidPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterSMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterSMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 sBotPillar = translate(mat4(1.0f), vec3(0.0f, 24.0f, -12.5f)) * scale(mat4(1.0f), vec3(2.5f, 1.0f, 1.0f));

        letterSMatrix = /*rotationMatrix **/ sBotPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterSMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterSMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 sLeftPillar = translate(mat4(1.0f), vec3(-1.0f, 27.0f, -12.5f)) * scale(mat4(1.0f), vec3(0.5f, 2.0f, 1.0f));

        letterSMatrix = /*rotationMatrix **/ sLeftPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterSMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterSMatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 sRightPillar = translate(mat4(1.0f), vec3(1.0f, 25.0f, -12.5f)) * scale(mat4(1.0f), vec3(0.5f, 2.0f, 1.0f));

        letterSMatrix = /*rotationMatrix **/ sRightPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterSMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterSMatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        glDisable(GL_PRIMITIVE_RESTART);

        //Draw letter I

        mat4 iMidPillar = translate(mat4(1.0f), vec3(0.0f, 14.5f, -12.5f)) * scale(mat4(1.0f), vec3(0.75f, 2.5f, 0.5f));

        mat4 letterIMatrix = /*rotationMatrix **/ iMidPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterIMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterIMatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 iBotPillar = translate(mat4(1.0f), vec3(0.0f, 12.5f, -12.5f)) * scale(mat4(1.0f), vec3(2.5f, 1.0f, 1.0f));

        letterIMatrix = /*rotationMatrix **/ iBotPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterIMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterIMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 iTopPillar = translate(mat4(1.0f), vec3(0.0f, 16.5f, -12.5f)) * scale(mat4(1.0f), vec3(2.5f, 1.0f, 1.0f));

        letterIMatrix = /*rotationMatrix **/ iTopPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterIMatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterIMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //draw letter d

        mat4 dleftpillar = translate(mat4(1.0f), vec3(-0.5f, 9.5f, -12.5f)) * scale(mat4(1.0f), vec3(1.0f, 4.0f, 1.0f));

        mat4 letterdmatrix = /*rotationmatrix **/ dleftpillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterdmatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterdmatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 dToppillar = translate(mat4(1.0f), vec3(0.5f, 10.25f, -12.5f)) * rotate(mat4(1.0f), radians(40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));

        letterdmatrix = /*rotationmatrix **/ dToppillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterdmatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterdmatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 dBotpillar = translate(mat4(1.0f), vec3(0.5f, 8.75f, -12.5f)) * rotate(mat4(1.0f), radians(-40.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));

        letterdmatrix = /*rotationmatrix **/ dBotpillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterdmatrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", letterdmatrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        glBindTexture(GL_TEXTURE_2D, metalTextureID);

        //Draw digit 4

        mat4 fourTopPillar = translate(mat4(1.0f), vec3(0.65f, 5.5f, -12.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));

        mat4 digit4Matrix = /*rotationMatrix **/ fourTopPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit4Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit4Matrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 fourBotPillar = translate(mat4(1.0f), vec3(-0.65f, 6.0f, -12.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));

        digit4Matrix = /*rotationMatrix **/ fourBotPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit4Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit4Matrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 fourMidPillar = translate(mat4(1.0f), vec3(-0.0f, 5.5f, -12.5f)) * scale(mat4(1.0f), vec3(1.0f, 0.75f, 1.0f));

        digit4Matrix = /*rotationMatrix **/ fourMidPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit4Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit4Matrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        ////draw digit 0


        mat4 zeroRightPillar = translate(mat4(1.0f), vec3(0.75f, 2.0f, -12.25f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));

        mat4 digit0Matrix = /*rotationMatrix **/ zeroRightPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit0Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit0Matrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 zeroLeftPillar = translate(mat4(1.0f), vec3(-0.75f, 2.0f, -12.25f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));

        digit0Matrix = /*rotationMatrix **/ zeroLeftPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit0Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit0Matrix);
        glDrawArrays(GL_TRIANGLES, 0, (1260 * 2));

        mat4 zeroTopPillar = translate(mat4(1.0f), vec3(0.0f, 3.0f, -12.25f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

        digit0Matrix = /*rotationMatrix **/ zeroTopPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit0Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit0Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 zeroBotPillar = translate(mat4(1.0f), vec3(0.0f, 1.0f, -12.25f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));

        digit0Matrix = /*rotationMatrix **/ zeroBotPillar;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &digit0Matrix[0][0]);
        SetUniformMat4(shaderScene, "model_matrix", digit0Matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //draw the grid

        glBindTexture(GL_TEXTURE_2D, tileTextureID);

        for (int i = 0; i < 16; ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                mat4 gridMatrix = translate(mat4(1.0f), vec3(-14.8f + i * 0.22f, 0.0f, -14.8f + j * 0.22f)) * scale(mat4(1.0f), vec3(0.2f, 0.01f, 0.2f)) /** rotate(mat4(1.0f), radians(180.0f), vec3(1.0f, 0.0f, 0.0f))*/;
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
                SetUniformMat4(shaderScene, "model_matrix", gridMatrix);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        //binding texture to the cloth texture
        glBindTexture(GL_TEXTURE_2D, clothTextureID);

        //building and rendering stage

        mat4 stageBackMatrix = translate(mat4(1.0f), vec3(0.0f, 0.25f, -3.0f)) * scale(mat4(1.0f), vec3(6.0f, 0.5f, 3.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &stageBackMatrix[0][0]);
        setModelMatrix(shaderScene, stageBackMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 stageLeftMatrix = translate(mat4(1.0f), vec3(2.0f, 0.25f, -1.5f)) * scale(mat4(1.0f), vec3(2.0f, 0.5f, 2.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &stageLeftMatrix[0][0]);
        setModelMatrix(shaderScene, stageLeftMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 stageRightMatrix = translate(mat4(1.0f), vec3(-2.0f, 0.25f, -1.5f)) * scale(mat4(1.0f), vec3(2.0f, 0.5f, 2.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &stageRightMatrix[0][0]);
        setModelMatrix(shaderScene, stageRightMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //building and rendering screen model

        mat4 screenMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, -4.0f)) * scale(mat4(1.0f), vec3(4.0f, 3.0f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &screenMatrix[0][0]);
        setModelMatrix(shaderScene, screenMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //rendering screen pillers

        mat4 leftScreenPillarMatrix = translate(mat4(1.0f), vec3(-1.5f, 0.75f, -4.0f)) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leftScreenPillarMatrix[0][0]);
        setModelMatrix(shaderScene, leftScreenPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        mat4 rightScreenPillarMatrix = translate(mat4(1.0f), vec3(1.5f, 0.75f, -4.0f)) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rightScreenPillarMatrix[0][0]);
        setModelMatrix(shaderScene, rightScreenPillarMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        if ((int)lastFrameTime % 20 < 10)
            glBindTexture(GL_TEXTURE_2D, scrsht1TextureID);
        else if ((int)lastFrameTime % 20 >= 10)
            glBindTexture(GL_TEXTURE_2D, cementTextureID);

        mat4 changingScreenMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, -3.725f)) * scale(mat4(1.0f), vec3(4.0f, 3.0f, 0.01f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &changingScreenMatrix[0][0]);
        setModelMatrix(shaderScene, changingScreenMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, scrsht2TextureID);

        mat4 skyBoxMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(75.0f, 75.0f, 75.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &skyBoxMatrix[0][0]);
        setModelMatrix(shaderScene, skyBoxMatrix);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = 1;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = 2;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = 3;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = 4;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = 5;
        }



        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // - Calculate mouse motion dx and dy
        // - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        // Use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPosition += cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }

        

        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
        mat4 viewMatrix(1.0f);

        if (cameraFirstPerson == 1)
        {
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
        }
        else if (cameraFirstPerson == 2)
        {
            viewMatrix = lookAt(vec3(camX, 5.0f, camZ), vec3(0.0f, 0.0f, 0.0f), cameraUp);
        }
        else if (cameraFirstPerson == 3) {
            viewMatrix = lookAt(vec3(0.0f, 21.0f, -10.5f), vec3(0.0f, 0.0f, 0.0f), cameraUp);
        }
        else if (cameraFirstPerson == 4) {
            viewMatrix = lookAt(vec3(0.0f, 21.0f, -14.5f), vec3(0.0f, 0.0f, -23.5f), cameraUp);
        }
        else if (cameraFirstPerson == 5) {
            viewMatrix = lookAt(vec3(0.01f, 37.5f, 0.01f), vec3(0.0f, 0.0f, -10.0f), cameraUp);
        }

        lightPosition = vec3(camX, 5.0f, camZ); // the location of the light in 3D space
        lightFocus = vec3(0.1f, 0.1f, 0.1f);      // the point in 3D space the light "looks" at
        lightDirection = normalize(lightFocus - lightPosition);

        //vec3 lightPosition2 = vec3(0.0f, 0.0f, 0.0f); // the location of the light in 3D space
        //vec3 lightFocus2(0.0f, 0.0f, 0.0f);      // the point in 3D space the light "looks" at
        //vec3 lightDirection2 = normalize(lightFocus2 - lightPosition2);

        lightViewMatrix = lookAt(lightPosition, lightFocus, vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shaderScene, "light_view_proj_matrix", lightSpaceMatrix);

        // Set light position on scene shader
        SetUniformVec3(shaderScene, "light_position", lightFocus);

        // Set light direction on scene shader
        SetUniformVec3(shaderScene, "light_direction", lightDirection);

        setViewMatrix(shaderScene, viewMatrix);
    }

    glfwTerminate();

    return 0;
}

GLuint loadTexture(const char *filename)
{
  // Step1 Create and bind textures
  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  assert(textureId != 0);


  glBindTexture(GL_TEXTURE_2D, textureId);

  // Step2 Set filter parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Step3 Load Textures with dimension data
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!data)
  {
    std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
    return 0;
  }

  // Step4 Upload the texture to the PU
  GLenum format = 0;
  if (nrChannels == 1)
      format = GL_RED;
  else if (nrChannels == 3)
      format = GL_RGB;
  else if (nrChannels == 4)
      format = GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
               0, format, GL_UNSIGNED_BYTE, data);

  // Step5 Free resources
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return textureId;
}

int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)(2*sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
                          );
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

int createlightCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);     //position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));   //normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));    //texture
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}
