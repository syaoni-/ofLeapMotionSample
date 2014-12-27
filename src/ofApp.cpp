#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableDepthTest(); //zを使うことを宣言する
    
    // カメラの初期位置と方向を指定
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    leapMotion.setup();
    
    //----------------------------------------------Cloth Particle
    clotheffect.setup();
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    /* Particle */
    for (int i = 0; i < particles.size(); i++) {
        particles[i].resetForce();
        particles[i].addForce(0, 0.1);
        particles[i].addDampingForce();
        particles[i].update();
        
        //画面外に出た際に消去する
        //        if (particles[i].outWindow())
        //            particles.erase(particles.begin() + i);
    }
    
    //----------------------------------------------Cloth Particle
    clotheffect.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    leapMotion.draw();
    
    //-----------------------------------------------------Sample Particle
    ofSetColor(255, 255, 255);
    
    ofNoFill();
    ofBeginShape();
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
        ofCurveVertex(particles[i].pos.x, particles[i].pos.y);
        
        //ある程度画面外に出た際にそのParticleを消去する
        if (particles[i].outWindow())
            particles.erase(particles.begin() + i);
    }
    ofEndShape();
    
    //-----------------------------------------------------Cloth Particle
    clotheffect.draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    Particle newParticle;
    float vx = ofRandom(-3, 3);
    float vy = ofRandom(-3, 3);
    newParticle.setInitialCondition(x, y, vx, vy);
    particles.push_back(newParticle);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
