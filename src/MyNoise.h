#ifndef MYNOISE_H
#define MYNOISE_H

#include <cmath>
#include <cstring>
#include <iostream>

class MyNoise{
public:
  MyNoise(){
    ind = 0;
  }

  float noise(int x, int y){
    int n = x+y*57;
    n = pow((n>>13),n);
    float res = 1.0 - ( (n * (n * n * prime1[ind] + prime2[ind]) + prime3[ind]) & 0x7fffffff) / 1073741824.0;
    //std::cout<<res<<std::endl;
    if(res>1||res<-1){
      std::cout<<"no"<<std::endl;
    }
    return res;
  }

  float smooth(int x, int y){
    float corner = (noise(x-1,y-1)+noise(x+1,y+1)+noise(x-1,y+1)+noise(x+1,y-1))/16;
    float side = (noise(x-1,y)+noise(x+1,y)+noise(x,y-1)+noise(x,y+1))/8;
    float center = noise(x,y)/4;
    return corner+side+center;
  }

  float linear_inter(float x, float y, float f){
    return x*(1-f)+y*f;
  }

  float interpolate(float x, float y){
    int ix = (int)x;
    int iy = (int)y;
    float fx = x-ix;
    float fy = y-iy;

    float v1 = smooth(x,y);
    float v2 = smooth(x+1,y);
    float v3 = smooth(x,y+1);
    float v4 = smooth(x+1,y+1);

    float i1 = linear_inter(v1,v2,fx);
    float i2 = linear_inter(v3,v4,fx);

    return linear_inter(i1,i2,fy);
  }

  float perlinNoise(float x, float y, int octaves, float persistence){
    float total =0;
    for(int i=0;i<octaves;i++){
      int freq = pow(2,i);
      float amp = pow(persistence,i);
      float ni = interpolate(x*freq,y*freq)*amp;
      std::cout<<ni<<std::endl;
      total += ni;
      ind++;
    }
    ind = 0;
    return total;
  }
private:
  int ind;
  int prime1[5] = {15731, 60493, 15527, 16111, 15307};
  int prime2[5] = {789221, 19990303, 780587, 782149, 782189};
  int prime3[5] = {1376312589, 1376312589,1376397709,1376343317,1376343541};
};

#endif
