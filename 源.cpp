#include <stdio.h>
#include "raylib.h"

class Fish {

public:
	float x;
	float y;
	Texture2D tex[7][2];//储存图片的数组

	Fish(float startX, float startY) {
		x = startX;
		y = startY;
	}

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

	void Draw() {

		DrawTexture(tex[0][0], x, y, WHITE);//绘制图片

	}

	void update() {
		if (IsKeyDown(KEY_W)) {//iskeydown是raylib函数，判断按键是否按下
			y -= 0.1;
		}
		if (IsKeyDown(KEY_S)) {
			y += 0.1;
		}
		if (IsKeyDown(KEY_A)) {
			x -= 0.1;
		}
		if (IsKeyDown(KEY_D)) {
			x += 0.1;
		}
	}

};

int main() {

	InitWindow(1280, 720, "Fish Game");//初始化窗口

	Fish gamer(450, 240);

	gamer.LoadTextures();//调用加载图片

	while (!WindowShouldClose()) {	//循环是否关闭窗口

		gamer.update();//调用更新函数

		BeginDrawing();//开始画画

		ClearBackground(BLUE);//设置背景颜色

		gamer.Draw();//调用绘画函数

		EndDrawing();//结束画画

	}

	return 0;

}