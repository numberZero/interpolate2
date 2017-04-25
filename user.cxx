#include "user.hxx"
#include <SDL.h>
#include "main.hxx"
#include "visual.hxx"

SDL_Window *win;
SDL_GLContext ctx;
int w = 800, h = 600;

static void step()
{
	static double t_prev = 0.0;
	double t = 1e-3 * SDL_GetTicks();
	double dt = t - t_prev;
	t_prev = t;
	pre_step(t, dt);
	visual_begin(w, h);
	visual_draw();
	visual_end();
	SDL_GL_SwapWindow(win);
}

static void processEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			throw ExitException();
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			SDL_GetWindowSize(win, &w, &h);
		if (e.type == SDL_KEYDOWN)
			handle_key(e.key.keysym.scancode);
	}
}

void user_init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow(
		"Magic Program",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ctx = SDL_GL_CreateContext(win);
	SDL_GL_MakeCurrent(win, ctx);
}

void user_close()
{
	SDL_Quit();
}

void user_mainloop()
{
	try
	{
		for (;;) {
			processEvents();
			step();
		}
	}
	catch(ExitException)
	{
	}
}
