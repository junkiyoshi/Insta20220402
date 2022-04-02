#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	// 配色デザイン P192 ソフトピンク
	this->color_palette.push_back(ofColor(221, 155, 165));
	this->color_palette.push_back(ofColor(224, 186, 212));
	this->color_palette.push_back(ofColor(229, 181, 161));
	this->color_palette.push_back(ofColor(208, 130, 139));
	this->color_palette.push_back(ofColor(202, 72, 90));
	this->color_palette.push_back(ofColor(163, 150, 197));
	this->color_palette.push_back(ofColor(184, 145, 26));
	this->color_palette.push_back(ofColor(250, 241, 239));


	auto radius = 6;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	ofColor tmp_color;
	for (float y = 0; y < 720; y += radius * 1.5) {

		for (float x = 0; x < 720; x += x_span) {

			glm::vec3 location;
			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			this->location_list.push_back(location);
			this->life_list.push_back(0);
			this->color_list.push_back(this->color_palette[ofRandom(color_palette.size())]);
		}
		flg = !flg;
	}

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 230, true, true, true);

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();
	
	vector<string> words = vector<string> { "NFT", "ETH", "XTZ", "GEN", "ART" };

	string word = words[(ofGetFrameNum() / 54) % words.size()];

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);
	this->font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) * 0.5);
	fbo.end();
	ofPixels pixels;
	fbo.readToPixels(pixels);

	for (int i = 0; i < this->location_list.size(); i++) {

		ofColor color = pixels.getColor(this->location_list[i].x, this->location_list[i].y);
		if (color != ofColor(0, 0)) {

			this->life_list[i] = this->life_list[i] < 50 ? this->life_list[i] + 2 : 50;
		}
		else {

			this->life_list[i] = this->life_list[i] > 0 ? this->life_list[i] - 5 : 0;
		}
	}

	for (int i = 0; i < this->location_list.size(); i++) {

		auto height = 5.f;
		if (this->life_list[i] > 30) {

			this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 6, height, this->color_list[i]);
		}
		else if (this->life_list[i] > 0) {

			this->setHexagonToMesh(this->face, this->frame, this->location_list[i], ofMap(this->life_list[i], 0, 30, 0, 6), height, this->color_list[i]);
		}
		else if (this->life_list[i] == 0) {

			this->color_list[i] = this->color_palette[ofRandom(color_palette.size())];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {


	this->face.draw();
	this->frame.draw();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height,ofColor face_color) {

	ofColor frame_color = ofColor(0);

	for (int deg = 90; deg < 450; deg += 60) {

		auto face_radius = radius - 0.5;
		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(location + glm::vec3(0, 0, 0));
		vertices.push_back(location + glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(0, 0, height));
		vertices.push_back(location + glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(location + glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		auto frame_index = frame_target.getNumVertices();

		vertices.clear();
		vertices.push_back(location + glm::vec3(0, 0, 0));
		vertices.push_back(location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(location + glm::vec3(0, 0, height));
		vertices.push_back(location + glm::vec3(radius * cos((deg + 60) * DEG_TO_RAD), radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height));

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}