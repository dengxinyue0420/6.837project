#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "vecmath.h"
#include <ctime>

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

    MyNoise r = MyNoise(1,width,height);
    MyNoise g = MyNoise(time(NULL),width,height);
    MyNoise b = MyNoise(rand(),width,height);
    Image output = Image(width,height);
    for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
        float r_sig = r.getNoise(i,j,octaves);
        float g_sig = g.getNoise(i,j,octaves);
        float b_sig = b.getNoise(i,j,octaves);
        Vector3f color = Vector3f(r_sig,g_sig,b_sig);
        output.setPixel(i,j,color);
      }
    }
    output.savePNG(argv[5]);
  }else{
    ArgParser argsParser(argc, argv);
    Renderer renderer(argsParser);
	for (int i = 0; i < 160; i++){
		renderer.Render(0.00666*i,i);
		std::cout << i << "\n";

	}

  }

  return 0;
}
