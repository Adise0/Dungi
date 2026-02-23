#include "MainMenu.h"

namespace Dungi {
void MainMenu::Load() {
  // #region Load
  GameObject &cameraGO = GetRoot().CreateChild("Camera");
  Camera &cam = cameraGO.AddComponent<Camera>();
  cam.SetAsActiveCamera();
  cameraGO.AddComponent<UIRenderer>("ui/MainMenu/MainMenu.html");

  // #endregion
}
} // namespace Dungi
