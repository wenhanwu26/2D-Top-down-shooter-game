
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "ai_system.hpp"
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "HelpMenu.h"

using Clock = std::chrono::high_resolution_clock;
const vec2 INTRO_LOCATION = {2400, 4900};
Entity storiesTutorial[5];
int toggleTutorial = 1;

// Entry point
int main()
{
	// Global systems
	WorldSystem world;
	RenderSystem renderer;
	PhysicsSystem physics;
	AISystem ai;
    HelpMenu helpMenu;


	// Initializing window
	GLFWwindow* window = world.create_window();

	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	renderer.init(window_width_px, window_height_px, window);
	world.init(&renderer);
	physics.bullet_hit_callbacks.emplace_back(WorldSystem::handle_bullet_hit);
	physics.item_callbacks.emplace_back(WorldSystem::handle_items);

	// variable timestep loop
	auto t = Clock::now();
	while (!world.is_over()) {
		// Processes system messages, if this wasn't present the window would become
		// unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;

        renderer.draw();
        // menu intro loop
        if(helpMenu.showInto) {
            if(toggleTutorial == 1) {
                storiesTutorial[0] = helpMenu.createStory1(&renderer, window, INTRO_LOCATION);
                toggleTutorial = 2;

            } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && toggleTutorial == 2) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                registry.remove_all_components_of(storiesTutorial[0]);
                storiesTutorial[1] = helpMenu.createStory2(&renderer, window, INTRO_LOCATION);
                toggleTutorial = 3;

            } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && toggleTutorial == 3) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                registry.remove_all_components_of(storiesTutorial[1]);
                storiesTutorial[2] = helpMenu.createStory3(&renderer, window, INTRO_LOCATION);
                toggleTutorial = 4;

            } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && toggleTutorial == 4) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                registry.remove_all_components_of(storiesTutorial[2]);
                storiesTutorial[3] = helpMenu.createStory4(&renderer, window, INTRO_LOCATION);
                toggleTutorial = 5;

            } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && toggleTutorial == 5) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                registry.remove_all_components_of(storiesTutorial[3]);
                storiesTutorial[4] = helpMenu.createStory5(&renderer, window, INTRO_LOCATION);
                toggleTutorial = 6;

            } else if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && toggleTutorial == 6) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                registry.remove_all_components_of(storiesTutorial[4]);
                helpMenu.showInto = false;
            }

        } else {
            world.step(elapsed_ms);
			ai.step(elapsed_ms, world.getCurrentMap());
			physics.step(elapsed_ms);
            //world.handle_collisions();
        }


		// TODO A2: you can implement the debug freeze here but other places are possible too.
	}

	return EXIT_SUCCESS;
}
