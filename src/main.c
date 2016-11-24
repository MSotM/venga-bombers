#include "game.h"

int main() {
  init_world();
  init_render();

  while (true) {
    // Handle input
    handle_events();
    update_world();
    render();
  }
}
