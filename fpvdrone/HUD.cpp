#include "HUD.h"
#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "text.h"
#include <format>
#include "drone.h"

// HUD::drawHud
void HUD::drawHud(int screenWidth, int screenHeight, Drone& drone,const int count) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glColor3f(1, 1, 1);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(screenWidth / 2 - 100, screenHeight / 2);
    glVertex2f(screenWidth / 2 + 100, screenHeight / 2);

    glVertex2f(screenWidth / 2, screenHeight / 2 - 100);
    glVertex2f(screenWidth / 2, screenHeight / 2 + 100);
    glEnd();

    RenderTextHUD(10, 20, 1, 1, 1, std::format("Height: {:.1f}", drone.camY).c_str(), screenWidth, screenHeight);
    RenderTextHUD(10, 40, 1, 1, 1, std::format("Kills: {}", drone.kills).c_str(), screenWidth, screenHeight);
    RenderTextHUD(10, 60, 1, 1, 1, std::format("Total enemyes: {}", count).c_str(), screenWidth, screenHeight);
    RenderTextHUD(screenWidth - 90, screenHeight - 90, 1, 1, 1, std::format("X: {:.1f}", drone.camX).c_str(), screenWidth, screenHeight);
    RenderTextHUD(screenWidth - 90, screenHeight - 130, 1, 1, 1, std::format("Z: {:.1f}", drone.camZ).c_str(), screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
