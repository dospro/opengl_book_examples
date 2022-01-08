#ifndef OPENGL_SHADERS_SDL_APP_H
#define OPENGL_SHADERS_SDL_APP_H
#include <SDL.h>
#include <GL/glew.h>

class SDLApp {
public:
    SDLApp(int width, int height);
    virtual ~SDLApp();
    virtual void init() = 0;
    virtual void update(double delta_time) = 0;
    virtual void onKeyPress(unsigned int scancode);
    void run();

protected:
    int width, height;
    SDL_Window *window;
    SDL_GLContext gl_context;
    SDL_Event event;
    bool is_running;
    double timestamp;
};


#endif//OPENGL_SHADERS_SDL_APP_H
