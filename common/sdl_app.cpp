#include "sdl_app.h"

#include <iostream>

SDLApp::SDLApp(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { throw "SDL Error"; }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
            "OpenGL Shaders",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    gl_context = SDL_GL_CreateContext(window);
    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;

    is_running = true;
    timestamp = (double) SDL_GetTicks();
    this->width = width;
    this->height = height;
}
SDLApp::~SDLApp() {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLApp::run() {
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: is_running = false; break;
            }
        }
        double delta_time = ((double) SDL_GetTicks() - timestamp) / 1000.0;
        timestamp = (double) SDL_GetTicks();
        update(delta_time);
        SDL_GL_SwapWindow(window);
    }
}
