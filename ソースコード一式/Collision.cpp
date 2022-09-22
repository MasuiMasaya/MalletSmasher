#include "Collision.h"


using namespace DirectX;


Collision::Collision()
	:m_registerNum(0)
{
}

Collision::~Collision()
{
	Uninit();
}



bool Collision::Init()
{
	

	for (int i = 0; i < MAX_COLLISION_REGISTER; ++i)
	{
		m_pList[i][0] = NULL;
		m_pList[i][1] = NULL;
	}

	return true;
}



void Collision::Uninit()
{

	for (int i = 0; i < m_registerNum; ++i)
	{
		m_pList[i][0] = NULL;
		m_pList[i][1] = NULL;
	}
}



bool Collision::Update()
{
	// m_pListの中にある当たり判定を調べる
	// 当たり判定処理
	for (int i = 0; i < MAX_COLLISION_REGISTER; i++)
	{
		GameObject *pA = m_pList[i][0];
		GameObject *pB = m_pList[i][1];

		// 登録されてないデータは実施しない
		if (pA == NULL || pB == NULL)
		{
			continue;
		}

		// 四角形同士の当たり判定
		// 2Dでも3Dでも
		// 中心までの距離 < 半分の大きさを足し合わせた数
		// は変わらない、計算する軸が増えただけ
		XMFLOAT3 aPos = pA->GetPos();
  		XMFLOAT3 bPos = pB->GetPos();
		XMFLOAT3 aSize = pA->GetSize();
		XMFLOAT3 bSize = pB->GetSize();

		XMFLOAT3 distance(fabsf(bPos.x - aPos.x), fabsf(bPos.y - aPos.y), fabsf(bPos.z - aPos.z));

		bool hitX = distance.x < (aSize.x + bSize.x)*0.5f;
		bool hitY = distance.y < (aSize.y + bSize.y)*0.5f;
		bool hitZ = distance.z < (aSize.z + bSize.z)*0.5f;

		if (hitX && hitY && hitZ)
		{
			// dynamic - static
			if (pA->GetCollisionType() == COLLISION_DYNAMIC && pB->GetCollisionType() == COLLISION_STATIC)
			{
				Push(pA, pB);
			}

			// 逆の場合
			if (pA->GetCollisionType() == COLLISION_STATIC && pB->GetCollisionType() == COLLISION_DYNAMIC)
			{
				Push(pB, pA);
			}

			// dynamic - dynamic
			if (pA->GetCollisionType() == COLLISION_DYNAMIC && pB->GetCollisionType() == COLLISION_DYNAMIC)
			{
				Push(pA, pB);
			}

			// dynamic - overrlap

		}
	}



	// 毎フレーム当たるものが変わるのでリストの中の判定ををなくす
	m_registerNum = 0;
	return false;
}



void Collision::Register(GameObject *pA, GameObject *pB)
{
	if (m_registerNum < MAX_COLLISION_REGISTER)
	{
		m_pList[m_registerNum][0] = pA;
		m_pList[m_registerNum][1] = pB;
		++m_registerNum;
	}
}


//  押し出し処理
void Collision::Push(GameObject *pDynamic, GameObject *pStatic)
{

	// 事前に各オブジェクトの情報を抜き出す
	XMFLOAT3 dPos = pDynamic->GetPos();
	XMFLOAT3 sPos = pStatic->GetPos();
	XMFLOAT3 dSize = pDynamic->GetSize();
	XMFLOAT3 sSize = pStatic->GetSize();
	XMFLOAT3 dMove = pDynamic->GetMove();

	// 計算に必要な情報を抜き出す
	// 移動量から移動する方向を求める(移動する方向は1か-1で表す)

	// 三項演算子 (A ? B : C)
	// Aの条件が成立していたらB,不成立ならC
	XMFLOAT3 dir(dMove.x > 0 ? 1 : -1, dMove.y > 0 ? 1 : -1, dMove.z > 0 ? 1 : -1);

	// 実際に当たり判定でめり込んだのはオブジェクトの座標ではなく、角だけがめり込んでいることもあるので
	// 対象の角を計算で求める

	// 移動後の角の座標
	XMFLOAT3 dPoint(dPos.x + dir.x*dSize.x*0.5f, dPos.y + dir.y*dSize.y*0.5f, dPos.z + dir.z*dSize.z*0.5f);

	// 移動前の角の座標
	// そのままの移動量で移動前の座標を表そうとすると浮動小数点の計算誤差により、移動ベクトルの始点が面の内側にあることになり
	// 貫通してないと判定されるために、移動前の位置が遠くなるように計算の時だけ移動量を大きくする
	XMFLOAT3 prePoint(dPoint.x - dMove.x*1.1f, dPoint.y - dMove.y*1.1f, dPoint.z - dMove.z*1.1f);

	// 衝突された側の角を計算
	// 衝突された側の角は移動方向を反転した方向に向かって
	// 大きさの半分を足すと求めることができる
	XMFLOAT3 sPoint(sPos.x - dir.x*sSize.x*0.5f, sPos.y - dir.y*sSize.y*0.5f, sPos.z - dir.z*sSize.z*0.5f);

	// 面上の一点から移動ベクトルの始点と終点へ向かうベクトルを計算する
	// 始点
	XMFLOAT3 vStart(prePoint.x - sPoint.x, prePoint.y - sPoint.y, prePoint.z - sPoint.z);
	// 終点
	XMFLOAT3 vEnd(dPoint.x - sPoint.x, dPoint.y - sPoint.y, dPoint.z - sPoint.z);

	// それぞれの面と計算
	// 各面の法線ベクトル
	XMFLOAT3 Normal[] =
	{
		XMFLOAT3(-dir.x,     0,     0),
		XMFLOAT3(     0,-dir.y,     0),
		XMFLOAT3(     0,     0,-dir.z),
	};

	for (int i = 0; i < _countof(Normal); i++)
	{
		// 面の法線との内積を計算 (dotが内積の結果)
		float dotS = vStart.x*Normal[i].x + vStart.y*Normal[i].y + vStart.z*Normal[i].z;

		float dotE = vEnd.x*Normal[i].x + vEnd.y*Normal[i].y + vEnd.z*Normal[i].z;

		// それぞれの内積の結果が正と負の値であれば移動ベクトルが面を貫通している
		// 正と負の組み合わせかどうかは掛け算で判定できる
		// (正×負＝負/正×正＝正/負×負＝負)
		if (dotS*dotE < 0)
		{
			// 移動ベクトルの方向にめり込んだ量を戻すのではなく、面の方向に向かって戻す
			// (戻す距離は内積の値と等しい)

			// float型の絶対値を取る
			dotE = fabsf(dotE);
			dPos.x += Normal[i].x*dotE;
			dPos.y += Normal[i].y*dotE;
			dPos.z += Normal[i].z*dotE;

			pDynamic->SetPos(dPos);

			// 全ての移動量を0にしてしまうと他のオブジェクトと押し出しの計算をする際に移動量がないことに
			// なるので、移動量を0のしなければならない成分だけ0にする
			dMove.x *= 1 - fabsf(Normal[i].x);
			dMove.y *= 1 - fabsf(Normal[i].y);
			dMove.z *= 1 - fabsf(Normal[i].z);
			pDynamic->SetMove(dMove);
			break;
		}
	}
}