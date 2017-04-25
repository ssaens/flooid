#include <eng/eng.h>
#include "Application.h"

using namespace std;
using namespace eng;

int main() {
    View view;

    AppConfig config;
    Application *app = new Application(config);
    view.set_renderer(app);

    view.init();
    view.run();

    return 0;
}