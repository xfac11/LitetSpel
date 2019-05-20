#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Model.h"
#include "Transform.h"
#include "ConstantBuffer.h"
#include "Geometry.h"
#include "btBulletDynamicsCommon.h"
#include "Primitives.h"
//#include "Player.h"
#include "joint.h"
#include "Animation.h"
//#include "KeyFrame.h"
class GameObject: public Transform//arv
{
public:
	GameObject();
	GameObject(Shader * shader);
	~GameObject();

	int getNrOfModels();
	Model*& getModel(int id);
	Model**& getTheModelPtr();

	void setHalfSize(float halfSize[3], float posOffset[3]);

	void calcAABB(std::vector<Vertex3D> mesh);

	void addModel(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices,  bool hasSkeleton);
	void setMesh(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices, int id);
	void setTexture(std::string file, int id, int mipLevels=-1);
	void setGlowMap(std::string file, int id);
	void draw();
	AABB getCollisionBox();
	Primitives * CollisionShape;
	btVector3 positionOffset;
	/*DatForaObject data;*/
	//void initAnimationData(AnimationShader* animShader);
	bool haveAnimation()const;
	void computeAnimationMatrix(float deltaTime);
	void setNewAnimation(float fps, float duration, std::string name, std::vector<std::vector<Luna::Keyframe>> keyframePack);
	void setSkeleton(std::vector<Luna::Joint> theJoints);
	
	DirectX::XMFLOAT3 interpolate1(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression);
	DirectX::XMFLOAT3 interpolate2(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression);
	JointTransformation interpolate1(JointTransformation frameA, JointTransformation frameB, float progression);
	JointTransformation interpolate2(JointTransformation frameA, JointTransformation frameB, float progression);
private:
	Model* *theModel;
	int cap;

	bool hasLoadedAABB;


	//bool gotSkeleton;
	std::vector<Joint> skeleton;
	Animation anims;
	float timePassed;
	float prevTimeIncrement;
	//std::vector<KeyFrame> keyframes;
	//AnimationShader* animShader;

	AABB colBox;
	//ConstantBuffer<WorldMatrix> worldConstBuffer;
	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	int nrOfModels;
};
#endif // !GAMEOBJECT_H	
