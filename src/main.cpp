#include "Scenes/MainMenu/MainMenu.h"
#include <Crow.h>

int main() {
  Dungi::MainMenu *mainMenu = new Dungi::MainMenu();
  Crow::Run(mainMenu);
}
