#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

#include "Bilde.h"


#pragma region "konstruktør/dekonstruktør"
Bilde::Bilde()
{
}


Bilde::~Bilde()
{
	delete[] header;
}
#pragma endregion

#pragma region "hovedfunksjoner"
bool Bilde::lesBitmapFil(string filename)
{
	fstream infile;
	infile.open(filename.c_str(), ios::binary | ios::in);
	if (infile.fail())
	{
		cout << "Feil ved aapning av fil\n";
		return false;
	}
	else
	{
		infile.seekg(10, ios::beg);
		infile.read((char*)&this->HEADERSIZE, 4);
		infile.seekg(10, ios::beg);
		infile.read((char*)&this->offset, 4);

		this->header = new unsigned char[this->HEADERSIZE];

		infile.seekg(0, ios::beg);
		infile.read((char*)this->header, this->HEADERSIZE);

		infile.seekg(18, ios::beg);
		infile.read((char*)&this->width, 4);
		infile.read((char*)&this->heigth, 4);

		infile.seekg(offset, ios::beg);
		this->stuffing = width % 4;

		for (unsigned int i = 0; i < this->heigth; i++)
		{
			vector<Pixel> rad;

			for (unsigned int j = 0; j < this->width; j++)
			{
				infile.read((char*)this->enPixel, 3);

				Pixel nyPixel((int)this->enPixel[2], (int)this->enPixel[1], (int)this->enPixel[0]);
				rad.push_back(nyPixel);
			}
			int p = infile.tellp();

			infile.seekg(p + stuffing, ios::beg);
			this->pixelData.push_back(rad);
		}
		infile.close();

		return true;
	}
}

bool Bilde::lagreBitmapFil(string filename)
{
	fstream outfile;
	outfile.open(filename.c_str(), ios::binary | ios::out);

	if (outfile.fail())
	{
		cout << "Feilet aa aapne utfil\n";

		return false;
	}
	else
	{
		unsigned char utdata[3];
		unsigned char stuffBytes[3] = { 0, 0, 0 };

		outfile.write((char*)this->header, this->HEADERSIZE);

		outfile.seekp(offset, ios::beg);

		for (int i = 0; i < this->heigth; i++)
		{
			vector<Pixel> rad = this->pixelData[i];
			for (int j = 0; j < this->width; j++)
			{
				Pixel *p = &(rad.at(j));
				utdata[0] = p->getB();
				utdata[1] = p->getG();
				utdata[2] = p->getR();
				outfile.write((char*)utdata, 3);
			}
			outfile.write((char*)stuffBytes, this->stuffing);
		}
		outfile.close();

		return true;
	}

}

bool Bilde::setPixel(int x, int y, Pixel &ny_pixel)
{
	this->pixelData[x][y] = ny_pixel;

	return true;
}

void Bilde::greyscale()
{
	int grey = 0;
	//gjennomløper alle rader:
	for (int i = 0; i < this->heigth; i++)
	{
		//henter rad nr i:
		vector<Pixel> *rad = &(this->pixelData[i]);
		//gjennomløper alle pikslene i rad nr i:
		for (int j = 0; j < this->width; j++)
		{
			Pixel *p = &(rad->at(j)); // henter referanse til piksel

			grey = (p->getR() + p->getG() + p->getB()) / 3; // gråfarge basert på snitt av rgb fargen
			//setter grå pikselfarge
			p->edit(grey, grey, grey); //endrer piksel
		}
	}
}

void Bilde::redtogreyscale()
{
	int grey = 0;

	for (int i = 0; i < this->heigth; i++)
	{
		vector<Pixel> *rad = &(this->pixelData[i]);

		for (int j = 0; j < this->width; j++)
		{
			Pixel *p = &(rad->at(j));

			grey = p->getR();
			p->edit(grey, grey, grey);
		}
	}
}

void Bilde::blackandwhite()
{
	int grey = 0;
	//gjennomløper alle rader:
	for (int i = 0; i < this->heigth; i++)
	{
		//henter rad nr i:
		vector<Pixel> *rad = &(this->pixelData[i]);
		//gjennomløper alle pikslene i rad nr i:
		for (int j = 0; j < this->width; j++)
		{
			Pixel *p = &(rad->at(j)); // henter referanse til piksel


			if ((p->getB() + p->getG()) <= p->getR() && p->getR() >= 100)
			{
				grey = 255;
			}
			else
			{
				grey = 0;
			}

			//grey = p->getR(); // gråfarge basert på snitt av rgb fargen
			//setter grå pikselfarge
			p->edit(grey, grey, grey); //endrer piksel
		}
	}
}

void Bilde::invert()
{
	int red = 0, green = 0, blue = 0;

	for (int i = 0; i < this->heigth; i++)
	{
		vector<Pixel> *rad = &(this->pixelData[i]);

		for (int j = 0; j < this->width; j++)
		{
			Pixel *p = &(rad->at(j));

			red = (255 - p->getR());
			green = (255 - p->getG());
			blue = (255 - p->getB());

			p->edit(red, green, blue);
		}
	}
}

void Bilde::rammInnBilde(Pixel &Farge, int rammeBredde)
{
	for (int i = 0; i < rammeBredde; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			this->pixelData[i][j] = Farge;
			this->pixelData[(this->heigth) - i - 1][j] = Farge;
		}
		for (int j = 0; j < this->heigth; j++)
		{
			this->pixelData[j][i] = Farge;
			this->pixelData[j][(this->width) - i - 1] = Farge;
		}
	}
}

int Bilde::antallPiksler(Pixel &Farge)
{
	int R = 0, G = 0, B = 0, pix = 0;
	int RR = Farge.getR(), GG = Farge.getG(), BB = Farge.getB();
	for (int i = 0; i < this->heigth; i++)
	{
		vector<Pixel> *rad = &(this->pixelData[i]);
		for (int j = 0; j < width; j++)
		{
			Pixel *p = &(rad->at(j));
			int R = p->getR();
			int G = p->getG();
			int B = p->getB();

			if (R == RR && G == GG && B == BB)
			{
				pix++;
			}
		}
	}


	return pix;
}

void Bilde::tegnTilfeldigePunkter(int antall, Pixel &Farge)
{
	srand(time(0));
	for (int i = 0; i < antall; i++)
	{
		int x = rand() % (this->heigth - 1);
		int y = rand() % (this->width - 1);

		this->pixelData[x][y] = Farge;
	}
}

void Bilde::flip()
{
	for (int i = 0; i < ((this->heigth) / 2); i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			Pixel midlertidig = pixelData[i][j];
			pixelData[i][j] = pixelData[(this->heigth) - i - 1][j];
			pixelData[(this->heigth) - i - 1][j] = midlertidig;

		}
	}
}

void Bilde::mirror()
{
	for (int i = 0; i < ((this->width) / 2); i++)
	{
		for (int j = 0; j < this->heigth; j++)
		{
			Pixel midlertidig = pixelData[j][i];
			pixelData[j][i] = pixelData[j][(this->width) - i - 1];
			pixelData[j][(this->width) - i - 1] = midlertidig;

		}
	}
}

void Bilde::filter(int AA, int BB, int CC, int DD, int EE, int FF, int GG, int HH, int II, int dele)
{
	/*int AA = 1;
	int BB = 1;
	int CC = 1;
	int DD = 0;
	int EE = 0;
	int FF = 0;
	int GG = -1;
	int HH = -1;
	int II = -1;
	int 
	*/

	int grey = 0;

	for (int i = 0; i < (this->heigth); i++)
	{
		vector<Pixel> rad;

		for (int j = 0; j < this->width; j++)
		{
			if (i == 0 || i == this->heigth - 1 || j == 0 || j == this->width - 1)
			{
				grey = 0;
			}
			else
			{
				Pixel *aaa = &pixelData[i - 1][j - 1];
				Pixel *bbb = &pixelData[i - 1][j];
				Pixel *ccc = &pixelData[i - 1][j + 1];
				Pixel *ddd = &pixelData[i][j - 1];
				Pixel *eee = &pixelData[i][j];
				Pixel *fff = &pixelData[i][j + 1];
				Pixel *ggg = &pixelData[i + 1][j - 1];
				Pixel *hhh = &pixelData[i + 1][j];
				Pixel *iii = &pixelData[i + 1][j + 1];

				grey = (abs((aaa->getR()*AA) + (bbb->getR()*BB) + (ccc->getR()*CC) + (ddd->getR()*DD) + (eee->getR()*EE) + (fff->getR()*FF) + (ggg->getR()*GG) + (hhh->getR()*HH) + (iii->getR()*II))/dele);
			}

			Pixel nyPixel(grey, grey, grey);
			rad.push_back(nyPixel);
		}

		this->mellomdata.push_back(rad);
	}

	this->pixelData = this->mellomdata;
	mellomdata.clear();
}

void Bilde::linefilter()
{
	int AA = 1;
	int BB = 1;
	int CC = 1;
	int DD = 0;
	int EE = 0;
	int FF = 0;
	int GG = -1;
	int HH = -1;
	int II = -1;

	int grey = 0;

	for (int i = 0; i < (this->heigth); i++)
	{
		vector<Pixel> rad;

		for (int j = 0; j < this->width; j++)
		{
			if (i == 0 || i == this->heigth-1 || j == 0 || j==this->width-1)
			{
				grey = 0;
			}
			else
			{
				Pixel *aaa = &pixelData[i - 1][j - 1];
				Pixel *bbb = &pixelData[i - 1][j];
				Pixel *ccc = &pixelData[i - 1][j + 1];
				Pixel *ddd = &pixelData[i][j - 1];
				Pixel *eee = &pixelData[i][j];
				Pixel *fff = &pixelData[i][j + 1];
				Pixel *ggg = &pixelData[i + 1][j - 1];
				Pixel *hhh = &pixelData[i + 1][j];
				Pixel *iii = &pixelData[i + 1][j + 1];

				grey = (abs((aaa->getR()*AA) + (bbb->getR()*BB) + (ccc->getR()*CC) + (ddd->getR()*DD) + (eee->getR()*EE) + (fff->getR()*FF) + (ggg->getR()*GG) + (hhh->getR()*HH) + (iii->getR()*II)));
			}

			Pixel nyPixel(grey, grey, grey);
			rad.push_back(nyPixel);
		}

		this->mellomdata.push_back(rad);
	}

	this->pixelData = this->mellomdata;
	mellomdata.clear();
}

int Bilde::pixeldistance()
{
	int piksler = 0;



	return piksler;
}
#pragma endregion

#pragma region "get funksjoner"
int Bilde::getwidth()
{
	return this->width;
}

int Bilde::getheigth()
{
	return this->heigth;
}

int Bilde::getheadersize()
{
	return this->HEADERSIZE;
}

int Bilde::getoffset()
{
	return this->offset;
}

int Bilde::getstuffing()
{
	return this->stuffing;
}
#pragma endregion

