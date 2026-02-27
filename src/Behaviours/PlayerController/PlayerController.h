#pragma once
#include <Crow.h>

namespace Dungi {
using namespace Crow;

class PlayerController : public Behaviour {
  using Behaviour::Behaviour;

  // #region Data
public:
  float movementSpeed = 1.0f;
  // #endregion

  // #region Methods
public:
  void Update() override;
  // #endregion
};
} // namespace Dungi
