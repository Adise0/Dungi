#include "./DungeonManager.h"
#include "../Scenes/Dungeon/Dungeon.h"
#include <GameObject.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <cstdio>
#include <fstream>
#include <string>

namespace Dungi {

using namespace Rendering;

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
      if (x == SIZE * 0.5 && y == SIZE * 0.5) {
        col.push_back(WALL);
        continue;
      }
      if (x == SIZE * 0.5 && y == SIZE * 0.5 + 1) {
        col.push_back(WALL);
        continue;
      }
      col.push_back(".");
    }
    dungeonMap.push_back(col);
  }



  // size_t slots = (SIZE - 2) / PREFAB_SIZE;

  // for (size_t x = 0; x < slots; ++x) {
  //   for (size_t y = 0; y < slots; ++y) {

  //     int prefab = rand() % N_OF_PREFABS;
  //     std::string path = "./assets/dgParts/" + std::to_string(prefab + 1) + ".dg";

  //     std::ifstream file(path);
  //     if (!file) {
  //       printf("Could not open: %s\n", path.c_str());
  //       continue;
  //     }

  //     std::vector<std::vector<char>> grid;
  //     std::string line;
  //     while (std::getline(file, line)) {
  //       grid.emplace_back(line.begin(), line.end());
  //     }

  //     if (grid.size() < PREFAB_SIZE) continue;
  //     bool ok = true;
  //     for (size_t r = 0; r < PREFAB_SIZE; ++r)
  //       if (grid[r].size() < PREFAB_SIZE) ok = false;
  //     if (!ok) continue;

  //     for (size_t px = 0; px < PREFAB_SIZE; ++px) {
  //       for (size_t py = 0; py < PREFAB_SIZE; ++py) {
  //         size_t dx = 1 + PREFAB_SIZE * x + px;
  //         size_t dy = 1 + PREFAB_SIZE * y + py;

  //         // optional safety guard while debugging
  //         if (dx >= SIZE - 1 || dy >= SIZE - 1) continue;

  //         dungeonMap[dx][dy] = grid[py][px];
  //       }
  //     }
  //   }
  // }
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
  // #region SpawnDungeon
  Dungeon *dungeon = new Dungeon();
  SceneManager::SetSceneAsActive(*dungeon);


  for (size_t x = 0; x <= SIZE; x++) {
    for (size_t y = 0; y <= SIZE; y++) {
      char tile = loadedMap[x][y][0];
      Vector2 pos(x, y);
      int variant = 0;

      switch (tile) {
      case '#':
        CreateWall(Vector2(x - (SIZE * 0.5f), y - (SIZE * 0.5f)),
                   GetConnectedNeighbours(pos, loadedMap));
        break;

      default:
        break;
      }
    }
  }
  // #endregion
}


std::vector<bool> DungeonManager::GetConnectedNeighbours(Vector2 pos, DungeonMap map) {
  // #region GetConnectedNeighbours
  const std::vector<Vector2> neighbours{Vector2::Up, Vector2::Down, Vector2::Left, Vector2::Right};
  std::vector<bool> neighbouringWalls;
  for (Vector2 neighbour : neighbours) {
    Vector2 neighbourPos = pos + neighbour;
    if (neighbourPos.x < 0 || neighbourPos.x >= SIZE || neighbourPos.y <= 0 ||
        neighbourPos.y > SIZE) {
      neighbouringWalls.push_back(false);
      continue;
    }
    neighbouringWalls.push_back((map[neighbourPos.x][neighbourPos.y] == WALL));
  }

  return neighbouringWalls;
  // #endregion
}

GameObject &DungeonManager::CreateWall(Vector2 pos, std::vector<bool> neighbours) {
  // #region CreateWall
  std::string name = "Wall" + pos.ToString();
  GameObject &wall = SceneManager::GetActiveScene()->GetRoot().CreateChild(name);
  wall.transform.position = pos;

  SDL_Texture *wallsprite = WindowManager::LoadSprite("sprites/map/walls_floor.png");
  SDL_Texture *wallsprite2 =
      WindowManager::LoadSprite("sprites/map/decorative_cracks_coasts_animation.png");

  GameObject &center = wall.CreateChild(name + "Center");
  Renderer &centerRenderer = center.AddComponent<Renderer>(SDL_Color{255, 255, 255, 255});
  centerRenderer.size = Vector2(0.1, 0.1);

  const int height = 40;
  const int extendedHeight = 80;

  GameObject &top = wall.CreateChild(name + "-Top");
  top.transform.position += Vector2::Up * 0.25f;
  Renderer &topRenderer = top.AddComponent<Renderer>(wallsprite);
  top.transform.zIndex = 10;


  SDL_FRect topSRect = {36, 203, 44 - 36, height};
  if (neighbours[0]) topSRect = {36, 206, 44 - 36, height};
  topRenderer.SetSRect(topSRect);
  topRenderer.size = Vector2(0.1f, 0.5f);



  GameObject &down = wall.CreateChild(name + "-Down");
  Renderer &downRenderer = down.AddComponent<Renderer>(wallsprite);

  printf("Tile: %s, %s\n", pos.ToString().c_str(), std::to_string(neighbours[1]).c_str());

  Vector2 downSize(0.1f, 1.0f);
  SDL_FRect downSRect = {36, 245, 44 - 36, extendedHeight};
  if (neighbours[1]) {
    downSRect = {36, 245, 44 - 36, height};
    downSize = Vector2(0.1f, 0.5f);
    down.transform.position += Vector2::Down * 0.25f;
  } else {
    down.transform.position += Vector2::Down * 0.5f;
  }
  downRenderer.SetSRect(downSRect);
  downRenderer.size = downSize;

  return wall;

  // GameObject &DungeonManager::CreateChest(Vector2 pos) {};
  // GameObject &DungeonManager::CreateEnemy(Vector2 pos) {};
  // GameObject &DungeonManager::CreatePlayer(Vector2 pos) {};
}

} // namespace Dungi
