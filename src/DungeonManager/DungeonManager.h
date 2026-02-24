#pragma once

#include <string>

#define WALL "#"
#define PLAYER "P"
#define CHEST "C"
#define ENEMY "E"

namespace Dungi {
class DungeonManager {
public:
  void CreateNewDungeon(std::string dugeonName);
};
} // namespace Dungi
