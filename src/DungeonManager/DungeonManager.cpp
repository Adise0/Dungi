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
      // if (x == SIZE * 0.5 && y == SIZE * 0.5) {
      //   col.push_back(WALL);
      //   continue;
      // }
      // if (x == SIZE * 0.5 + 1 && y == SIZE * 0.5 + 1) {
      //   col.push_back(WALL);
      //   continue;
      // }
      // if (x == SIZE * 0.5 + 1 && y == SIZE * 0.5) {
      //   col.push_back(WALL);
      //   continue;
      // }
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

  std::string map;
  for (size_t y = 0; y < SIZE; ++y) {
    for (size_t x = 0; x < SIZE; ++x) {
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


void DungeonManager::LoadDungeon(std::string name) {}

void DungeonManager::SpawnDungeon() {
  // #region SpawnDungeon
  Dungeon *dungeon = new Dungeon();
  SceneManager::SetSceneAsActive(*dungeon);
  Camera::activeCamera->SetSize(60);



  for (size_t x = 0; x < SIZE; x++) {
    for (size_t y = 0; y < SIZE; y++) {
      char tile = loadedMap[x][y][0];
      Vector2 pos(x, y);
      float wx = (float)x - (SIZE * 0.5f);
      float wy = (float)(SIZE - 1 - y) - (SIZE * 0.5f);



      switch (tile) {
      case '#':
        CreateWall(Vector2(wx, wy), GetConnectedNeighbours(pos, loadedMap));
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


  Vector2 centerSize = Vector2(1, 1);
  Vector2 origin = Vector2(3, 1);
  Vector2 pixelSize = Vector2(42, 77);

  GameObject &center = wall.CreateChild(name + "Center");
  center.transform.zIndex = pos.y;

  Renderer &centerRenderer = center.AddComponent<Renderer>(wallsprite);


  bool hasNeighbours =
      std::any_of(neighbours.begin(), neighbours.end(), [](bool neighbour) { return neighbour; });
  float ppu = WindowManager::resolutionX / Camera::activeCamera->size;

  const Vector2 tileSize(48, 40);

  // if (hasNeighbours) {
  //   if (neighbours[0]) {
  //     origin += Vector2(0, 6);
  //     pixelSize -= Vector2(0, 6);
  //   }
  //   if (neighbours[1]) {
  //     float heightToRemove = 45;
  //     if (neighbours[0]) heightToRemove += 8;

  //     pixelSize += Vector2(0, -heightToRemove);
  //   }
  //   if (neighbours[2]) {
  //     origin += Vector2(10, 0);
  //     pixelSize -= Vector2(11, 0);
  //   }

  //   if (neighbours[3]) {
  //     pixelSize -= Vector2(15, 0);
  //   }
  //   if (!neighbours[1]) {
  //     centerSize += Vector2(0, 48.0f / ppu);
  //     center.transform.position -= Vector2(0, (48.0f / ppu) * 0.5f);
  //   }
  // }



  centerRenderer.size = centerSize;
  SDL_FRect srect{origin.x, origin.y, pixelSize.x, pixelSize.y};
  centerRenderer.SetSRect(srect);

  return wall;

  // GameObject &DungeonManager::CreateChest(Vector2 pos) {};
  // GameObject &DungeonManager::CreateEnemy(Vector2 pos) {};
  // GameObject &DungeonManager::CreatePlayer(Vector2 pos) {};
}

} // namespace Dungi
