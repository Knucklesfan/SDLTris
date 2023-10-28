// #include "cube.h"
// #include "math.h"

// wireframecube::wireframecube(SDL_Renderer* render, int x0, int y0, int w, int h) {
//     texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,w,h);
//     rotateZ3D(rad(30));
//     rotateY3D(rad(30));
//     rotateX3D(rad(30));
//     width = w;
//     height = h;
//     x = x0;
//     y = y0;
//     SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

// }

// void wireframecube::logic(double deltatime) {
//     rotateZ3D(rad(deltatime / 50));
//     rotateY3D(rad(deltatime / 50));
//     //rotateX3D(rad(deltatime/50));
// }
// void wireframecube::render(SDL_Renderer* render, int r, int g, int b) {

//     SDL_Texture* temp = SDL_GetRenderTarget(render);
//     SDL_SetRenderTarget(render, texture);
//     SDL_RenderClear(render);
//     SDL_SetRenderDrawColor(render, r, g, b, 255);
//     for (int* indices : edges) {
//         double* n1 = nodes[indices[0]];
//         double* n2 = nodes[indices[1]];
//         double x1 = n1[0] * (width / 640) + x;
//         double y1 = n1[1] * (height / 480) + y;
//         double x2 = n2[0] * (width / 640) + x;
//         double y2 = n2[1] * (height / 480) + y;

//         SDL_RenderDrawLine(render, x1,y1,x2,y2);
//     }
//     SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
//     SDL_SetRenderTarget(render, temp);

// }
// void wireframecube::rotateZ3D(double t) {
//     double c = cos(t), s = sin(t);
//     for (double* xyz : nodes) {
//         double x = xyz[0], y = xyz[1];
//         xyz[0] = x * c - y * s;
//         xyz[1] = y * c + x * s;
//     }
// }

// void wireframecube::rotateY3D(double t) {
//     double c = cos(t), s = sin(t);
//     for (double* xyz : nodes) {
//         double x = xyz[0], z = xyz[2];
//         xyz[0] = x * c - z * s;
//         xyz[2] = z * c + x * s;
//     }
// }

// void wireframecube::rotateX3D(double t) {
//     double c = cos(t), s = sin(t);
//     for (double* xyz : nodes) {
//         double y = xyz[1], z = xyz[2];
//         xyz[1] = y * c - z * s;
//         xyz[2] = z * c + y * s;
//     }
// }

// double wireframecube::rad(double deg) {
//     return deg * M_PI / 180;
// }
