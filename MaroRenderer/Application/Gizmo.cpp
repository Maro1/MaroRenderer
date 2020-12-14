#include "Gizmo.h"

void Gizmo::UpdateObject(SceneNode* node, Camera* camera, const glm::vec2& mouseCoords)
{
    glm::vec2 normalized = normalizedCoords(camera, mouseCoords);

    glm::vec4 rayStart, rayEnd;
    glm::mat4 viewProjInv = glm::inverse(camera->GetView() * camera->GetProjection());

    rayStart = viewProjInv * glm::vec4(normalized.x, normalized.y, 0.0f, 1.0f);
    rayStart *= 1.0f / rayStart.w;

    rayEnd = viewProjInv * glm::vec4(normalized.x, normalized.y, 1.0f, 1.0f);
    rayEnd *= 1.0f / rayEnd.w;
}

glm::vec2 Gizmo::normalizedCoords(Camera* camera, const glm::vec2& mouseCoords)
{
    glm::vec2 size = camera->GetViewPortSize();

    int x = -1.0 + 2.0 * mouseCoords.x / size.x;
    int y = 1.0 - 2.0 * mouseCoords.y / size.y;

    return glm::vec2(x, y);
}
