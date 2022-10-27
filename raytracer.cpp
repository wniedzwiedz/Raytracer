#include "objfile.h"

#define AMBIENT_AMOUNT 0.1  //iloœæ œwiat³a w cieniu
#define OBJ_LIGHT 1        //przez tê liczbê jest mno¿ony kolor obiektów

Vec3 black(0,0,0);
Vec3 red(255,0,0);
Vec3 white(255,255,255);

Vec3 getNormal(const triangle& p){
	Vec3 a=cross(p.x-p.y,p.y-p.z);
	return (black-a).normalize();
}


struct thing:public Vec3 {
	Vec3* color;
};

struct object:public thing {
	obj* shape;
	object(char* name,int scale=1, int xx=0, int yy=0, int zz=0, double r=0, double g=0, double b=0){
		shape=new obj(name,scale,xx,yy,zz);
		x=xx;
		y=yy;
		z=zz;
		color=new Vec3(r,g,b);
	}
	
	bool backFacing(Vec3 rayVector, 
                           int num);
	
	double intersectTriangle(Vec3 rayOrigin, 
                           Vec3 rayVector, 
                           int q);
	
	double intersect(const Vec3 &rayorig, const Vec3 &raydir);
};

// **************************************************** kule ********************************************************

struct sphere:public thing{
	double rad;
	Vec3 getNormal(const Vec3& p) const { return (p - *this) / rad; }
	sphere(double xx=0,double yy=0,double zz=0, double ra=1, double r=0, double g=0, double b=0){
		x=xx;
		y=yy;
		z=zz;
		rad=ra;
		color=new Vec3(r,g,b);
		
	}
	
    double intersect(const Vec3 &rayorig, const Vec3 &raydir) const 
    { 
	    
	    
	    const Vec3 o = rayorig;
	    const Vec3 d = raydir;
	    double t = 0;
	    
	    double a1=(d.x-o.x)*(d.x-o.x)+(d.y-o.y)*(d.y-o.y)+(d.z-o.z)*(d.z-o.z);
	    double b1=2*((d.x-o.x)*(o.x-x)+(d.y-o.y)*(o.y-y)+(d.z-o.z)*(o.z-z));
	    double c1=(o.x-x)*(o.x-x)+(o.y-y)*(o.y-y)+(o.z-z)*(o.z-z)-rad*rad;
	    
	    double delta=b1*b1-4*a1*c1;
	    
	    if(delta<0.001){
		return false;
	}
		
	else{
		double t0=(-b1+sqrt(delta))/(2*a1);
		double t1=(-b1-sqrt(delta))/(2*a1);
		
		if (t1<=0.001&&t0<0.001){return false;
		}
		
		else if(t1<=0.001){
			t=t0;
		}
		
		else if(t0<=0.001){
			t=t1;
		}
		
		else{
			t = (t0 < t1) ? t0 : t1;
		}
		t=t*(d-o).len();

		return t;
		
	}
	
	
	
    } 
	
};

// ***************************************************** OBJ **************************************************

bool object::backFacing(Vec3 rayVector, 
                           int num){
	         	
	triangle intriangle=shape->triangles[num];
	Vec3 vertex0 = *this+intriangle.x;
    Vec3 vertex1 = *this+intriangle.y;  
    Vec3 vertex2 = *this+intriangle.z;
    Vec3 edge1, edge2;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    
    Vec3 N=cross(edge1,edge2);
	N=N.normalize();    
    if (dot(rayVector, N) > 0){
    	return 0;
	}
}


double object::intersectTriangle(Vec3 rayOrigin, 
                           Vec3 rayVector, 
                           int num)
{
	triangle intriangle=shape->triangles[num];
    const float EPSILON = 0.0000001; 
    Vec3 vertex0 = *this+intriangle.x;
    Vec3 vertex1 = *this+intriangle.y;  
    Vec3 vertex2 = *this+intriangle.z;
    Vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    
    
    h = cross(rayVector,edge2);
    a = dot(edge1,h);
    
    if (a > -EPSILON && a < EPSILON)
        return 0;
    f = 1/a;
    s = rayOrigin - vertex0;
    u = f * (dot(s,h));
    if (u < 0.0 || u > 1.0)
        return 0;
    q = cross(s,edge1);
    v = f * dot(rayVector,q);
    if (v < 0.0 || u + v > 1.0)
        return 0;
    float t = f * dot(edge2,q);
    if (t > EPSILON)
    {
        return t;
    }
    else
        return 0;
}



double object::intersect(const Vec3 &rayorig, const Vec3 &raydir) {

double t,dist,num;
dist=10000;
num=-1;
int tri=shape->triangles.size();
for (int q=0;q<tri;q++){
	t=intersectTriangle(rayorig, raydir, q);
	if (t>0&&t<dist){
		dist=t;
		num=q;
	}
}
return num;

}



// **************************************************************************************************************
	
class light:public Vec3 {

public:
	double intensity,reach;
	Vec3 *color;
	light(double xx=0,double yy=0,double zz=0, double i=0, double rea=1000, double r=0, double g=0, double b=0){
		x=xx;
		y=yy;
		z=zz;
		
		intensity=i;
		reach=rea;
		color=new Vec3(r,g,b);
	}
};
    
class camera:public Vec3 {
public:
	Vec3 *dir;
    Vec3 *up;
    double upAngle, sideAngle;
	camera(double xx=0,double yy=0,double zz=0,double aa=0,double bb=0,double cc=0,double q=100,double w=100,double e=100,double u=45, double s=45){
		x=xx;y=yy;z=zz;
		dir=new Vec3(aa,bb,cc);
		up=new Vec3(q,w,e);
		upAngle=u;
		sideAngle=s;
		
	}
	double getA(){
		return dir->x;
	}
	double getB(){
		return dir->y;
	}
	double getC(){
		return dir->z;
	}
	
    
};

// ************************************** odczytywanie argumentów ***********************************************
double doubleArg(std::string token){
	char *xx = new char[token.length()];
				strcpy(xx, token.c_str());
				double x=atof(xx);
	return x;
	
}

char* charArg(std::string token){
	char *xx = new char[token.length()];
				strcpy(xx, token.c_str());
	return xx;
	
}

void colorCheck(Vec3& col) {
  col.x = (col.x > 255) ? 255 : (col.x < 0) ? 0 : col.x;
  col.y = (col.y > 255) ? 255 : (col.y < 0) ? 0 : col.y;
  col.z = (col.z > 255) ? 255 : (col.z < 0) ? 0 : col.z;
}

class scene{
	public:
	camera cam;
	std::vector<sphere*> spheres;
	std::vector<light*> lights;
	std::vector<object*> objects;
	JiMP2::BMP *bmp;
	scene(char* name);
	void render(int W, int H);
	camera getCam(){
		return cam;
	}
};

scene::scene(char* name){
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
		std::cout<<"executing."<<std::endl;
		
	
		for(int i=0;i<tokens.size();){
				
			if (tokens[i]=="s") {
				std::cout<<"Adding sphere..."<<spheres.size()<<std::endl;
				i++;
				double x=doubleArg(tokens[i++]);
				
				double y=doubleArg(tokens[i++]);
				
				double z=doubleArg(tokens[i++]);
				
				double ra=doubleArg(tokens[i++]);
				
				double r=doubleArg(tokens[i++]);
				
				double g=doubleArg(tokens[i++]);
				
				double b=doubleArg(tokens[i++]);

				sphere *q=new sphere(x,y,z,ra,r,g,b); 
				spheres.push_back(q);
				std::cout<<"Added sphere at "<<x<<" "<<y<<" "<<z<<" radius = "<<ra<<std::endl;
				command="";
			}
			
			else if (tokens[i]=="l") {
				std::cout<<"Adding light..."<<lights.size()<<std::endl;
				i++;
				double x=doubleArg(tokens[i++]);
				
				double y=doubleArg(tokens[i++]);
				
				double z=doubleArg(tokens[i++]);
				
				double in=doubleArg(tokens[i++]);
				
				double reach=doubleArg(tokens[i++]);
				
				double r=doubleArg(tokens[i++]);
				
				double g=doubleArg(tokens[i++]);
				
				double b=doubleArg(tokens[i++]);

				light *q=new light(x,y,z,in,reach,r,g,b); 
				lights.push_back(q);
				std::cout<<"Added light at "<<x<<" "<<y<<" "<<z<<" intensity = "<<in<<std::endl;
				command="";
			}
			
			else if (tokens[i]=="c") {
				std::cout<<"Adding camera..."<<std::endl;
				i++;
				double x=doubleArg(tokens[i++]);
				
				double y=doubleArg(tokens[i++]);
				
				double z=doubleArg(tokens[i++]);
				
				double a=doubleArg(tokens[i++]);
				
				double b=doubleArg(tokens[i++]);
				
				double c=doubleArg(tokens[i++]);
				
				double q=doubleArg(tokens[i++]);
				
				double w=doubleArg(tokens[i++]);
				
				double e=doubleArg(tokens[i++]);
				
				double n=doubleArg(tokens[i++]);
				
				double m=doubleArg(tokens[i++]);

				camera *aa=new camera(x,y,z,a,b,c,q,w,e,n,m); 
				cam=*aa;
				std::cout<<"Added camera at "<<x<<" "<<y<<" "<<z<<std::endl;
				std::cout<<"Direction "<<a<<" "<<b<<" "<<c<<std::endl;
				std::cout<<"Up "<<q<<" "<<w<<" "<<e<<std::endl;
				std::cout<<"Angles "<<n<<" "<<m<<std::endl;
				command="";
			}
			
			else if (tokens[i]=="o") {
				std::cout<<"Adding object..."<<spheres.size()<<std::endl;
				i++;
				
				char* name= charArg(tokens[i++]);
				
				double s=doubleArg(tokens[i++]);
				
				double x=doubleArg(tokens[i++]);
				
				double y=doubleArg(tokens[i++]);
				
				double z=doubleArg(tokens[i++]);
				
				double r=doubleArg(tokens[i++]);
				
				double g=doubleArg(tokens[i++]);
				
				double b=doubleArg(tokens[i++]);

				object *q=new object(name,s,x,y,z,r,g,b); 
				objects.push_back(q);
				std::cout<<"Added object "<<name<<" at "<<x<<" "<<y<<" "<<z<<" scale = "<<s<<" with faces: "<< q->shape->triangles.size()<<std::endl;
				command="";
			}
			
			
			else{
				i++;
			}
		}
		std::cout<<"file read."<<std::endl;
	}

// ******************************************************* tworzenie sceny *************************************************************

void scene::render(int W, int H){
bmp=new BMP(W,H);


Vec3 point=getCam();
Vec3 dir=cam.dir->normalize();
Vec3 up=cam.up->normalize();
Vec3 side=cross(up,dir).normalize();
Vec3 upNew(0,0,0);
double Xres=cam.upAngle;
double Yres=cam.sideAngle;
std::cout<<"Up vector "<<up.x<<" "<<up.y<<" "<<up.z<<" "<<Yres<<" "<<std::endl;
std::cout<<"side vector "<<side.x<<" "<<side.y<<" "<<side.z<<" "<<Xres<<" "<<std::endl;
while(upNew.len()<H/2){
	point=point+dir;
	upNew=upNew+(up*(Yres/45));	
}
std::cout<<"point "<<point.x<<" "<<point.y<<" "<<point.z<<std::endl;
std::cout<<up.x<<" "<<up.y<<" "<<up.z<<std::endl;
 


int res=sqrt(W*W+H*H);

int sph=spheres.size();
int ob=objects.size();

std::cout<<"rendering "<<sph<<" spheres and "<<ob<<" objects"<<std::endl;
int dist,num,flag;
Vec3 pix_col,ambient;
const Vec3 p(point.x,point.y,point.z);
Vec3 where=point+(up*H/2)+(side*W/2);
const Vec3 start=getCam();
double t;
bool isShadow;
Vec3 temp1,temp2,normal;
int tri;

for (int x=0;x<H;++x){
	
	for (int y=0;y<W;++y){
		
		flag=0;
		dist=1000;
		t=1000;
		
      for (int q=0;q<sph;q++){
      	t=spheres[q]->intersect(cam,where);
      	
        if (t>0) { 
        	flag=1;
            if (t <= dist) { 
                dist = t; 
                flag=1;
	  			pix_col = *(spheres[q]->color);
	  			temp2=black;
	  			ambient=pix_col*AMBIENT_AMOUNT;
	  			Vec3 pi,L,N;
	  			double dt;
	  			
	  			
	  			pi = cam +( (where-cam).normalize() )*t;
	  			
	  			
	  			for(int a=0;a<lights.size();a++){
			        
			        L = *lights[a] - pi;
			    	N = spheres[q]->getNormal(pi);
			        dt = dot(L.normalize(), N.normalize());
					
		        	
		        	
		        	isShadow=false;

						for (int q=0;q<sph;q++){
			      			t=spheres[q]->intersect(pi,*lights[a]);
			      			if (t>0&&t<L.len()){
			      				isShadow=true;
			      				break;
			      				
							  }
			      		}
			      		
			      		for (int q=0;q<ob;q++){
			      			t=objects[q]->intersect(pi,L.normalize());
			      			if (t>=0&&objects[q]->intersectTriangle(pi,*lights[a],t)<L.len()){
			      				isShadow=true;
			      				break;
			      				
							  }
			      		}
			      		
			      		if(isShadow==false){
						 
			      			temp1 = pix_col+(*(lights[a]->color))*(lights[a]->intensity/100);
		        			temp2= temp2 + (temp1*std::max(dt,0.0))/lights.size()*((lights[a]->reach-dist)/lights[a]->reach);
			      	 	} 
		        }
		        pix_col=(temp2)+ambient;
		        temp2=black; 
		        colorCheck(pix_col);
            } 
        }
      } //koniec szukania kulek
      
      
        for (int q=0;q<ob;q++){
      	tri=objects[q]->intersect(cam,where);
      	if (tri>=0){
		  	flag=1;
      		t=objects[q]->intersectTriangle(cam,where,tri);
      		
      		if(t<dist&&t>0){
      			
      			dist=t;
      			pix_col = *(objects[q]->color);
      			temp2=black;
	  			ambient=pix_col*AMBIENT_AMOUNT;
	  			Vec3 pi,L,N;
				double dt;
      			normal=getNormal(objects[q]->shape->triangles[tri]);
      			
      			if(objects[q]->backFacing(where-cam,q)==0){
		        
					pi = cam +( (where-cam).normalize() )*dist;
      			
	      			for(int a=0;a<lights.size();a++){
				        
				        L = *lights[a] - pi;
				        dt = dot(L.normalize(), normal);
				        
				        
				        isShadow=false;
	
							for (int q=0;q<sph;q++){
				      			t=spheres[q]->intersect(pi,*lights[a]);
				      			if (t>0&&t<L.len()){
				      				isShadow=true;
	
				      				break;
				      				
								  }
							}
								  
							for (int q=0;q<ob;q++){
				      			t=objects[q]->intersect(pi,L.normalize());
				      			if (t>=0&&objects[q]->intersectTriangle(pi,*lights[a],t)<L.len()){
				      				isShadow=true;
				      				break;
				      				
								  }
				      		}
								  
								if(isShadow==false){
				      			temp1 = pix_col+(*(lights[a]->color))*(lights[a]->intensity/100);
			        			temp2= temp2 +((temp1*std::max(-dt,0.0))/lights.size())*((lights[a]->reach-dist)/lights[a]->reach)*OBJ_LIGHT;
	
				      	 	}
				      		}
			    }}
			    
			pix_col=(temp2)+ambient;
		    temp2=black;
		    
		        
		    colorCheck(pix_col);
			    
			    
			  
		}
  }
      
		
		if(flag==1){
			
			bmp->setPixel(y,x,pix_col.x,pix_col.y,pix_col.z);
	}
		else {
			bmp->setPixel(y,x,0,0,0);  //t³o
		}
	where=where-side;
}
where=p+(up*((H/2)-x-1))+(side*W/2);
}


}


