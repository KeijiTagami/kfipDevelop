#pragma once

/*�ȉ��ɒ萔�̒�`(define)*/
#define CX 320//���S��x
#define CY 240//���S��y
#define MX 640//�ő�x
#define MY 480//�ő�y
#define PI acos(-1)//�~������

/*�ȉ��ɍ\���̂̒�`*/

//�v���C���[�̍\����
typedef struct {
	double x, y;//���W
	int size_x, size_y;//�摜�̑傫��
	double v;//4�����̑���
	double exRate;//�g�嗦
	int interval;//�V���b�g�̊Ԋu
	int sCount;//�V���b�g��ł��n�߂�J�E���g
	int graph[3];//3���̉摜�n���h��
}Player;

//�e�̍\����
typedef struct {
	double x, y;//���W
	double vx, vy;//���x
	double r;//���a
	int color;//�F
}Bullet;
//���e�̍\���̃��X�g
typedef struct PlayerBullet {
	Bullet s;//�v���C���[�̃V���b�g�f�[�^
	struct PlayerBullet *before;//�O�̃f�[�^�ւ̃|�C���^
	struct PlayerBullet *next;//���̃f�[�^�ւ̃|�C���^
}PBullet;

//�G�̍\����
typedef struct {
	double x, y;//���W
	double vx, vy;//���x
	int hp;//�q�b�g�|�C���g
	int size_x, size_y;//�摜�̑傫��
	double exRate;//�g�嗦
	int *graph;//�摜�n���h��(2�R�}��)
}Enemy;

//�G�̍\���̃��X�g
typedef struct EnemyList {
	Enemy e;//�G�f�[�^
	struct EnemyList *before;//�O�̃f�[�^�ւ̃|�C���^
	struct EnemyList *next;//���̃f�[�^�ւ̃|�C���^
}Elist;



					


/*�ȉ��Ɋ֐��̃v���g�^�C�v�錾������*/
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
