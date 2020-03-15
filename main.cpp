#include <iostream>
#include <queue>
#include <SDL2/SDL.h>
#include <deque>
#include <math.h>
#undef main


using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SQUARE = 20;
const int FPS = 60;
const int FRAMETIME = 1000 / FPS;


void drawrect(SDL_Renderer *ren, int x, int y, int w, int h) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(ren, 64, 64, 64, 255);
    SDL_RenderDrawRect(ren, &rect);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
}

void fillrectlines(SDL_Renderer *ren, int x, int y, int w, int h, int r, int g, int b) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderFillRect(ren, &rect);
    drawrect(ren, x, y, w + 1, h + 1);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
}

void fillrect(SDL_Renderer *ren, int x, int y, int w, int h, int r, int g, int b) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderFillRect(ren, &rect);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
}

void draw(SDL_Renderer *ren, int maderect[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE], int delay) {
     for(int i = 0; i <= SCREEN_WIDTH; i += SQUARE) {
            for(int y = 0; y <= SCREEN_HEIGHT; y += SQUARE) {
                if(maderect[i / SQUARE][y / SQUARE] == 1) {
                    fillrect(ren, i + 1, y + 1, SQUARE - 2, SQUARE - 2, 64, 64, 64);
                }
                else if(maderect[i / SQUARE][y / SQUARE] == 2) {
                    fillrect(ren, i, y, SQUARE, SQUARE, 33, 166, 53);
                }
                
                else if(maderect[i / SQUARE][y / SQUARE] == 3) {
                    fillrect(ren, i, y, SQUARE, SQUARE, 166, 33, 33);
                }
                else if(maderect[i / SQUARE][y / SQUARE] == 4) {
                    fillrect(ren, i, y, SQUARE, SQUARE, 66, 135, 245);
                }
                else if(maderect[i / SQUARE][y / SQUARE] == 5) {
                    fillrect(ren, i, y, SQUARE, SQUARE, 181, 255, 171);
                }
                else {
                    drawrect(ren, i, y, SQUARE, SQUARE);
                }
            }
        }
    SDL_Delay(delay);
    SDL_RenderPresent(ren);
}

void shortestdist(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int dist[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE], int maderect[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE]) {
    bool visted[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {false};
    int vertx, verty, cdist;

    queue<int> que[2];
    que[0].push(ending.x);
    que[1].push(ending.y);

    visted[ending.x][ending.y] = true;

    while(!que[0].empty() || !que[1].empty()) {
        vertx = que[0].front();
        verty = que[1].front();
        cdist = dist[vertx][verty];
        cout << vertx << " " << verty << " " << cdist << endl;
        
        que[0].pop();
        que[1].pop();

        if(vertx == starting.x && verty == starting.y) {
            maderect[vertx][verty] = 2;
            break;
        }

        if(visted[vertx + 1][verty] == false && vertx + 1 < SCREEN_WIDTH / SQUARE && dist[vertx + 1][verty] < cdist && maderect[vertx + 1][verty] != 1) {
            que[0].push(vertx + 1);
            que[1].push(verty);
            visted[vertx + 1][verty] = true;
            maderect[vertx + 1][verty] = 5;
        }
        else if(visted[vertx - 1][verty] == false && vertx - 1 >= 0 && dist[vertx - 1][verty] < cdist && maderect[vertx - 1][verty] != 1) {
            que[0].push(vertx - 1);
            que[1].push(verty);
            visted[vertx - 1][verty] = true;
            maderect[vertx - 1][verty] = 5;
        }
        else if(visted[vertx][verty - 1] == false && verty - 1 >= 0 && dist[vertx][verty - 1] < cdist && maderect[vertx][verty - 1] != 1) {
            que[0].push(vertx);
            que[1].push(verty - 1);
            visted[vertx][verty - 1] = true;
            maderect[vertx][verty - 1] = 5;
        }
        else if(visted[vertx][verty + 1] == false && verty + 1 < SCREEN_HEIGHT / SQUARE && dist[vertx][verty + 1] < cdist &&  maderect[vertx][verty + 1] != 1) {
            que[0].push(vertx);
            que[1].push(verty + 1);
            visted[vertx][verty + 1] = true;
            maderect[vertx][verty + 1] = 5;
        }
        draw(ren, maderect, 0);
    }

}


void bfs(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int maderect[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE]) {
    bool visted[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {false};
    int vertx, verty, dist[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {0};
    queue<int> que[2];
    que[0].push(starting.x);
    que[1].push(starting.y);

    for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++)
        for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++)
            if(maderect[i][x] == 4 || maderect[i][x] == 5)
                maderect[i][x] = 0;

    visted[starting.x][starting.y] = true;

    while(!que[0].empty() || !que[1].empty()) {
        vertx = que[0].front();
        verty = que[1].front();

        que[0].pop();
        que[1].pop();

        if(vertx == ending.x && verty == ending.y) {
            cout << "worked" << endl;
            maderect[vertx][verty] = 3;
            break;
        }

        if(visted[vertx + 1][verty] == false && vertx + 1 < SCREEN_WIDTH / SQUARE && maderect[vertx + 1][verty] != 1) {
            que[0].push(vertx + 1);
            que[1].push(verty);
            visted[vertx + 1][verty] = true;
            if(maderect[vertx + 1][verty] != 3)
                maderect[vertx + 1][verty] = 4;
            dist[vertx + 1][verty] = dist[vertx][verty] + 1;
        }
        if(visted[vertx - 1][verty] == false && vertx - 1 >= 0 && maderect[vertx - 1][verty] != 1) {
            que[0].push(vertx - 1);
            que[1].push(verty);
            visted[vertx - 1][verty] = true;
            if(maderect[vertx - 1][verty] != 3)
                maderect[vertx - 1][verty] = 4;
            dist[vertx - 1][verty] = dist[vertx][verty] + 1;
        }
        if(visted[vertx][verty - 1] == false && verty - 1 >= 0 && maderect[vertx][verty - 1] != 1) {
            que[0].push(vertx);
            que[1].push(verty - 1);
            visted[vertx][verty - 1] = true;
            if(maderect[vertx][verty - 1] != 3)
                maderect[vertx][verty - 1] = 4;
            dist[vertx][verty - 1] = dist[vertx][verty] + 1;
        }
        if(visted[vertx][verty + 1] == false && verty + 1 < SCREEN_HEIGHT / SQUARE && maderect[vertx][verty + 1] != 1) {
            que[0].push(vertx);
            que[1].push(verty + 1);
            visted[vertx][verty + 1] = true;
            if(maderect[vertx][verty + 1] != 3)
                maderect[vertx][verty + 1] = 4;
            dist[vertx][verty + 1] = dist[vertx][verty] + 1;
        }
        draw(ren, maderect, 0);

    }
    for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++)
        for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++)
            if(dist[i][x] == 0)
                dist[i][x] = 999;
    shortestdist(ren, starting, ending, dist, maderect);

}

void astar(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int maderect[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE]) {
    bool visted[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {false};
    int vertx, verty, dist[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {0}, best, index;
    deque<int> que[3];
    que[0].push_front(starting.x);
    que[1].push_front(starting.y);

    for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++)
        for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++)
            if(maderect[i][x] == 4 || maderect[i][x] == 5)
                maderect[i][x] = 0;

    visted[starting.x][starting.y] = true;
    que[2].push_front(pow(ending.x - starting.x, 2) + pow(ending.y - starting.y, 2));

    while(!que[0].empty() || !que[1].empty()) {
        best = 99999;

        cout << que[0].size() << endl;
        for(int i = 0; i <= que[0].size(); i++) {
            if(que[2][i] <= best && que[2][i] != 0) {
                vertx = que[0][i];
                verty = que[1][i];
                best = que[2][i];
                index = i;
            }
        }

        que[0].erase(que[0].begin() + index);
        que[1].erase(que[1].begin() + index);
        que[2].erase(que[2].begin() + index);

        cout << vertx << " " << verty << " " << best << endl;

        if(vertx == ending.x && verty == ending.y) {
            cout << "worked" << endl;
            maderect[vertx][verty] = 3;
            cout << "break" << endl;
            break;
        }

        if(visted[vertx + 1][verty] == false && vertx + 1 < SCREEN_WIDTH / SQUARE && maderect[vertx + 1][verty] != 1) {
            que[0].push_back(vertx + 1);
            que[1].push_back(verty);
            visted[vertx + 1][verty] = true;
            if(maderect[vertx + 1][verty] != 3)
                maderect[vertx + 1][verty] = 4;
            dist[vertx + 1][verty] = dist[vertx][verty] + 1;
            que[2].push_back(dist[vertx + 1][verty] + pow((vertx + 1 - ending.x), 2) + pow((verty - ending.y), 2));
        }
        if(visted[vertx - 1][verty] == false && vertx - 1 >= 0 && maderect[vertx - 1][verty] != 1) {
            que[0].push_back(vertx - 1);
            que[1].push_back(verty);
            visted[vertx - 1][verty] = true;
            if(maderect[vertx - 1][verty] != 3)
                maderect[vertx - 1][verty] = 4;
            dist[vertx - 1][verty] = dist[vertx][verty] + 1;
           que[2].push_back(dist[vertx - 1][verty] + pow((vertx - 1 - ending.x), 2) + pow((verty - ending.y), 2));
        }
        if(visted[vertx][verty - 1] == false && verty - 1 >= 0 && maderect[vertx][verty - 1] != 1) {
            que[0].push_back(vertx);
            que[1].push_back(verty - 1);
            visted[vertx][verty - 1] = true;
            if(maderect[vertx][verty - 1] != 3)
                maderect[vertx][verty - 1] = 4;
            dist[vertx][verty - 1] = dist[vertx][verty] + 1;
            que[2].push_back(dist[vertx][verty - 1] + pow((vertx - ending.x), 2) + pow((verty - 1 - ending.y), 2));
        }
        if(visted[vertx][verty + 1] == false && verty + 1 < SCREEN_HEIGHT / SQUARE && maderect[vertx][verty + 1] != 1) {
            que[0].push_back(vertx);
            que[1].push_back(verty + 1);
            visted[vertx][verty + 1] = true;
            if(maderect[vertx][verty + 1] != 3)
                maderect[vertx][verty + 1] = 4;
            dist[vertx][verty + 1] = dist[vertx][verty] + 1;
           que[2].push_back(dist[vertx][verty + 1] + pow((vertx - ending.x), 2) + pow((verty + 1 - ending.y), 2));
        }
        draw(ren, maderect, 0);
    }
    for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++)
        for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++)
            if(dist[i][x] == 0)
                dist[i][x] = 999;
    shortestdist(ren, starting, ending, dist, maderect);
    for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++)
        for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++)
            dist[i][x] = 0;
    que[0].erase(que[0].begin(), que[0].end());
    que[1].erase(que[0].begin(), que[0].end());
    que[2].erase(que[0].begin(), que[0].end());
}

int main () {
    SDL_Window *win = SDL_CreateWindow("Grid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;
    Uint32 starttime, endtime, delta;
    bool quit = false, startingpoint = false, endingpoint = false;
    int maderect[SCREEN_WIDTH / SQUARE][SCREEN_HEIGHT / SQUARE] = {0};
    SDL_Point mouse = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, starting, ending;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderPresent(ren);

    while(!quit) {
        starttime = SDL_GetTicks();
        SDL_RenderClear(ren);
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if(e.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if(maderect[mouse.x / SQUARE][mouse.y / SQUARE] != 2 && maderect[mouse.x / SQUARE][mouse.y / SQUARE] != 3) {
                    if(keystates[SDL_SCANCODE_LSHIFT] && !startingpoint) {
                        maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 2;
                        startingpoint = true;
                        starting.x = mouse.x / SQUARE;
                        starting.y = mouse.y / SQUARE;
                    }
                    else if(keystates[SDL_SCANCODE_Z] && !endingpoint) {
                        maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 3;
                        endingpoint = true;
                        ending.x = mouse.x / SQUARE;
                        ending.y = mouse.y / SQUARE;
                        cout << ending.x << " " << ending.y << endl;
                    }
                    else if(keystates[SDL_SCANCODE_X]) {
                        maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 0;
                    }
                    else {
                        maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 1;
                    }
                }
                else if(keystates[SDL_SCANCODE_X]) {
                    if(maderect[mouse.x / SQUARE][mouse.y / SQUARE] == 2) {
                        startingpoint = false;
                        starting.x = NULL;
                        starting.y = NULL;
                    }
                    else if (maderect[mouse.x / SQUARE][mouse.y / SQUARE] == 3) {
                        endingpoint = false;
                        ending.x = NULL;
                        ending.y = NULL;
                    }
                    maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 0;
                }
            }
            if(keystates[SDL_SCANCODE_1] && starting.x != NULL && starting.y != NULL  && ending.x != NULL && ending.y != NULL) {
                bfs(ren, starting, ending, maderect);
            }
            if(keystates[SDL_SCANCODE_2] && starting.x != NULL && starting.y != NULL  && ending.x != NULL && ending.y != NULL) {
                astar(ren, starting, ending, maderect);
            }
            if(keystates[SDL_SCANCODE_D]) {
                for(int i = 0; i < SCREEN_WIDTH / SQUARE; i++) {
                    for(int x = 0; x < SCREEN_HEIGHT / SQUARE; x++) {
                        maderect[i][x] = 0;
                    }
                }
                
                starting.x = NULL;
                starting.y = NULL;
                ending.x = NULL;
                ending.y = NULL;
                startingpoint = false;
                endingpoint = false;
            }
        }
        endtime = SDL_GetTicks();
        delta = endtime - starttime;
        if(delta < FRAMETIME){
            SDL_Delay(FRAMETIME - delta);
        }

        draw(ren, maderect, 0);
    }


    SDL_Quit();
    return 0;
}