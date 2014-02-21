#include <unistd.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>

using namespace std;

struct rsquare {
	float x, y;
	float s;
};

#define SSIZE 5

int main(int, char**)
{
	Display *d = XOpenDisplay((char *) NULL);

	const struct rsquare positions[] = {
		{0.1, 0.1, SSIZE},
		{0.9, 0.1, SSIZE},
		{0.1, 0.9, SSIZE},
		{0.9, 0.9, SSIZE},

		{0.3, 0.3, SSIZE},
		{0.7, 0.3, SSIZE},
		{0.3, 0.7, SSIZE},
		{0.7, 0.7, SSIZE},

		{0.4, 0.4, SSIZE},
		{0.6, 0.4, SSIZE},
		{0.4, 0.6, SSIZE},
		{0.6, 0.6, SSIZE},

		{0.5, 0.5, SSIZE}
	};

	if (d == NULL) {
		return 1;
	}

	const int nPositions = sizeof(positions) / sizeof(struct rsquare);
	XImage *image;

	while (1) {
		int rSum = 0, gSum = 0, bSum = 0, nSamples = 0;
		int screen = DefaultScreen (d);
		int width = DisplayWidth(d, screen);
		int height = DisplayHeight(d, screen);

		int colormap = DefaultColormap(d, screen);
		int rootWindow = RootWindow (d, screen);

		for (int i=0; i < nPositions; i++) {
			int x = width * positions[i].x;
			int y = height * positions[i].y;
			int s = positions[i].s;
			int sx = (s+x >= width) ? (width-x) : s;
			int sy = (s+y >= height) ? (height-y) : s;

			XColor c;

			image = XGetImage (d, rootWindow, x, y, sx, sy, AllPlanes, XYPixmap);

			for (int xi=0; xi < sx; xi++) {
				for (int yi=0; yi < sy; yi++) {
					c.pixel = XGetPixel (image, xi, yi);
					//cout << c.red/256 << " " << c.green/256 << " " << c.blue/256 << "\n";

					rSum += ((c.pixel >> 16) & 0xff);
					gSum += ((c.pixel >> 8) & 0xff);
					bSum += ((c.pixel) & 0xff);
					nSamples++;
				}
			}

			XFree (image);
		}

		cout  << rSum / nSamples << " " << gSum / nSamples << " " << bSum / nSamples << endl;

		// 1/60 second
		usleep(16666);
	}

	return 0;
}
