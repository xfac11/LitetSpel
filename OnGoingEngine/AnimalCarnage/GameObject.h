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
#include <map>
class GameObject: public Transform//arv
{
public:
	GameObject();
	GameObject(Shader * shader);
	~GameObject();

	int getNrOfModels();
	shared_ptr<Model>  getModel();

	void setHalfSize(float halfSize[3], float posOffset[3]);

	void calcAABB(std::vector<Vertex3D> mesh);
	void addModel(shared_ptr<Model> m , bool gotSkeleton);
	//void addModel(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices,  bool hasSkeleton);
	//void setMesh(std::vector<Vertex3D> mesh, DWORD *indices, int numberOfIndices, int id);
	void setTexture(std::string file, int id, int mipLevels=-1);
	void setGlowMap(std::string file, int id);
	void setMask(std::string file, int id);
	void setRepeat(float x, float y);
	void setColorMask(DirectX::XMFLOAT4 colorMask);
	DirectX::XMFLOAT4& getColorMask();
	DirectX::XMFLOAT4& getRepeat();
	//void draw();
	AABB getCollisionBox();
	Primitives * CollisionShape;
	btVector3 positionOffset;
	
	//void initAnimationData(AnimationShader* animShader);
	void setActiveDraw(bool arg);
	bool getActiveDraw();
	bool haveAnimation()const;
	void computeAnimationMatrix(float deltaTime, std::string animName, bool isLooping = true);
	void setNewAnimation(float fps, float duration, std::string name, std::vector<std::vector<Luna::Keyframe>> & keyframePack);
	void setSkeleton(std::vector<Luna::Joint> & theJoints, std::string attackJoint);

	DirectX::XMFLOAT3 interpolate1(DirectX::XMFLOAT3 &start, DirectX::XMFLOAT3 & end, float & progression);
	DirectX::XMFLOAT3 interpolate2(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3  end, float progression);
	JointTransformation interpolate1(JointTransformation & frameA, JointTransformation & frameB, float & progression);
	JointTransformation interpolate2(JointTransformation frameA, JointTransformation frameB, float progression);
	bool checkIfAnimExist(std::string animName, std::string animalType);

	DirectX::XMMATRIX getJointPos()const;

private:
	shared_ptr<Model> theModel;
	int cap;

	bool hasLoadedAABB;
	bool activeDraw;

	
	std::vector<Joint> skeleton;
	std::map<std::string,Animation> anims;
	std::string lastAnimation;
	std::string lastAnimal;
	float timePassed;
	int frameCounter;
	int hitboxJointID; //testing indivdual joints
	DirectX::XMMATRIX hitboxJointPos;

	std::vector<DirectX::XMMATRIX> pose_global;
	std::vector<DirectX::XMMATRIX> matrixPallete; //to pipeline
	
	std::map<int, std::vector<DirectX::XMMATRIX>> calculatedFrames;
	//std::vector<KeyFrame> keyframes;
	//AnimationShader* animShader;
	DirectX::XMFLOAT4 colorMask;
	DirectX::XMFLOAT4 repeat;
	AABB colBox;
	//ConstantBuffer<WorldMatrix> worldConstBuffer;
	//Hitbox theHitbox; a model with just the index and vertices and a color. no textures etc
	int nrOfModels;
};
#endif // !GAMEOBJECT_H	
