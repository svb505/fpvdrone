#include <windows.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <ctime>
#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "text.h"
#include "drone.h"
#include "enemyes.h"
#include "environement.h"
#include "effects.h"
#include "HUD.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#define COUNT 55

ALCdevice* audioDevice = nullptr;
ALCcontext* audioContext = nullptr;

ALuint droneBuffer = 0;
ALuint explosionBuffer = 0;

ALuint droneSource = 0;
ALuint explosionSource = 0;

ALuint LoadWav(const char* filename)
{
    SF_INFO info{};
    SNDFILE* file = sf_open(filename, SFM_READ, &info);
    if (!file) {
        std::cerr << "Failed to load sound: " << filename << "\n";
        return 0;
    }

    std::vector<short> samples(info.frames * info.channels);
    sf_readf_short(file, samples.data(), info.frames);
    sf_close(file);

    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALenum format =
        (info.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    alBufferData(
        buffer,
        format,
        samples.data(),
        (ALsizei)(samples.size() * sizeof(short)),
        info.samplerate
    );

    return buffer;
}

float fov = 70.0f;
float aspect = 1600.0f / 1000.0f;
float nearPlane = 0.1f;
float farPlane = 300.0f;

float top = tan(fov * 3.1415926f / 360.0f) * nearPlane;
float bottom = -top;
float right = top * aspect;
float left = -right;

struct Info {
    Enemy* enemy;
    std::vector<float> positions = { 0.0f, 0.0f, 0.0f };
    std::string type = "";
};

int main()
{
    srand((unsigned)time(nullptr));

    audioDevice = alcOpenDevice(nullptr);
    audioContext = alcCreateContext(audioDevice, nullptr);
    alcMakeContextCurrent(audioContext);

    droneBuffer = LoadWav("sounds/drone.wav");
    explosionBuffer = LoadWav("sounds/explosion.wav");

    alGenSources(1, &droneSource);
    alSourcei(droneSource, AL_BUFFER, droneBuffer);
    alSourcei(droneSource, AL_LOOPING, AL_TRUE);
    alSourcef(droneSource, AL_GAIN, 0.7f);
    alSourcePlay(droneSource);

    alGenSources(1, &explosionSource);
    alSourcei(explosionSource, AL_BUFFER, explosionBuffer);
    alSourcef(explosionSource, AL_GAIN, 1.0f);

    int width, height, channels;
    unsigned char* pixels = stbi_load("icon.png", &width, &height, &channels, 4);

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1600, 1000, "FPV Drone", nullptr, nullptr);
    if (!window) return -1;

    if (pixels) {
        GLFWimage icon{ width, height, pixels };
        glfwSetWindowIcon(window, 1, &icon);
        stbi_image_free(pixels);
    }

    glfwMakeContextCurrent(window);

    BuildFont();
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);

    std::map<int, Info> enemyes;
    Drone drone;
    HUD hud;

    for (int i = 0; i < COUNT; i++) {
        Enemy* e;
        std::string buffer;
        if (i < 10) { e = new Vehicle(); buffer = "Vehicle"; }
        else if (i < 30) { e = new Soldat(); buffer = "Soldat"; }
        else if (i < 36) { e = new Tank(0, 0); buffer = "Tank"; }
        else if (i < 38) { e = new AmmoStorage(); buffer = "AmmoStorage"; }
        else if (i < 39) { e = new Radar(); buffer = "Radar"; }
        else if (i < 43) { e = new FuelTruck(); buffer = "FuelTruck"; }
        else if (i < 44) { e = new CommTower(50.0f); buffer = "CommTower"; }
        else if (i < 47) { e = new MLRS(45.0f); buffer = "MLRS"; }
        else if (i < 49) { e = new SAM(); buffer = "SAM"; }
        else { e = new ApartmentBuilding((int)(rand() % 21 + 5)); buffer = "Apartament"; }

        float x = (float)(rand() % 300 - 50);
        float y = 0.0f;
        float z = -(float)(rand() % 250);
        e->angle = (float)(rand() % 360);
        e->position = { x, y, z };
        enemyes[i] = {e,{ x,y,z },buffer};
    }

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    std::vector<int> toDelete;
    std::vector<ExplosionEffect*> explosions;
    std::vector<SmokeEffect*> smokes;

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime(); 
        deltaTime = currentTime - lastTime;  
        lastTime = currentTime;

        drone.processInput(window,deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawSky();
        glLoadIdentity();

        glRotatef(-drone.roll, 0, 0, 1);
        glRotatef(-drone.pitch, 1, 0, 0);
        glRotatef(-drone.yaw, 0, 1, 0);
        glTranslatef(-drone.camX, -drone.camY, -drone.camZ);

        drawGround(drone.camX, drone.camZ);

        for (auto& en : enemyes) {
            float step = (en.second.type == "Apartament") ? 11.0f : 2.0f;

            if (en.second.type != "AmmoStorage" && en.second.type != "Apartament" && en.second.type != "Radar"
                && en.second.type != "CommTower") en.second.enemy->position[2] += 0.1f * deltaTime;
            en.second.enemy->UpdateVertices();
            en.second.enemy->Update(deltaTime);
            en.second.enemy->Draw();
            auto& p = en.second.enemy->position;
            if (!en.second.enemy->IsDestroyed()) {
                RenderTextWorld(p[0],p[1] + step,p[2],0, 1, 0, en.second.type.c_str()
                );

                RenderTextWorld(p[0],p[1] + step + 1.5f ,p[2],1, 0, 0,
                    std::format("{}/{}",
                        en.second.enemy->health,
                        en.second.enemy->maxHealth).c_str()
                );
            }
        }

        for (auto& en : enemyes) {
            if (drone.checkCollision(en.second.enemy)) {

                alSourceStop(explosionSource);
                alSourcePlay(explosionSource);
                float infos[6] = { 400.0f,6.0f,2.0f,1.8f,300,3.0f};
                if (en.second.type == "AmmoStorage") {
                    infos[0] = 700.0f;
                    infos[1] = 9.0f;
                    infos[2] = 5.0f;
                    infos[3] = 7.0f;
                    infos[4] = 850.0f;
                    infos[5] = 8.0f;
                }
                else if (en.second.type == "Apartaments") {
                    infos[0] = 600.0f;
                    infos[1] = 9.0f;
                    infos[2] = 2.0f;
                    infos[3] = 3.0f;
                    infos[4] = 750.0f;
                    infos[5] = 10.0f;
                }
                else if (en.second.type == "Soldat") {
                    infos[0] = 100.0f;
                    infos[1] = 3.0f;
                    infos[2] = 1.0f;
                    infos[3] = 1.0f; 
                    infos[5] = 1.0f;
                }
                else if (en.second.type == "FuelTruck") {
                    infos[0] = 600.0f;
                    infos[1] = 9.0f;
                    infos[2] = 3.0f;
                    infos[3] = 7.0f;
                    infos[4] = 550.0f;
                    infos[5] = 5.0f;
                }
               
                explosions.push_back(new ExplosionEffect(en.second.positions[0], en.second.positions[1], en.second.positions[2],
                    (int)infos[0],infos[1],infos[2],infos[3]));
                smokes.push_back(new SmokeEffect(en.second.positions[0], en.second.positions[1], en.second.positions[2],
                    (int)infos[4],infos[5]));
                
                en.second.enemy->health -= drone.damage;
                if (en.second.enemy->health <= 0) {
                    en.second.enemy->SetDestroyed(true);
                    toDelete.push_back(en.first);
                    ++drone.kills;
                }
                drone.respawn();
            }
        }
        for (auto it = explosions.begin(); it != explosions.end();) {
            (*it)->Update(deltaTime);
            (*it)->Draw();
            if ((*it)->IsFinished()) {
                delete* it;
                it = explosions.erase(it);
            }
            else {
                ++it;
            }
        }

        for (auto smoke : smokes) {
            smoke->Update(deltaTime);
            smoke->Draw();
        }

        for (int id : toDelete) {
            delete enemyes[id].enemy;
            enemyes.erase(id);
        }

        hud.drawHud(1600, 1000, drone,COUNT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    alSourceStop(droneSource);
    alSourceStop(explosionSource);

    alSourcei(droneSource, AL_BUFFER, 0);
    alSourcei(explosionSource, AL_BUFFER, 0);

    alDeleteSources(1, &droneSource);
    alDeleteSources(1, &explosionSource);

    alDeleteBuffers(1, &droneBuffer);
    alDeleteBuffers(1, &explosionBuffer);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
