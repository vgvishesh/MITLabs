#include "testApp.h"
#include<iostream>
using namespace std;

struct pt
{
	double x,y;
}points[20];

//int count=0;

double factorial(double n)
{
	double fac=1;
	if(n==0)
		return 1;
	for(int i=1;i<=n;i++)
	{
		fac*=i;
	}
	return fac;
}

struct pt bz[30];

void testApp::setSelectColorPallete()
{
	int n;
	cout<<"Select the color pallete-";
	cout<<"\n1.Basic\t Red, Yellow, Green, Blue";
	cout<<"\n2.Grayscale\t Black, Gray, Dark Gray, Gainsboro";
	cout<<"\n3.Funk\t Deep Lily, Bluey, Purple, Pale Peach";
	cout<<"\n4.Angry\t Black, Deep Red, Rusty, Angry Red";
	cout<<"\n5.Excited\t Excited Pink, Light Blue, Light Straw, Light Green\n";
	cin>>n;
	switch(n)
	{
	case 1:color[0].set(255,0,0);
			color[1].set(255,255,0);
			color[2].set(0,255,0);
			color[3].set(0,0,255);
			break;
	case 2:color[0].set(0,0,0);
			color[1].set(128,128,128);
			color[2].set(168,168,168);
			color[3].set(219,219,219);
			break;
	case 3:color[0].set(206,39,72);
			color[1].set(51,142,139);
			color[2].set(76,10,52);
			color[3].set(255,218,183);
			break;
	case 4:color[0].set(0,0,0);
			color[1].set(104,0,24);
			color[2].set(165,63,58);
			color[3].set(137,17,13);
			break;
	case 5:color[0].set(226,45,63);
			color[1].set(151,216,186);
			color[2].set(255,235,175);
			color[3].set(183,226,95);
			break;
	}	
}

void bezier(double steps, int controlPoints)	//parameter steps decides the sharpness of bezier ciurve more the value more curved 
{
	struct pt tmp;
	double u=0,mu,mun,k,fn,fk,fnk,n=controlPoints-1,s=0;
	//cout<<"\n::count\t"<<::count;
	while(s<=steps)
	{
		tmp.x=0;
		tmp.y=0;
		//cout<<"\nu\t"<<u;
		for(int i=0;i<=n;i++)
		{
			k=i;
			fn=factorial(n);
			fk=factorial(k);
			fnk=factorial(n-k);
			mu=pow(u,k);
			mun=pow(1-u,n-k);
			//cout<<"\n"<<n<<"\t"<<k<<"\t"<<fn<<"\t"<<fk<<"\t"<<fnk<<"\t"<<mu<<"\t"<<mun;
			//cout<<"\nxy\t"<<points[i].x<<"\t"<<points[i].y;
			tmp.x+=points[i].x*fn*mu*mun/(fk*fnk);
			tmp.y+=points[i].y*fn*mu*mun/(fk*fnk);
		}
		bz[int(s)].x=tmp.x;
		bz[int(s)].y=tmp.y;
		//cout<<"\nb\t"<<bz[int(s)].x<<"\t"<<bz[int(s)].y;
		u+=1/steps;
		s++;
	}
}

void testApp::setup() {
    //ofBackground(0,0,0);
	//ofSetBackgroundColor(255,255,255);
	ofEnableSmoothing();
	ofSetBackgroundAuto(false);
    ofBackground(0,0,0);
	ofEnableAlphaBlending();
    ofSetFrameRate(150);
	clickCount=0;		// to detect if mouse clicked
	//Hue1=164;		// for neon pink color
	//cout<<Hue1;
    
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

	for(int i=0;i<NUM_MARKERS;i++)
	{
		start[i]=new Position();
		elementCount[i]=0;
	}
	
	setSelectColorPallete();
	/*color[1].set(255,0,0);
	color[0].set(255,255,0);
	color[2].set(0,255,0);
	color[3].set(0,0,255);*/

}

//--------------------------------------------------------------
void testApp::update(){
    //static long int elementCount=0;
	static Position *pos[NUM_MARKERS],*last[NUM_MARKERS];
	static bool t=0;

	if(t==0)
	{
		for(int i=0;i<NUM_MARKERS;i++)
			pos[i]=last[i]=start[i];
		t=1;
	}
	
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

		if(clickCount>4)
		{
			for(int i=0;i<NUM_MARKERS;i++)
				pos[i]=last[i];

			for(int j=0;j<NUM_MARKERS;j++)
			for (int i=0; i<contours[j].nBlobs; i++) 
			{
				pos[j]->x=contours[j].blobs[i].centroid.x*4.7;
				pos[j]->y=contours[j].blobs[i].centroid.y*3.2;
				//if(i!=contours[j].nBlobs-1)
				{
					pos[j]->next=new Position();
					pos[j]=pos[j]->next;
					elementCount[j]++;
				}
			}
			//elementCount--;
			for(int i=0;i<NUM_MARKERS;i++)
				last[i]=pos[i];
		}
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	//ofSetColor(255,255,255);
	//int px[4]={512,512,512,512},py[4]={384,384,384,384};
    
    //draw all cv images
    //hsb.draw(640,0);
    //hue.draw(0,240);
    //sat.draw(320,240);
    //bri.draw(640,240;

	//draw images 
	if(clickCount<5)
	{
		rgb.draw(0,0);
		filtered[0].draw(320,0);
		filtered[1].draw(0,240);
		filtered[2].draw(320,240);
		filtered[3].draw(640,240);

		for(int i=0;i<4;i++)
			contours[i].draw(0,480);
		
    
		//ofSetColor(255, 0, 0);
		//ofFill();
	}
	else
	{
    //draw red circles for found blobs
		
		Position *pos;

		ofSetLineWidth(10);
		

		for(int k=0;k<NUM_MARKERS;k++)
		{
			ofSetColor(color[k],30);
			//ofSetColor(rand()%255,rand()%255,rand()%255,50);
			pos=start[k];
			int j=0;
			while(pos!=NULL && pos->x!=-4 && pos->y!=-4 && (elementCount[k]-j*3)>4)
			{
				int z=0;
				while(z<3)
				{				// 4 control points for bezier curves....
					points[z].x=pos->x;
					points[z].y=pos->y;
					pos=pos->next;
					z++;
				}
				/*if(k==1){
					continue;
				}*/
				points[z].x=pos->x;
				points[z].y=pos->y;

				/*points[0].x=pos->x;points[0].y=pos->y;
				pos=pos->next;
				points[1].x=pos->x;points[1].y=pos->y;
				pos=pos->next;
				points[2].x=pos->x;points[2].y=pos->y;*/
				bezier(15,4);

				for(int i=1;i<=15;i++)
				{
					ofCircle(bz[i].x,bz[i].y,20 );
					ofCircle(bz[i].x+30,bz[i].y,20);
					ofCircle(bz[i].x+10,bz[i].y-30,20);
					ofLine(bz[i-1].x,bz[i-1].y,bz[i].x,bz[i].y);
				}
					//ofLine(bz[i-1].x,bz[i-1].y,bz[i].x,bz[i].y);
				j++;
			}
			//pos=start[k];
			//while(pos!=NULL)
			//{
			//	ofSetColor(rand()%255,rand()%255,rand()%255,50);
			//	ofCircle(pos->x,pos->y, 10);
			//	float tx=pos->x,ty=pos->y;
			//	pos=pos->next;
			//	/*if(pos!=NULL)
			//	{
			//		ofSetColor(255,0,0);
			//		if(pos->x!=-4 && pos->y!=-4)
			//			ofLine(tx,ty,pos->x,pos->y);
			//	}*/
			//}
		}
			
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    //calculate local mouse x,y in image
    int mx = x % w;
    int my = y % h;
    
    //get hue value on mouse position
	switch(clickCount)
	{
	case 0: Hue1 = hue.getPixels()[my*w+mx];
					cout<<Hue1;
					clickCount++;
					break;
	case 1: Hue2 = hue.getPixels()[my*w+mx];
					cout<<Hue2;
					clickCount++;
					break; 
	case 2: Hue3 = hue.getPixels()[my*w+mx];
					cout<<Hue3;
					clickCount++;
					break; 
	case 3: Hue4 = hue.getPixels()[my*w+mx];
					cout<<Hue4;
					//ofSetColor(255,255,255);
					//ofRect(0,0,ofGetWidth(),ofGetHeight());
					clickCount++;
					break; 
	case 4: ofSetColor(255,255,255);
			ofRect(0,0,ofGetWidth(),ofGetHeight());
			clickCount++;
			break;
	}
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