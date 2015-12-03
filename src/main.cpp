#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "vecmath.h"

#include "ArgParser.h"
#include "Renderer.h"
#include "Image.h"
#include "MyNoise.h"
#include "Noise.h"

int
main(int argc, const char *argv[])
{
  // Report help usage if no args specified.
  if (argc == 1) {
    std::cout << "Usage: a5 <args>\n"
    << "\n"
    << "Args:\n"
    << "\t-input <scene>\n"
    << "\t-size <width> <height>\n"
    << "\t-output <image.png>\n"
    << "\t-depth <depth_min> <depth_max> <depth_image.png>\n"
    << "\t-bounces <max_bounces>\n"
    << "\t-shadows\n"
    << "\t[-jitter]\n"
    << "\t[-filter]\n"
    << "\t[-normals <normals_image.png>]\n"
    << "\n"
    ;
    return 1;
  }

  if(!strcmp(argv[1],"-compare")){
    Image *image1 = Image::loadPNG(argv[2]);
    Image *image2 = Image::loadPNG(argv[3]);
    Image *imageDiff = Image::compare(image1,image2);
    imageDiff->savePNG(argv[4]);
  }else if(!strcmp(argv[1],"-noise")){
    int octaves = atoi(argv[2]);
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);

    MyNoise mn = MyNoise(width,height);
    Image output = Image(width,height);
    for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
        float n = mn.getNoise(i,j,octaves);
        Vector3f color =n*Vector3f(1,1,1);
        output.setPixel(i,j,color);
      }
    }
    output.savePNG(argv[5]);
  }else{
    ArgParser argsParser(argc, argv);
    Renderer renderer(argsParser);
    renderer.Render();
  }

  return 0;
}
