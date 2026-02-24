#include "./MainMenuController.h"

namespace Dungi {

void MainMenuController::Awake() {
  ui = gameObject.GetComponent<UIRenderer>();

  ui->bridge->On("Play", [this](std::string type, std::string message) {
    CreateNewDungeon(message);
    Play(message);
  });
}

void MainMenuController::CreateNewDungeon(std::string dungeonName) {
  // #region CreateNewDungeon

  // #endregion
}

void MainMenuController::Play(std::string dungeonName) {}



} // namespace Dungi
