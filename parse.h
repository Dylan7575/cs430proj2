//
// Created by Dylan La Frenz on 9/27/16.
//
double line=1;
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    double center[3];
    double color[3];
    double radius;
}Sphere;
typedef struct{
    double height;
    double width;
}Camera;
typedef struct{
    double position[3];
    double color[3];
    double normal[3];

}Plane;
typedef struct{
    int kind;
    Camera cam[1];
    Sphere sphere[1];
    Plane plane[1];

}Object;
int next_c(FILE* json) {
    int c = fgetc(json);
#ifdef DEBUG
    printf("next_c: '%c'\n", c);
#endif
    if (c == '\n') {
        line += 1;
    }
    if (c == EOF) {
        fprintf(stderr, "Error: Unexpected end of file on line number %d.\n", line);
        exit(1);
    }
    return c;
}


// expect_c() checks that the next character is d.  If it is not it emits
// an error.
void expect_c(FILE* json, int d) {
    int c = next_c(json);
    if (c == d) return;
    fprintf(stderr, "Error: Expected '%c' on line %d.\n", d, line);
    exit(1);
}


// skip_ws() skips white space in the file.
void skip_ws(FILE* json) {
    int c = next_c(json);
    while (isspace(c)) {
        c = next_c(json);
    }
    ungetc(c, json);
}


// next_string() gets the next string from the file handle and emits an error
// if a string can not be obtained.
char* next_string(FILE* json) {
    char buffer[129];
    int c = next_c(json);
    if (c != '"') {
        fprintf(stderr, "Error: Expected string on line %d.\n", line);
        exit(1);
    }
    c = next_c(json);
    int i = 0;
    while (c != '"') {
        if (i >= 128) {
            fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
            exit(1);
        }
        if (c == '\\') {
            fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
            exit(1);
        }
        if (c < 32 || c > 126) {
            fprintf(stderr, "Error: Strings may contain only ascii characters.\n");
            exit(1);
        }
        buffer[i] = c;
        i += 1;
        c = next_c(json);
    }
    buffer[i] = 0;
    return strdup(buffer);
}

double next_number(FILE* json) {
    double value;
    fscanf(json, "%lf", &value);
    // Error check this..
    //printf("%lf\n",value);
    return value;
}

double* next_vector(FILE* json) {
    double* v = malloc(3*sizeof(double));
    expect_c(json, '[');
    skip_ws(json);
    v[0] = next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[1] = next_number(json);
    skip_ws(json);
    expect_c(json, ',');
    skip_ws(json);
    v[2] = next_number(json);
    skip_ws(json);
    expect_c(json, ']');
    //printf("%f\n",v[0]);
    return v;
}



void read_scene(char* filename,Object* object) {
    int c,i=0;


    FILE* json = fopen(filename, "r");

    if (json == NULL) {
        fprintf(stderr, "Error: Could not open file \"%s\"\n", filename);
        exit(1);
    }

    skip_ws(json);

    // Find the beginning of the list
    expect_c(json, '[');

    skip_ws(json);

    // Find the objects

    while (1) {
        Plane plane;
        Sphere sphere;
        Camera cam;
        c = fgetc(json);
        if (c == ']') {
            fprintf(stderr, "Error: This is the worst scene file EVER.\n");
            fclose(json);
            return;
        }
        if (c == '{') {
            skip_ws(json);

            // Parse the object
            char* key = next_string(json);
            if (strcmp(key, "type") != 0) {
                fprintf(stderr, "Error: Expected \"type\" key on line number %d.\n", line);
                exit(1);
            }

            skip_ws(json);

            expect_c(json, ':');

            skip_ws(json);

            char* value = next_string(json);

            if (strcmp(value, "camera") == 0) {
                object[i].kind=0;

            } else if (strcmp(value, "sphere") == 0) {
                object[i].kind = 1;
            } else if (strcmp(value, "plane") == 0) {
                object[i].kind = 2;
            } else {
                fprintf(stderr, "Error: Unknown type, \"%s\", on line number %d.\n", value, line);
                exit(1);
            }

            skip_ws(json);

            while (1) {
                // , }
                c = next_c(json);
                if (c == '}') {
                    // stop parsing this object
                    break;
                } else if (c == ',') {
                    // read another field
                    skip_ws(json);
                    char* key = next_string(json);
                    skip_ws(json);
                    expect_c(json, ':');
                    skip_ws(json);
                    if ((strcmp(key, "width") == 0) ||
                        (strcmp(key, "height") == 0) ||
                        (strcmp(key, "radius") == 0)) {
                        double value = next_number(json);
                        if (strcmp(key, "width")==0) {
                            cam.width=value;
                        } else if (strcmp(key, "height") == 0) {
                            cam.height=value;
                        } else if (strcmp(key, "radius") == 0) {
                            sphere.radius=value;
                        }
                    } else if ((strcmp(key, "color") == 0) ||
                               (strcmp(key, "position") == 0) ||
                               (strcmp(key, "normal") == 0)) {
                        double* value = next_vector(json);
                        if (strcmp(key, "color") == 0) {
                            if (object[i].kind == 1) {
                                sphere.color[0] = value[0];
                                sphere.color[1] = value[1];
                                sphere.color[2] = value[2];
                            }
                            if (object[i].kind == 2) {
                                plane.color[0] = value[0];
                                plane.color[1] = value[1];
                                plane.color[2] = value[2];
                            }
                        }
                        if (strcmp(key, "position") == 0) {
                            if (object[i].kind == 1) {
                                sphere.center[0] = value[0];
                                sphere.center[1]  = value[1];
                                sphere.center[2]  = value[2];
                            }
                            if (object[i].kind == 2) {
                                plane.position[0]  = value[0];
                                plane.position[1]  = value[1];
                                plane.position[2]  = value[2];
                            }
                        }
                        if (strcmp(key, "normal") == 0) {
                            if (object[i].kind == 2) {
                                plane.normal[0]  = value[0];
                                plane.normal[1]  = value[1];
                                plane.normal[2]  = value[2];
                            }
                        }



                    } else {
                        fprintf(stderr, "Error: Unknown property, \"%s\", on line %d.\n",
                                key, line);
                        //char* value = next_string(json);
                    }
                    skip_ws(json);
                } else {
                    fprintf(stderr, "Error: Unexpected value on line %d\n", line);
                    exit(1);
                }
                object[i].cam[0]=cam;
                object[i].sphere[0]=sphere;
                object[i].plane[0]=plane;
            }
            skip_ws(json);
            c = next_c(json);
            if (c == ',') {
                // noop
                skip_ws(json);
            } else if (c == ']') {
                fclose(json);
                return;
            } else {
                fprintf(stderr, "Error: Expecting ',' or ']' on line %d.\n", line);
                exit(1);
            }
        }
        i++;
    }
}


