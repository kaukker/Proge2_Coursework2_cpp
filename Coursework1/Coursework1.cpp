#include "Main.h"
#include "Control.h"
#include "Consumer.h"
#include "Application.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetConsoleOutputCP(1257);

	Application app;

	app.Run();

	return 0;
}
