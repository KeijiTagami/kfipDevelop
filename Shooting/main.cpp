//�ҏW�F�c��c��
#include "DxLib.h"
#include <math.h>
#include "Shooting.h"

PBullet *pbHead = NULL;//�v���C���[�̒e�̋󃊃X�g(�擪)���쐬
PBullet *pbTail = NULL;//�v���C���[�̒e�̋󃊃X�g(����)���쐬
Elist *eHead = NULL;//�G�̋󃊃X�g(�擪)���쐬
Elist *eTail = NULL;//�G�̋󃊃X�g(����)���쐬
/*�ȉ��ɒ萔�̒�`(define)*/

/*�ȉ��ɍ\���̂̒�`*/


//�G�e�̃��X�g
typedef struct EnemyBullet {
	Bullet s;//�e�f�[�^
	struct EnemyBullet *before;//�O�̃f�[�^�ւ̃|�C���^
	struct EnemyBullet *next;//���̃f�[�^�ւ̃|�C���^
}EBullet;
EBullet *ebHead = NULL;//�G�e�̋󃊃X�g(�擪)���쐬
EBullet *ebTail = NULL;//�G�e�̋󃊃X�g(����)���쐬
//�������u��(pushNow),�������u��(pullNow),����ȊO�̏��(other)
enum KeyState {
	pushNow = 1,
	pullNow = -1,
	other = 0
};
typedef struct _keys {
	char preKeys[256];//�O�̃L�[�̉������(1or0)
	char nowKeys[256];//���݂̃L�[�̉������(1or0)
	KeyState state[256];//���݂̃L�[�̏��
}Key;
Key *keys;//�L�[�\���̂̃|�C���^


/*�ȉ��Ɋ֐��̃v���g�^�C�v�錾������*/
void calcPlayerBullet();
void drawPlayerBullet();
void createPlayerShot(Player *p, int count);

void addEnemyBullet(Bullet b);
EBullet* delEnemyBullet(EBullet *p);
void delAllEnemyBullet();
void calcEnemyBullet();
void drawEnemyBullet();
void createEnemyShot(Player p,int count);
double getAngle(Bullet b, Player p);

void updateKeys();
//head->...->tail�̖�����node��ǉ�
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//node��head-tail�̃��X�g�������(node�̎��̃m�[�h��Ԃ�)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tail�̃��X�g�̃m�[�h�����ׂď���
template<typename T>
void deleteAllNode(T **head, T **tail);
/*���C���֐�*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	DxLib_Init(); // DX���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);//�`���𗠉�ʂɂ���

	keys = (Key*)malloc(sizeof(Key));//�L�[���͂̏�Ԃ��i�[����\����
	GetHitKeyStateAll(keys->nowKeys);//�ŏ��̃L�[���͂̏�Ԃ�������
	
	/*�ȉ��ɏ���������������*/
	//x,y,size_x,size_y,v,exrate,interval,scount,(graph)
	Player p = { CX,CY,60,100 ,3,0.5,5, 0};//�v���C���[�̍\���̂Ə�����
	SetTransColor(0, 0, 0);
	p.graph[0] = LoadGraph("graphic/player_l.gif");//�摜�̃��[�h
	p.graph[1] = LoadGraph("graphic/player.gif");
	p.graph[2] = LoadGraph("graphic/player_r.gif");

	int fairy[4];//�d���̉摜�n���h��
	LoadDivGraph("graphic/fairy.png", 4, 2, 2, 100, 100, fairy);
	int fairy1[2] = { fairy[0],fairy[1] };//�F�̗d��
	int fairy2[2] = { fairy[2],fairy[3] };//�ԐF�̗d��

	PlayMovieToGraph(p.graph[0]); //GIF�A�j���[�V�����̊J�n
	PlayMovieToGraph(p.graph[1]);
	PlayMovieToGraph(p.graph[2]);


	int count = 0;//�X�e�[�W�J�E���g
	/*���C�����[�v*/
	while (1) {
		if (ProcessMessage() != 0) {//���b�Z�[�W����
			break;//�~�{�^���Ń��[�v�I��
		}
		ClearDrawScreen();//��ʂ�����
		clsDx();//���[�v���ƂɑO�̌��ʂ�����
		updateKeys();

		/*�ȉ��ɕ`�揈��������*/
		drawEnemy(count);//�G�̕`�揈��
		drawPlayer(&p);
		drawPlayerBullet();
		drawEnemyBullet();

		

		/*�ȉ��ɍX�V(+�␳)����������*/
		calcEnemy();//�G�̌v�Z����
		movePlayer(&p);//�v���C���[�̓���
		calcPlayerBullet();
		calcEnemyBullet();
		//�G��2�ʂ���
		
		Enemy e1 = { 0,0,1,1,10,100,100,0.7,fairy1 };
		Enemy e2 = { MX,MY,-1,-1,10,100,100,0.7,fairy2 };
		if (count % 60 == 0) {
			addEnemy(e1);
		}
		else if(count%60==30) {
			addEnemy(e2);
		}
		
		//��������e�𐶂ݏo������
		createPlayerShot(&p,count);
		createEnemyShot(p,count);

		count++;
		ScreenFlip();//����ʂ�\��ʂɔ��f
	}
	delAllEnemy();//�ꊇ�t���[
	delAllPlayerBullet();//�ꊇ�t���[
	delAllEnemyBullet();

	free(keys);
	DxLib_End(); // DX���C�u�����I������
	return 0;
}

//�L�[�\���̂̍X�V
void updateKeys() {
	memcpy(keys->preKeys, keys->nowKeys, sizeof(keys->nowKeys));//�O�̃L�[�̉�����Ԃ��X�V
	GetHitKeyStateAll(keys->nowKeys);//���݂̃L�[�̉�����Ԃ��擾
	for (int i = 0; i < 256; i++)//�S�ẴL�[�ɂ���
		keys->state[i] = (KeyState)(keys->nowKeys[i] - keys->preKeys[i]);//�p���X�̌��o���������u��,�������u�Ԃ���ԂƂ��Ċi�[
}

/*�ȉ��Ɋ֐��̒�`������*/
template<typename T>
void pushBack(T **node,T ** head,T **tail) {
	(*node)->next = NULL;//�ǉ��m�[�h�̎���NULL

	if (*head == NULL) //��̃��X�g�̏ꍇ
		*head = *node;//�w�b�h�͒ǉ��m�[�h���w��
	if (*tail)//�����̃m�[�h�����ɂ���Ƃ�
		(*tail)->next = *node;//�O�m�[�h�̐��ǉ��m�[�h��
	(*node)->before = *tail;//�ǉ��m�[�h�̑O��O�m�[�h��
	*tail = *node;//Tail�͍Ō�̃m�[�h���w��
}
template<typename T>
T* deleteNode(T **node,T **head, T **tail) {
	T * pReturn = (*node)->next;//���̃m�[�h���w���|�C���^�̗p��
	if ((*node)->before)//�m�[�h���擪�łȂ��Ƃ�
		(*node)->before->next = pReturn;//��O�̎������̃m�[�h�ɂ���
	else
		*head = pReturn;//�w�b�h�̎w���m�[�h�����ɂ���

	if (pReturn)//p���������w���Ȃ��Ƃ�
		pReturn->before = (*node)->before;//���̃m�[�h����2�O�̃m�[�h���Ȃ�
	else//p���������w���Ƃ�
		*tail = (*node)->before;//�e�[���͑O�̃m�[�h���w��

	free(*node);//�m�[�h������
	return pReturn;//���̃|�C���^��Ԃ�

}
template<typename T>
void deleteAllNode(T **head,T **tail) {
	T *p, *pnext;//�|�C���^��2�p��
	pnext = *head;//�w�b�h�̃m�[�h���w��
	while (pnext) {//�m�[�h���Ȃ��Ȃ�܂ŌJ��Ԃ�
		p = pnext;//p��i�߂�
		pnext = p->next;//pnext��i�߂�
		free(p);//p�̃m�[�h���J��
	}
	*head = NULL;//�k��
	*tail = NULL;//�k��
}

void drawEnemyBullet() {
	int sum = 0;//���X�g�̗v�f��
	EBullet *itr = ebHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�~�̕`��
		DrawCircle(itr->s.x, itr->s.y, itr->s.r, itr->s.color, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("eBullet=%d\n", sum);//�v�f����\��
}
void calcEnemyBullet() {
	EBullet *itr = ebHead;
	while (itr != NULL) {//�����܂�
		itr->s.x += itr->s.vx;//x�����̈ړ�
		itr->s.y += itr->s.vy;//y�����̈ړ�
		
		//�͈͓�
		if (Btwn(-20, itr->s.x, MX + 20) && Btwn(-20, itr->s.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delEnemyBullet(itr);//�Y���̒e��EBullet�������
		}


	}
}
void createEnemyShot(Player p,int count) {
	Bullet b = { 0, 0, 4, 0, 5 ,GetColor(255,255,255) };//�e�f�[�^
	if (count % 60 == 0) {//(0,30...)�̃^�C�~���O
		Elist *itr = eHead;
		int j;
		//�S�Ă̓G�Ń��[�v
		while (itr != NULL) {
			b.x = itr->e.x, b.y = itr->e.y;//�G�̍��W�ɏd�˂�
			double v = hypot(b.vx, b.vy);
			double rad = getAngle(b, p) / 180 * PI;
			b.vx = v * cos(rad),b.vy=v*sin(rad);
			addEnemyBullet(b);//�e��ǉ�
			
			itr=itr->next;
		}
	}
}
//�e����v���C���[�ւ̊p�x��x���@�ŕԂ�
double getAngle(Bullet b, Player p) {
	double rad = atan2(p.y - b.y, p.x - b.x);
	return 180 * rad / PI;
}
//�G�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemyBullet(Bullet b) {
	//�V����node�f�[�^
	EBullet *pnode = (EBullet *)malloc(sizeof(EBullet));
	pnode->s = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &ebHead, &ebTail);//�����ɒǉ�
	
}

//�G�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
EBullet* delEnemyBullet(EBullet *p) {
	return deleteNode(&p, &ebHead, &ebTail);
}

//�G�e���X�g�S�̂�����
void delAllEnemyBullet() {
	deleteAllNode(&ebHead, &ebTail);
}




void drawPlayerBullet() {
	int sum = 0;//���X�g�̗v�f��
	PBullet *itr = pbHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�~�̕`��
		DrawCircle(itr->s.x, itr->s.y, itr->s.r, itr->s.color, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("pBullet=%d\n", sum);//�v�f����\��
}
void calcPlayerBullet(){
	PBullet *itr = pbHead;
	while (itr != NULL) {//�����܂�
		itr->s.x += itr->s.vx;//x�����̈ړ�
		itr->s.y += itr->s.vy;//y�����̈ړ�
							  //�͈͓�
		if (Btwn(-20, itr->s.x, MX + 20) && Btwn(-20, itr->s.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delPlayerBullet(itr);//�Y���̒e��PBullet�������
		}

	}
}

void createPlayerShot(Player *p,int count) {
	int isPush = keys->state[KEY_INPUT_Z] == pushNow;//z����������
	int isPull = keys->state[KEY_INPUT_Z] == pullNow;//z�𗣂�����
	int isEnoughInterval = p->sCount + p->interval <= count;//�A�ł��Ă��Ȃ���
	if (isPull)p->sCount = count;//���˃^�C�~���O�̊���X�V
	if ( isPush&&isEnoughInterval) p->sCount = count;//���˃^�C�~���O�̊���X�V
	
	if (!keys->nowKeys[KEY_INPUT_Z]) return;//z�L�[����������
	if (!((count - p->sCount) % p->interval == 0)) return;//�C���^�[�o�����Ȃ�
	
	//�ȉ��̓V���b�g�̌���Ɛ���
	Bullet b1 = { p->x-10, p->y, 0, -5, 5 ,GetColor(255,0,0) };//�e�f�[�^��p��
	Bullet b2 = { p->x+10, p->y, 0, -5, 5 ,GetColor(255,0,0) };//�e�f�[�^��p��
	addPlayerBullet(b1);//�e��ǉ�
	addPlayerBullet(b2);//�e��ǉ�
}


//���+blank���ɂ��邩
int isInWall(double x, double y, double blank) {
	return Btwn(-blank, x, MX + blank) && Btwn(-blank, y, MY + blank);
}

//�G�̕`�揈��
void drawEnemy(int count) {
	int sum = 0;//���X�g�̗v�f��
	Elist *itr = eHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
		//�摜�̕`��
		if (count % 30 < 15) {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.exRate, 0, itr->e.graph[0], 1);
		}
		else {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.exRate, 0, itr->e.graph[1], 1);
		}
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("Enemy=%d\n", sum);//�v�f����\��
}
//�G�̌v�Z����
void calcEnemy() {
	Elist *itr = eHead;
	while (itr != NULL) {//�����܂�
		itr->e.x += itr->e.vx;//x�����̈ړ�
		itr->e.y += itr->e.vy;//y�����̈ړ�
		
		if (itr->e.hp>0 && isInWall(itr->e.x,itr->e.y,100)) {//�����Ă��邩�͈͓��ɂ���ꍇ
			itr = itr->next;//�i�߂�
		}
		else {//hp��0�����܂��͉�ʂ͈̔͊O�ɂ���Ƃ�
			itr = delEnemy(itr);//�Y���̓G��Elist�������
		}

	}
}


//�G�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemy(Enemy e) {
	// �V����node�f�[�^
	Elist *pnode = (Elist *)malloc(sizeof(Elist));
	pnode->e = e;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &eHead, &eTail);//�����ɒǉ�

}

//�G�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
Elist* delEnemy(Elist *p) {
	return deleteNode(&p, &eHead, &eTail);
}

//���X�g�S�̂�����
void delAllEnemy() {
	deleteAllNode(&eHead, &eTail);
}

//�v���C���[�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addPlayerBullet(Bullet b) {
	// �V����node�f�[�^
	PBullet *pnode = (PBullet *)malloc(sizeof(PBullet));
	pnode->s = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &pbHead, &pbTail);//�����ɒǉ�

}

//�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
PBullet* delPlayerBullet(PBullet *p) {
	return deleteNode(&p, &pbHead, &pbTail);

}

//���X�g�S�̂�����
void delAllPlayerBullet() {
	deleteAllNode(&pbHead, &pbTail);
}

// p1����p2�͈̔͂�p������Ƃ�1��Ԃ�
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}
void movePlayer(Player *p) {
	if (keys->nowKeys[KEY_INPUT_RIGHT]) {
		if (p->x<MX - p->size_x / 2 * p->exRate)
			p->x += p->v;
	}
	if (keys->nowKeys[KEY_INPUT_LEFT]) {
		if (p->x>0 + p->size_x / 2 * p->exRate)
			p->x -= p->v;
	}
	if (keys->nowKeys[KEY_INPUT_DOWN]) {
		if (p->y<MY - p->size_y / 2 * p->exRate)
			p->y += p->v;
	}
	if (keys->nowKeys[KEY_INPUT_UP]) {
		if (p->y>0 + p->size_y / 2 * p->exRate)
			p->y -= p->v;
	}
}
//�v���C���[�̕`��
void drawPlayer(Player *p) {
	if (keys->nowKeys[KEY_INPUT_RIGHT]) {
		DrawRotaGraph(p->x, p->y, p->exRate, 0, p->graph[2], 1);
	}
	else if (keys->nowKeys[KEY_INPUT_LEFT]) {
		DrawRotaGraph(p->x, p->y, p->exRate, 0, p->graph[0], 1);
	}
	else {
		DrawRotaGraph(p->x, p->y, p->exRate, 0, p->graph[1], 1);
	}
}
//2d�x�N�g��(x,y)�����v����deg����]����
//�����Fx,y�̃|�C���^,��]�p�xdeg
void rotate2d(double *x, double *y, double deg) {
	double rad = deg / 180.0 * PI;//�x���@���ʓx�@
	double x0 = *x, y0 = *y;//�X�V�O��x,y��x0,y0�Ƃ��Ċi�[
	*x = x0 * cos(rad) - y0 * sin(rad);//x�̉�]
	*y = x0 * sin(rad) + y0 * cos(rad);//y�̉�]
}