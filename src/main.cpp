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
    float persistence = atof(argv[3]);
    MyNoise mn = MyNoise();

    Image output = Image(50,50);
    std::vector<float> noise;
    float max = -100000.0f;
    float min = 1000000.0f;
    for(int i=0;i<50;i++){
      for(int j=0;j<50;j++){
        std::cout<<"========"<<std::endl;
        float x = (float)i/25;
        float y = (float)j/25;
        std::cout<<x<<"\t"<<y<<std::endl;
        float n = mn.perlinNoise(x,y,octaves,persistence);
        float abs_n =(float)fabs((double)n);
        //std::cout<<abs_n<<std::endl;
        // if(abs_n<min){
        //   min = abs_n;
        // }
        // if(abs_n>max){
        //   max = abs_n;
        // }
        // noise.push_back(abs_n);
        Vector3f color =abs_n*Vector3f(1,1,1);
        //color.print();
        output.setPixel(i,j,color);
      }
    }
    output.savePNG(argv[4]);
  }else{
    ArgParser argsParser(argc, argv);
    Renderer renderer(argsParser);
    renderer.Render();
  }

  return 0;
}
