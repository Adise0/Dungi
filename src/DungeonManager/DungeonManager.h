#pragma once

#include <string>
#include <vector>

#define SIZE 20
#define WALL "#"
#define PLAYER "P"
#define CHEST "C"
#define ENEMY "E"

namespace Dungi {
class DungeonManager {
  using DungeonMap = std::vector<std::vector<std::string>>;


public:
  static void CreateNewDungeon(std::string dugeonName);

private:
  static void SaveToFile(std::string name, std::string map);
};
} // namespace Dungi
