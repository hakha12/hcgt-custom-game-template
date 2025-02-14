#include "../include/engine.h"

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

hcgt::Engine engine;

static void MainLoop();

int main(int argc, char** argv){
	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(MainLoop, 0, 1);
	#else
		SetTargetFPS(60);

		while (!engine.IsDone()){
			MainLoop();
		}
	#endif
}

static void MainLoop(){
	engine.HandleEvent();
	engine.Update();
	engine.Draw();
	engine.LateUpdate();
}