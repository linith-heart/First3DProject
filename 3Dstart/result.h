//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
HRESULT InitResult(int oldMode);
void UninitResult(int oldMode);
void UpdateResult(void);
void DrawResult(void);

#endif
