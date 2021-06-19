#include "lodepng.h"
#include <iostream>
#include <cmath>


void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	unsigned error = lodepng::encode(filename, image, width, height);

	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

const int MAXITERS = 80;

int mandelBrot(float real, float imag) {
	float zreal = 0;
	float zimag = 0;
	float newimag;
	float newreal;
	int n = 0;
	while (n < MAXITERS && std::sqrt(zimag * zimag + zreal * zreal) <= 2) {
		newreal = zreal * zreal - zimag * zimag + real;
		newimag = zreal * zimag * 2 + imag;
		zreal = newreal;
		zimag = newimag;

		n++;
	}
	return n;
}


const int WIDTH = 1920*15;
const int HEIGHT = 1080*15;

const int RE_START = -2;
const int RE_END = 1;
const int IM_START = -1;
const int IM_END = 1;


std::vector<unsigned char> data;

int main(int argc, char* argv) {

	data.resize(WIDTH*HEIGHT*4);

	for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
		float x = (i/4) % WIDTH;
		float y = (i/4) / WIDTH;

		int8_t val = ((float)mandelBrot(RE_START + (x / WIDTH) * (RE_END - RE_START), IM_START + (y / HEIGHT) * (IM_END - IM_START)) / (float)MAXITERS) * 255;

		data[i] = (unsigned char) val;
		data[i + 1] = (unsigned char) val;
		data[i + 2] = (unsigned char) val;
		data[i + 3] = (unsigned char) 255;

		//std::cout << x << " " << y << " " << (int) val << "|" <<std::endl;
	}

	encodeOneStep("test.png", data, WIDTH, HEIGHT);
}