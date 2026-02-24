#pragma once
#include <Crow.h>
#include <string>

namespace Dungi {
using namespace Crow;

class MainMenuController : public Behaviour {

  using Behaviour::Behaviour;

  // #region Data
private:
  UIRenderer *ui;
  // #endregion

  // #region Methods
public:
  void Awake() override;
  // void Update() override;

private:
  void Play(std::string dugeonName);
  // #endregion
};
} // namespace Dungi
