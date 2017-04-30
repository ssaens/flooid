#include <eng/eng.h>
#include "display/Application.h"

int main(int argc, char** argv) {

    std::string cube_path;
    if (argc > 1) {
        cube_path = argv[1];
    } else {
        cube_path = "src/cubemaps/textures/"; // Don't use non jpegs :b
    }

    Application app;
    app.set_cube_path(cube_path);
    eng::View view;

    view.set_renderer(&app);
    view.init();

    view.begin();

    return 0;

}