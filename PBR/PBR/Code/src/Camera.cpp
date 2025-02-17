#include "Camera.h"

#include <algorithm>

glm::mat4 Camera::GetViewMatrix() const
{
    return lookAt(Position, Position + Front, Up);
}

void Camera::SetPitch(const float value)
{
    Pitch = value;
    UpdateCameraVectors();
}

void Camera::SetYaw(const float value)
{
    Yaw = value;
    UpdateCameraVectors();
}

float Camera::GetPitch() const
{
    return Pitch;
}

float Camera::GetYaw() const
{
    return Yaw;
}

void Camera::ProcessKeyboard(const Camera_Movement direction, const float deltaTime)
{
    const float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;

    if (direction == BACKWARD)
        Position -= Front * velocity;

    if (direction == LEFT)
        Position -= Right * velocity;

    if (direction == RIGHT)
        Position += Right * velocity;

    if (direction == UP)
        Position += WorldUp * velocity;

    if (direction == DOWN)
        Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, const GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        Pitch = std::min(Pitch, 89.0f);
        Pitch = std::max(Pitch, -89.0f);
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(const float yoffset)
{
    Zoom -= yoffset;
    Zoom = std::max(Zoom, 1.0f);
    Zoom = std::min(Zoom, 45.0f);
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = normalize(front);
    // also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = normalize(cross(Right, Front));
}
