#include "Renderer.h"

#include "ArgParser.h"
#include "Camera.h"
#include "Hit.h"
#include "Image.h"
#include "Ray.h"
#include "VecUtils.h"

#include <limits>
#include <cstdlib>

Renderer::Renderer(const ArgParser &args) :
_args(args),
_scene(args.input_file),
_bounces(args.bounces)
{

}

Renderer::~Renderer()
{
}
void Renderer::printNoise() const{
    for(int i=0;i<_scene.getNumMaterials();i++){
        Material *m = _scene.getMaterial(i);
        std::cout<<m<<std::endl;
    }
}
// IMPLEMENT THESE FUNCTIONS
// These function definitions are mere suggestions. Change them as you like.
Vector3f
mirrorDirection(const Vector3f &normal, const Vector3f &incoming)
{
    // TODO: IMPLEMENT
    float cos = Vector3f::dot(incoming,normal);
    return (incoming - 2*cos*normal).normalized();
}

bool
transmittedDirection(const Vector3f &normal,
    const Vector3f &incoming,
    float index_i,
    float index_t,
    Vector3f &transmitted)
    {
        // TODO: IMPLEMENT
        float cos = Vector3f::dot(incoming,normal);
        float ratio = index_i/index_t;
        float check = 1-std::pow(ratio,2)*(1-std::pow(cos,2));
        if(check>0){
            transmitted = ratio*(incoming-cos*normal)-std::sqrt(check)*normal;
            transmitted.normalize();
            return true;
        }
        return false;
    }

    Vector3f
    Renderer::traceRay(const Ray &ray,
        float tmin,
        int bounces,
        float refr_index,
        Hit &h,
        float t) const
        {
            // TODO: IMPLEMENT
            Hit hit = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
            Group *g = _scene.getGroup();
            float range_x = t * t * 0.3f;
            float range_y = t * 0.55f + 0.45f;
			float clip = 0.7 - t*0.3;
            //std::cout<<"=============="<<std::endl;
            if(g->intersect(ray,tmin,hit,range_x,range_y,clip)){
                Material *m = hit.getMaterial();
                /*
                float d = m->getD(hit.getTexCoord(),range_x,range_y);
                if(d>clip){
                    return _scene.getBackgroundColor(ray.getDirection());
                }
                */
                Vector3f color =m->getDiffuse(hit)*_scene.getAmbientLight();
                Vector3f hitPoint = ray.pointAtParameter(hit.getT());
                for(int i=0; i<_scene.getNumLights();i++){
                    Vector3f dirL, colorL; float dis;
                    _scene.getLight(i)->getIllumination(hitPoint,dirL,colorL,dis);
                    if(_args.shadows){
                        Hit shadowH = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
                        Ray shadowR = Ray(hitPoint,dirL.normalized());
                        //std::cout<<"shadow"<<std::endl;
                        g->intersect(shadowR,0.05,shadowH, range_x,range_y,clip);
                        if(shadowH.getT()>=dis){
                            color+=m->shade(ray,hit,dirL,colorL, range_x, range_y);
                        }
                    }else{
                        color+=m->shade(ray,hit,dirL,colorL,range_x,range_y);
                    }
                }
                
                if(bounces>0){
                    //reflection
                    Vector3f mirrorD = mirrorDirection(hit.getNormal().normalized(),ray.getDirection());
                    Ray reflectR = Ray(hitPoint,mirrorD);
                    Hit reflectH = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
                    Vector3f reflection = m->getSpecularColor()*traceRay(reflectR,0.05f,bounces-1,refr_index,reflectH, t);

                    //refraction
                    Vector3f transmit=Vector3f::ZERO;
                    float newIndex = m->getRefractionIndex();
                    if(newIndex==0){
                        color+=reflection;
                    }else{
                        if(newIndex==refr_index){
                            newIndex = 1.0f;
                            hit.setNormal(-hit.getNormal());
                        }
                        if(transmittedDirection(hit.getNormal().normalized(),ray.getDirection(),refr_index,newIndex,transmit)){
                            Ray refractR = Ray(hitPoint,transmit);
                            Hit refractH = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
                            Vector3f refraction = m->getSpecularColor()*traceRay(refractR,0.05,bounces-1,newIndex,refractH, t);

                            float R0 = std::pow((newIndex-refr_index)/(newIndex+refr_index),2);
                            float c = refr_index<=newIndex ? std::abs(Vector3f::dot(ray.getDirection(),hit.getNormal())) : std::abs(Vector3f::dot(transmit,hit.getNormal()));

                            float R = R0+(1-R0)*std::pow((1-c),5);

                            color+=R*reflection;
                            color+=(1-R)*refraction;

                        }else{
                            color+=reflection;
                        }
                    }

                }
                
                return color;
            }else{
                return _scene.getBackgroundColor(ray.getDirection());
            }
        }

        float randomF(){
            return (float)std::rand()/RAND_MAX-0.5f;
        }

        Vector3f validPixel(Image &image, int i, int j){
            if(i>=0 && i<image.getWidth() && j>=0 && j<image.getHeight()){
                return image.getPixel(i,j);
            }else{
                return Vector3f::ZERO;
            }
        }

        Image blur(Image &image){
            float K[5] = {0.1201, 0.2339, 0.2931, 0.2339, 0.1201};
            int width = image.getWidth();
            int height = image.getHeight();
            Image blurH = Image(width,height);
            for(int i=0;i<width;i++){
                for(int j=0;j<height;j++){
                    Vector3f bluredH = K[0]*validPixel(image,i,j-2)+
                    K[1]*validPixel(image,i,j-1)+
                    K[2]*validPixel(image,i,j)+
                    K[3]*validPixel(image,i,j+1)+
                    K[4]*validPixel(image,i,j+2);
                    blurH.setPixel(i,j,bluredH);
                }
            }
            Image blurV = Image(width,height);
            for(int i=0;i<width;i++){
                for(int j=0;j<height;j++){
                    Vector3f bluredV = K[0]*validPixel(blurH,i-2,j)+
                    K[1]*validPixel(blurH,i-1,j)+
                    K[2]*validPixel(blurH,i,j)+
                    K[3]*validPixel(blurH,i+1,j)+
                    K[4]*validPixel(blurH,i+2,j);
                    blurV.setPixel(i,j,bluredV);
                }
            }
            return blurV;
        }

        void
        Renderer::Render(float t, int frame)
        {
            // TODO: IMPLEMENT

            // This is the main loop that should be based on your assignment 4 main
            // loop. You will need to modify it to switch from ray casting to ray
            // tracing, to cast shadows and anti-aliasing (via jittering and
            // filtering).
            int width = this->_args.width;
            int height = this->_args.height;
            int stats = this->_args.stats;
            int minDepth = this->_args.depth_min;
            int maxDepth = this->_args.depth_max;
            std::string outputFilename;
            if(frame<10){
                outputFilename = this->_args.output_file +"00"+ std::to_string(frame) + ".png";
            }else if(frame<100){
                outputFilename = this->_args.output_file +"0"+ std::to_string(frame) + ".png";
            }
            else{
                outputFilename = this->_args.output_file + std::to_string(frame) + ".png";
            }
            float x_step = 2.0/width;
            float y_step = 2.0/height;
            Camera * cam = this->_scene.getCamera();
            Group * group = this->_scene.getGroup();
            Image outputF = Image(width,height);
            for (int i = 0; i < this->_scene.getNumMaterials(); i++){
                this->_scene.getMaterial(i)->preRender(t);
            }

            group->preRender(t);

            if(_args.jitter){
                x_step /=3;
                y_step /=3;
                Image jitterOut = Image(3*width,3*height);
                for(int i=0;i<3*width;i++){
                    for(int j=0;j<3*height;j++){
                        Ray r = cam->generateRay(Vector2f(-1+(i+randomF())*x_step,-1+(j+randomF())*y_step));
                        Hit h = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
                        Vector3f color = traceRay(r,0,_bounces,1.0f,h, t);
                        jitterOut.setPixel(i,j,color);

                        if(i%50==0 && j==899){
                            std::cout<<"i="<<i<<"\tj="<<j<<std::endl;
                        }
                    }
                }
                if(_args.filter){
                    Image blured = blur(jitterOut);
                    for(int oi=0;oi<width;oi++){
                        for(int oj=0;oj<height;oj++){
                            int i = 3*oi+1;
                            int j = 3*oj+1;
                            Vector3f downSample = validPixel(blured,i-1,j-1)+validPixel(blured,i-1,j)+validPixel(blured,i-1,j+1)+
                            validPixel(blured,i,j-1)+validPixel(blured,i,j)+validPixel(blured,i,j+1)+
                            validPixel(blured,i+1,j-1)+validPixel(blured,i+1,j)+validPixel(blured,i+1,j+1);
                            outputF.setPixel(oi,oj,downSample/9.0);
                        }
                    }
                }else{
                    outputF = jitterOut;
                }

            }else{

                std::cout<<"Iterate over pixel"<<std::endl;
                for(int i=0;i<width;i++){
                    for(int j=0;j<height;j++){
                        Ray r = cam->generateRay(Vector2f(-1+i*x_step, -1+j*y_step));
                        Hit h = Hit(std::numeric_limits<float>::max(),NULL,Vector3f::ZERO);
                        Vector3f color = traceRay(r,0,_bounces,1.0f,h,t);
                        outputF.setPixel(i,j,color);
                    }
                }
            }
            outputF.savePNG(outputFilename);

        }
