// stdlib includes
#include <algorithm>
#include <iostream>
#include <string>

// sdl includes
#include <SDL.h>

// opengl includes
#include "glcorearb.h"

// local includes
#include "scopeguard.h"

// a few helpers
bool isdigit(char ch){
	return ch >= '0' && ch <= '9';
}

bool next_number(const char *str, int &num){
	if(str == nullptr || !isdigit(*str))
		return false;
	for(num = 0; *str != 0 && isdigit(*str); str += 1)
		num = num * 10 + (*str - 48);
	return true;
}

void gldiag(void){
	SDL_Window *window;
	SDL_GLContext ctx;
	int version;
	PFNGLGETSTRINGPROC glGetString;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		std::clog << "Failed to initialize SDL video system\n";
		std::clog << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	SCOPE_EXIT(SDL_Quit);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	//	SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("dummy", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 100, 100,
		SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	if(window == nullptr){
		std::clog << "Failed to create SDL window\n";
		std::clog << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	SCOPE_EXIT([&](void){ SDL_DestroyWindow(window); });

	ctx = SDL_GL_CreateContext(window);
	if(ctx == nullptr){
		std::clog << "Failed to create GL context\n";
		std::clog << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	SCOPE_EXIT([&](void){ SDL_GL_DeleteContext(ctx); });

	// retrieve opengl info
	glGetString = (PFNGLGETSTRINGPROC)SDL_GL_GetProcAddress("glGetString");
	if(glGetString == nullptr){
		std::clog << "failed to load `glGetString` proc address";
		return;
	}
	if(!next_number((const char*)glGetString(GL_VERSION), version)){
		std::clog << "failed to parse OpenGL version string\n";
		return;
	}

	// overall info
	std::cout << "OpenGL Info"
		<< "\n============================="
		<< "\nVendor: " << glGetString(GL_VENDOR)
		<< "\nRenderer: " << glGetString(GL_RENDERER)
		<< "\nVersion: " << glGetString(GL_VERSION)
		<< "\nShading Language Version: "
			<< glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< "\n\n";

	// TODO: add limits

	// supported extensions
	std::cout << "Supported Extensions"
		<< "\n=============================";
	if(version >= 3){
		int extension_count;
		// load needed functions
		auto glGetStringi = (PFNGLGETSTRINGIPROC)SDL_GL_GetProcAddress("glGetStringi");
		auto glGetIntegerv = (PFNGLGETINTEGERVPROC)SDL_GL_GetProcAddress("glGetIntegerv");
		if(glGetStringi == nullptr || glGetIntegerv == nullptr){
			std::clog << "failed to load `glGetStringi` proc address\n";
			std::clog << "failed to load `glGetIntegerv` proc address\n";
			std::cout << "\nFailed to parse extensions";
		}
		// parse extensions
		glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);
		for(int i = 0; i < extension_count; i++){
			std::cout << '\n' << glGetStringi(GL_EXTENSIONS, i);
		}
	}else{
		std::string str = (const char*)glGetString(GL_EXTENSIONS);
		if(str.length() > 0){
			std::replace(str.begin(), str.end(), ' ', '\n');
			std::cout << str;
		}else{
			std::cout << "\nFailed to parse extensions";
		}
	}
	std::cout << "\n\n";
}

int main(int argc, char **argv){
	std::ios_base::sync_with_stdio(false);
	gldiag();
	return 0;
}
