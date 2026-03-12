#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

class Enemy {
public:
    int maxHealth = 100;
    int health = 0;
    bool destroyed = false;
    float angle = 0;
    std::vector<float> position = { 0,0,0 };

    std::vector<std::vector<float>> vertices;

    void SetDestroyed(bool d) { destroyed = d; }
    bool IsDestroyed() const { return destroyed; }

    void Draw() {
        if (destroyed) return;
        OnDraw();
    }

    virtual void OnDraw() = 0;
    virtual void Update(float dt) {}
    virtual void UpdateVertices() = 0;
};
class Soldat : public Enemy {
public:
    Soldat() {
        maxHealth = 50;
        health = 50;
    }
    void OnDraw() override {
        float height = 0.7f;  // вертикальная высота
        float width = 0.3f;   // горизонтальная ширина

        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        glBegin(GL_QUADS);

        // Front
        glColor3f(0, 0.7f, 0);
        glVertex3f(-width, 0, width);
        glVertex3f(width, 0, width);
        glVertex3f(width, height, width);
        glVertex3f(-width, height, width);

        // Back
        glColor3f(0, 0.7f, 0);
        glVertex3f(width, 0, -width);
        glVertex3f(-width, 0, -width);
        glVertex3f(-width, height, -width);
        glVertex3f(width, height, -width);

        // Left
        glColor3f(0, 1, 0.3f);
        glVertex3f(-width, 0, -width);
        glVertex3f(-width, 0, width);
        glVertex3f(-width, height, width);
        glVertex3f(-width, height, -width);

        // Right
        glColor3f(0, 1, 0.3f);
        glVertex3f(width, 0, -width);
        glVertex3f(width, height, -width);
        glVertex3f(width, height, width);
        glVertex3f(width, 0, width);

        // Top
        glColor3f(0, 1, 0);
        glVertex3f(-width, height, -width);
        glVertex3f(-width, height, width);
        glVertex3f(width, height, width);
        glVertex3f(width, height, -width);

        // Bottom
        glColor3f(0, 1, 0);
        glVertex3f(-width, 0, -width);
        glVertex3f(width, 0, -width);
        glVertex3f(width, 0, width);
        glVertex3f(-width, 0, width);

        glEnd();
        glPopMatrix();
    }

    void UpdateVertices() override {
        float height = 1.5f;
        float width = 0.5f;
        vertices = {
            {position[0] - width, position[1], position[2] - width},
            {position[0] + width, position[1] + height, position[2] + width},
            {position[0] - width, position[1] + height, position[2] + width},
            {position[0] + width, position[1], position[2] - width},
        };
    }
};

class Vehicle : public Enemy {
public:
    Vehicle() {
        maxHealth = 100;
        health = 100;
    }

	void OnDraw() override {
        float step = 0.6f;

        glPushMatrix();                  

        glTranslatef(position[0],position[1],position[2]);
        glRotatef(angle, 0, 1, 0);

        glBegin(GL_QUADS);

        // Front
        glColor3f(0, 1, 0);
        glVertex3f(-1, -step, 1);
        glVertex3f(1, -step, 1);
        glVertex3f(1, step, 1);
        glVertex3f(-1, step, 1);

        // Back
        glColor3f(0, 1, 0);
        glVertex3f(1, -step, -1);
        glVertex3f(-1, step, -1);
        glVertex3f(1, step, -1);
        glVertex3f(1, -step, -1);

        // Left
        glColor3f(0, 1, 0.7);
        glVertex3f(-1, -step, -1);
        glVertex3f(-1, -step, 1);
        glVertex3f(-1, step, 1);
        glVertex3f(-1, step, -1);

        // Right
        glColor3f(0, 1, 0.7);
        glVertex3f(1, -step, -1);
        glVertex3f(1, step, -1);
        glVertex3f(1, step, 1);
        glVertex3f(1, -step, 1);

        // Top
        glColor3f(0, 1, 0);
        glVertex3f(-1, step, -1);
        glVertex3f(-1, step, 1);
        glVertex3f(1, step, 1);
        glVertex3f(1, step, -1);

        // Bottom
        glColor3f(0, 1, 0);
        glVertex3f(-1, -step, -1);
        glVertex3f(1, -step, -1);
        glVertex3f(1, -step, 1);
        glVertex3f(-1, -step, 1);

        glEnd();

        glPopMatrix();
	}
    void UpdateVertices() override {
        float step = 0.6f;
        vertices = {
            {position[0] - 1, position[1] - step, position[2] - 1},
            {position[0] + 1, position[1] + step, position[2] + 1},
            {position[0] - 1, position[1] + step, position[2] + 1},
            {position[0] + 1, position[1] - step, position[2] - 1},
            // можно добавить остальные вершины куба, чтобы точнее
        };
    }
};
class Tank : public  Enemy {
private:
    float turretAngle = 0.0f;
    float gunAngle = 0.0f;
public:
    Tank(float tAngle,float gAngle) {
        turretAngle = tAngle;
        gunAngle = gAngle;
        maxHealth = 200;
        health = 200;
    }
    void OnDraw() override {
        float bodyH = 0.6f;

        glPushMatrix();

        // ===== WORLD TRANSFORM =====
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // ================= BODY =================
        glColor3f(0, 0.8f, 0);

        glBegin(GL_QUADS);
        // Front
        glVertex3f(-1, -bodyH, 1);
        glVertex3f(1, -bodyH, 1);
        glVertex3f(1, bodyH, 1);
        glVertex3f(-1, bodyH, 1);

        // Back
        glVertex3f(-1, -bodyH, -1);
        glVertex3f(-1, bodyH, -1);
        glVertex3f(1, bodyH, -1);
        glVertex3f(1, -bodyH, -1);

        // Left
        glVertex3f(-1, -bodyH, -1);
        glVertex3f(-1, -bodyH, 1);
        glVertex3f(-1, bodyH, 1);
        glVertex3f(-1, bodyH, -1);

        // Right
        glVertex3f(1, -bodyH, -1);
        glVertex3f(1, bodyH, -1);
        glVertex3f(1, bodyH, 1);
        glVertex3f(1, -bodyH, 1);

        // Top
        glVertex3f(-1, bodyH, -1);
        glVertex3f(-1, bodyH, 1);
        glVertex3f(1, bodyH, 1);
        glVertex3f(1, bodyH, -1);

        // Bottom
        glVertex3f(-1, -bodyH, -1);
        glVertex3f(1, -bodyH, -1);
        glVertex3f(1, -bodyH, 1);
        glVertex3f(-1, -bodyH, 1);
        glEnd();

        // ================= TURRET =================
        glPushMatrix();

        glTranslatef(0.0f, bodyH + 0.4f, 0.0f);
        glRotatef(turretAngle, 0, 1, 0);

        float t = 0.5f;
        glColor3f(0, 0.6f, 0.2f);

        glBegin(GL_QUADS);
        // Front
        glVertex3f(-t, -t, t);
        glVertex3f(t, -t, t);
        glVertex3f(t, t, t);
        glVertex3f(-t, t, t);

        // Back
        glVertex3f(-t, -t, -t);
        glVertex3f(-t, t, -t);
        glVertex3f(t, t, -t);
        glVertex3f(t, -t, -t);

        // Left
        glVertex3f(-t, -t, -t);
        glVertex3f(-t, -t, t);
        glVertex3f(-t, t, t);
        glVertex3f(-t, t, -t);

        // Right
        glVertex3f(t, -t, -t);
        glVertex3f(t, t, -t);
        glVertex3f(t, t, t);
        glVertex3f(t, -t, t);

        // Top
        glVertex3f(-t, t, -t);
        glVertex3f(-t, t, t);
        glVertex3f(t, t, t);
        glVertex3f(t, t, -t);

        // Bottom
        glVertex3f(-t, -t, -t);
        glVertex3f(t, -t, -t);
        glVertex3f(t, -t, t);
        glVertex3f(-t, -t, t);
        glEnd();

        // ================= GUN =================
        glPushMatrix();

        glTranslatef(0.0f, 0.0f, t);
        glRotatef(gunAngle, 1, 0, 0);

        float w = 0.12f;
        float h = 0.12f;
        float len = 1.8f;

        glColor3f(0.2f, 0.2f, 0.2f);

        glBegin(GL_QUADS);
        // Front
        glVertex3f(-w, -h, len);
        glVertex3f(w, -h, len);
        glVertex3f(w, h, len);
        glVertex3f(-w, h, len);

        // Top
        glVertex3f(-w, h, 0);
        glVertex3f(-w, h, len);
        glVertex3f(w, h, len);
        glVertex3f(w, h, 0);

        // Bottom
        glVertex3f(-w, -h, 0);
        glVertex3f(w, -h, 0);
        glVertex3f(w, -h, len);
        glVertex3f(-w, -h, len);

        // Left
        glVertex3f(-w, -h, 0);
        glVertex3f(-w, -h, len);
        glVertex3f(-w, h, len);
        glVertex3f(-w, h, 0);

        // Right
        glVertex3f(w, -h, 0);
        glVertex3f(w, h, 0);
        glVertex3f(w, h, len);
        glVertex3f(w, -h, len);
        glEnd();

        glPopMatrix(); // gun
        glPopMatrix(); // turret
        glPopMatrix(); // tank
    }
    void UpdateVertices() override {
        float bodyH = 0.6f;
        float turretH = 0.5f + 0.12f; // башня + пушка
        float gunLen = 1.8f;
        vertices = {
            {position[0] - 1, position[1] - bodyH, position[2] - 1},
            {position[0] + 1, position[1] + bodyH + turretH, position[2] + 1 + gunLen},
            // добавить остальные вершины тела, башни и пушки
        };
    }

};
class Radar : public Enemy {
public:
    float rotationSpeed = 90.0f; // градусов в секунду

    Radar() {
        maxHealth = 120;
        health = 120;
    }

    void Update(float dt) override {
        angle += rotationSpeed * dt;
        if (angle >= 360.0f) angle -= 360.0f;
    }

    // ===== Примитив: цилиндр =====
    void DrawCylinder(float r, float h, int slices) {
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            float a = i * 2.0f * 3.14159f / slices;
            float x = cos(a) * r;
            float z = sin(a) * r;
            glVertex3f(x, 0, z);
            glVertex3f(x, h, z);
        }
        glEnd();
    }

    // ===== Примитив: параболическая тарелка =====
    void DrawDish(float radius, float depth) {
        int slices = 20;
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0); // центр
        for (int i = 0; i <= slices; i++) {
            float a = i * 2.0f * 3.14159f / slices;
            float x = cos(a) * radius;
            float y = sin(a) * radius;
            glVertex3f(x, y, -depth);
        }
        glEnd();
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);

        // ===== БЕТОННОЕ ОСНОВАНИЕ =====
        glColor3f(0.4f, 0.4f, 0.4f);
        glBegin(GL_QUADS);
        glVertex3f(-0.8f, 0, -0.8f);
        glVertex3f(0.8f, 0, -0.8f);
        glVertex3f(0.8f, 0.3f, -0.8f);
        glVertex3f(-0.8f, 0.3f, -0.8f);
        glEnd();

        // ===== МАЧТА =====
        glTranslatef(0, 0.3f, 0);
        glColor3f(0.6f, 0.6f, 0.6f);
        DrawCylinder(0.12f, 1.4f, 16);

        // ===== АНТЕННА (ВРАЩАЕТСЯ) =====
        glTranslatef(0, 1.5f, 0);
        glRotatef(angle, 0, 1, 0);

        // рамка
        glColor3f(0.1f, 0.8f, 0.1f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 32; i++) {
            float a = i * 2.0f * 3.14159f / 32;
            glVertex3f(cos(a) * 1.0f, sin(a) * 0.7f, 0);
        }
        glEnd();

        // тарелка
        glTranslatef(0, 0, -0.05f);
        glColor3f(0.0f, 0.6f, 0.0f);
        DrawDish(1.0f, 0.3f);

        // излучатель
        glColor3f(0.9f, 0.9f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(-0.05f, -0.05f, 0.1f);
        glVertex3f(0.05f, -0.05f, 0.1f);
        glVertex3f(0.05f, 0.05f, 0.1f);
        glVertex3f(-0.05f, 0.05f, 0.1f);
        glEnd();

        glPopMatrix();
    }

    void UpdateVertices() override {
        vertices = {
            {position[0] - 1.0f, position[1], position[2] - 1.0f},
            {position[0] + 1.0f, position[1] + 2.0f, position[2] + 1.0f}
        };
    }
};

class FuelTruck : public Enemy {
public:
    FuelTruck() {
        maxHealth = 150;
        health = 150;
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // ===== Кабина =====
        glColor3f(0.2f, 0.6f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(-1.2f, -0.4f, 1.2f);
        glVertex3f(-0.2f, -0.4f, 1.2f);
        glVertex3f(-0.2f, 0.6f, 1.2f);
        glVertex3f(-1.2f, 0.6f, 1.2f);
        glEnd();

        // ===== Цистерна =====
        glTranslatef(0.6f, 0.0f, 0.0f);
        glColor3f(0.6f, 0.6f, 0.6f);

        float r = 0.5f;
        float len = 2.2f;

        glBegin(GL_QUADS);
        glVertex3f(-r, -r, -len);
        glVertex3f(r, -r, -len);
        glVertex3f(r, r, len);
        glVertex3f(-r, r, len);
        glEnd();

        glPopMatrix();
    }

    void UpdateVertices() override {
        vertices = {
            {position[0] - 1.2f, position[1] - 0.5f, position[2] - 2.2f},
            {position[0] + 1.2f, position[1] + 0.7f, position[2] + 2.2f}
        };
    }
};

class AmmoStorage : public Enemy {
public:
    AmmoStorage() {
        maxHealth = 400;
        health = 400;
    }
    void OnDraw() override {
        glPushMatrix();

        // ===== WORLD TRANSFORM =====
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // ================= MAIN BUILDING =================
        float h = 0.8f;
        glColor3f(0.5f, 0.5f, 0.5f);

        glBegin(GL_QUADS);
        // Front
        glVertex3f(-3, -h, 2);
        glVertex3f(3, -h, 2);
        glVertex3f(3, h, 2);
        glVertex3f(-3, h, 2);

        // Back
        glVertex3f(-3, -h, -2);
        glVertex3f(-3, h, -2);
        glVertex3f(3, h, -2);
        glVertex3f(3, -h, -2);

        // Left
        glVertex3f(-3, -h, -2);
        glVertex3f(-3, -h, 2);
        glVertex3f(-3, h, 2);
        glVertex3f(-3, h, -2);

        // Right
        glVertex3f(3, -h, -2);
        glVertex3f(3, h, -2);
        glVertex3f(3, h, 2);
        glVertex3f(3, -h, 2);

        // Top
        glVertex3f(-3, h, -2);
        glVertex3f(-3, h, 2);
        glVertex3f(3, h, 2);
        glVertex3f(3, h, -2);

        // Bottom
        glVertex3f(-3, -h, -2);
        glVertex3f(3, -h, -2);
        glVertex3f(3, -h, 2);
        glVertex3f(-3, -h, 2);
        glEnd();

        // ================= AMMO CRATES =================
        glColor3f(0.3f, 0.6f, 0.3f);

        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(-2.0f + i * 1.3f, -h + 0.4f, 1.4f);

            float c = 0.3f;

            glBegin(GL_QUADS);
            // Front
            glVertex3f(-c, -c, c);
            glVertex3f(c, -c, c);
            glVertex3f(c, c, c);
            glVertex3f(-c, c, c);

            // Back
            glVertex3f(-c, -c, -c);
            glVertex3f(-c, c, -c);
            glVertex3f(c, c, -c);
            glVertex3f(c, -c, -c);

            // Left
            glVertex3f(-c, -c, -c);
            glVertex3f(-c, -c, c);
            glVertex3f(-c, c, c);
            glVertex3f(-c, c, -c);

            // Right
            glVertex3f(c, -c, -c);
            glVertex3f(c, c, -c);
            glVertex3f(c, c, c);
            glVertex3f(c, -c, c);

            // Top
            glVertex3f(-c, c, -c);
            glVertex3f(-c, c, c);
            glVertex3f(c, c, c);
            glVertex3f(c, c, -c);

            // Bottom
            glVertex3f(-c, -c, -c);
            glVertex3f(c, -c, -c);
            glVertex3f(c, -c, c);
            glVertex3f(-c, -c, c);
            glEnd();

            glPopMatrix();
        }

        // ================= LONG AMMO CONTAINERS =================
        glColor3f(0.4f, 0.4f, 0.2f);

        for (int i = 0; i < 2; i++) {
            glPushMatrix();
            glTranslatef(0.0f, -h + 0.35f, -1.0f + i * 1.4f);

            float w = 2.2f;
            float hh = 0.3f;
            float d = 0.4f;

            glBegin(GL_QUADS);
            // Front
            glVertex3f(-w, -hh, d);
            glVertex3f(w, -hh, d);
            glVertex3f(w, hh, d);
            glVertex3f(-w, hh, d);

            // Back
            glVertex3f(-w, -hh, -d);
            glVertex3f(-w, hh, -d);
            glVertex3f(w, hh, -d);
            glVertex3f(w, -hh, -d);

            // Left
            glVertex3f(-w, -hh, -d);
            glVertex3f(-w, -hh, d);
            glVertex3f(-w, hh, d);
            glVertex3f(-w, hh, -d);

            // Right
            glVertex3f(w, -hh, -d);
            glVertex3f(w, hh, -d);
            glVertex3f(w, hh, d);
            glVertex3f(w, -hh, d);

            // Top
            glVertex3f(-w, hh, -d);
            glVertex3f(-w, hh, d);
            glVertex3f(w, hh, d);
            glVertex3f(w, hh, -d);

            // Bottom
            glVertex3f(-w, -hh, -d);
            glVertex3f(w, -hh, -d);
            glVertex3f(w, -hh, d);
            glVertex3f(-w, -hh, d);
            glEnd();

            glPopMatrix();
        }

        glPopMatrix();
    }
    void UpdateVertices() override {
        float h = 0.8f;
        float w = 3.0f;
        float d = 2.0f;
        vertices = {
            {position[0] - w, position[1] - h, position[2] - d},
            {position[0] + w, position[1] + h, position[2] + d},
            // при желании добавить контейнеры и длинные ящики для точности
        };
    }
};
class ApartmentBuilding : public Enemy {
public:
    int floors = 5;          // количество этажей
    float floorHeight = 0.6f;
    float width = 4.0f;
    float depth = 2.0f;

    ApartmentBuilding(int f = 5) {
        floors = f;
        maxHealth = 1000;
        health = 1000;
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        float totalH = floors * floorHeight;

        // ===== ОСНОВНОЙ КОРПУС =====
        glColor3f(0.75f, 0.75f, 0.7f);

        glBegin(GL_QUADS);
        // Front
        glVertex3f(-width, 0, depth);
        glVertex3f(width, 0, depth);
        glVertex3f(width, totalH, depth);
        glVertex3f(-width, totalH, depth);

        // Back
        glVertex3f(-width, 0, -depth);
        glVertex3f(-width, totalH, -depth);
        glVertex3f(width, totalH, -depth);
        glVertex3f(width, 0, -depth);

        // Left
        glVertex3f(-width, 0, -depth);
        glVertex3f(-width, 0, depth);
        glVertex3f(-width, totalH, depth);
        glVertex3f(-width, totalH, -depth);

        // Right
        glVertex3f(width, 0, -depth);
        glVertex3f(width, totalH, -depth);
        glVertex3f(width, totalH, depth);
        glVertex3f(width, 0, depth);

        // Roof
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-width, totalH, -depth);
        glVertex3f(-width, totalH, depth);
        glVertex3f(width, totalH, depth);
        glVertex3f(width, totalH, -depth);
        glEnd();

        // ===== ОКНА =====
        glColor3f(0.2f, 0.4f, 0.8f);

        for (int f = 0; f < floors; f++) {
            float y = f * floorHeight + 0.15f;

            for (int i = -3; i <= 3; i += 2) {
                glBegin(GL_QUADS);
                glVertex3f(i * 0.6f - 0.2f, y, depth + 0.01f);
                glVertex3f(i * 0.6f + 0.2f, y, depth + 0.01f);
                glVertex3f(i * 0.6f + 0.2f, y + 0.25f, depth + 0.01f);
                glVertex3f(i * 0.6f - 0.2f, y + 0.25f, depth + 0.01f);
                glEnd();
            }
        }

        // ===== ПОДЪЕЗД =====
        glColor3f(0.3f, 0.2f, 0.1f);
        glBegin(GL_QUADS);
        glVertex3f(-0.5f, 0, depth + 0.02f);
        glVertex3f(0.5f, 0, depth + 0.02f);
        glVertex3f(0.5f, 0.8f, depth + 0.02f);
        glVertex3f(-0.5f, 0.8f, depth + 0.02f);
        glEnd();

        glPopMatrix();
    }

    void UpdateVertices() override {
        float totalH = floors * floorHeight;
        vertices = {
            {position[0] - width, position[1], position[2] - depth},
            {position[0] + width, position[1] + totalH, position[2] + depth},
        };
    }
};
class CommTower : public Enemy {
private:
    float antennaHeight = 2.0f;
public:
    CommTower(float height) {
        antennaHeight = height;
        maxHealth = 100;
        health = 100;
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // Основание
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_QUADS);
        glVertex3f(-0.5f, 0, -0.5f);
        glVertex3f(0.5f, 0, -0.5f);
        glVertex3f(0.5f, 1.0f, -0.5f);
        glVertex3f(-0.5f, 1.0f, -0.5f);
        glEnd();

        // Антенна
        glColor3f(0.8f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 1.0f, 0);
        glVertex3f(0, 1.0f + antennaHeight, 0);
        glEnd();

        glPopMatrix();
    }

    void UpdateVertices() override {
        vertices = {
            {position[0] - 0.5f, position[1], position[2] - 0.5f},
            {position[0] + 0.5f, position[1] + 1.0f + antennaHeight, position[2] + 0.5f}
        };
    }
};
class MLRS : public Enemy {
private:
    float launcherAngle = 45.0f; // угол пусковой установки
public:
    MLRS(float angle) {
        launcherAngle = angle;
        maxHealth = 150;
        health = 150;
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // ================= КОРПУС =================
        glColor3f(0.3f, 0.3f, 0.3f);
        float h = 0.5f, w = 1.0f, d = 0.6f;
        glBegin(GL_QUADS);
        // Front
        glVertex3f(-w, 0, d);
        glVertex3f(w, 0, d);
        glVertex3f(w, h, d);
        glVertex3f(-w, h, d);
        // Back
        glVertex3f(-w, 0, -d);
        glVertex3f(-w, h, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, 0, -d);
        // Left
        glVertex3f(-w, 0, -d);
        glVertex3f(-w, 0, d);
        glVertex3f(-w, h, d);
        glVertex3f(-w, h, -d);
        // Right
        glVertex3f(w, 0, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, h, d);
        glVertex3f(w, 0, d);
        // Top
        glVertex3f(-w, h, -d);
        glVertex3f(-w, h, d);
        glVertex3f(w, h, d);
        glVertex3f(w, h, -d);
        // Bottom
        glVertex3f(-w, 0, -d);
        glVertex3f(w, 0, -d);
        glVertex3f(w, 0, d);
        glVertex3f(-w, 0, d);
        glEnd();

        // ================= ПУСКОВАЯ УСТАНОВКА =================
        glPushMatrix();
        glTranslatef(0, h, 0);
        glRotatef(-launcherAngle, 1, 0, 0);

        glColor3f(0.6f, 0.2f, 0.2f);
        float lw = 0.15f, lh = 0.15f, ld = 2.0f;
        glBegin(GL_QUADS);
        // Front
        glVertex3f(-lw, -lh, ld);
        glVertex3f(lw, -lh, ld);
        glVertex3f(lw, lh, ld);
        glVertex3f(-lw, lh, ld);
        // Back
        glVertex3f(-lw, -lh, 0);
        glVertex3f(-lw, lh, 0);
        glVertex3f(lw, lh, 0);
        glVertex3f(lw, -lh, 0);
        // Left
        glVertex3f(-lw, -lh, 0);
        glVertex3f(-lw, -lh, ld);
        glVertex3f(-lw, lh, ld);
        glVertex3f(-lw, lh, 0);
        // Right
        glVertex3f(lw, -lh, 0);
        glVertex3f(lw, lh, 0);
        glVertex3f(lw, lh, ld);
        glVertex3f(lw, -lh, ld);
        // Top
        glVertex3f(-lw, lh, 0);
        glVertex3f(-lw, lh, ld);
        glVertex3f(lw, lh, ld);
        glVertex3f(lw, lh, 0);
        // Bottom
        glVertex3f(-lw, -lh, 0);
        glVertex3f(lw, -lh, 0);
        glVertex3f(lw, -lh, ld);
        glVertex3f(-lw, -lh, ld);
        glEnd();

        glPopMatrix(); // launcher
        glPopMatrix(); // mlrs
    }

    void UpdateVertices() override {
        vertices = {
            {position[0] - 1.0f, position[1], position[2] - 0.6f},
            {position[0] + 1.0f, position[1] + 0.5f + 0.15f, position[2] + 2.0f}
        };
    }
};

class SAM : public Enemy {
private:
    float radarRotation = 0.0f;
public:
    SAM() {
        maxHealth = 180;
        health = 180;
    }

    void OnDraw() override {
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glRotatef(angle, 0, 1, 0);

        // ================= БАЗА =================
        glColor3f(0.4f, 0.4f, 0.4f);
        float w = 0.7f, h = 0.5f, d = 0.7f;
        glBegin(GL_QUADS);
        // Front
        glVertex3f(-w, 0, d);
        glVertex3f(w, 0, d);
        glVertex3f(w, h, d);
        glVertex3f(-w, h, d);
        // Back
        glVertex3f(-w, 0, -d);
        glVertex3f(-w, h, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, 0, -d);
        // Left
        glVertex3f(-w, 0, -d);
        glVertex3f(-w, 0, d);
        glVertex3f(-w, h, d);
        glVertex3f(-w, h, -d);
        // Right
        glVertex3f(w, 0, -d);
        glVertex3f(w, h, -d);
        glVertex3f(w, h, d);
        glVertex3f(w, 0, d);
        // Top
        glVertex3f(-w, h, -d);
        glVertex3f(-w, h, d);
        glVertex3f(w, h, d);
        glVertex3f(w, h, -d);
        // Bottom
        glVertex3f(-w, 0, -d);
        glVertex3f(w, 0, -d);
        glVertex3f(w, 0, d);
        glVertex3f(-w, 0, d);
        glEnd();

        // ================= РАДАР =================
        glPushMatrix();
        glTranslatef(0, h, 0);
        glRotatef(radarRotation, 0, 1, 0);

        glColor3f(0.0f, 0.6f, 0.6f);
        float rw = 0.4f, rh = 0.05f, rd = 0.4f;
        glBegin(GL_QUADS);
        // Front
        glVertex3f(-rw, -rh, rd);
        glVertex3f(rw, -rh, rd);
        glVertex3f(rw, rh, rd);
        glVertex3f(-rw, rh, rd);
        // Back
        glVertex3f(-rw, -rh, -rd);
        glVertex3f(-rw, rh, -rd);
        glVertex3f(rw, rh, -rd);
        glVertex3f(rw, -rh, -rd);
        // Left
        glVertex3f(-rw, -rh, -rd);
        glVertex3f(-rw, -rh, rd);
        glVertex3f(-rw, rh, rd);
        glVertex3f(-rw, rh, -rd);
        // Right
        glVertex3f(rw, -rh, -rd);
        glVertex3f(rw, rh, -rd);
        glVertex3f(rw, rh, rd);
        glVertex3f(rw, -rh, rd);
        // Top
        glVertex3f(-rw, rh, -rd);
        glVertex3f(-rw, rh, rd);
        glVertex3f(rw, rh, rd);
        glVertex3f(rw, rh, -rd);
        // Bottom
        glVertex3f(-rw, -rh, -rd);
        glVertex3f(rw, -rh, -rd);
        glVertex3f(rw, -rh, rd);
        glVertex3f(-rw, -rh, rd);
        glEnd();

        glPopMatrix(); // radar
        glPopMatrix(); // sam
    }

    void UpdateVertices() override {
        vertices = {
            {position[0] - 0.7f, position[1], position[2] - 0.7f},
            {position[0] + 0.7f, position[1] + 0.5f + 0.05f, position[2] + 0.7f}
        };
    }
};

