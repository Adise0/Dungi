#pragma once

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
class DungeonManager {
  using DungeonMap = std::vector<std::vector<std::string>>;


public:
  static void CreateNewDungeon(std::string name);
  static void LoadDungeon(std::string name);

private:
  static void SaveToFile(std::string name, std::string map);
};
} // namespace Dungi
