//=============================================================================
//
// UI�̏��� [ui.h]
// Author : 
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"


// �}�N����`
#define	NUM_UI		(2)					// �|���S����


//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �v���C���[�\����
{
	bool			use;						// true:�g�p  false:���g�p44
	D3DXVECTOR3		pos;						// �|���S���̈ړ���
	D3DXVECTOR3		rot;						// �|���S���̉�]��
	int				type;
	int				size_x;
	int				size_y;

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

} UI;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void SetUi(D3DXVECTOR3 pos);
UI *GetUi(int no);


#endif
