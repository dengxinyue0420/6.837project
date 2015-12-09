#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "Texture.h"
#include "Noise.h"

class Material
{
  public:
    Material(const Vector3f &diffuseColor,
             const Vector3f &specularColor = Vector3f::ZERO,
             float shininess = 0,
             float refractionIndex = 0) :
        _diffuseColor(diffuseColor),
        _specularColor(specularColor),
        _shininess(shininess),
        _refractionIndex(refractionIndex),
		_hasDisplacement(false),
		_isExplosion(false)
    {
    }

    virtual ~Material()
    {
    }

    const Vector3f & getDiffuseColor() const {
        return _diffuseColor;
    }

    const Vector3f & getSpecularColor() const {
        return _specularColor;
    }

    float getRefractionIndex() const {
        return _refractionIndex;
    }

    void setNoise(const Noise &n) {
        _noise = n;
    }

    Noise *getNoise(){
      return &_noise;
    }

    Vector3f getDiffuse(Hit &hit){
      if(hit.hasTex() && _texture.isValid()){
	Vector3f kd = _texture.getTexel(hit.getTexCoord()[0], hit.getTexCoord()[1]);
	return kd;
      }else{
	return _diffuseColor;
      }
    }

    float getD(Vector2f texCoord, float range_x, float range_y){
        float d = Vector3f::dot(_displacementMap.getTexel(texCoord[0], texCoord[1]), _channelVector) / 1.2f;
        return (d*(range_y-range_x)+range_x);
    }

    void wtf(int x){
        std::cout<<x<<std::endl;
    }
    Vector3f shade(const Ray &ray,
                   const Hit &hit,
                   const Vector3f &dirToLight,
                   const Vector3f &lightColor,
                   const float range_x,
                   const float range_y)
    {
        Vector3f kd;
		if (_isExplosion){
			const Vector2f &texCoord = hit.getTexCoord();
			float d = Vector3f::dot(_displacementMap.getTexel(texCoord[0], texCoord[1]), _channelVector) / 1.2f;
			if(d>0.99) d = 0.99;
			if (d < 0 ) d = 0;
			Vector3f diffuse = _ramp -> getPixel((d*(range_y-range_x)+range_x) * _ramp->getWidth(), 1);
			Vector3f n = hit.getNormal().normalized();
			Vector3f color = clampedDot(dirToLight, n) * pointwiseDot(lightColor, diffuse);
			return diffuse;
		}


        if (_noise.isValid()) {
            kd = _noise.getColor(ray.getOrigin() + ray.getDirection() * hit.getT());
        } else if (_texture.isValid() && hit.hasTex()) {
            const Vector2f &texCoord = hit.getTexCoord();
            kd = _texture.getTexel(texCoord[0], texCoord[1]);
        } else {
            kd = _diffuseColor;
        }
        Vector3f n = hit.getNormal().normalized();
        Vector3f color = clampedDot(dirToLight, n) * pointwiseDot(lightColor, kd);

        return color;
    }

    void loadTexture(const std::string &filename)
    {
		std::cout << filename;
        _texture.load(filename);
    }

	void setExplosion(){
		_isExplosion = true;
		_channelVector = Vector3f(1,0,0);
	}

	void loadDisplacement(const std::string &filename){
		_displacementMap.load(filename);
		_hasDisplacement = true;
	}

	void preRender(float t){
		if(_isExplosion){
			float r = sin(t * 2 * 3.14) * 0.5f + 0.25f ;
			float g = sin((t + 0.333f) * 2 * 3.14) * 0.5f + 0.25f ;
			float b = sin((t + 0.667f) * 2 * 3.14) * 0.5f + 0.25f ;
			float c = 1;// / (r+g+b);
			_channelVector = Vector3f(r * c, g*c, b*c);
			//_channelVector.print();
		}
	}

	void loadRamp(const std::string &filename){
		_ramp = Image::loadPNG(filename);
	}


	float getDisplacement(Vector2f &uv){
		if (!_isExplosion){
			return  _displacementMap.getTexel(uv.x(), uv.y()).x() * 0.1f;
		} else {
			return Vector3f::dot(_displacementMap.getTexel(uv.x(), uv.y()), _channelVector);
		}
	}

	bool hasDisplacement(){
		return _hasDisplacement;
	}

    static Vector3f pointwiseDot(const Vector3f &v1, const Vector3f &v2) {
        return Vector3f(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
    }
	bool _isExplosion;

protected:
    static float clampedDot(const Vector3f &L, const Vector3f &N) {
        float d = Vector3f::dot(L,N);
        return (d > 0) ? d : 0;
    }

    Vector3f _diffuseColor;
    Vector3f _specularColor;
    float _shininess;
    float _refractionIndex;
    Texture _texture;
    Noise _noise;
	Texture _displacementMap;
	Vector3f _channelVector;
	Image *_ramp;
	bool _hasDisplacement;
};

#endif // MATERIAL_H
