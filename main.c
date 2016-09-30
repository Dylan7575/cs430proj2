
#include "parse.h"
#include <stdbool.h>
#include "raycaster.h"


// next_c() wraps the getc() function and provides error checking and line
// number maintenance
//use colors in objects write to ppm file write black if not

int main(int c, char** argv) {
    Object objects[129];
    read_scene(argv[3],objects);
    raycast(objects,argv[1],argv[2],argv[4]);
    printf("%f",objects[2].plane->normal[0]);
    return 0;
}
