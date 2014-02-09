#include "testApp.h"

void testApp::setup() {
    ofBackground(0,0,0);
	
    
    w = 320;
    h = 240;
    
    movie.initGrabber(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered[0].allocate(w, h);
	filtered[1].allocate(w, h);
	filtered[2].allocate(w, h);
	filtered[3].allocate(w, h);
}

//--------------------------------------------------------------
void testApp::update(){
    
    movie.update();
    
    if (movie.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            //filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
			filtered[0].getPixels()[i] = ofInRange(hue.getPixels()[i],Hue1-5,Hue1+5) ? 255 : 0;
			filtered[1].getPixels()[i] = ofInRange(hue.getPixels()[i],Hue2-5,Hue2+5) ? 255 : 0;
			filtered[2].getPixels()[i] = ofInRange(hue.getPixels()[i],Hue3-5,Hue3+5) ? 255 : 0;
			filtered[3].getPixels()[i] = ofInRange(hue.getPixels()[i],Hue4-5,Hue4+5) ? 255 : 0;
        }
        filtered[0].flagImageChanged();
		filtered[1].flagImageChanged();
		filtered[2].flagImageChanged();
		filtered[3].flagImageChanged();


        //run the contour finder on the filtered image to find blobs with a certain hue
        contours[0].findContours(filtered[0], 50, w*h/2, 1, false);
		contours[1].findContours(filtered[1], 50, w*h/2, 1, false);
		contours[2].findContours(filtered[2], 50, w*h/2, 1, false);
		contours[3].findContours(filtered[3], 50, w*h/2, 1, false);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);
	
    //draw all cv images
    //hsb.draw(640,0);
    //hue.draw(0,240);
    //sat.draw(320,240);
    //bri.draw(640,240;

	//draw images 
	rgb.draw(0,0);
    filtered[0].draw(320,0);
	filtered[1].draw(0,240);
	filtered[2].draw(320,240);
	filtered[3].draw(640,240);

	for(int i=0;i<4;i++)
		contours[i].draw(0,480);
    
    ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs
	for(int j=0;j<4;j++)
    for (int i=0; i<contours[j].nBlobs; i++) {
        ofCircle(contours[j].blobs[i].centroid.x, contours[j].blobs[i].centroid.y, 10);
	}    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    static int clickCount=0;
    //calculate local mouse x,y in image
    int mx = x % w;
    int my = y % h;
    
    //get hue value on mouse position
	switch(clickCount)
	{
	case 0: Hue1 = hue.getPixels()[my*w+mx];
					clickCount++;
					break;
	case 1: Hue2 = hue.getPixels()[my*w+mx];
					clickCount++;
					break; 
	case 2: Hue3 = hue.getPixels()[my*w+mx];
					clickCount++;
					break; 
	case 3: Hue4 = hue.getPixels()[my*w+mx];
					clickCount++;
					break; 
	}
	clickCount%=4;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}