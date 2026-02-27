#include "./PlayerController.h"

namespace Dungi {

void PlayerController::Update() {

  Vector2 dir;
  if (InputSystem::GetKey("W").isPressed) dir += Vector2::Up;
  if (InputSystem::GetKey("A").isPressed) dir += Vector2::Left;
  if (InputSystem::GetKey("S").isPressed) dir += Vector2::Down;
  if (InputSystem::GetKey("D").isPressed) dir += Vector2::Right;

  if (dir == Vector2::Zero) return;

  transform.position += dir * movementSpeed * Time::deltaTime;
  transform.zIndex = -transform.GetPosition().y + 0.25f;
}
} // namespace Dungi
