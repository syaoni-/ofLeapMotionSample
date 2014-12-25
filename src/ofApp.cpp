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
    
    
    //----------------------------------------------Leap Motion
    controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
    controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
    //サークルジェスチャの設定
    //controller.config().setFloat("Gesture.Circle.MinRadius", 10.0);
    //controller.config().setFloat("Gesture.Circle.MinArc", 0.5);
    //キータップの設定
    //controller.config().setFloat("Gesture.KeyTap.MinDownVelocity", 40.0);
    //controller.config().setFloat("Gesture.KeyTap.HistorySeconds", .2);
    //controller.config().setFloat("Gesture.KeyTap.MinDistance", 8.0);
    //スクリーンタップジェスチャの設定
    //controller.config().setFloat("Gesture.ScreenTap.MinForwardVelocity", 30.0);
    //controller.config().setFloat("Gesture.ScreenTap.HistorySeconds", .5);
    //controller.config().setFloat("Gesture.ScreenTap.MinDistance", 1.0);
    //スワイプの設定
    //controller.config().setFloat("Gesture.Swipe.MinLength", 200.0);
    //controller.config().setFloat("Gesture.Swipe.MinVelocity", 750);
    
    //ジェスチャ設定の保存
    //controller.config().save();
    
    
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

    
    //---------------------------------------------------------AceLog
    for (int i=0; i < LOG_NUM; i++)
        AceLog.push_back(0);
    for (int i = 0; i <LOG_NUM; i++)
        moveDirectionLog.push_back(ofVec3f(0,0,0));
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
    
    //手のひらの位置
    ofVec3f handPos;
    
    // フレームを取得
    Frame frame = controller.frame();
    // Handをあるだけ列挙
    for(int i=0; i<frame.hands().count(); i++) {
        Hand hand = frame.hands()[i];
        handPos.x = hand.palmPosition().x*2;
        handPos.y = -hand.palmPosition().y*2 + ofGetHeight();
        handPos.z = hand.palmPosition().z; + 50;
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
            AceLog.pop_front();
            AceLog.push_back(difPos.length());
            moveDirection = difPos;
        }
        // Handを描画
        drawPalm(hand);
    }
    camera.end();
    
    
    
    
    //LeapMotion Gesture
    GestureList gestures = frame.gestures();
    for (GestureList::const_iterator gl = gestures.begin(); gl != frame.gestures().end(); gl++) {
        switch ((*gl).type()) {
            case Gesture::TYPE_CIRCLE: //円のジェスチャ
                printf("Circle :: %f s\n",(*gl).durationSeconds());
                std::cout << (*gl).toString() << std::endl; //ジェスチャーの取得したID(Gesture Id:n というString型で返す)
                printf("ID : %d",(*gl).id()); //Idのみを返す。Int型
                break;
                
            case Gesture::TYPE_KEY_TAP: //キータップ
                printf("Key Tap :: %fs",(*gl).durationSeconds());
                break;
                
            case Gesture::TYPE_SCREEN_TAP: //スクリーンタップ
                printf("Screen Tap %fs\n",(*gl).durationSeconds());
                break;
                
            case Gesture::TYPE_SWIPE:
                printf("Swipe :: %fs\n",(*gl).durationSeconds());
                break;
                
            default:
                break;
        }
    }
    
    
    
    //Tool
    Pointable pointable = frame.pointables().frontmost();
    if (pointable.isTool())
        printf("isTool\n");
    
    
    //------------------------------------------------------Acceleration
    logTime += ofGetElapsedTimef() - preElapsedTime;
    preElapsedTime = ofGetElapsedTimef();
    if ( logTime > LOG_INTERVAL ) {
        logTime = 0;
    }
    
    
    //-----------------------------------------------------Move direction
    moveDirectionLog.pop_front();
    moveDirectionLog.push_back(aboutMoveDirection);
    preMoveDirection = aboutMoveDirection;
    aboutMoveDirection = moveDirection.normalize();
    aboutMoveDirection.x > 0.8 ? aboutMoveDirection.x = 1 : NULL;
    aboutMoveDirection.x < -0.8 ? aboutMoveDirection.x = -1 : NULL;
    aboutMoveDirection.y > 0.8 ? aboutMoveDirection.y = 1 : NULL;
    aboutMoveDirection.y < -0.8 ? aboutMoveDirection.y = -1 : NULL;

    
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
    if( guuDic(fingerPos) ) guu.draw(20, 150, 50, 50); //グー描画
    if( chokiDic(handPos, fingerPos) ) choki.draw(60, 150, 50, 50); //チョキ描画
    if( paaDic(handPos, fingerPos) ) paa.draw(100, 150, 50, 50); //パー描画
    
    /* 手の動く方向 */
    ofTriangle(50, 250, 100, 250, 75, 250 + 50*aboutMoveDirection.y);
    ofTriangle(75, 300, 75, 350, 75+50*aboutMoveDirection.x, 325);
    
    
    /* 叩き */
    if (slapDic()) ofCircle(50, 400, 50);
    
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
bool ofApp::guuDic(ofVec3f *fPos){
    for(int i=1; i<5; i++) {
        ofVec3f difPos = fPos[0] - fPos[i];
        if ( difPos.length() > 100 )
            return false;
    }
    return true;
}


//TODO
//チョキの形状検知
bool ofApp::chokiDic(ofVec3f hPos, ofVec3f *fPos){
    
    //手のひらの外側に出している指の数
    int outFingerCount = 0;
    for (int i = 0; i < 5; i++){
        if ( (hPos - fPos[i]).z > 20 )
            outFingerCount++;
    }
    
    //手のひらの外側に出している指の数が2本の場合
    if (outFingerCount == 2)
        return true;
    
    return false;
}


//TODO
//パーの形状検知
bool ofApp::paaDic(ofVec3f hPos, ofVec3f *fPos){
    
    //手のひらの外側に出している指の数
    int outFingerCount = 0;
    for (int i = 0; i < 5; i++){
        if ( (hPos - fPos[i]).z > 0 )
            outFingerCount++;
    }

    
    //手のひらの外側に出している指の数が2本の場合
    if (outFingerCount >= 4)
        return true;
    
    return false;
}


//TODO
//叩き検知
bool ofApp::slapDic(){
    
    int counter=0;
    float transVal1; //BAD Name
    float transVal2;
    
    for (list<ofVec3f>::iterator moveLog = moveDirectionLog.begin() ; moveLog != moveDirectionLog.end(); moveLog++) {
        if (counter < LOG_NUM/2) {
            transVal1 += (*moveLog).y;
        } else {
            transVal2 += (*moveLog).y;
        }
        counter++;
    }
    
    transVal1 /= LOG_NUM/2;
    transVal2 /= LOG_NUM/2;
    
    if ( !signbit(transVal1) )
        return false;
    
    if ( signbit(transVal2) )
        return false;
    
    
    //-----------------------------------
    //平均加速
    
    float averageAce;
    
    for (list<float>::iterator accel = AceLog.begin(); accel != AceLog.end(); accel++) {
        averageAce += (*accel);
    }
    averageAce /= LOG_NUM;
    
    if (averageAce < 30) {
        return false;
    }
    
    return true;
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
