#include "core/CameraManager.hpp"
#include "core/Input.hpp"
#include <raymath.h>

#include <cstdio>

CameraManager::CameraManager() : 
    isFirstPerson(true), yaw(0.0f), pitch(0.0f), distanceToPlayer(10.0f) {
    camera.position = { 0.0f, 0.0f, 0.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    DisableCursor();
}

void CameraManager::Update(Vector3 playerPosition, float deltaTime) { 
    if (Input::IsCameraTogglePressed()) isFirstPerson = !isFirstPerson;

    Vector2 mouseDelta = GetMouseDelta();
    float sensibility = 5.0f;
    yaw -= mouseDelta.x * 0.003f * sensibility;
    pitch -= mouseDelta.y * 0.003f * sensibility;

    pitch = Clamp(pitch, -PI / 2.2f, PI / 2.2f);
    yaw = fmodf(yaw, 2.0f * PI);
    if (yaw < 0.0f) yaw += 2.0f * PI;

    char buf[64];
    sprintf(buf, "Yaw: %.2f Pitch: %.2f", yaw, pitch);
    DrawText(buf, 10, 250, 20, WHITE);

    const char* direction = "Desconhecido";
    float deg = yaw * RAD2DEG;

    if (deg >= 337.5f || deg < 22.5f)         direction = "Norte";
    else if (deg >= 22.5f  && deg < 67.5f)    direction = "Noroeste";
    else if (deg >= 67.5f  && deg < 112.5f)   direction = "Oeste";
    else if (deg >= 112.5f && deg < 157.5f)   direction = "Sudoeste";
    else if (deg >= 157.5f && deg < 202.5f)   direction = "Sul";
    else if (deg >= 202.5f && deg < 247.5f)   direction = "Sudeste";
    else if (deg >= 247.5f && deg < 292.5f)   direction = "Leste";
    else if (deg >= 292.5f && deg < 337.5f)   direction = "Nordeste";

    DrawText(TextFormat("Direção: %s", direction), 10, 280, 20, WHITE);

    if (isFirstPerson) {
        SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
        UpdateFPS(playerPosition);
    }
    else UpdateTPS(playerPosition);
}

void CameraManager::UpdateFPS(Vector3 playerPosition) {
    Vector3 direction = { 
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * cosf(yaw) 
    };

    camera.position = { playerPosition.x, playerPosition.y + 1.8f, playerPosition.z };
    camera.target = Vector3Add(camera.position, direction);
    camera.up = { 0.0f, 1.0f, 0.0f };
}

void CameraManager::UpdateTPS(Vector3 playerPosition) {
    Vector3 offset = { 
        distanceToPlayer * cosf(pitch) * sinf(yaw),
        distanceToPlayer * sinf(pitch),
        distanceToPlayer * cosf(pitch) * cosf(yaw)
    };

    camera.position = Vector3Add(playerPosition, offset);
    camera.target = playerPosition;
    camera.up = { 0.0f, 1.0f, 0.0f };
}

Camera3D CameraManager::GetCamera3D() const { return camera; }