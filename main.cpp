#include "game.h"

int main()
{
	Game game;
	if (game.Construct(var::width, var::height, var::pixel_size, var::pixel_size))
		game.Start();

	return 0;
}
