#include "ofApp.h"

//--------------------------------------------------------------
bool gamestart;
float dogsize = 50; //犬のサイズ

int score, enemycounter, misscounter; //ボールさんを消した数、画面上にいるボールさんの数,、画面端に逃がしたボールさんの数
static const int ARRAYSIZE = 10; //配列サイズ ボールさんの上限
float ball_x[ARRAYSIZE], ball_y[ARRAYSIZE], r[ARRAYSIZE]; //ボールの位置と半径
int ball_speed[ARRAYSIZE]; //各ボールの速さ
int ball_hp[ARRAYSIZE]; // 各ボールのHP
int red[ARRAYSIZE], green[ARRAYSIZE], blue[ARRAYSIZE]; //ボールの色
int white_eye_r, white_eye_g, white_eye_b;//白目の色
int black_eye_r, black_eye_g, black_eye_b;//黒目の色
int timecounter; //時間を測る。ボールさんの追加間隔を決めるのに使用
bool checker_ball[ARRAYSIZE]; //どのボールさんが場に出ているかを判定1なら出ている、0なら出ていない。
bool boss_checker[ARRAYSIZE]; //そのボールさんがHP4のボス個体かどうかを判定

static const int BULLET = 7;  //いちどに何発打てるか
int bullet_x[BULLET], bullet_y[BULLET]; //弾の位置
float bullet_d[BULLET]; //ボールさんと弾の距離
bool shot; //弾を打つかどうか判定する
bool bullets_flag[BULLET]; //それぞれの弾が場に出ているかどうかを判定


void ofApp::setup() {
	bullet_shot.load("bullet_shot.mp3");
	bullet_shot.setVolume(0.2);
	missed.load("missed.mp3");
	oneup.load("oneup.mp3");
	damage.load("damage.mp3");
	timeup.load("timeup.mp3");
	warp.load("warp.mp3");
	warp.setVolume(0.3);
	speedup.load("speedup.mp3");
	speedup.setLoop(false);

	ofBackground(0, 0, 0);
	ofSetCircleResolution(32);
	ofSetFrameRate(30);

	timecounter = 0;
	score = 0;
	misscounter = 0;
	for (int i = 0; i < ARRAYSIZE; i++) {
		r[i] = 30.0;
	}
	//ボールさんの眼の色設定
	//白目
	white_eye_r = 255;
	white_eye_g = 255;
	white_eye_b = 255;
	//黒目
	black_eye_r = 0;
	black_eye_g = 0;
	black_eye_b = 0;
	for (int i = 0; i < ARRAYSIZE; i++) {
		checker_ball[i] = 0;
	}

	for (int i = 0; i < BULLET; i++) {
		bullet_x[i] = mouseX;
		bullet_y[i] = mouseY;
		bullets_flag[i] = 0;
	}
	shot = 0;

	cout << "please pless any keys!!" << "\n";
}

//--------------------------------------------------------------
int interval; //ボールさんを追加する間隔
int ball_random; //ボスボールを出すか決める
void ofApp::update() {
	if (gamestart) {

		//敵の追加  intervalのフレーム数をむかえるごとに追加していく
		switch (score)
		{
		case(0):
			interval = 60; //60フレームに1回
			break;
		case(5):
			interval = 40; //40フレームに1回
			break;
		case(10):
			interval = 30;
			break;
		case(15):
			interval = 20;
			break;
		default:
			break;
		}
		timecounter = timecounter + 1;
		if (timecounter > interval && enemycounter < ARRAYSIZE) {
			for (int i = 0; i < ARRAYSIZE; i++) {
				if (!checker_ball[i]) {
					//1番目から数えてもしi番目のボールが場に出ていなければi番目のボールを場に出す
					checker_ball[i] = 1;
					ball_x[i] = ofRandom(r[i], ofGetWindowWidth() - r[i]);
					ball_y[i] = 0;
					red[i] = ofRandom(0, 255);
					green[i] = ofRandom(0, 255);
					blue[i] = ofRandom(0, 255);
					//ボールさんを倒した数が15体以上のとき、ボスボールさんが出現する
					if (score < 15) {
						ball_hp[i] = ofRandom(1, 3);
						boss_checker[i] = 0;
					}
					else {
						//ball_randomの数値で出すボールのHPを決める。1/7の確率でボスボールさんを出す
						ball_random = ofRandom(0, 7);
						if (ball_random < 6) {
							ball_hp[i] = ofRandom(1, 3);
							boss_checker[i] = 0;
						}
						else {
							ball_hp[i] = 5;
							boss_checker[i] = 1;
						}
					}
					enemycounter = enemycounter + 1;
					timecounter = 0;
					cout << "\n" << "ball appearerd!" << "\n";
					cout << "enemycounter :" << enemycounter << "\n";
					/*for (int i = 0; i < ARRAYSIZE; i++) {
						cout << checker_ball[i];
					}*/
					cout << "\n";
					break;
				}
			}
		}
		for (int i = 0; i < ARRAYSIZE; i++) {
			switch (ball_hp[i]) {
			case(1):
				r[i] = 30.0;
				break;
			case(2):
				r[i] = 40.0;
				break;
			case(3):
				r[i] = 50.0;
				break;
			case(5):
				r[i] = 80.0;
			default:
				break;
			}
		}
		//1から調べてi番目のボールが場に出ていれば、画面上から下に向けて進行
		for (int i = 0; i < ARRAYSIZE; i++) {
			if (checker_ball[i]) {
				switch (score)
				{
				case(0):
					for (int j = 0; j < ARRAYSIZE; j++) {
						ball_speed[j] = 1;
					}
					break;
				case(5):
					for (int j = 0; j < ARRAYSIZE; j++) {
						ball_speed[j] = 5;
					}
					break;
				case(10):
					for (int j = 0; j < ARRAYSIZE; j++) {
						ball_speed[j] = 7;
					}
					break;
				case(15):
					for (int j = 0; j < ARRAYSIZE; j++) {
						ball_speed[j] = 10;
					}
					break;
				case(20):
					for (int j = 0; j < ARRAYSIZE; j++) {
						ball_speed[j] = ofRandom(5, 15);
						if (ball_hp[j] == 4) {
							ball_speed[j] = ofRandom(5, 10);
						}
					}
				default:
					break;
				}
				ball_y[i] = ball_y[i] + ball_speed[i];
			}
		}

		switch (score) {
		case(4):
			speedup.play();
			break;
		case(9):
			speedup.play();
			break;
		case(14):
			speedup.play();
			break;
		case(19):
			speedup.play();
			break;
		default:
			break;
		}

		for (int i = 0; i < ARRAYSIZE; i++) {
			//もし画面下端に到着したら、そのボールを上端に配置しなおす
			if ((ball_y[i]) > ofGetWindowHeight()) {
				ball_y[i] = -r[i];
				checker_ball[i] = 0;
				enemycounter = enemycounter - 1;
				misscounter = misscounter + 1;
				if (misscounter < 2) {
					missed.play();
				}
				cout << "\n" << "you missed " << misscounter << " / 3" << " balls!" << "\n";
			}
		}

		//弾の前進
		for (int i = 0; i < BULLET; i++) {
			if (bullets_flag[i]) {
				bullet_y[i] = bullet_y[i] - 10;
			}
		}
		//弾が画面端に行ったらその弾を消す
		for (int i = 0; i < BULLET; i++) {
			if (bullet_y[i] < 0) {
				bullets_flag[i] = 0;
			}
		}

		//当たり判定
		//すべての場に出ているボールを調べる
		for (int i = 0; i < ARRAYSIZE; i++) {
			//もしi番目のボールが場に出ていれば
			if (checker_ball[i]) {
				//すべての場に出ている弾を調べる
				for (int j = 0; j < BULLET; j++) {
					//もしj番目の弾が場に出ていれば
					if (bullets_flag[j]) {
						//弾とボールの距離を調べる
						bullet_d[j] = sqrt(pow(bullet_x[j] - ball_x[i], 2) + pow(bullet_y[j] - ball_y[i], 2));
						//もし弾が当たっていたら(弾がボールの中にあるなら)
						if (bullet_d[j] < r[i]) {
							bullets_flag[j] = 0;
							if (ball_hp[i] > 1) {
								damage.play();
							}
							//当該ボールのHPを1減らす
							ball_hp[i] = ball_hp[i] - 1;
							//もしボールのHPが0になったら
							if (ball_hp[i] < 1) {
								warp.play();
								//そのボールを退場させる
								checker_ball[i] = 0;
								score = score + 1;
								enemycounter = enemycounter - 1;
								//もし倒したボールがボスボールだった場合、ミスカウントを1回復
								if (misscounter > 0 && boss_checker[i]) {
									score = score + 2;
									misscounter = misscounter - 1;
									oneup.play();
									cout << "\n" << "you defeated the BOSS!!" << "\n";
									cout << "your misscounter is " << misscounter << "/3 !" << "\n";
								}
							}
							//もし弾が短時間に連続して同じ弾に当たるなどによって、enemycounterがマイナスになった場合の措置
							if (enemycounter < 0) {
								enemycounter = 0;
							}
							cout << "\n";
							cout << "\n" << "checker_ball[" << i << "] was beaten!" << "\n";
							cout << "bullets_flag[" << j << "] was hit!" << "\n";
							cout << "enemycounter : " << enemycounter << "\n";
							cout << score << " balls were beaten!" << "\n";
							for (int i = 0; i < ARRAYSIZE; i++) {
								cout << checker_ball[i];
							}
							cout << "\n";
						}
					}
				}
			}
		}
	}
	//ボールさんを3つ画面外に逃せばゲーム終了
	if (misscounter > 2) {
		//すべてのボールを非表示に
		gamestart = 0;
		for (int i = 0; i < ARRAYSIZE; i++) {
			checker_ball[i] = 0;
		}
		timeup.play();
		cout << "\n";
		cout << "GAME OVER" << "\n";
		cout << "your score is " << score << "!!" << "\n";
		cout << "thank you for playing!" << "\n" << "write  1  to console or close this window!" << "\n";
		score = 0;
		int retry;
		cin >> retry;
		if (retry == 1) {
			gamestart = 1;
			misscounter = 0;
			enemycounter = 0;
			cout << "\n" << "restart!!" << "\n";
		}
	}
}

//--------------------------------------------------------------
int dogx, dogy;
void ofApp::draw() {
	//犬
	dogx = mouseX - (dogsize / 2);
	dogy = ofGetWindowHeight() - 100;
	//顔
	ofSetColor(220, 220, 220);
	ofDrawRectangle(dogx, dogy, dogsize, dogsize * 1.5);
	ofDrawTriangle(
		dogx, dogy,
		dogx, dogy + (dogsize * 1.5),
		dogx - (dogsize / 2.5), dogy + (dogsize * 1.5)
	);
	ofDrawTriangle(
		dogx + dogsize, dogy,
		dogx + dogsize, dogy + (dogsize * 1.5),
		(dogx + dogsize) + (dogsize / 2.5), dogy + (dogsize * 1.5)
	);
	//耳
	ofSetColor(50, 50, 50);
	ofDrawTriangle(
		dogx, dogy,
		dogx + (dogsize / 4), dogy - (dogsize / 2),
		dogx + (dogsize / 2), dogy
	);
	ofDrawTriangle(
		dogx + (dogsize / 2), dogy,
		(dogx + (dogsize / 2)) + (dogsize / 4), dogy - (dogsize / 2),
		(dogx + (dogsize / 2)) + (dogsize / 2), dogy
	);
	//左目
	int eyeXl, eyeYl;
	eyeXl = dogx + (dogsize / 5);
	eyeYl = dogy + (dogsize / 3);
	ofSetColor(255, 255, 255);
	ofDrawCircle(eyeXl, eyeYl, dogsize / 8);
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(0, 0, 0);
	ofDrawCircle(eyeXl, eyeYl, dogsize / 7);
	ofFill();
	ofDrawCircle(eyeXl, eyeYl, dogsize / 30);
	//右目
	int eyeXr, eyeYr;
	eyeXr = dogx + (dogsize / 1.2);
	eyeYr = dogy + (dogsize / 3);
	ofSetColor(255, 255, 255);
	ofDrawCircle(eyeXr, eyeYr, dogsize / 8);
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(0, 0, 0);
	ofDrawCircle(eyeXr, eyeYr, dogsize / 7);
	ofFill();
	ofDrawCircle(eyeXr, eyeYr, dogsize / 30);
	//舌
	ofSetColor(196, 122, 122);
	int mousedistance = dogsize / 5;
	int tangueX, tangueY;
	tangueX = mouseX - mousedistance;
	tangueY = dogy + 40;
	ofDrawRectangle(tangueX, tangueY, dogsize / 2.5, dogsize / 2);
	ofSetColor(145, 89, 89);
	ofDrawLine(tangueX + dogsize / 5, tangueY, tangueX + dogsize / 5, tangueY + dogsize / 2);
	//口
	ofSetColor(0, 0, 0);
	ofDrawLine(mouseX, tangueY - 15, mouseX, tangueY);
	//口左側
	ofDrawLine(mouseX, tangueY, mouseX - mousedistance, tangueY);
	ofDrawLine(mouseX - mousedistance, tangueY, mouseX - mousedistance * 2, tangueY - (dogsize / 8));
	//口右側
	ofDrawLine(mouseX, tangueY, mouseX + mousedistance, tangueY);
	ofDrawLine(mouseX + mousedistance, tangueY, mouseX + mousedistance * 2, tangueY - (dogsize / 8));
	//鼻
	ofDrawEllipse(mouseX, tangueY - 15, dogsize / 4, dogsize / 5);


	//ボール氏
	for (int i = 0; i < ARRAYSIZE; i++) {
		if (checker_ball[i]) {
			//顔面
			ofSetColor(red[i], green[i], blue[i]);
			ofDrawCircle(ball_x[i], ball_y[i], r[i]);
			//目
			int eye_d1, eye_r1, eye_r2;//目の座標
			eye_d1 = r[i] / 2.5;
			eye_r1 = r[i] / 7;
			eye_r2 = r[i] / 5;
			//左眼
			ofSetColor(white_eye_r, white_eye_g, white_eye_b);
			ofDrawCircle(ball_x[i] + eye_d1, ball_y[i], eye_r2);
			ofSetColor(black_eye_r, black_eye_g, black_eye_b);
			ofDrawCircle(ball_x[i] + eye_d1, ball_y[i], eye_r1);
			//右眼
			ofSetColor(white_eye_r, white_eye_g, white_eye_b);
			ofDrawCircle(ball_x[i] - eye_d1, ball_y[i], eye_r2);
			ofSetColor(black_eye_r, black_eye_g, black_eye_b);
			ofDrawCircle(ball_x[i] - eye_d1, ball_y[i], eye_r1);
		}
	}


	//弾
	ofSetColor(255, 255, 255);
	ofSetLineWidth(3);
	for (int i = 0; i < BULLET; i++) {
		if (bullets_flag[i]) {
			ofDrawLine(bullet_x[i], bullet_y[i], bullet_x[i], bullet_y[i] + 20);
		}
	}
}

//--------------------------------------------------------------
int num = 0;
int bullet_timecounter;
bool first_shot;
void ofApp::keyPressed(int key) {
	if (!gamestart) {
		gamestart = 1;
	}
	shot = 1;
	bullet_timecounter = bullet_timecounter + 1;
	//一発目
	if ((!bullets_flag[num]) && !(first_shot)) {
		first_shot = 1;
		bullets_flag[num] = 1;
		bullet_x[num] = mouseX;
		bullet_y[num] = dogy;
		num = num + 1;
		bullet_shot.play();
	}
	if (num >= BULLET) {
		num = 0;
	}
	//2発明以降の連射
	if (bullet_timecounter / 7 == 1) {
		if ((!bullets_flag[num])) {
			bullets_flag[num] = 1;
			bullet_x[num] = mouseX;
			bullet_y[num] = dogy;
			num = num + 1;
			bullet_shot.play();
		}
		if (num >= BULLET) {
			num = 0;
		}
		bullet_timecounter = 0;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	shot = 0;
	first_shot = 0;
	bullet_timecounter = 0;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
