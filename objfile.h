#include <iostream>
#include <fstream>
#include "bmp.h"
#include "bmpfile.h"
#include <vector>
#include <sstream>
#include "vec3.h"
using namespace JiMP2;

class triangle {
public:
	Vec3 x,y,z;
    triangle(Vec3 xx = *new Vec3(), Vec3 yy = *new Vec3(), Vec3 zz = *new Vec3()) : x(xx), y(yy), z(zz) {}
    
};


struct Ray {
  Vec3 o,d;
  Ray(const Vec3& o, const Vec3& d) : o(o), d(d) {}
};
	

class obj {
public:
  	std::vector<Vec3> points;
	std::vector<triangle> triangles;
    obj();
   ~obj();
   void addPoint(Vec3 a);
   void addTriangle(triangle a);
   void addPoint(double xx, double yy, double zz);
   void addTriangle(Vec3 xx, Vec3 yy,Vec3 zz);
   void addTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);
   void addTriangle(int x,int y, int z);
   void draw(BMP *bmp);
   void file(char* name);
   Vec3 getPoint(int x);
   
   obj(char* name,int scale, int moveX, int moveY, int moveZ);
    
};

	obj::obj(){
	}

	void obj::addPoint(Vec3 a){
		points.push_back(a);
	}
	
	Vec3 obj::getPoint(int x){
		return points[x];
	}
	
	void obj::addTriangle(triangle a){
		triangles.push_back(a);
	}
	
	void obj::addPoint(double xx, double yy, double zz){
		Vec3 *a=new Vec3(xx,yy,zz);
		points.push_back(*a);
	}
	
	void obj::addTriangle(Vec3 xx, Vec3 yy,Vec3 zz){
		triangle *a=new triangle(xx,yy,zz);
		triangles.push_back(*a);
	}
	
	void obj::addTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3){
		Vec3 *a=new Vec3(x1,y1,z1);
		Vec3 *b=new Vec3(x2,y2,z2);
		Vec3 *c=new Vec3(x3,y3,z3);
		triangle *d=new triangle(*a,*b,*c);
		triangles.push_back(*d);
	}
	
	void obj::addTriangle(int x,int y, int z){
		if (x<=(int)points.size()&&y<=(int)points.size()&&z<=(int)points.size()){
		
		Vec3 a=getPoint(x);
		Vec3 b=getPoint(y);
		Vec3 c=getPoint(z);

		addTriangle(a,b,c);}
		else{
			std::cout<<"No such Vec3!"<<std::endl;
		}
	}
	
	
	void obj::file(char* name){
		std::ofstream openfile((name,".obj"), std::ofstream::binary);
		openfile << this;
	}
	
	
	double readParas(string command,int a){
		int i=command.length()-1;
		int flag=0;
		while (flag<a){
			if (command[i]==' '){
				flag++;
			}
			i--;
		}
		double res=0.0;
		int power = 0;
		int num=0;
			while (command[i]!=' '){
				num=command[i]-48;
				if(0<=num&&num<=9){
				res+=(command[i]-48)*pow(10,power);	
				power++;
				i--;}
				if (command[i] == '.') {
					res /= pow(10,power);
					power = 0;
					i--;
				}
				if (command[i] == '-') {
					return -res;
				}
		}

		return res;
		
	}
	
	obj::obj(char* name,int scale=1, int moveX=0, int moveY=0, int moveZ=0){
		ifstream infile;
  		infile.open (name);
  		string command;
  		std::vector<string> tokens;
  		
  		while(getline(infile, command)){   // get a whole line
       std::istringstream tokenStream(command);
       std::string token;
   while (std::getline(tokenStream, token, ' '))
   {
      tokens.push_back(token);
   }}
		std::cout<<"read."<<std::endl;
		infile.close();
		
		char *a;
		for(int i=0;i<tokens.size();i++){
				
				strcpy(a, tokens[i].c_str());
				
				std::basic_string<char> vertex = "v";
				
			if ((tokens[i]==vertex)) {
				
				char *xx = new char[tokens[i+1].length()];
				strcpy(xx, tokens[i+1].c_str());
				double x=atof(xx)+moveX;
				x*=scale;
				
				
				char *yy = new char[tokens[i+2].length()];
				strcpy(yy, tokens[i+2].c_str());
				double y=moveY+atof(yy);
				y*=scale;
				
				char *zz = new char[tokens[i+3].length()];
				strcpy(zz, tokens[i+3].c_str());
				double z=moveZ+atof(zz);
				z*=scale;
				
				
				Vec3 *a=new Vec3(x,y,z); 
				points.push_back(*a);
				command="";
			//	std::cout<<x<<"**"<<y<<"**"<<z<<std::endl;
			}
			
			
			
			else if (strcmp(a,"f")==0) {
				
				char *xx = new char[tokens[i+1].length() + 1];
				strcpy(xx, tokens[i+1].c_str());
				int x=::atoi(xx);
				
				char *yy = new char[tokens[i+2].length() + 1];
				strcpy(yy, tokens[i+2].c_str());
				int y=::atoi(yy);
				
				char *zz = new char[tokens[i+3].length() + 1];
				strcpy(zz, tokens[i+3].c_str());
				int z=::atoi(zz);
				

				addTriangle(x-1,y-1,z-1);
				

				command="";			
				}
		}
				
				
			std::cout<<"done."<<std::endl;
        
        
        
	}

