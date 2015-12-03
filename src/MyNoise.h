#ifndef MYNOISE_H
#define MYNOISE_H

#include <cmath>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>

class MyNoise{
public:
  MyNoise(int width, int height){
    _width = width;
    _height = height;
    generateNoise();
  }

  float getNoise(float x, float y, int octaves){
    float value = 0.0f;
    int initial = pow(2,octaves);
    for(int i=octaves;i>=0;i--){
      int size = pow(2,i);
      value += smooth(x/size,y/size)*size;
    }
    float res = 0.5*value/initial;
    //std::cout<<res<<std::endl;
    return res;
  }

  float noiseAt(int x, int y){
    return _noise[x*_width+y];
  }

private:
  int _width;
  int _height;
  std::vector<float> _noise;

  void generateNoise(){
    for(int i=0;i<_width;i++){
      for(int j=0;j<_height;j++){
        _noise.push_back(float((rand()%32768)/32768.0));
      }
    }
  }

  float smooth(float x, float y){
    int ix = int(x); int iy = int(y);
    float fx = x-ix; float fy = y-iy;

    //point
    int x1 = (ix+_width)%_width;
    int y1 = (iy+_height)%_height;
    int x2 = (x1+_width-1)%_width;
    int y2 = (y1+_height-1)%_height;

    float result = 0.0f;
    result += fx*fy*noiseAt(x1,y1);
    result += fx*(1-fy)*noiseAt(x1,y2);
    result += (1-fx)*fy*noiseAt(x2,y1);
    result += (1-fx)*(1-fy)*noiseAt(x2,y2);
    return result;
  }

};

#endif
