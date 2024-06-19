#include "pch.h"
#include "SoundBytes.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>

BAKKESMOD_PLUGIN(SoundBytes, "Sound Bytes", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
bool soundEnabled = false;
ma_engine soundEngine;
int soundEngineVolume = 100;

char carHitPath[256] = "";
int carHitSpeed = 75;

void SoundBytes::onLoad()
{
    _globalCvarManager = cvarManager;

    // Initialize MiniAudio
    if (ma_engine_init(NULL, &soundEngine) != MA_SUCCESS) {
        return;
    }

    // Registering cvars
    // Volume of the sound
    cvarManager->registerCvar("soundbytes_engine_volume", "100", "Volume of the sound")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        soundEngineVolume = cvar.getIntValue();
        });

    // Enable the SoundBytes plugin
    cvarManager->registerCvar("soundbytes_enabled", "0", "Enable the SoundBytes plugin")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        soundEnabled = cvar.getBoolValue();
        });

    // Car hit sound path
    cvarManager->registerCvar("soundbytes_car_hit_path", "example/path/sound.wav", "Path of sound to play when the ball touches a car")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        std::string newPath = cvar.getStringValue();
        // Remove quotes from the path if present
        newPath.erase(std::remove(newPath.begin(), newPath.end(), '\"'), newPath.end());
        strcpy_s(carHitPath, newPath.c_str());
        });
    

    // Car hit speed threshold
    cvarManager->registerCvar("soundbytes_car_hit_speed", "75", "Minimum speed of ball when hit to play sound")
        .addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        carHitSpeed = cvar.getIntValue();
        });

    // Hooking the car hit event
    gameWrapper->HookEvent("Function TAGame.Ball_TA.OnCarTouch",
        [this](std::string eventName) {
            if (!soundEnabled) { return; }
            gameWrapper->SetTimeout([this](GameWrapper* gameWrapper) {
                carHit();
            }, 0.01f);  // Delay of 0.01 seconds (10 milliseconds)
        });
}

void SoundBytes::carHit() {
    ServerWrapper server = gameWrapper->GetCurrentGameState();
    if (!server) { return; }
    BallWrapper ball = server.GetBall();
    if (!ball) { return; }

    ma_engine_set_volume(&soundEngine, float(soundEngineVolume / 500.0f));

    Vector ballVelocity = ball.GetVelocity();
    float ballSpeed = ballVelocity.magnitude();
    float ballSpeedMPH = float(ballSpeed / 44.77);

    // Play the sound if the ball speed is greater than a threshold
    if (ballSpeedMPH > carHitSpeed) {  // Example threshold, adjust as needed
        ma_result result = ma_engine_play_sound(&soundEngine, &carHitPath[0], NULL);
    }
}

void SoundBytes::onUnload() {
	ma_engine_uninit(&soundEngine);
}