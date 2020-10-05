//編集：田上慶治
#include "DxLib.h"
#include <math.h>

/*以下に定数の定義(define)*/

/*以下に構造体の定義*/

/*以下に関数のプロトタイプ宣言を書く*/

/*メイン関数*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	DxLib_Init(); // DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面にする

	char keys[256];//キー入力用の配列
	/*以下に初期化処理を書く*/
	
  /*メインループ*/
	while (1) {
		if (ProcessMessage() != 0) {//メッセージ処理
			break;//×ボタンでループ終了
		}
		ClearDrawScreen();//画面を消す
		clsDx();//ループごとに前の結果を消す
		GetHitKeyStateAll(keys);//キーの押下状態を取得
    
		/*以下に描画処理を書く*/
		


		/*以下に更新(+補正)処理を書く*/
		


		ScreenFlip();//裏画面を表画面に反映
	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}

/*以下に関数の定義を書く*/
