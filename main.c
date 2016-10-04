#include "parse.h"
#include "raycaster.h"

int main(int c, char** argv) {
    Object objects[129];
    read_scene(argv[3],objects);
    raycast(objects,argv[1],argv[2],argv[4]);
    return 0;
}
