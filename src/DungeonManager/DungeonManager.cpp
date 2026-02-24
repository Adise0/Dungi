#include "./DungeonManager.h"
#include "../Scenes/Dungeon/Dungeon.h"
#include <fstream>

namespace Dungi {

std::vector<std::vector<std::string>> DungeonManager::loadedMap;

void DungeonManager::CreateNewDungeon(std::string dungeonName) {
  // #region CreateNewDungeon
  DungeonMap dungeonMap;
  srand(time(NULL));

  for (size_t x = 0; x <= SIZE; x++) {
    std::vector<std::string> col;
    for (size_t y = 0; y <= SIZE; y++) {
      if (y == 0 || x == 0 || y == SIZE - 1 || x == SIZE - 1) {
        col.push_back(WALL);
        continue;
      }
      col.push_back(".");
    }
    dungeonMap.push_back(col);
  }



  size_t slots = (SIZE - 2) / PREFAB_SIZE;

  for (size_t x = 0; x < slots; ++x) {
    for (size_t y = 0; y < slots; ++y) {

      int prefab = rand() % N_OF_PREFABS;
      std::string path = "./assets/dgParts/" + std::to_string(prefab + 1) + ".dg";

      std::ifstream file(path);
      if (!file) {
        printf("Could not open: %s\n", path.c_str());
        continue;
      }

      std::vector<std::vector<char>> grid;
      std::string line;
      while (std::getline(file, line)) {
        grid.emplace_back(line.begin(), line.end());
      }

      if (grid.size() < PREFAB_SIZE) continue;
      bool ok = true;
      for (size_t r = 0; r < PREFAB_SIZE; ++r)
        if (grid[r].size() < PREFAB_SIZE) ok = false;
      if (!ok) continue;

      for (size_t px = 0; px < PREFAB_SIZE; ++px) {
        for (size_t py = 0; py < PREFAB_SIZE; ++py) {
          size_t dx = 1 + PREFAB_SIZE * x + px;
          size_t dy = 1 + PREFAB_SIZE * y + py;

          // optional safety guard while debugging
          if (dx >= SIZE - 1 || dy >= SIZE - 1) continue;

          dungeonMap[dx][dy] = grid[py][px];
        }
      }
    }
  }
  printf("Done\n");
  loadedMap = dungeonMap;
  std::string map = "";

  for (size_t x = 0; x < SIZE; x++) {
    for (size_t y = 0; y < SIZE; y++) {
      map += dungeonMap[y][x];
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


void DungeonManager::LoadDungeon(std::string name) {}

void DungeonManager::SpawnDungeon() {

  Dungeon *dungeon = new Dungeon();
  SceneManager::SetSceneAsActive(*dungeon);

  for (size_t x = 0; x <= SIZE; x++) {
    for (size_t y = 0; y <= SIZE; y++) {
      char tile = loadedMap[y][x][0];

      switch (tile) {
      case '#':
        CreateWall(Vector2(x, y));
        break;

      default:
        break;
      }
    }
  }
}

GameObject &DungeonManager::CreateWall(Vector2 pos) {

  std::string name = "Wall" + pos.ToString();
  GameObject &wall = SceneManager::GetActiveScene()->GetRoot().CreateChild(name);
  wall.transform.position = pos;
  // wall.AddComponent<Renderer>("sprites/Wall.png");

  return wall;
};
// GameObject &DungeonManager::CreateChest(Vector2 pos) {};
// GameObject &DungeonManager::CreateEnemy(Vector2 pos) {};
// GameObject &DungeonManager::CreatePlayer(Vector2 pos) {};


} // namespace Dungi
