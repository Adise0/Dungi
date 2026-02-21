#include <Crow.h>

namespace Dungi {
using namespace Crow;

class MainMenu : public Scene {

public:
  inline MainMenu() : Scene("Main Menu") {}

public:
  void Load() override;
};
} // namespace Dungi
