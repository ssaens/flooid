#include <eng/eng.h>
#include "display/Application.h"

int main() {

    Application app;
    eng::View view;

    view.set_renderer(&app);
    view.init();
    view.begin();

    return 0;

}