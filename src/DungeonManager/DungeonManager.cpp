#include "./DungeonManager.h"
#include "../Behaviours/PlayerController/PlayerController.h"
#include "../Scenes/Dungeon/Dungeon.h"
#include <GameObject.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <cstdio>
#include <fstream>
#include <string>

namespace Dungi {

#define NEIGHBOUR_TOP_LEFT 0
#define NEIGHBOUR_TOP_CENTER 1
#define NEIGHBOUR_TOP_RIGHT 2
#define NEIGHBOUR_CENTER_LEFT 3
#define NEIGHBOUR_CENTER_RIGHT 4
#define NEIGHBOUR_BOTTOM_LEFT 5
#define NEIGHBOUR_BOTTOM_CENTER 6
#define NEIGHBOUR_BOTTOM_RIGHT 7

#define MASK_NONE 0
#define MASK_TOP 1
#define MASK_RIGHT 2
#define MASK_TOP_RIGHT 3
#define MASK_BOTTOM 4
#define MASK_TOP_BOTTOM 5
#define MASK_BOTTOM_RIGHT 6
#define MASK_TOP_RIGHT_BOTTOM 7
#define MASK_LEFT 8
#define MASK_TOP_LEFT 9
#define MASK_RIGHT_LEFT 10
#define MASK_TOP_RIGHT_LEFT 11
#define MASK_BOTTOM_LEFT 12
#define MASK_TOP_BOTTOM_LEFT 13
#define MASK_RIGHT_BOTTOM_LEFT 14
#define MASK_ALL 15



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
  std::srand(time(NULL));
  int x = 1 + std::rand() % (SIZE - 2);
  int y = 2 + std::rand() % (SIZE - 3);

  while (dungeonMap[x][y] != "." || dungeonMap[x][y - 1] != "." || dungeonMap[x][y + 1] != ".") {
    x = 1 + std::rand() % (SIZE - 2);
    y = 2 + std::rand() % (SIZE - 3);
  }

  Vector2 playerPos(x, y);

  dungeonMap[playerPos.x][playerPos.y] = PLAYER;


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
  // Camera::activeCamera->SetSize(50);



  for (size_t x = 0; x < SIZE; x++) {
    for (size_t y = 0; y < SIZE; y++) {
      char tile = loadedMap[x][y][0];
      Vector2 pos(x, y);

      switch (tile) {
      case WALL[0]:
        CreateWall(ToWorldPos(x, y), GetConnectedNeighbours(pos, loadedMap));
        break;

      default:
        break;
      }
    }
  }

  SpawnPlayer();
  // #endregion
}


Vector2 DungeonManager::ToWorldPos(int mapX, int mapY) {
  // #region ToWorldPos
  int worldX = mapX - (SIZE * 0.5f);
  int worldY = (SIZE - 1 - mapY) - (SIZE * 0.5f);
  return Vector2(worldX, worldY);
  // #endregion
}

void DungeonManager::SpawnPlayer() {
  // #region SpawnPlayer
  Vector2 foundSpawnPos;
  for (size_t x = 0; x < SIZE; x++) {
    for (size_t y = 0; y < SIZE; y++) {
      if (loadedMap[x][y] != PLAYER) continue;
      foundSpawnPos = ToWorldPos(x, y);
    }
  }
  if (foundSpawnPos == Vector2::Zero) {
    printf("Player position not found!\n");
    return;
  }

  GameObject &playerObject = SceneManager::GetActiveScene()->GetRoot().CreateChild("Player");
  playerObject.transform.SetPosition(foundSpawnPos);
  Camera::activeCamera->gameObject.transform.SetParent(&playerObject.transform);
  Camera::activeCamera->gameObject.transform.SetLocalPosition(Vector2::Zero);

  Renderer &playerRenderer = playerObject.AddComponent<Renderer>(SDL_Color{255, 255, 255, 255});
  playerRenderer.size = Vector2(0.5, 0.5);

  playerObject.AddComponent<PlayerController>();
  playerObject.AddComponent<BoxCollider>(Vector2(0.5, 0.5));
  playerObject.AddComponent<RigidBody>();

  // #endregion
}


std::vector<bool> DungeonManager::GetConnectedNeighbours(Vector2 pos, DungeonMap map) {
  // #region GetConnectedNeighbours
  const Vector2 neighbours[8] = {Vector2(-1, -1), Vector2(0, -1), Vector2(1, -1), Vector2(-1, 0),
                                 Vector2(1, 0),   Vector2(-1, 1), Vector2(0, 1),  Vector2(1, 1)};
  std::vector<bool> neighbouringWalls;
  for (Vector2 neighbour : neighbours) {
    Vector2 neighbourPos = pos + neighbour;
    if (neighbourPos.x <= 0 || neighbourPos.x >= SIZE || neighbourPos.y <= 0 ||
        neighbourPos.y >= SIZE) {
      neighbouringWalls.push_back(true);
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
  GameObject &colHolder = wall.CreateChild(name + "Collider");
  colHolder.transform.position += Vector2(0, -0.3f);
  colHolder.AddComponent<BoxCollider>(Vector2(1, 1));

  wall.transform.position = pos;

  SDL_Texture *wallsprite = WindowManager::LoadSprite("sprites/map/walls.png");


  Vector2 origin;
  Vector2 pixelSize(46, 40);

  GameObject &main = wall.CreateChild(name + "Main");
  main.transform.zIndex = -pos.y;

  Renderer &mainRenderer = main.AddComponent<Renderer>(wallsprite);


  bool hasNeighbours =
      std::any_of(neighbours.begin(), neighbours.end(), [](bool neighbour) { return neighbour; });
  Vector2 ppu = pixelSize;

  bool hasBottom = false;
  if (!hasNeighbours || !neighbours[NEIGHBOUR_BOTTOM_CENTER]) hasBottom = true;

  if (!hasNeighbours) {
    origin = Vector2(0, 164);
  } else {

    uint neighbourMask = 0;
    neighbourMask |= neighbours[NEIGHBOUR_TOP_CENTER] ? 1 << 0 : 0;
    neighbourMask |= neighbours[NEIGHBOUR_CENTER_RIGHT] ? 1 << 1 : 0;
    neighbourMask |= neighbours[NEIGHBOUR_BOTTOM_CENTER] ? 1 << 2 : 0;
    neighbourMask |= neighbours[NEIGHBOUR_CENTER_LEFT] ? 1 << 3 : 0;


    switch (neighbourMask) {
    case MASK_TOP:
      origin = Vector2(143, 82);
      break;
    case MASK_RIGHT:
      origin = Vector2(143, 41);
      break;
    case MASK_BOTTOM:
      origin = Vector2(143, 123);
      break;
    case MASK_LEFT:
      origin = Vector2(143, 0);
      break;
    case MASK_TOP_RIGHT:
      origin = Vector2(49, 123);
      break;
    case MASK_TOP_LEFT:
      origin = Vector2(49, 82);
      break;
    case MASK_BOTTOM_RIGHT:
      origin = Vector2(49, 41);
      break;
    case MASK_BOTTOM_LEFT:
      origin = Vector2(49, 0);
      break;
    case MASK_TOP_RIGHT_BOTTOM:
      origin = Vector2(96, 41);
      break;
    case MASK_TOP_BOTTOM_LEFT:
      origin = Vector2(96, 82);
      break;
    case MASK_TOP_RIGHT_LEFT:
      origin = Vector2(96, 123);
      break;
    case MASK_RIGHT_BOTTOM_LEFT:
      origin = Vector2(96, 0);
      break;
    case MASK_TOP_BOTTOM:
      origin = Vector2(143, 164);
      break;
    case MASK_RIGHT_LEFT:
      origin = Vector2(96, 164);
      break;
    case MASK_ALL:
      origin = Vector2(49, 164);
      break;

    case MASK_NONE:
      origin = Vector2(0, 164);
      break;
    }



    //     if (!neighbours[NEIGHBOUR_TOP_LEFT]) {
    //       GameObject &topLeft = wall.CreateChild(name + "topLeft");
    //       Renderer &topLeftRenderer = topLeft.AddComponent<Renderer>(wallsprite);
    //       SDL_FRect topLeftSRect{183, 41, pixelSize.x, pixelSize.y};
    //       topLeftRenderer.SetSRect(topLeftSRect);
    //     }
    // if (!neighbours[NEIGHBOUR_TOP_RIGHT] && neighbours[NEIGHBOUR_TOP_CENTER] &&
    //     neighbours[NEIGHBOUR_CENTER_RIGHT]) {
    //   GameObject &topRight = wall.CreateChild(name + "topRight");
    //   Renderer &topRightRenderer = topRight.AddComponent<Renderer>(wallsprite);
    //   SDL_FRect topRightSRect{190, 0, pixelSize.x, pixelSize.y};
    //   topRightRenderer.SetSRect(topRightSRect);
    // }
    //     if (!neighbours[NEIGHBOUR_BOTTOM_LEFT]) {
    //       GameObject &bottomLeft = wall.CreateChild(name + "bottomLeft");
    //       Renderer &bottomLeftRenderer = bottomLeft.AddComponent<Renderer>(wallsprite);
    //       SDL_FRect bottomLeftSRect{183, 123, pixelSize.x, pixelSize.y};
    //       bottomLeftRenderer.SetSRect(bottomLeftSRect);
    //     }
    //     if (!neighbours[NEIGHBOUR_BOTTOM_LEFT]) {
    //       GameObject &bottomRight = wall.CreateChild(name + "bottomRight");
    //       Renderer &bottomRightRenderer = bottomRight.AddComponent<Renderer>(wallsprite);
    //       SDL_FRect bottomRightSRect{183, 82, pixelSize.x, pixelSize.y};
    //       bottomRightRenderer.SetSRect(bottomRightSRect);
    //     }
  }


  SDL_FRect srect{origin.x, origin.y, pixelSize.x, pixelSize.y};
  mainRenderer.SetSRect(srect);



  if (hasBottom) {
    GameObject &bottom = wall.CreateChild(name + "Bottom");
    Renderer &bottomRenderer = bottom.AddComponent<Renderer>(wallsprite);
    float bottomYSize = 30.f / ppu.y;
    bottomRenderer.size = Vector2(1, bottomYSize);
    bottom.transform.position += Vector2(0, -(0.5f + bottomYSize * 0.5f));
    bottom.transform.zIndex = bottom.transform.parent->zIndex + 0.5f;

    Vector2 bottomOrigin(0, 0);
    Vector2 bototmPixelSize = pixelSize;


    if (neighbours[NEIGHBOUR_CENTER_LEFT] && !neighbours[NEIGHBOUR_CENTER_RIGHT]) {
      bottomOrigin = Vector2(0, 41);
      bototmPixelSize += Vector2(2, 0);
      bottomRenderer.size += Vector2(2, 0) / ppu;
      bottom.transform.position -= Vector2(1, 0) / ppu;
    }
    if (!neighbours[NEIGHBOUR_CENTER_LEFT] && neighbours[NEIGHBOUR_CENTER_RIGHT]) {
      bottomOrigin = Vector2(0, 82);
      bototmPixelSize += Vector2(2, 0);
      bottomRenderer.size += Vector2(2, 0) / ppu;
      bottom.transform.position += Vector2(1, 0) / ppu;
    }
    if (neighbours[NEIGHBOUR_CENTER_LEFT] && neighbours[NEIGHBOUR_CENTER_RIGHT]) {
      bottomOrigin = Vector2(0, 123);
      bototmPixelSize += Vector2(2, 0);
    }

    SDL_FRect bttomSRect{bottomOrigin.x, bottomOrigin.y, bototmPixelSize.x, bototmPixelSize.y};
    bottomRenderer.SetSRect(bttomSRect);
  }



  return wall;
  // #endregion

  // GameObject &DungeonManager::CreateChest(Vector2 pos) {};
  // GameObject &DungeonManager::CreateEnemy(Vector2 pos) {};
  // GameObject &DungeonManager::CreatePlayer(Vector2 pos) {};
}

} // namespace Dungi
