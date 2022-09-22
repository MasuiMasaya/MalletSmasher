#ifndef __SCENERESULT_H__
#define __SCENERESULT_H__

#include "Defines.h"
#include "Camera.h"

class SceneResult
{
public:
	SceneResult();
	~SceneResult();


	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	Camera* m_pCamera;


};

#endif // !__SCENERESULT_H__