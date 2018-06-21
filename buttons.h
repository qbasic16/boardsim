#ifndef __DE2_BUTTONS__
#define __DE2_BUTTONS__

#include <SDL2/SDL.h>

#define lin_scale(start, stop, count, idx) (((stop)-(start))*(idx)/((count)-1)+(start))

#define UP   (1)
#define DOWN (0)

struct board_button {
	SDL_Keycode sym;
	int vecidx;
	int x;
	int y;
	int state;
};

PLI_INT32 DE2_buttons_calltf(PLI_BYTE8 *user_data);

PLI_INT32 DE2_buttons_sizetf(PLI_BYTE8 *user_data);

void DE2_buttons_register();

#endif
