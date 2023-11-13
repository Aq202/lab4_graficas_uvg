#include <SDL2/SDL.h>
#include "framebuffer.h"
#include "triangle.h"
#include "loadOBJ.h"
#include <iostream>
#include "setupVertexArray.h"
#include "uniforms.h"
#include "shaders.h"
#include "fragment.h"
#include "camera.h"
#include "glm/ext.hpp"
#include "sun.h"
#include "earth.h"
#include "moon.h"
#include "jupiter.h"
#include "planetRandom.h"
#include "planetRandom2.h"
#include "spaceship.h"

void printVertex(glm::vec3 vertex)
{
    SDL_Log("Vértice: (%f, %f, %f)\n", vertex.x, vertex.y, vertex.z);
}

void renderBuffer(SDL_Renderer *renderer)
{
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH,
        FRAMEBUFFER_HEIGHT);

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
        texture,
        NULL,
        framebuffer,
        FRAMEBUFFER_WIDTH * sizeof(Color));

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

Uint32 frameStart, frameTime;

int main(int argc, char *argv[])
{

    // iniciar ventana sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Universe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Initialize a Camera object
    Camera camera;
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 50.0f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, -1.0f, 0.0f);

    SunModel sunModel = SunModel(camera);
    EarthModel earthModel = EarthModel(camera);
    MoonModel moonModel = MoonModel(camera);
    JupiterModel jupiterModel = JupiterModel(camera);
    PlanetRandomModel planetRandomModel = PlanetRandomModel(camera);
    PlanetRandom2Model planetRandom2Model = PlanetRandom2Model(camera);

    SpaceshipModel spaceship = SpaceshipModel(camera);
    spaceship.setTranslation(glm::vec3(0.0f ,0.0f,25.0f));

    float cameraSpeed = 2.0f;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        frameStart = SDL_GetTicks();
        Model* selectedModel = NULL;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    spaceship.forward(-2.0f);
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                spaceship.forward(2.0f);

                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    spaceship.rotateY(-5.0f); 

                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    spaceship.rotateY(5.0f);                    
                }
                else if (event.key.keysym.sym == SDLK_w)
                {
                    spaceship.up(2.0f);
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    spaceship.up(-2.0f);

                }
                else if (event.key.keysym.sym == SDLK_t){
                    selectedModel = &earthModel;
                }
                else if (event.key.keysym.sym == SDLK_j){
                    selectedModel = &jupiterModel;
                }
                else if (event.key.keysym.sym == SDLK_y){
                    selectedModel = &planetRandomModel;
                }
                else if (event.key.keysym.sym == SDLK_u){
                    selectedModel = &planetRandom2Model;
                }
                else if (event.key.keysym.sym == SDLK_r){
                    spaceship.setTranslation(glm::vec3(0.0f, 0.0f, 25.0f ));
                    spaceship.setRotationY(180.0f);
                }
                
               
            }
        }

        clear();

        earthModel.rotateY();
        earthModel.translate();

        moonModel.rotateY(3.0f);
        moonModel.setPosition(earthModel.getCurrentPosition());
        moonModel.translate(15.0f);

        jupiterModel.rotateY();
        jupiterModel.translate(1.0f);

        planetRandomModel.rotateY();
        planetRandomModel.translate(2.5f);

        planetRandom2Model.rotateY();
        planetRandom2Model.translate(2.0f);

        if(selectedModel != NULL){
            glm::vec3 gap = glm::vec3(-1.0f, 0.0f, 1.0f);
            spaceship.setTranslation(selectedModel->getCurrentPosition() + gap);
        }

        camera.targetPosition = spaceship.getCurrentPosition();
        camera.cameraPosition =  spaceship.getCurrentPosition() + glm::vec3(0.0f, 0.0f, 10.0f);


        earthModel.render();
        moonModel.render();
        sunModel.render();
        jupiterModel.render();
        planetRandomModel.render();
        planetRandom2Model.render();
        spaceship.render();

        // Render the framebuffer to the screen
        renderBuffer(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime > 0)
        {
            std::ostringstream titleStream;
            titleStream << "FPS: " << 1000.0 / frameTime; // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}