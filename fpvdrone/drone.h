#pragma once
#include "HUD.h"

class Enemy;
class Drone {
public:
	int kills = 0;
	float camX = 0.0f;
	float camY = 15.0f;
	float camZ = 5.0f;

	float yaw = 0.0f;   // поворот влево / вправо
	float pitch = 0.0f;   // наклон вверх / вниз
	float roll = 0.0f;   // крен

	float rotSpeed = 0.1f;
	float moveSpeed = 50.0f;

	int damage = 70;

	void processInput(GLFWwindow* window,float deltaTime);
	bool checkCollision(const Enemy* enemy);
	void respawn();
	friend class HUD;
};