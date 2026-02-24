#include "./MainMenuController.h"
#include "../../DungeonManager/DungeonManager.h"

namespace Dungi {

void MainMenuController::Awake() {
  ui = gameObject.GetComponent<UIRenderer>();

  ui->bridge->On("Play", [this](std::string type, std::string message) {
    DungeonManager::CreateNewDungeon(message);
    Play(message);
  });
}



void MainMenuController::Play(std::string dungeonName) {}



} // namespace Dungi
