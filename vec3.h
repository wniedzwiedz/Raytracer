#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


struct Vec3 {
  double x,y,z;
  Vec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
  Vec3 operator + (const Vec3& v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
  Vec3 operator - (const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
  Vec3 operator * (const Vec3& v) const { return Vec3(x*v.x, y*v.y, z*v.z); }
  Vec3 operator * (double d) const { return Vec3(x*d, y*d, z*d); }
  Vec3 operator / (double d) const { return Vec3(x/d, y/d, z/d); }
  double len()const{return sqrt(x*x+y*y+z*z);}
  double dist(const Vec3& v)const{
  	return Vec3(x-v.x,y-v.y,z-v.z).len();
	  }
	double sum()const{return (x+y+z);}
  Vec3 normalize() const {
  	//std::cout<<"Normalizing... "<<x<<" "<<y<<" "<<z<<std::endl;
    double mg = len();
    //std::cout<<"len "<<mg<<std::endl;
    return Vec3(x/mg,y/mg,z/mg);
  }

};

inline double dot(const Vec3& a, const Vec3& b) {
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}
inline Vec3 cross(const Vec3 &vec1, const Vec3 &vec2) {
	return Vec3(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}


