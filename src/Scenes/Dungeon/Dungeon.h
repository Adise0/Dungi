#include <Crow.h>


namespace Dungi {
using namespace Crow;

class Dungeon : public Scene {


public:
  Dungeon() : Scene("Dungeon") {}

private:
  void Load() override;
};
} // namespace Dungi
