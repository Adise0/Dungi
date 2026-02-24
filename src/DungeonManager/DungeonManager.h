#pragma once

#include <Crow.h>
#include <string>
#include <vector>

#define SIZE 22
#define WALL "#"
#define PLAYER "P"
#define CHEST "C"
#define ENEMY "E"
#define N_OF_PREFABS 4
#define PREFAB_SIZE 5


namespace Dungi {

using namespace Crow;

class DungeonManager {



  using DungeonMap = std::vector<std::vector<std::string>>;


private:
  static std::vector<std::vector<std::string>> loadedMap;



public:
  static void CreateNewDungeon(std::string name);
  static void LoadDungeon(std::string name);
  static void SpawnDungeon();

private:
  static void SaveToFile(std::string name, std::string map);

  static GameObject &CreateWall(Vector2 pos);
  // GameObject &CreateChest(Vector2 pos);
  // GameObject &CreateEnemy(Vector2 pos);
  // GameObject &CreatePlayer(Vector2 pos);
};
} // namespace Dungi
