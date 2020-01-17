# jade-engine
A small, programmer-focused, 2D game engine written in modern C++ on top of SDL2.

[Documentation can be found here](https://vhonzik.github.io/jade-engine/html/index.html)

## Why You should NOT use it
* First and foremost this is a toy, not a production ready piece of software
* No games were shipped with it (I am not counting my Ludum Dare game-dev jam entries)
* Documentation is work in progress (VS solution with samples and mentioned doxygen docs are included)
* Windows only (with few modifications I was able to compile it on Ubuntu but some things were not working correctly)
* Setting-up project using the engine is awkward (The engine is intended to be compiled directly into the target exe which is a costly trade-off when it comes to setting up new projects)
* No support for packaging assets
* No editor, preview mode, scripting - common features of modern game engines
* No multiplayer code
* Not optimized (to be fair there was no need yet and it is 2D only engine where performance usually doesn't matter as much)
* No deployment processes
* No particles support

## If you are still with me, here are some neat features
* Written in modern C++
* Small size and the fact it is intended to be compiled directly into your target exe means everything is at your fingertips and extending it should be "super easy barely an inconvenience"™
* Allows quick prototyping, e.g. comes with main menu scene, options scenes, keybindings, etc.
* Consistent approach to creating game objects - you fill a creation structure with data and call templated Create function of Game class
* Reasonable amount of built-in UI game objects
* Spritesheet support using free version of TexturePacker

