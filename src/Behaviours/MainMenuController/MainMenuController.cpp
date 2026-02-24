#include "./MainMenuController.h"

#include "../../DungeonManager/DungeonManager.h"

namespace Dungi {

void MainMenuController::Awake() {
  ui = gameObject.GetComponent<UIRenderer>();

  ui->bridge->On("Play", [this](std::string type, std::string message) {
    DungeonManager::CreateNewDungeon(message);
    Play();
  });
  DungeonManager::CreateNewDungeon("Test");
  Play();
}



void MainMenuController::Play() {
  DungeonManager::SpawnDungeon();
  printf("New Scene loaded!\n");
}



} // namespace Dungi
