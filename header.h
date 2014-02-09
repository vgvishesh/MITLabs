#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// To get the live video feed

struct Position
{
	float x,y;
	Position *next;
	Position():x(-4),y(-4),next(NULL)
	{}
};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		#ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		movie;
		#endif

		ofxCvColorImage rgb,hsb;
		ofxCvGrayscaleImage hue,sat,bri,filtered[4];    
		ofxCvContourFinder contours[4];
		
    
		int w,h;
		int Hue1,Hue2,Hue3,Hue4;
		bool click;
		Position *start;
		long elementCount;
};
