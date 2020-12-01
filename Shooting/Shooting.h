#pragma once

/*以下に定数の定義(define)*/
#define CX 320//中心のx
#define CY 240//中心のy
#define MX 640//最大x
#define MY 480//最大y
#define PI acos(-1)//円周率π

/*以下に構造体の定義*/

//プレイヤーの構造体
typedef struct {
	double x, y;//座標
	int size_x, size_y;//画像の大きさ
	double v;//4方向の速さ
	double exRate;//拡大率
	int interval;//ショットの間隔
	int sCount;//ショットを打ち始めるカウント
	int graph[3];//3つ分の画像ハンドル
}Player;

//弾の構造体
typedef struct {
	double x, y;//座標
	double vx, vy;//速度
	double r;//半径
	int color;//色
}Bullet;
//自弾の構造体リスト
typedef struct PlayerBullet {
	Bullet s;//プレイヤーのショットデータ
	struct PlayerBullet *before;//前のデータへのポインタ
	struct PlayerBullet *next;//次のデータへのポインタ
}PBullet;

//敵の構造体
typedef struct {
	double x, y;//座標
	double vx, vy;//速度
	int hp;//ヒットポイント
	int size_x, size_y;//画像の大きさ
	double exRate;//拡大率
	int *graph;//画像ハンドル(2コマ分)
}Enemy;

//敵の構造体リスト
typedef struct EnemyList {
	Enemy e;//敵データ
	struct EnemyList *before;//前のデータへのポインタ
	struct EnemyList *next;//次のデータへのポインタ
}Elist;



					


/*以下に関数のプロトタイプ宣言を書く*/
void movePlayer(Player *p);
void drawPlayer(Player *p);
void addPlayerBullet(Bullet b);
PBullet* delPlayerBullet(PBullet *p);
void delAllPlayerBullet();
int Btwn(int p1, int p, int p2);
void rotate2d(double *x, double *y, double deg);
void addEnemy(Enemy e);
Elist* delEnemy(Elist *p);
void delAllEnemy();
void drawEnemy(int count);
void calcEnemy();
int isInWall(double x, double y, double blank);
