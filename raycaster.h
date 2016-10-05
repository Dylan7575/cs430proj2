#include "includes.h"

//struct for storing pixels to write to ppm file
typedef struct{
    int r;
    int g;
    int b;

}Pixel;

/***************Writing to pixel buffer to output file*****************/
void write_p3(Pixel* pixel,int w,int h,char* filename){

    FILE *fh = fopen(filename,"w+");//opening the file handle
    if(fh==NULL){
        fprintf(stderr,"Could not Open Output File");
        exit(1);
    }
    fprintf(fh,"P3 %d %d 255 \n",w,h);//writing header
    for(int i = 0;i<w*h;i++){
        //looping through pix and writing it to the file
        fprintf(fh,"%d ",pixel[i].r);
        fprintf(fh,"%d ",pixel[i].g);
        fprintf(fh,"%d \n",pixel[i].b);
    }
    fclose(fh);//closing file handle
}

/********************helper Functions*****************/
static inline double sqr(double v){
    return v*v;
}

static inline void normalize(double* v){
    double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}
static inline double v3_dot(double* a, double* b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/*********************Sphere Intersection*************/
double sphere_intersection(double* ro,double* rd,double rad,double* center ){
    //doing math
    double a = sqr(rd[0]) + sqr(rd[1]) + sqr(rd[2]);
    double b = 2 * (rd[0] * (ro[0] - center[0]) + rd[1] * (ro[1] - center[1]) + rd[2] * (ro[2] - center[2]));
    double c = sqr(ro[0] - center[0]) + sqr(ro[1] - center[1]) + sqr(ro[2] - center[2]) - sqr(rad);


    double det = sqr(b) - 4 * a * c;//finding determinant


    if (det < 0)
        return -1;

    det = sqrt(det);

    //for quadratic equation can be positive and it can be negative so we do it twice.
    double t0 = (-b - det) / (2 * a);// One for it being negative

    if (t0 > 0)
        return t0;//returning t value

    double t1 = (-b + det) / (2 * a);// One for it being positive
    if (t1 > 0)
        return t1;//returing t value

    return -1;
}

/*******************Plane Intersection*****************/
double plane_intersection(double*ro,double*rd,double* normal,double* position){
    //doing math;
    normalize(normal);
    double D = -(v3_dot(position,normal)); // distance from origin to plane
    double t = -(normal[0] * ro[0] + normal[1] * ro[1] + normal[2] * ro[2] + D) /
                (normal[0] * rd[0] + normal[1] * rd[1] + normal[2] * rd[2]);

    if (t > 0)
        return t;//returning t value

    return -1;
}

/*******************Raycasting*************************/
void raycast(Object* objects,char* picture_width,char* picture_height,char* output_file){

    int j=0,k=0;//loops

    //camera position//
    double cx=0;
    double cy=0;
    /***********/

    //finding the camera in the Objects
    for(j=0;j< sizeof(objects);j++){
        if(objects[j].kind==0){
            break;
        }
    }
    /*****************/

    /*setting the height and width for the camera and the scene and finding the pixheight and width from it*/
    double h =objects[j].cam.height;
    double w=objects[j].cam.width;
    int m =atoi(picture_height);
    int n = atoi(picture_width);
    double pixheight =h/m;
    double pixwidth =w/n;
    /****************/

    Pixel p[m*n];//creating pixel array

    //Looping thorough each x and y coordinates flipping y coordinates to make the image correct
    for(int y=m;y>0;y--){
        for (int x=0;x<n;x++){
            double ro[3]={0,0,0};//ray origin
            double rd[3] = {     //ray direction
                    cx - (w/2) + pixwidth * (x + 0.5),
                    cy - (h/2) + pixheight * (y + 0.5),
                    1
            };
            normalize(rd);//normalizing the ray direction
            double best_t=INFINITY;
            Object object; //creating a temp Object to hold the object that is intersecting with  the ray

            //looping through the objects
            for(int i=0;i<sizeof(objects);i++) {
                double t = 0;
                //switching based upon what type the object is and sending it to the right intersection
                switch (objects[i].kind) {
                    case 1:
                        t = sphere_intersection(ro, rd, objects[i].sphere.radius, objects[i].sphere.center);
                        break;
                    case 2:
                        t = plane_intersection(ro, rd, objects[i].plane.normal, objects[i].plane.position);
                        break;
                    case 0:
                        break;
                    default:
                        fprintf(stderr,"Unknown Type Found");
                        exit(-1);
                }

                //finding the best intersection and then assigning object to it and resetting best t so it can check with other objects
                if(t>0 &&t<best_t){
                    best_t=t;
                    object=objects[i];
                }
            }
            //if best_t shows the ray is intersecting it assign the color of the object to the pixel array
                if(best_t > 0 && best_t != INFINITY){
                    //casting to int
                    if(object.kind==1){//if sphere
                        p[k].r=(int)(object.sphere.color[0]*255);
                        p[k].g=(int)(object.sphere.color[1]*255);
                        p[k].b=(int)(object.sphere.color[2]*255);
                    }
                    if(object.kind==2){//if plane
                        p[k].r=(int)(object.plane.color[0]*255);
                        p[k].g=(int)(object.plane.color[1]*255);
                        p[k].b=(int)(object.plane.color[2]*255);
                    }
                } else{
                    //if not intersecting make it black
                    p[k].r=0;
                    p[k].g=0;
                    p[k].b=0;
                }
            k++;//incrementing pixel
        }
    }

    //writing to ppm file
    write_p3(p,atoi(picture_height),atoi(picture_width),output_file);
}