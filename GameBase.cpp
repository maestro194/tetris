#include "GameBase.h"

void ErrorLog(std::string msg, int error_code) {
	if (error_code == SDL_ERROR) {
		std::cout << msg << " " << SDL_GetError() << '\n';
	}
	if (error_code == IMG_ERROR) {
		std::cout << msg << " " << IMG_GetError() << '\n';
	}
}