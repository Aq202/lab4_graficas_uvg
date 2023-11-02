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

    float cameraSpeed = 0.5f;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        frameStart = SDL_GetTicks();

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
                    // Mover la cámara hacia arriba
                    camera.targetPosition.y -= cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    // Mover la cámara hacia abajo
                    camera.targetPosition.y += cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    // Mover la cámara hacia la izquierda
                    camera.targetPosition.x -= cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    // Mover la cámara hacia la derecha
                    camera.targetPosition.x += cameraSpeed;
                    SDL_Log("%f", camera.targetPosition.x);
                }
                else if (event.key.keysym.sym == SDLK_q)
                {
                    // Acercar camara
                    camera.cameraPosition.x += cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_a)
                {
                    // Alejar camara
                    camera.cameraPosition.x -= cameraSpeed;
                    if (camera.cameraPosition.x < 0)
                        camera.cameraPosition.x = 0;
                }
                else if (event.key.keysym.sym == SDLK_w)
                {
                    // Acercar camara
                    camera.cameraPosition.y += cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    // Alejar camara
                    camera.cameraPosition.y -= cameraSpeed;
                    if (camera.cameraPosition.x < 0)
                        camera.cameraPosition.x = 0;
                }
                else if (event.key.keysym.sym == SDLK_e)
                {
                    // Acercar camara
                    camera.cameraPosition.z += cameraSpeed;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    // Alejar camara
                    camera.cameraPosition.z -= cameraSpeed;
                    if (camera.cameraPosition.x < 0)
                        camera.cameraPosition.x = 0;
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
                    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
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

        earthModel.render();
        moonModel.render();
        sunModel.render();
        jupiterModel.render();
        planetRandomModel.render();
        planetRandom2Model.render();

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