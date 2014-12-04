#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableDepthTest(); //zを使うことを宣言する
    
    // カメラの初期位置と方向を指定
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    camdistance = ofGetWidth()/4;
    camdegree = 90;
    camera.setFov(60);
    camera.setPosition(0, 200, camdistance);
    camera.lookAt(ofVec3f(0, 200, 0));
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
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    /* LeapMotion */
    camera.begin();
    // 背景を黒に塗りつぶし
    ofBackground(0, 0, 0);
    
    // フレームを取得
    Frame frame = controller.frame();
    // Handをあるだけ列挙
    for(int i=0; i<frame.hands().count(); i++) {
        Hand hand = frame.hands()[i];
        // Hand内のFingerをあるだけ描画
        for(int j=0; j<hand.fingers().count(); j++) {
            Finger finger = frame.fingers()[j];
            drawFinger(finger);
            if (i == 0 && j == 1) {
                Particle newParticle;
                float vx = ofRandom(-3, 3);
                float vy = ofRandom(-3, 3);
                newParticle.setInitialCondition(finger.tipPosition().x*2 + ofGetWidth()/2, -finger.tipPosition().y*2 + ofGetHeight(), vx, vy);
                particles.push_back(newParticle);
            }
                
        }
        // Handを描画
        drawPalm(hand);
    }
    camera.end();
    
    
    /* particle */
//    ofPushStyle();
//    glDepthFunc(GL_ALWAYS); // draw on top of everything
    
    ofSetColor(255, 255, 255);
    
    //画面左上にメッセージを表示
    string message = "current particle num = " + ofToString(particles.size(),0);
    ofDrawBitmapString(message, 20, 20);
    
    ofNoFill();
    ofBeginShape();
    for (int i = 0; i < particles.size(); i++) {
        particles[i].draw();
        ofCurveVertex(particles[i].pos.x, particles[i].pos.y);
        
        //画面外に出た際に消去する
        if (particles[i].outWindow())
            particles.erase(particles.begin() + i);
    }
    ofEndShape();

}

void ofApp::drawSphere(Vector vector, float radius) {
    // 球体の描画処理
    ofNoFill();
    ofPushMatrix();
    ofPoint point = ofPoint(vector.x, vector.y, vector.z);
    ofTranslate(point);
    ofRotateX(point.x);
    ofRotateY(point.y);
    ofRotateZ(point.z);
    ofSetColor(0xCC,0,0,255);
    ofSphere(radius);
    ofPopMatrix();
}


void ofApp::drawFinger(Finger finger) {
    
    // 指先の点を描画
    ofPoint tip(finger.tipPosition().x, finger.tipPosition().y, finger.tipPosition().z);
    drawPoint(tip);
    
    // 指の付け根の座標を計算
    ofPoint base = ofPoint(tip.x + finger.direction().x * finger.length() * -1,
                           tip.y + finger.direction().y * finger.length() * -1,
                           tip.z + finger.direction().z * finger.length() * -1);
    // 指の付け根を描画
    drawPoint(base);
    
    // 指先から付け根に線を描く
    ofLine(tip.x, tip.y, tip.z, base.x, base.y, base.z);
    
    // 付け根から掌に線を描く
    ofLine(base.x, base.y, base.z,
           finger.hand().palmPosition().x,
           finger.hand().palmPosition().y,
           finger.hand().palmPosition().z);
    
    // 指の箱を描画
    drawFingerBox(finger, tip, base);
}


// 点を描画
void ofApp::drawPoint(ofPoint point) {
    
    ofPushMatrix();
    ofTranslate(point);
    ofNoFill();
    ofSetColor(0xFF, 0xFF, 0xFF, 255);
    ofSphere(3);
    ofPopMatrix();
}


// 指の箱を描画
void ofApp::drawFingerBox(Finger finger, ofPoint tip, ofPoint base) {
    
    // 指の中間の座標
    ofPoint middle = base.middle(tip);
    ofPushMatrix();
    ofTranslate(middle);
    
    // 指の方向に従い回転
    ofQuaternion quat;
    quat.makeRotate(ofPoint(0, -1, 0), ofPoint(finger.direction().x, finger.direction().y, finger.direction().z));
    ofMatrix4x4 matrix;
    quat.get(matrix);
    glMultMatrixf(matrix.getPtr());
    
    ofNoFill();
    ofSetColor(0xCC,0,0,255);
    ofScale(1, finger.length()/10, 1);
    ofBox(10);
    ofPopMatrix();
}

void ofApp::drawPalm(Hand hand) {
    // 掌の描画処理
    
    ofPoint point = ofPoint(hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
    drawPoint(point);
    
    ofPushMatrix();
    ofTranslate(point);
    
    // 掌を回転
    ofQuaternion quat;
    quat.makeRotate(ofPoint(0, -1, 0), ofPoint(hand.palmNormal().x, hand.palmNormal().y, hand.palmNormal().z));
    ofMatrix4x4 matrix;
    quat.get(matrix);
    glMultMatrixf(matrix.getPtr());
    
    ofNoFill();
    ofSetColor(0xCC,0x0,0x0,255);
    ofScale(1, 0.25, 1.0);
    ofBox(0, 0, 0, 60);
    
    ofPopMatrix();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    /* Particle */
    if (key == 'c') {
        particles.clear();
    }
    
    if (key == 'f') {
        ofToggleFullscreen();
    }

    
    /* move camera */
    switch (key) {
        case 358: // 右
            camdegree += 5;
            if(camdegree > 360) camdegree = 0;
            break;
        case 356: // 左
            camdegree -= 5;
            if(camdegree < 0) camdegree = 360;
            break;
        case 357: // 上
            camdistance -= 10;
            break;
        case 359: // 下
            camdistance += 10;
            break;
        default:
            break;
    }
    
    float radian = ofDegToRad(camdegree);
    float x = camdistance * cos(radian);
    float z = camdistance * sin(radian);
    
    camera.setPosition(x, camera.getY(), z);
    camera.lookAt(ofVec3f(0, 200, 0));
    
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
