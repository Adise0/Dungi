#include "./DungeonManager.h"
#include <fstream>


namespace Dungi {

void DungeonManager::CreateNewDungeon(std::string dungeonName) {
  // #region CreateNewDungeon
  DungeonMap dungeonMap;

  for (size_t x = 0; x < SIZE; x++) {
    std::vector<std::string> col;
    for (size_t y = 0; y < SIZE; y++) {
      if (y == 0 || x == 0 || y == SIZE - 1 || x == SIZE - 1) {
        col.push_back(WALL);
        continue;
      }
      col.push_back(".");
    }
    dungeonMap.push_back(col);
  }

  std::string map = "";

  for (size_t x = 0; x < SIZE; x++) {
    for (size_t y = 0; y < SIZE; y++) {
      map += dungeonMap[x][y];
    }
    map += "\n";
  }


  SaveToFile(dungeonName, map);
  // #endregion
}

void DungeonManager::SaveToFile(std::string name, std::string map) {
  // #region SaveToFile
  std::string path = "./Dungeons/" + name + ".dg";
  std::ofstream file(path);

  if (file.is_open()) {
    file.write(map.c_str(), map.size());
    file.close();
  } else {
    printf("Failed to write to file %s\n", path.c_str());
  }

  // #endregion
}
} // namespace Dungi
