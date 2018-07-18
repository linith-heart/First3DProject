//=============================================================================
//
// �C�m�V�V�̏��� [boar.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _BOAR_H_
#define _BOAR_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// �}�N����`

#define BOAR_MAX		(6)

// �G�l�~�[�̃f�[�^�̃t�@�C���p�X
#define ENEMY_DATA_FILE	("data/FILE/enemy_data.csv")

enum
{
	CHASING,
	ROAMING,
	IDLING,
	CHECKING,
};

//=============================================================================
// �\���̐錾

typedef struct
{
	ENE			ene;			// �G�l�~�[�̊�{�\����
	D3DXVECTOR3 StaringPos;		// �Q�[���J�n���̈ʒu
	D3DXVECTOR3 RoamPos;		// �����ړI�̈ʒu
	D3DXVECTOR2 RoamDistance;	// �����͈�

	int			Stat;			// ���������Ă��邩
	int			IdleTime;		// �A�C�h������
}BOAR;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitBoar(void);
void UninitBoar(void);
void UpdateBoar(void);
void DrawBoar(void);
BOAR *GetBoar(int no);
#endif
