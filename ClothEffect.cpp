//
//  ClothEffect.cpp
//  LeapMotionTest
//
//  Created by 上原伸一 on 2014/12/27.
//
//

#include "ClothEffect.h"

//---------------------------------------------------------------------
void ClothEffect::setup(){
    
//    ofSetFrameRate(60);
//    ofSetVerticalSync(true);
    
    printf("hoge");
    
    //position all particles
    for (int i=0; i<ROWS*COLS; i++) {
        int x = int(i) % COLS * 20;
        int y = int(i) / COLS * 20;
        ClothParticle p(ofVec3f(x,y,0), y==0 ? 0 : 1, 0.96);
        ps.push_back(p);
    }
    
    //create all springs
    for (int i=0; i<ROWS*COLS; i++) {
        int x = int(i) % COLS;
        int y = int(i) / COLS;
        
        //horizontal structural springs
        if (x<COLS-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+1]));
        }
        
        //vertical structural springs
        if (y<ROWS-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+COLS]));
        }
        
        //shear springs left to right
        if (x<COLS-1 && y<ROWS-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+COLS+1]));
        }
        
        //shear springs right to left
        if (y>0 && x<COLS-1 && y<ROWS) {
            sp.push_back(ClothSpring(&ps[i],&ps[i-COLS+1]));
        }
        
        //bending springs horizontal
        if (x<COLS-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2]));
        }
        
        //bending springs vertical
        if (y<ROWS-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2*COLS]));
        }
        
        //bending springs diagonal
        if (y<ROWS-2 && x<COLS-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2+2*COLS]));
        }
    }

}


//---------------------------------------------------------------------
void ClothEffect::update(){
    
    //----------------------------------------------Cloth Particle
    //apply forces
    ofVec3f fingerDir;
    for (int i=0; i<ps.size(); i++) {
        ps[i].addForce(ofVec3f(0,1.1,0)); //Gravity?
        ps[i].addForce(ofVec3f(0,0,sin(ofGetElapsedTimef() * 1.6)*8)); //前後に振る
        
        //指との距離が一定値以下なら力を加える
//        for (int j = 0; j < 5; j++) {
//            fingerDir = ps[i].pos - fingerPos[j];
//            if ( fingerDir.length() < 40)
//                ps[i].addForce( fingerDir.normalize()*1000 );
//        }
    }
    
    //update springs
    for (int i=0; i<sp.size(); i++) {
        sp[i].update();
    }
    
    //update particles
    for (int i=0; i<ps.size(); i++) {
        ps[i].update();
    }

}

//---------------------------------------------------------------------
void ClothEffect::draw(){
    
    //ofTranslate(ofGetWidth()/2-200, 100, -300);
    
    //springs
    ofSetColor(255, 255, 255);
    glBegin(GL_LINES);
    for (int i=0; i<sp.size(); i++) {
        glVertex3f(sp[i].a->pos.x, sp[i].a->pos.y, sp[i].a->pos.z);
        glVertex3f(sp[i].b->pos.x, sp[i].b->pos.y, sp[i].b->pos.z);
    }
    glEnd();
    
    //particles
    ofSetColor(0, 0, 255);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i=0; i<ps.size(); i++) {
        glVertex3f(ps[i].pos.x, ps[i].pos.y, ps[i].pos.z);
    }
    glEnd();

}


//---------------------------------------------------------------------
void ClothEffect::keyPressed(int key){
    
}

//---------------------------------------------------------------------
void ClothEffect::keyReleased(int key){
    
}

//---------------------------------------------------------------------
void ClothEffect::mouseMoved(int x, int y ){
    
}

//---------------------------------------------------------------------
void ClothEffect::mouseDragged(int x, int y, int button){
    
}

//---------------------------------------------------------------------
void ClothEffect::mousePressed(int x, int y, int button){
    
}

//---------------------------------------------------------------------
void ClothEffect::mouseReleased(int x, int y, int button){
    
}

//---------------------------------------------------------------------
void ClothEffect::windowResized(int w, int h){
    
}

//---------------------------------------------------------------------
void ClothEffect::dragEvent(ofDragInfo dragInfo){
    
}

//---------------------------------------------------------------------
void ClothEffect::gotMessage(ofMessage msg){
    
}
