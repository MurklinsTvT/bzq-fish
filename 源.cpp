#include <stdio.h>
#include "raylib.h"//raylib库，画图
#include <vector>//动态数组
#include <cstdlib>//随机数库
#include <ctime>//时间库

const int MAX_ENEMIES = 20;//常量，设最大敌鱼数量为10

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

class FishTransform {
public:
	float x;
	float y;
	float speed;
};

class Sprite {
public:	
	int texIndex;
	int fishead = 0;
};

class Collider {
public:	
	float radius;
	bool alive;
};

class Fish {

public:

	FishTransform transform;
	Sprite sprite;
	Collider collider;

	bool isPlayer = false;
	float vx = 0.0f;
	float vy = 0.0f;
	float lifeTime = 0.0f;

	Fish() = default;

	Fish(double startX, double startY,float r,float spd,int texIdx,bool isPlayerFlag = false) {
		transform.x = startX;
		transform.y = startY;
		transform.speed = spd;

		sprite.texIndex=texIdx;
		sprite.fishead = 0;

		collider.radius = r;
		collider.alive = true;

		isPlayer = isPlayerFlag;
		if (!isPlayer) {
			vx = (rand() % 2 == 0 ? 1.0f : -1.0f) * transform.speed;
			vy = 0.0f;
		}
	}

	
	void Draw() {
		if (!collider.alive) return;//死了不画

		Texture2D currentTex = tex[sprite.texIndex][sprite.fishead];//根据鱼的索引和方向选择图片

		float scale = (collider.radius * 2.0f) / (float)currentTex.width;

		float drawWidth = currentTex.width * scale;
		float drawHeight = currentTex.height * scale;
		//  计算缩放后的实际宽高

		float drawX = transform.x - drawWidth / 2.0f;
		float drawY = transform.y - drawHeight / 2.0f;
		// 计算居中坐标（让图片中心对准碰撞圆心）

		DrawTextureEx(currentTex, { drawX, drawY }, 0.0f, scale, WHITE);

		DrawCircleLines((int)transform.x, (int)transform.y, collider.radius * 0.7f, RED);
		// 画出碰撞圈
	}

	void update() {

		if (!collider.alive) return;

		float r = collider.radius * 0.7f;

		if (isPlayer) {
			if (IsKeyDown(KEY_W)) {//iskeydown是raylib函数，判断按键是否按下
				transform.y -= transform.speed;
			}
			if (IsKeyDown(KEY_S)) {
				transform.y += transform.speed;
			}
			if (IsKeyDown(KEY_A)) {
				transform.x -= transform.speed;
				sprite.fishead = 0;
			}
			if (IsKeyDown(KEY_D)) {
				transform.x += transform.speed;
				sprite.fishead = 1;
			}

			if (transform.x < r) {
				transform.x = r;
			}
			if (transform.x > GetScreenWidth() - r) {
				transform.x = GetScreenWidth() - r;
			}
			if (transform.y < r) {
				transform.y = r;
			}
			if (transform.y > GetScreenHeight() - r) {
				transform.y = GetScreenHeight() - r;
			}
		}else {
			lifeTime += GetFrameTime();
			if (lifeTime > 1.0f) {
				transform.x += vx;
			}
			if (vx > 0) sprite.fishead = 1;
			else if (vx < 0) sprite.fishead = 0;
		
			if (transform.x < r) {
				transform.x = r;
				vx = -vx;
				sprite.fishead = 1;
			}

			if (transform.x > GetScreenWidth() - r) {
				transform.x = GetScreenWidth() - r;
				vx = -vx;
				sprite.fishead = 0;			
			}
		}
	}
};

int main() {

	InitWindow(1600, 900, "Fish Game");//初始化窗口

	SetTargetFPS(60);//设置帧率

	srand((unsigned)time(NULL));//设置随机数种子,敌鱼位置每次不一样

	LoadTextures();//调用加载图片函数

	std::vector<Fish> fishes;//一个动态数组，储存鱼类对象

	fishes.push_back(Fish(800, 400, 80.0f, 5.0f, 0,true));//玩家鱼

	for (int i = 0; i < MAX_ENEMIES; i++) {
		float randX, randY, randRadius;
		float randSpeed = 0.5f + (rand() % 15) * 0.1f;
		float randTexIndex = 1 + (rand() % 6);
		do {
			randX = (float)(rand() % 1600);
			randY = (float)(rand() % 900);

			randRadius = 40.0f + (float)(rand() % 25);

			float dx = randX - 800.0f;
			float dy = randY - 400.0f;
			float distance = dx * dx + dy * dy;

			if (distance > 300.0f * 300.0f) {
				break;
			}
		} while (true);

			fishes.push_back(Fish(randX, randY, randRadius, randSpeed, randTexIndex));
	}

	bool gameOver = false;
	bool gameWin = false;
	

	while (!WindowShouldClose()) {	//循环是否关闭窗口

		if (!gameOver&&!gameWin) {

			for (int i = 0; i < fishes.size(); i++) {
				fishes[i].update();
			}

			for (int i = 0; i < fishes.size(); i++) {

				if (!fishes[i].collider.alive) continue;

				for (int j = 0; j < fishes.size(); j++) {

					if (i == j || !fishes[j].collider.alive) continue;

					if (!fishes[i].isPlayer && !fishes[j].isPlayer) continue;

					if (CheckCollisionCircles(

						{ fishes[i].transform.x,fishes[i].transform.y }, fishes[i].collider.radius,

						{ fishes[j].transform.x,fishes[j].transform.y }, fishes[j].collider.radius))
					{
						if (fishes[i].collider.radius > fishes[j].collider.radius) {

							fishes[j].collider.alive = false;

							fishes[i].collider.radius += fishes[j].collider.radius * 0.2f;
						}
					}
				}
			}

			bool allEnemiesDead = true;
			for(int i=1;i<fishes.size();i++){
				if (fishes[i].collider.alive) {
					allEnemiesDead = false;
					break;
				}
			}
			if (allEnemiesDead) {
				gameWin = true;
			}
		}	

		BeginDrawing();//开始画画

		ClearBackground(BLUE);//设置背景颜色

		for (int i = 0; i < fishes.size(); i++) {

			fishes[i].Draw();//调用画鱼函数
		}

		if (gameOver) {

			DrawText("Game Over!", 500, 400, 100, RED);//游戏结束文字
		}
		else if (gameWin) {
			DrawText("Victory!", 500, 400, 100, RED);
		}

		EndDrawing();//结束画画

	}

	for (int i = 0; i < 7; i++) {

		UnloadTexture(tex[i][0]);//卸载图片

		UnloadTexture(tex[i][1]);
	}

	return 0;

}