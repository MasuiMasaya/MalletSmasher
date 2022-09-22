#pragma once
//======================================
//	�T�E���h�t�@�C�����d�ǂݍ��ݖh�~
//======================================
#include "Sound.h"


enum SE_KIND
{
	SE_PuckToWall,		
	SE_PuckToWall2,
	SE_PuckToPuck,
	SE_SmashHit,
	SE_VOLUME_1,
	SE_VOLUME_2,
	SE_VOLUME_3,

	SE_MAX
};

enum BGM_KIND
{
	BGM_TUTORIAL_STAGE,
	BGM_GAME_STAGE,
	BGM_RESULT_STAGE,

	BGM_MAX
};

void InitSoundEffect();
void InitBGM();
void UninitSoundEffect();
void UninitBGM();

XAUDIO2_BUFFER* GetSoundEffect(SE_KIND);
XAUDIO2_BUFFER* GetBGM(BGM_KIND);