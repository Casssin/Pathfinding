#include <iostream>
#include <queue>
#include <SDL2/SDL.h>
#include <deque>
#include <math.h>
#include <utility>
#undef main


using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 760;
const int SQUARE = 20;
const int FPS = 60;
const int FRAMETIME = 1000 / FPS;
const int W_SQUARES = SCREEN_WIDTH / SQUARE;
const int H_SQUARES = SCREEN_HEIGHT / SQUARE;
const SDL_Point NULLPOINT = {NULL, NULL};

inline bool operator==(SDL_Point const &a, SDL_Point const &b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(SDL_Point const &a, SDL_Point const &b)
{
    return !(a == b);
}

inline bool operator<(SDL_Point const &a, SDL_Point const &b) {
    return a.x < b.x && a.y < b.y;
}

template<typename T, typename priority_t> struct PriorityQueue {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>, greater<PQElement>> elements;

  inline bool empty() const {
     return elements.empty();
  }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};


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

void clearScreen(int maderect[W_SQUARES][H_SQUARES]) {
    for(int i = 0; i < W_SQUARES; i++)
        for(int x = 0; x < H_SQUARES; x++)
            if(maderect[i][x] == 4 || maderect[i][x] == 5)
                maderect[i][x] = 0;
} 

void draw(SDL_Renderer *ren, int maderect[W_SQUARES][H_SQUARES], int delay) {
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

queue<SDL_Point> getNeighborsBFS(queue<SDL_Point> que, SDL_Point current, SDL_Point cameFrom[W_SQUARES][H_SQUARES], int maderect[W_SQUARES][H_SQUARES]) {
    if(cameFrom[current.x + 1][current.y] == NULLPOINT && current.x + 1 < W_SQUARES && maderect[current.x + 1][current.y] != 1) {
        SDL_Point temp = {current.x + 1, current.y};
        que.push(temp);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(cameFrom[current.x - 1][current.y] == NULLPOINT && current.x - 1 >= 0 && maderect[current.x - 1][current.y] != 1) {
        SDL_Point temp = {current.x - 1, current.y};
        que.push(temp);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(cameFrom[current.x][current.y + 1] == NULLPOINT && current.y + 1 < H_SQUARES && maderect[current.x][current.y + 1] != 1) {
        SDL_Point temp = {current.x, current.y + 1};
        que.push(temp);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(cameFrom[current.x][current.y - 1] == NULLPOINT && current.y - 1 >= 0 && maderect[current.x][current.y - 1] != 1) {
        SDL_Point temp = {current.x, current.y - 1};
        que.push(temp);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    return que;
}

void shortestdist(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int maderect[W_SQUARES][H_SQUARES], SDL_Point cameFrom[W_SQUARES][H_SQUARES]) {
    SDL_Point current = ending;
    int distance = 0;
    vector<SDL_Point> path;
    while (current != starting) {
        path.push_back(current);
        current = cameFrom[current.x][current.y];
        distance++;
    }
    int pathSize = path.size();
    for(int i = 0; i < pathSize - 1; i++) {
        maderect[path.back().x][path.back().y] = 5;
        path.pop_back();
        draw(ren, maderect, 0);
    }
    cout << distance << endl;
}


void bfs(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int maderect[W_SQUARES][H_SQUARES]) {
    queue<SDL_Point> que;
    que.push(starting);

    clearScreen(maderect);

    SDL_Point cameFrom[W_SQUARES][H_SQUARES] = {NULLPOINT};
    cameFrom[que.front().x][que.front().y] = NULLPOINT;

    while(que.size() > 0) {
        SDL_Point current = que.front();
        que.pop();

        if(current == ending)
            break;

        que = getNeighborsBFS(que, current, cameFrom, maderect);
        draw(ren, maderect, 0);
    }
    shortestdist(ren, starting, ending, maderect, cameFrom);
}

double heuristic(SDL_Point a, SDL_Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

PriorityQueue<SDL_Point, double> getNeighborsAStar(PriorityQueue<SDL_Point, double> que, SDL_Point current, SDL_Point cameFrom[W_SQUARES][H_SQUARES], int maderect[W_SQUARES][H_SQUARES], double cost[W_SQUARES][H_SQUARES], SDL_Point ending) {
    double newCost = cost[current.x][current.y] + 1;
    if(current.x + 1 < W_SQUARES && maderect[current.x + 1][current.y] != 1 && (cost[current.x + 1][current.y] == NULL || cost[current.x + 1][current.y] > newCost)) {
        SDL_Point temp = {current.x + 1, current.y};
        double priority = newCost + heuristic(ending, temp);
        cost[temp.x][temp.y] = newCost;
        que.put(temp, priority);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(current.x - 1 >= 0 && maderect[current.x - 1][current.y] != 1 && (cost[current.x - 1][current.y] == NULL || cost[current.x - 1][current.y] > newCost)) {
        SDL_Point temp = {current.x - 1, current.y};
        double priority = newCost + heuristic(ending, temp);
        cost[temp.x][temp.y] = newCost;
        que.put(temp, priority);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(current.y + 1 < H_SQUARES && maderect[current.x][current.y + 1] != 1 && (cost[current.x][current.y + 1] == NULL || cost[current.x][current.y + 1] > newCost)) {
        SDL_Point temp = {current.x, current.y + 1};
        double priority = newCost + heuristic(ending, temp);
        cost[temp.x][temp.y] = newCost;
        que.put(temp, priority);;
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    if(current.y - 1 >= 0 && maderect[current.x][current.y - 1] != 1 && (cost[current.x][current.y - 1] == NULL || cost[current.x][current.y - 1] > newCost)) {
        SDL_Point temp = {current.x, current.y - 1};
        double priority = newCost + heuristic(ending, temp);
        cost[temp.x][temp.y] = newCost;
        que.put(temp, priority);
        cameFrom[temp.x][temp.y] = current;
        if(maderect[temp.x][temp.y] == 0)
            maderect[temp.x][temp.y] = 4;
    }
    return que;
}

void astar(SDL_Renderer *ren, SDL_Point starting, SDL_Point ending, int maderect[W_SQUARES][H_SQUARES]) {
    PriorityQueue<SDL_Point, double> que;
    que.put(starting, 0);

    clearScreen(maderect);

    SDL_Point cameFrom[W_SQUARES][H_SQUARES] = {NULLPOINT};
    cameFrom[starting.x][starting.y] = NULLPOINT;

    double cost[W_SQUARES][H_SQUARES] = {NULL};
    cost[starting.x][starting.y] = 0;

    while(!que.empty()) {
        SDL_Point current = que.get();

        if(current == ending) 
            break;
        
        que = getNeighborsAStar(que, current, cameFrom, maderect, cost, ending);
        draw(ren, maderect, 0);
    }
    shortestdist(ren, starting, ending, maderect, cameFrom);
}


int main () {
    SDL_Window *win = SDL_CreateWindow("Grid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;
    Uint32 starttime, endtime, delta;
    bool quit = false, startingpoint = false, endingpoint = false;
    int maderect[W_SQUARES][H_SQUARES] = {0};
    SDL_Point mouse = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, starting, ending;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    starting.x = -1;
    starting.y = -1;
    ending.x = -1;
    ending.y = -1;

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
                        starting.x = -1;
                        starting.y = -1;
                    }
                    else if (maderect[mouse.x / SQUARE][mouse.y / SQUARE] == 3) {
                        endingpoint = false;
                        ending.x = -1;
                        ending.y = -1;
                    }
                    maderect[mouse.x / SQUARE][mouse.y / SQUARE] = 0;
                }
            }
            if(keystates[SDL_SCANCODE_1] && starting.x != -1 && ending.x != -1) {
                bfs(ren, starting, ending, maderect);
            }
            if(keystates[SDL_SCANCODE_2] && starting.x != -1 && ending.x != -1) {
                astar(ren, starting, ending, maderect);
            }
            if(keystates[SDL_SCANCODE_D]) {
                for(int i = 0; i < W_SQUARES; i++) {
                    for(int x = 0; x < H_SQUARES; x++) {
                        maderect[i][x] = 0;
                    }
                }
                
                starting.x = -1;
                starting.y = -1;
                ending.x = -1;
                ending.y = -1;
                startingpoint = false;
                endingpoint = false;
            }
        }
        endtime = SDL_GetTicks();
        delta = endtime - starttime;
        /*
        if(delta < FRAMETIME){
            SDL_Delay(FRAMETIME - delta);
        }
        */
        draw(ren, maderect, 0);
    }


    SDL_Quit();
    return 0;
}