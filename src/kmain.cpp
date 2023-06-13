#include "graphics.hpp"

void kmain()
{
  graphics::initialize();
  graphics::print("Hello world!");
  graphics::printf("Hello %s %d", "world", 1);
}
