#include <iostream>
#include "model/Playlist.h"
#include "view/Canvas.h"


int main()
{
	Playlist playlist("test");
	Canvas canvas(playlist, "test");
	canvas.start();
}
