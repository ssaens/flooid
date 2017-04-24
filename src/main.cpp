#include <eng/eng.h>

#include <iostream>
#include "PBFRenderer.h"

using namespace std;
using namespace eng;

int main() {
    View view;
    PBFRenderer *pbf = new PBFRenderer();
    view.set_renderer(pbf);
    view.init();
    view.run();
    return 0;
}