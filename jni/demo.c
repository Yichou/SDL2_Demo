#include <jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "SDL.h"
#include "SDL_test_font.h"
#include "SDL_ttf.h"

#define SHAPED_WINDOW_X 150
#define SHAPED_WINDOW_Y 150
#define SHAPED_WINDOW_DIMENSION 640

struct _Env {
	SDL_Window *wnd;
	SDL_Renderer *rnd;
	TTF_Font *font;
	int ww;
	int wh;
	struct timeval ts;
	int fps;
};

static struct _Env env;


static int _init()
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	//配置一个图像缩放的效果,linear效果更平滑,也叫抗锯齿
	//SDL_setenv(SDL_HINT_RENDER_SCALE_QUALITY,"linear",0);
	// 初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail.");
		return 1;
	}

	if (TTF_Init() < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init fail.");
		return 1;
	}

	env.font = TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", 24);

	// 创建一个窗口
	env.wnd = SDL_CreateWindow("SDL_RenderClear", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_SHOWN);
	SDL_GetWindowSize(env.wnd, &env.ww, &env.wh);
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "window w=%d h=%d %p", env.ww, env.wh, env.wnd);

	// 创建一个渲染器
	env.rnd = SDL_CreateRenderer(env.wnd, -1, 0);

	return 0;
}

static void _uninit()
{
	//释放渲染器
	SDL_DestroyRenderer(env.rnd);

	//释放窗口
	SDL_DestroyWindow(env.wnd);

	//延时
	//SDL_Delay(8000);

	//退出
	SDL_Quit();
}

void ss()
{
	gettimeofday(&env.ts, NULL);
}

int se()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int t = (tv.tv_sec - env.ts.tv_sec) * 1000 + (tv.tv_usec - env.ts.tv_usec) / 1000;
	env.fps = 1000 / t;
	return t;
}

void render(SDL_Renderer *renderer, SDL_Texture *texture,
		SDL_Rect texture_dimensions) {
	/* Clear render-target to blue. */
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
	SDL_RenderClear(renderer);

	/* Render the texture. */
	SDL_RenderCopy(renderer, texture, &texture_dimensions, &texture_dimensions);

	SDL_RenderPresent(renderer);
}

void a() {
	SDL_Window *window;
	SDL_WindowShapeMode mode;

	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect texture_dimensions;
	Uint32 pixelFormat = 0;
	int access = 0;


	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Shape---------------------------.");

	surface = SDL_LoadBMP("sample.bmp");

	window = SDL_CreateShapedWindow("SDL_Shape test",
		0, 0,
		640, 800, 0);

	SDL_SetWindowPosition(window, 0, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(texture, (Uint32 *) &pixelFormat, (int *) &access,
			&texture_dimensions.w, &texture_dimensions.h);

	SDL_SetWindowSize(window, texture_dimensions.w,texture_dimensions.h);
	SDL_SetWindowShape(window, surface,&mode);

	render(renderer, texture, texture_dimensions);

//	SDL_Delay(10);
//
//	SDL_DestroyTexture(texture);
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	SDL_FreeSurface(surface);
//	SDL_VideoQuit();

}

void dt(SDL_Surface* screen, TTF_Font* font, const char* text) {
	SDL_Color fg = { 0xff, 0, 0 }; //颜色
	SDL_Color bg = { 0, 0, 0, 0}; //颜色
	SDL_Surface *text_surface;

	text_surface = TTF_RenderUTF8_Shaded(font, text, fg, bg);
	if (text_surface != NULL) {
		SDL_BlitSurface(text_surface, NULL, screen, NULL); //将文字复制到屏幕的surface上面
		SDL_FreeSurface(text_surface);
	}
}

/*bmp ttf 演示*/
int b() {
	SDL_Event event;
//	SDL_Surface *surface;
	SDL_Rect dstR, srcR;

	// 创建一个Surface
	SDL_Surface *bmp = SDL_LoadBMP("sample.bmp");
	//设置图片中的白色为透明色
	SDL_SetColorKey(bmp, SDL_TRUE, 0x0);
	dt(bmp, env.font, "你好世界!Abc");

	// 创建一个Texture
	SDL_Texture *texture = SDL_CreateTextureFromSurface(env.rnd, bmp);
	SDL_QueryTexture(texture, NULL, NULL, &srcR.w, &srcR.h);
	SDL_Log("texture w=%d h=%d", srcR.w, srcR.h);
	srcR.x = 0;
	srcR.y = 0;

	int w = srcR.w * 2;
	int h = srcR.h * 2;

	dstR.x = (env.ww - w)/2;
	dstR.y = (env.wh - h)/2;
	dstR.w = w;
	dstR.h = h;

	//清除所有事件
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	while (1)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
				break;
		}

		//使用红色填充背景
		SDL_SetRenderDrawColor(env.rnd, 0x18, 0x18, 0x18, 255);
		SDL_RenderClear(env.rnd);

		// 将纹理布置到渲染器
		dt(bmp, env.font, "***********Abc");
		SDL_RenderCopy(env.rnd, texture, &srcR, &dstR);

		SDL_SetRenderDrawColor(env.rnd, 0xff, 0x0, 0x0, 0xf0);
		SDLTest_DrawString(env.rnd, 50, 50, "Hello Wrold!");

		// 刷新屏幕
		SDL_RenderPresent(env.rnd);
	}

	// 释放Surface
	SDL_FreeSurface(bmp);

	//  释放Texture
	SDL_DestroyTexture(texture);

	return 0;
}

/*基本绘图函数使用，触屏事件*/
int c()
{
	SDL_Event event;
	SDL_Texture *texture;
	SDL_Rect r, rb;
	char buf[64];

	r.w = 100;
	r.h = 50;

//	texture = SDL_CreateTexture(env.rnd, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 480, 800);

	rb.w = 480;
	rb.h = 800;
	rb.x = (env.ww - rb.w) / 2;
	rb.y = (env.wh - rb.h) / 2;

	texture = SDL_CreateTexture(env.rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rb.w, rb.h);


	while (1)
	{
		ss();

		SDL_PollEvent(&event);

		if(event.type == SDL_QUIT)
			break;

		switch(event.type)
		{
		case SDL_FINGERDOWN:
			r.x = event.tfinger.x * env.ww;
			r.y = event.tfinger.y * env.wh;
			break;

		case SDL_FINGERUP:
			break;

		case SDL_FINGERMOTION:
			r.x = event.tfinger.x * env.ww;
			r.y = event.tfinger.y * env.wh;
			break;
		}

//		r.x=rand()%500;
//		r.y=rand()%500;

		SDL_SetRenderDrawColor(env.rnd, 0x00, 0x00, 0x00, 0xff);

		SDL_RenderClear(env.rnd);

		SDL_SetRenderTarget(env.rnd, texture);
		SDL_RenderClear(env.rnd);

		SDL_RenderDrawRect(env.rnd, &r);
		SDL_SetRenderDrawColor(env.rnd, 0xff, 0x00, 0x00, 0xff);
		SDL_RenderFillRect(env.rnd, &r);
		SDL_SetRenderTarget(env.rnd, NULL);

//		texture->pixels

//		SDL_RenderCopy(env.rnd, texture, NULL, NULL);

		SDL_RenderDrawRect(env.rnd, &rb);
		SDL_RenderFillRect(env.rnd, &r);

		sprintf(buf, "Hello Wrold! fps:%d", env.fps);
		SDLTest_DrawString(env.rnd, 50, 50, buf);
		SDL_RenderPresent(env.rnd);

		se();

//		SDL_Delay(100);
	}

	return 0;
}

int SDL_main(int argc, char *argv[])
{
	int ret = _init();

//	ret = b();
	ret = c();

	_uninit();

	return ret;
}

JNIEXPORT void JNICALL Java_com_example_sdl_MainActivity_test(JNIEnv *env) {
}
