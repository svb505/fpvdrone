#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "drone.h"
#include <cmath>
#include <format>
#include <iostream>
#include <algorithm>
#include "enemyes.h"
#include <windows.h>
#undef min
#undef max


void Drone::processInput(GLFWwindow* window,float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw -= rotSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pitch += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pitch -= rotSpeed;

    pitch = std::clamp(pitch, -89.0f, 89.0f);

    float speed = moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        speed *= 4.5f; // буст
    }

    // ===== ANGLES =====
    float yawRad = yaw * 3.1415926f / 180.0f;
    float pitchRad = pitch * 3.1415926f / 180.0f;

    // ===== DIRECTION VECTORS =====
    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = sin(pitchRad);
    float forwardZ = cos(yawRad) * cos(pitchRad);

    float rightX = cos(yawRad);
    float rightZ = -sin(yawRad);

    // ===== MOVEMENT INPUT =====
    float moveForward = -0.5f;
    float moveRight = 0.0f;
    float moveUp = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        moveForward -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        moveForward += 1.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        moveRight -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        moveRight += 1.0f;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        moveUp += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        moveUp -= 1.0f;

    // ===== APPLY MOVEMENT =====
    camX += (forwardX * moveForward + rightX * moveRight) * speed * deltaTime;
    camY += (forwardY * moveForward + moveUp) * speed * deltaTime;
    camZ += (forwardZ * moveForward + rightZ * moveRight) * speed * deltaTime;

    // ===== GROUND LIMIT =====
    if (camY < 0.1f)
        camY = 0.1f;
}

bool Drone::checkCollision(const Enemy* enemy)
{
    if (!enemy) return false;

    float minX = enemy->vertices[0][0], maxX = enemy->vertices[0][0];
    float minY = enemy->vertices[0][1], maxY = enemy->vertices[0][1];
    float minZ = enemy->vertices[0][2], maxZ = enemy->vertices[0][2];

    for (auto v : enemy->vertices) {
        minX = std::min(minX, v[0]);
        maxX = std::max(maxX, v[0]);
        minY = std::min(minY, v[1]);
        maxY = std::max(maxY, v[1]);
        minZ = std::min(minZ, v[2]);
        maxZ = std::max(maxZ, v[2]);
    }

    return (camX >= minX && camX <= maxX) &&
        (camY >= minY && camY <= maxY) &&
        (camZ >= minZ && camZ <= maxZ);
}


void Drone::respawn() {
    camX = 0.0f;
    camY = 15.0f;
    camZ = 5.0f;
    yaw = 0.0f;   
    pitch = 0.0f;  
    roll = 0.0f;   
}
