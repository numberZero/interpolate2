#include "user.hxx"
#include <stdexcept>
#include <sys/time.h>
#ifdef HAVE_SDL
#include <SDL.h>
#endif
#ifdef HAVE_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif
#include "main.hxx"
#include "visual.hxx"

#ifdef HAVE_SDL
SDL_Window *win;
SDL_GLContext ctx;
#endif
#ifdef HAVE_X11
Display *display;
Window win;
GLXContext ctx;
#endif

int w = 800, h = 600;

static double get_time()
{
	timeval t;
	gettimeofday(&t, nullptr);
	return 1e-6 * t.tv_usec + t.tv_sec;
}

static void step()
{
	static double t_prev = get_time();
	double t = get_time();
	double dt = t - t_prev;
	t_prev = t;
	pre_step(t, dt);
	visual_begin(w, h);
	visual_draw();
	visual_end();
#ifdef HAVE_SDL
	SDL_GL_SwapWindow(win);
#else
	glXSwapBuffers(display, win);
#endif
}

static void processEvents()
{
#ifdef HAVE_SDL
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			throw ExitException();
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			SDL_GetWindowSize(win, &w, &h);
		if (e.type == SDL_KEYDOWN)
			handle_key(e.key.keysym.scancode);
	}
#endif
#ifdef HAVE_X11
	while (XPending(display)) {
		XEvent e;
		XNextEvent(display, &e);
		if (e.type == ClientMessage)
			throw ExitException();
		if (e.type == Expose) {
			XWindowAttributes attrs;
			XGetWindowAttributes(display, win, &attrs);
			w = attrs.width;
			h = attrs.height;
		}
		if (e.type == KeyPress) {
			if (e.xkey.keycode == 9)
				throw ExitException();
			handle_key(e.xkey.keycode);
		}
	}
#endif
}

void user_init()
{
	static char const *title = "Magic Program";
#ifdef HAVE_SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ctx = SDL_GL_CreateContext(win);
	SDL_GL_MakeCurrent(win, ctx);
#endif
#ifdef HAVE_X11
	static GLint attrs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	display = XOpenDisplay(NULL);
	if (!display)
		throw std::runtime_error("Can't open the X display");
	Window root = DefaultRootWindow(display);
	XVisualInfo *vi = glXChooseVisual(display, 0, attrs);
	if (!vi)
		throw std::runtime_error("Can't find an appropriate visual");
	Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;
	win = XCreateWindow(display, root, 0, 0, w, h, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	Atom delWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0 );
	XSetWMProtocols(display, win, &delWindow, 1);
	XMapWindow(display, win);
	XStoreName(display, win, title);
	ctx = glXCreateContext(display, vi, NULL, GL_TRUE);
	glXMakeCurrent(display, win, ctx);
#endif
}

void user_close()
{
#ifdef HAVE_SDL
	SDL_Quit();
#endif
#ifdef HAVE_X11
#endif
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
