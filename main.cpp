#include "raytracer.cpp"


int main() {
	
	scene *qq = new scene("krowy.txt");                  //plik do odczytu
	std::cout <<"rendering..."<< std::endl;
	qq->render(300, 300);                               //rozmiar obrazu
	std::cout <<"rendered"<< std::endl;
	std::ofstream outfile("result.bmp", std::ofstream::binary);    //wynik
		outfile << *(qq->bmp);

	
}

//o cow.txt 5 0 -1 0 255 255 255
