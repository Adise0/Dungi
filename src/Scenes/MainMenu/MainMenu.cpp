#include "MainMenu.h"
#include "../../Behaviours/MainMenuController/MainMenuController.h"
namespace Dungi {
void MainMenu::Load() {
  // #region Load
  GameObject &cameraGO = GetRoot().CreateChild("Camera");
  Camera &cam = cameraGO.AddComponent<Camera>();
  cam.SetAsActiveCamera();
  cameraGO.AddComponent<UIRenderer>("ui/MainMenu/MainMenu.html");
  cameraGO.AddComponent<MainMenuController>();

  // #endregion
}
} // namespace Dungi
