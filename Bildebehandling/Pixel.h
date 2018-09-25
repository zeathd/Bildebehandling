#pragma once
class Pixel {
public:
	//Konstrukt�r med flere argumenter:
	Pixel(int _r, int _g, int _b);
	//Funksjon for � endre fargene til en piksel:
	void edit(int _r, int _g, int _b);
	//Funksjoner for � lese ut R,G og B verdiene:
	int getR();
	int getG();
	int getB();
	void changeComponent(char component, unsigned char newValue);
	int getComponent(char component);
private:
	//Medlemsvariabler:
	int r;
	int g;
	int b;
};