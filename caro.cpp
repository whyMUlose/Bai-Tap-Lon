#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "botbaseline.h"
using namespace std;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;
const int h_w = 24;
const int FRAME_DELAY = 16;
SDL_Window* window;
SDL_Renderer* renderer;

const string WINDOW_TITLE = "XO Game";

int board_game[HEIGHT][WIDTH] = {0};

void logSDLError(ostream& os, const string &msg, bool fatal = false)
{
    os << msg << "Error: " << SDL_GetError() << endl;
    if(fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while(true)
    {
        if(SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) return;
        SDL_Delay(100);
    }
}
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr)
    {
        cout<<"Unable to  load image " << path << " SDL_image Error: "
            << IMG_GetError() << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == nullptr)
            cout<<"Unable to create texture from " << path << " SDL Error: "
                << SDL_GetError()<< endl;
        SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
}
void renderTexture(string path, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_Texture *file = loadTexture(path.c_str(), renderer);
    if(file == nullptr)
    {
        logSDLError(cout, "Fail", true);
        SDL_DestroyTexture(file);
        quitSDL(window, renderer);
        //break;
    }
    SDL_RenderCopy(renderer, file, NULL, &rect);
    SDL_DestroyTexture(file);
}

int convertX(int n)
{
    return n * h_w;
}

int convertY(int n)
{
    return n * h_w;
}


int win()
{
    for(int i = 0; i < 30; ++i)
    {
        for(int j = 0; j < 50; ++j)
        {
            int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
            for(int k = 0; k < 5; ++k)
            {
                s1 += board_game[i][j + k];
                s2 += board_game[i + k][j];
            }

            for(int k = 0; k < min(5, min(30 - i, 50 - j)); ++k)
            {
                s3 += board_game[i + k][j + k];
            }

            for(int k = 0; k < min(5, min(i, 50 - j)); ++k)
            {
                s4 += board_game[i - k][j + k];
            }

            if(s1 == 5 ) {
            	if ((j > 0 && board_game[i][j - 1] == -1) && (j + 5 < 50 && board_game[i][j + 5] == -1)) {continue;}
            	else {return 1;}
            }

			if(s1 == -5 ) {
				if ((j > 0 && board_game[i][j - 1] == 1) && (j + 5 < 50 && board_game[i][j + 5] == 1)) {continue;}
				else {return 2;}
			}
             if(s2 == 5 ) {
            	if ((i > 0 && board_game[i-1][j] == -1) && (i + 5 < 30 && board_game[i+5][j] == -1)) {continue;}
            	else {return 1;}
            }

			if(s2 == -5 ) {
				if ((i > 0 && board_game[i-1][j] == 1) && (i + 5 < 30 && board_game[i+5][j] == 1)) {continue;}
				else {return 2;}
			}
			 if(s3 == 5 ) {
            	if ((i > 0 && j>0 && board_game[i-1][j-1] == -1) && (i + 5 < 30 && j+5<50 && board_game[i+5][j+5] == -1)) {continue;}
            	else {return 1;}
            }
            			 if(s3 == -5 ) {
            	if ((i > 0 && j>0 && board_game[i-1][j-1] == 1) && (i + 5 < 30 && j+5<50 && board_game[i+5][j+5] == 1)) {continue;}
            	else {return 2;}
            }
             if(s4 == 5 ) {
            	if ((i > 4 && j >0 && board_game[i-1][j+1] == -1) && (i < 30 && j+5<50 && board_game[i-5][j+5] == -1)) {continue;}
            	else {return 1;}
            }
            if(s4 == -5 ) {
            	if ((i > 4 && j >0 && board_game[i-1][j+1] == 1) && (i < 30 && j+5<50 && board_game[i-5][j+5] == 1)) {continue;}
            	else {return 2;}
            }
        }
    }
    return 0;
}
int main(int argc, char* argv[] )
{

    initSDL(window, renderer);
    SDL_Texture* background = loadTexture("backgroud.png", renderer);
    SDL_RenderCopy( renderer, background, NULL, NULL);

    bool quit_game = false;
    bool bot = false;
    Uint32 frameStart;
    int frameTime;

    while(!quit_game)
    {
        //cout << 1 << endl;
        frameStart = SDL_GetTicks();
        if(!bot)
        {
            Point a1 = player_baseline(board_game, 1);
            cout << a1.x << " 2 " << a1.y << endl;
            board_game[a1.x][a1.y] = 1;
            bot = true;
            renderTexture("O.png",convertX(a1.y), convertY(a1.x),24, 24);
        }
        if(win() != 0) quit_game = true;
        Point a2;

        SDL_Event e;
        SDL_PollEvent(&e);

        if(e.type == SDL_QUIT)
        {
            SDL_Quit();
            quit_game = true;
        }
        if(e.type == SDL_MOUSEBUTTONDOWN && !quit_game)
        {
            int x = e.button.x;
            int y = e.button.y;
            cout << x << " 2 " << y << endl;

            x /= h_w;
            y /= h_w;

            if(bot && board_game[y][x] == 0)
            {
                a2 = {y, x};
                board_game[y][x] = -1;
                renderTexture("X.png",convertX(a2.y), convertY(a2.x),24, 24);
                bot = false;
            }
            if(win() != 0) quit_game = true;
        }

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;



        if(FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }

    }
    if(quit_game)
    {if (win()==1)
        {
            renderTexture("loser.jpg",240, 120 , 480,480);
        }
        if(win()==2)
        {
            renderTexture("win.jpg",240, 120 , 480,480);
        }
    }
    cout << win();

    SDL_RenderPresent(renderer);
    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
