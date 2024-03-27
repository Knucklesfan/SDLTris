#include "wireframecube.h"
#include "math.h"
#include "utils/defs.h"
wireframecube::wireframecube(int x0, int y0, int w, int h) {
    buff = new buffermanager(w,h,true);
    rotateZ3D(rad(30));
    rotateY3D(rad(30));
    rotateX3D(rad(30));
    width = w;
    height = h;
    x = x0;
    y = y0;

}

void wireframecube::logic(double deltatime) {
    rotateZ3D(rad(deltatime / 50));
    rotateY3D(rad(deltatime / 50));
    //rotateX3D(rad(deltatime/50));
}
void wireframecube::render(int r, int g, int b) {
    buff->enable();
    for (int* indices : edges) {
        double* n1 = nodes[indices[0]];
        double* n2 = nodes[indices[1]];
        double x1 = n1[0] * (width / 640) + x;
        double y1 = n1[1] * (height / 480) + y;
        double x2 = n2[0] * (width / 640) + x;
        double y2 = n2[1] * (height / 480) + y;
        graphics::line->render(graphics::shaders.at(1),{x1,y1},{x2,y2},1,{r,g,b,1});
    }
    buff->disable(640,480,true);
}
void wireframecube::rotateZ3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double x = xyz[0], y = xyz[1];
        xyz[0] = x * c - y * s;
        xyz[1] = y * c + x * s;
    }
}

void wireframecube::rotateY3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double x = xyz[0], z = xyz[2];
        xyz[0] = x * c - z * s;
        xyz[2] = z * c + x * s;
    }
}

void wireframecube::rotateX3D(double t) {
    double c = cos(t), s = sin(t);
    for (double* xyz : nodes) {
        double y = xyz[1], z = xyz[2];
        xyz[1] = y * c - z * s;
        xyz[2] = z * c + y * s;
    }
}

double wireframecube::rad(double deg) {
    return deg * M_PI / 180;
}
