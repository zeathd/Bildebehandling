#include "Pixel.h"
//Konstruktør:
Pixel::Pixel(int _r, int _g, int _b) {
	r = _r; g = _g; b = _b;
};
//Funksjon for å endre fargene til en piksel:
void Pixel::edit(int _r, int _g, int _b) {
	r = _r;
	g = _g;
	b = _b;
};
//Funksjoner for å lese ut R,G og B verdiene:
int Pixel::getR() {
	return r;
};
int Pixel::getG() {
	return g;
};
int Pixel::getB() {
	return b;
};
//Funksjon som endrer verdien til en av fargekomponentene:
void Pixel::changeComponent(char component, unsigned char newValue) {
	switch (component) {
	case 'r':
		r = newValue;
		break;
	case 'R':
		r = newValue;
		break;
	case 'g':
		g = newValue;
		break;
	case 'G':
		r = newValue;
		break;
	case 'b':
		b = newValue;
		break;
	case 'B':
		r = newValue;
		break;
	default:
		break;
	}
};
//Returnerer en av fargekomponentene:
int Pixel::getComponent(char component) {
	switch (component) {
	case 'r':
		return r;
	case 'R':
		return r;
	case 'g':
		return g;
	case 'G':
		return g;
	case 'b':
		return b;
	case 'B':
		return b;
	default:
		break;
	}
};