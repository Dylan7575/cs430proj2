
#include "parse.h"
#include <stdbool.h>
static inline double sqr(double v){
    return v*v;
}
static inline void normalize(double* v){
    double len =  sqr(v[0])+sqr(v[1])+sqr(v[2]);
}
double sphere_intersection(double* ro,double* rd,double radius,double*center ){
    double a=sqr(rd[0])+sqr(rd[1])+sqr(rd[2]);
    double b=2*(rd[0]*(ro[0]-center[0]))+rd[1]*(ro[1]-center[1])+rd[2]*(ro[2]-center[2]);
    double c=sqr((ro[0]-center[0]))+sqr((ro[1]-center[1]))+sqr((ro[2]-center[2]))-sqr(radius);
    double d = sqr(b)-4.0*a*c;
    if(d<0){
        return 0;
    }
    d=sqrt(d);
    double t=-.5*(b+d)/a;
    if(t>0){
        return t;
    }
    return 0;
}
double plane_intersection(double*ro,double*rd,double* normal,double* center){

}

// next_c() wraps the getc() function and provides error checking and line
// number maintenance


int main(int c, char** argv) {
    Object *objects=malloc(sizeof(Object*)*128);
    read_scene(argv[1],&objects);
    //printf("%u", sizeof(objects));
    return 0;
}