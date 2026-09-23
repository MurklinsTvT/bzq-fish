#include <stdio.h>
#include "raylib.h"
#include <vector>
#include <cstdlib>//随机数库
#include <ctime>//时间库

const int MAX_ENEMIES = 10;

Texture2D tex[7][2] = { 0 };//储存图片的数组

void LoadTextures() {
	char buf[100];//临时储存图片路径的数组
	int dex = 1;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			sprintf_s(buf, 100, "RESOURCE/%d.png", dex++);//格式化字符串，储存图片路径，类似填空题
			tex[i][j] = LoadTexture(buf);//调用函数加载图片
		}
	}
}

class Fish {

public:
	float x;
	float y;
	float radius;
	float speed;
	bool alive;
	int texIndex;
	int fishead = 0;

	Fish() = default;

	Fish(double startX, double startY,float r,float spd,int texIdx) {
		x = startX;
		y = startY;
		radius = r;
		speed = spd;
		texIndex=texIdx;
		alive = true;
	}

	
	void Draw() {
		if (!alive) return;//死了不画

		Texture2D currentTex = tex[texIndex][fishead];//根据鱼的索引和方向选择图片
		DrawTextureEx(currentTex, { x, y }, 0.0f, radius / 50.0f, WHITE);

	}

	void update() {
		if (!alive) return;

		if (IsKeyDown(KEY_W)) {//iskeydown是raylib函数，判断按键是否按下
			y -= speed;
		}
		if (IsKeyDown(KEY_S)) {
			y += speed;
		}
		if (IsKeyDown(KEY_A)) {
			x -= speed;
			fishead = 0;
		}
		if (IsKeyDown(KEY_D)) {
			x += speed;
			fishead = 1;
		}
	}

};

int main() {

	InitWindow(1600, 900, "Fish Game");//初始化窗口

	SetTargetFPS(60);//设置帧率

	srand((unsigned)time(NULL));//设置随机数种子,敌鱼位置每次不一样

	LoadTextures();//调用加载图片函数

	std::vector<Fish> fishes;

	fishes.push_back(Fish(600, 300, 20.0f, 5.0f, 0));//玩家鱼

	for (int i = 0; i < MAX_ENEMIES; i++) {
		float randX = (float)(rand() % 1600);
		float randY = (float)(rand() % 900);
		float randRadius = (float)(rand() % 30);
		float randSpeed = 0.5f + (rand() % 20) * 0.1f;
		float randTexIndex = 1 + (rand() % 6);
		fishes.push_back(Fish(randX, randY, randRadius, randSpeed, randTexIndex));
	}

	while (!WindowShouldClose()) {	//循环是否关闭窗口

		fishes[0].update();//调用更新函数

		BeginDrawing();//开始画画

		ClearBackground(BLUE);//设置背景颜色

		for (int i = 0; i < fishes.size(); i++) {
			fishes[i].Draw();//调用画鱼函数
		}

		EndDrawing();//结束画画

	}

	return 0;

}