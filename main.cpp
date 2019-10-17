#include "common.h"
#include "cmath"

bool Init();
void CleanUp();
void Run();
void Julia();
double ScaleNum(double n, double minN, double maxN, double min, double max);
void colors();

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;

int screenWidth = 500;
int screenHeight = 500;
double zoom = 2;
double x = 0;
double y = 0;
int maxIters = 50;
int resolution = 1;
double r = 0;
double i = 0;
double ang = 0;
double inc = 1;
const double PI = 3.141592;

vector<vector<int>> pColors;

bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");

    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;
    srand(time(NULL));
    colors();

    while (gameLoop)
    {   
        Julia();
        SDL_RenderPresent(renderer);
        
        ang += inc;
        double in = .7885;
        r = in * cos(ang * (PI/180));
        i = in * sin(ang * (PI/180));
        if(ang > 360){
            ang = 1;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    gameLoop = false;
                    break;
                default:
                    break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                default:
                    break;
                }
            }
        }
    }
}

void colors(){
    int r = 255;
    int b = 0;
    int g = 0;
    vector<int> color;

    for(b = 0; b < 255; b++){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    for(r = 255; r > 0; r--){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    for(g = 0; g < 255; g++){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    for(b = 255; b > 0; b--){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    for(r = 0; r < 255; r++){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    for(g = 255; g > 0; g--){
        color.push_back(r);
        color.push_back(b);
        color.push_back(g);
        pColors.push_back(color);
        color.clear();
    }
    color.push_back(r);
    color.push_back(b);
    color.push_back(g);
    pColors.push_back(color);
    color.clear();
}

void Julia(){
    double minx = x - zoom;
    double maxx = x + zoom;
    double miny = y - zoom;
    double maxy = y + zoom;

    for(int x = 0; x < screenWidth; x+=resolution){
        for(int y = 0; y < screenHeight; y+=resolution){
            double zr = ScaleNum(x, 0, screenWidth, minx, maxx);
            double zi = ScaleNum(y, 0, screenHeight, miny, maxy);
            double cr = r;
            double ci = i;

            double n = 0;

            while (n < maxIters) {
                double aa = zr * zr;
                double bb = zi * zi;
                
                if (aa + bb > 4.0) {
                    break;  // Bail
                }
                double twoab = 2.0 * zr * zi;
                zr = aa - bb + cr;
                zi = twoab + ci;
                n++;
            }
            //double color = ScaleNum(n, 0, maxIters, 1, 0);
            //color = ScaleNum(sqrt(color), 1, 0, 255, 0);
            double color = sqrt(static_cast<double>(n) / maxIters);
            color = ScaleNum(color, 0, 1, 0, pColors.size() - 1);

            SDL_Rect pos;
            pos.x = x;
            pos.y = y;
            pos.w = resolution;
            pos.h = resolution;
            SDL_SetRenderDrawColor(renderer, pColors[color][0], pColors[color][2], pColors[color][1], 255);
            SDL_RenderFillRect(renderer, &pos);
        }
    }
}

double ScaleNum(double n, double minN, double maxN, double min, double max){
    return (((n - minN) / (maxN - minN)) * (max - min)) + min;
}