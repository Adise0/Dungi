#include "./Dungeon.h"

namespace Dungi {

void Dungeon::Load() {
  GameObject &cameraGO = GetRoot().CreateChild("Camera");
  Camera &cam = cameraGO.AddComponent<Camera>();
  cam.SetAsActiveCamera();
}
} // namespace Dungi
