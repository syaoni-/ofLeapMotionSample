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
    
    //----------------------------------------------set image
    
    guu.loadImage("guu.png");
    choki.loadImage("choki.png");
    paa.loadImage("paa.jpeg");
    
    //----------------------------------------------Cloth Particle
    
    int rows = 20;
    int cols = 20;
    
    //position all particles
    for (int i=0; i<rows*cols; i++) {
        int x = int(i) % cols * 20;
        int y = int(i) / cols * 20;
        ClothParticle p(ofVec3f(x,y,0), y==0 ? 0 : 1, 0.96);
        ps.push_back(p);
    }
    
    //create all springs
    for (int i=0; i<rows*cols; i++) {
        int x = int(i) % cols;
        int y = int(i) / cols;
        
        //horizontal structural springs
        if (x<cols-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+1]));
        }
        
        //vertical structural springs
        if (y<rows-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+cols]));
        }
        
        //shear springs left to right
        if (x<cols-1 && y<rows-1) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+cols+1]));
        }
        
        //shear springs right to left
        if (y>0 && x<cols-1 && y<rows) {
            sp.push_back(ClothSpring(&ps[i],&ps[i-cols+1]));
        }
        
        //bending springs horizontal
        if (x<cols-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2]));
        }
        
        //bending springs vertical
        if (y<rows-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2*cols]));
        }
        
        //bending springs diagonal
        if (y<rows-2 && x<cols-2) {
            sp.push_back(ClothSpring(&ps[i],&ps[i+2+2*cols]));
        }
    }

    
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
    //apply forces
    ofVec3f fingerDir;
    for (int i=0; i<ps.size(); i++) {
        ps[i].addForce(ofVec3f(0,1.1,0)); //Gravity?
        //ps[i].addForce(ofVec3f(0,0,sin(ofGetElapsedTimef() * 1.6)*8)); //前後に振る
        
        //指との距離が一定値以下なら力を加える
        for (int j = 0; j < 5; j++) {
            fingerDir = ps[i].pos - fingerPos[j];
            if ( fingerDir.length() < 40)
                ps[i].addForce( fingerDir.normalize()*1000 );
        }
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

//--------------------------------------------------------------
void ofApp::draw(){
    
    /* 毎回加速度をリセット */
    for(int i=0; i<FINGER_NUM; i++)
        fingerAcceleration[i] = 0;
    
    
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
            
            /*Finger acceleration*/
            preFingerPos[(i+1)*j] = fingerPos[(i+1)*j];
            fingerPos[(i+1)*j].x = finger.tipPosition().x*2;
            fingerPos[(i+1)*j].y = -finger.tipPosition().y*2 + ofGetHeight();
            fingerPos[(i+1)*j].z = finger.tipPosition().z + 50;
            ofVec3f difPos = ( fingerPos[(i+1)*j] - preFingerPos[(i+1)*j] );
            fingerAcceleration[(i+1)*j] = difPos.length();
        }
        // Handを描画
        drawPalm(hand);
    }
    camera.end();
    
    
    
    
    //------------------------------------------------------Acceleration
    logTime += ofGetElapsedTimef() - preElapsedTime;
    preElapsedTime = ofGetElapsedTimef();
    if ( logTime > LOG_INTERVAL ) {
        logTime = 0;
    }
    
    
    
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
    
    
    
    
    
    //--------------------------------------------------Debug Log
    //画面左上にメッセージを表示
    
    /* 人差し指の座標 */
    string clothMessage = "current fingerPos[1] x : " + ofToString(fingerPos[1].x) + " y : " + ofToString(fingerPos[1].y) + " z : " + ofToString(fingerPos[1].z);
    ofDrawBitmapString(clothMessage, 20, 40);
    
    /* 人差し指の加速度 */
    string accelMessage = "current finger[1] acceleration = " + ofToString(fingerAcceleration[1],0);
    ofDrawBitmapString(accelMessage, 20, 60);
    
    /* 人差し指の加速度を色で分かりやすく */
    ofSetColor(fingerAcceleration[1], fingerAcceleration[1], 0);
    ofFill();
    ofCircle(50, 100, 30);
    
    /* 手の形状検知 */
    ofSetColor(255, 255, 255);
    int leftHandPatern = LHandPaternDic(fingerPos);
    if( guuDic(fingerPos) ) guu.draw(20, 150, 50, 50); //グー描画
    if( chokiDic(fingerPos) ) choki.draw(60, 150, 50, 50); //チョキ描画
    if( paaDic(fingerPos) ) paa.draw(100, 150, 50, 50); //パー描画
    
    
    
    //-----------------------------------------------------Leap Motion
    ofSetColor(255, 255, 0);
    glBegin(GL_POINT);
    for(int i = 0; i < 10; i++) {
        glVertex3f(fingerPos[i].x, fingerPos[i].y, fingerPos[i].z);
    }
    glEnd();

    
    //-----------------------------------------------------Cloth Particle
    ofTranslate(ofGetWidth()/2-200, 100, -300);
    
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


//グーの形状検知
bool guuDic(ofVec3f fPos[]){
    
    return false;
}

//チョキの形状検知
bool chokiDic(ofVec3f fPos[]){
    return false;
}


//パーの形状検知
bool paaDic(ofVec3f fPos[]){
    return false;
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
