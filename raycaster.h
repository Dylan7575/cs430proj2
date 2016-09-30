#include "write ppm.c"
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include <string.h>
#include <ctype.h>
typedef struct{
    int r[1];
    int g[1];
    int b[1];

}Pixel;
static inline double sqr(double v){
    return v*v;
}
//normal is abc is plane intersection to get D dot product the normal and the position
static inline void normalize(double* v){
    double len =  sqr(v[0])+sqr(v[1])+sqr(v[2]);
}
static inline double v3_dot(double* a, double* b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

double sphere_intersection(double* ro,double* rd,double radius,double*center ){
    double a=sqr(rd[0])+sqr(rd[1])+sqr(rd[2]);
    double b=2*(rd[0]*(ro[0]-center[0]))+rd[1]*(ro[1]-center[1])+rd[2]*(ro[2]-center[2]);
    double c=sqr((ro[0]-center[0]))+sqr((ro[1]-center[1]))+sqr((ro[2]-center[2]))-sqr(radius);
    double det = sqr(b)-4.0*a*c;
    if(det<0){
        return -1;
    }
    double d=sqrt(det);
    double t=(b+d)/(2*a);
    if(t>0){
        return t;
    }
    double t1=(b-d)/(2*a);
    if(t>0){
        return -1;
    }
    return -1;
}
double plane_intersection(double*ro,double*rd,double* normal,double* position){
    normalize(normal);
    double d =v3_dot(normal,rd);
    double t=-1*(normal[0]*ro[0]+normal[1]*ro[1]+normal[2]+d)/(normal[0]*rd[0]+normal[1]+rd[1]+normal[2]*rd[2]);
    if (t>0){
        return t;
    }
    else{
        return -1;
    }



//fill //normalize the normal
}

// next_c() wraps the getc() function and provides error checking and line
// number maintenance

void raycast(Object* objects,char* picture_height,char* picture_width,char* output_file){
    //FILE * fp= fopen(output_file,"rw");
    //write_header(fp);
    int j;
    double cx=0;
    double cy=0;
    for(j=0;j< sizeof(objects);j++){
        if(objects[j].kind==0){
            break;
        }
    }
    double h =objects[j].cam->height;
    double w=objects[j].cam->width;
    int m=picture_height;
    int n = picture_width;
    Pixel p[m*n];
    double pixheight =h/m;
    double pixwidth =w/m;
    //work on this part ANDY
    for(int y=0;y<m;y++){
        for (int x=0;x<n;x++){
            double ro[3]={0,0,0};
            double rd[3]={(cx-w)/2+pixwidth*(x+.5),cy-(h/2)+pixheight*(y+.5),-1};
            normalize(rd);
            for(int i=0;i< sizeof(objects);i++){
                double t=0;
                switch (objects[i].kind){
                    case 1:
                        t=sphere_intersection(ro,rd,objects[i].sphere[0].radius,objects[i].sphere[0].center);
                        break;
                    case 2:
                        t=plane_intersection(ro,rd,objects[i].plane[0].normal,objects[i].plane[0].position);
                    default:
                        exit(0);
                }
                if(t>0){
                    if(kind==1){
                        p[i].r=objects[i].sphere->color[0];
                        p[i].g=objects[i].sphere->color[1];
                        p[i].b=objects[i].sphere->color[2];
                    }
                    if(kind==2){
                        p[i].r=objects[i].plane->color[0];
                        p[i].g=objects[i].plane->color[1];
                        p[i].b=objects[i].plane->color[2];
                    }

                    break;
                } else
                    p.r=0;
                    p.r=0;
                    p.r=0;
            }
        }

    }
    write_p3(p,w,h,output_file);

}
