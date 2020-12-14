#pragma once

#include "Engine/Objects/SceneNode.h"
#include "Engine/Camera/Camera.h"

enum class GizmoMode
{
    None,
    Translate,
    Rotate,
    Scale
};

class Gizmo
{
public:

	void Draw();
    void UpdateObject(SceneNode* node, Camera* camera, const glm::vec2& mouseCoords);
    bool IsActive();

private:

    glm::vec2 normalizedCoords(Camera* camera, const glm::vec2& mouseCoords);
    GizmoMode m_Mode = GizmoMode::None;
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, 0.0f);


};