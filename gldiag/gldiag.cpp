// stdlib includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

// sdl includes
#include <SDL.h>

// opengl includes
#include "glcorearb.h"

// local includes
#include "scopeguard.h"

// a few string overloads to help outputting
std::string &operator<<(std::string &self, const std::string &rhs){
	self += rhs;
	return self;
}

std::string &operator<<(std::string &self, const GLubyte *rhs){
	self += (const char*)rhs;
	return self;
}

std::string &operator<<(std::string &self, const GLubyte rhs){
	self += (const char)rhs;
	return self;
}

std::string &operator<<(std::string &self, const char *rhs){
	self += rhs;
	return self;
}

std::string &operator<<(std::string &self, const char rhs){
	self += rhs;
	return self;
}

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

// output diag to buf
void gldiag(std::string &buf){
	SDL_Window *window;
	SDL_GLContext ctx;
	int version;
	PFNGLGETSTRINGPROC glGetString;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "Failed to initialize SDL video system\n";
		std::cout << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	auto sdl_guard = make_scope_guard(SDL_Quit);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	//	SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("dummy", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 100, 100,
		SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	if(window == nullptr){
		std::cout << "Failed to create SDL window\n";
		std::cout << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	auto window_guard = make_scope_guard(
		[&](void){ SDL_DestroyWindow(window); });

	ctx = SDL_GL_CreateContext(window);
	if(ctx == nullptr){
		std::cout << "Failed to create GL context\n";
		std::cout << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	auto ctx_guard = make_scope_guard(
		[&](void){ SDL_GL_DeleteContext(ctx); });

	// retrieve opengl info
	glGetString = (PFNGLGETSTRINGPROC)SDL_GL_GetProcAddress("glGetString");
	if(glGetString == nullptr){
		std::cout << "failed to load `glGetString` proc address";
		return;
	}
	if(!next_number((const char*)glGetString(GL_VERSION), version)){
		std::cout << "failed to parse OpenGL version string\n";
		return;
	}

	// overall info
	buf << "OpenGL Info"
		<< "\n============================="
		<< "\nVendor: " << glGetString(GL_VENDOR)
		<< "\nRenderer: " << glGetString(GL_RENDERER)
		<< "\nVersion: " << glGetString(GL_VERSION)
		<< "\nShading Language Version: "
			<< glGetString(GL_SHADING_LANGUAGE_VERSION)
		<< "\n\n";

	// TODO: add limits

	// supported extensions
	buf << "Supported Extensions"
		<< "\n=============================";
	if(version >= 3){
		int extension_count;
		// load needed functions
		auto glGetStringi = (PFNGLGETSTRINGIPROC)SDL_GL_GetProcAddress("glGetStringi");
		auto glGetIntegerv = (PFNGLGETINTEGERVPROC)SDL_GL_GetProcAddress("glGetIntegerv");
		if(glGetStringi == nullptr || glGetIntegerv == nullptr){
			std::cout << "failed to load `glGetStringi` proc address\n";
			std::cout << "failed to load `glGetIntegerv` proc address\n";
			buf << "\nFailed to parse extensions";
		}
		// parse extensions
		glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);
		for(int i = 0; i < extension_count; i++){
			buf << '\n' << glGetStringi(GL_EXTENSIONS, i);
		}
	}else{
		std::string str = (const char*)glGetString(GL_EXTENSIONS);
		if(str.length() > 0){
			std::replace(str.begin(), str.end(), ' ', '\n');
			buf << str;
		}else{
			buf << "\nFailed to parse extensions";
		}
	}
	buf << "\n\n";
}

int main(int argc, char **argv){
	std::ofstream f;
	std::string buffer;

	// this will speedup cout
	std::ios_base::sync_with_stdio(false);
	// 32KB should be enough
	buffer.reserve(32768);

	// collect data
	gldiag(buffer);

	// write buffer to file
	f.open("gldiag.txt", std::ios_base::out | std::ios_base::trunc);
	if(f.fail()){
		std::cout << "failed to open file for writing\n";
		return -1;
	}
	f << buffer;
	f.close();
	std::cout << "File 'gldiag.txt' generated\n";
	return 0;
}
