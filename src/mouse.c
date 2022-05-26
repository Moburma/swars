#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"
#include "bfscreen.h"
#include "bfplanar.h"
#include "bfsprite.h"
#include "bfmouse.h"
#include "display.h"

#pragma pack(1)

struct PointerHotspot {
    sbyte x;
    sbyte y;
    sbyte z;
};

struct PointerHotspot pointer_hotspot[] = {
  {-116, 0, 0,},
  {  0,  0, 0,},
  {  0,  0, 0,},
  { -7, -7, 0,},
  { -7, -7, 0,},
  { -5, -6, 0,},
  { -2, -2, 0,},
  { -7,-13, 0,},
};

#pragma pack()

extern struct TbSprite *pointer_sprites;

void mouse_initialise(void)
{
    SDL_ShowCursor(SDL_DISABLE);
}

void do_change_mouse(ushort mouse)
{
  LbMouseChangeSprite(&pointer_sprites[mouse]);
  LbMouseChangeSpriteOffset(pointer_hotspot[mouse].x, pointer_hotspot[mouse].y);
}
