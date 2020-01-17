# jade-engine Introduction {#mainpage}

`jade-engine` is a small, programmer-focused, 2D game engine written in C++ on top of SDL2.

## Getting started

### Prerequisites
* [Visual Studio](https://developer.microsoft.com/en-us/windows/downloads) - 15.7 or newer

### Set-up
jade-engine is intended to be compiled directly into the target executable so there are no pre-built libraries to download and install.

Instead we recommend to set-up `jade-engine` as [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules), include all the files in `include` and `source` folders and use provided Visual Studio Project Property files to pull in 3rd party dependencies. Detailed guide can be found in [Installation page](@ref installationpage).

### Usage
`jade-engine` comes with a samples solution that can be found in `samples` folder and should be the first stop when exploring `jade-engine`.

When browsing the documentation some of the key classes are: [Game](@ref JadeEngine::Game), [IGameObject](@ref JadeEngine::IGameObject), [IScene](@ref JadeEngine::IScene).





