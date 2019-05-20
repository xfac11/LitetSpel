#include "GameObject.h"
#include "System.h"

GameObject::GameObject()
{
	this->hasLoadedAABB = false;
	this->cap=5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->colBox = AABB();
	//this->gotSkeleton = false;

	this->timePassed = 0.f;
	this->prevTimeIncrement = 0.f;
}

GameObject::GameObject(Shader * shader)
{
	this->hasLoadedAABB = false;
	this->cap = 5;
	this->nrOfModels = 0;
	this->theModel = new Model*[this->cap];
	for (int i = 0; i < cap; i++)
	{
		this->theModel[i] = nullptr;
	}
	this->colBox = AABB();

	this->timePassed = 0.f;
	this->prevTimeIncrement = 0.f;
	//this->gotSkeleton = false;
	/*this->theModel[0] = new Model;
	this->theModel[0]->setShader(shader);
	this->nrOfModels++;*/

	//this->worldConstBuffer.initialize();
}


GameObject::~GameObject()
{
	for (int i = 0; i < this->nrOfModels; i++)
	{
		if(this->theModel[i]!=nullptr)
			delete this->theModel[i];
	}
	delete[] this->theModel;
}


int GameObject::getNrOfModels()
{
	return this->nrOfModels;
}

Model *& GameObject::getModel(int id)
{
	return this->theModel[id];
}

Model **& GameObject::getTheModelPtr()
{
	return this->theModel;
}

void GameObject::setHalfSize(float halfSize[3], float posOffset[3])
{
	
	this->hasLoadedAABB = true;
	this->colBox.width = halfSize[0];
	this->colBox.height = halfSize[1];
	this->colBox.depth = halfSize[2];
	//wthis->colBox.positionOffset = btVector3(posOffset[0], posOffset[1], posOffset[2]);
	this->CollisionShape = new Primitives();
	this->CollisionShape->Initialize(1,btVector3(posOffset[0], posOffset[1], posOffset[2]), btVector3(halfSize[0]*2, halfSize[1]*2, halfSize[2]*2));
	positionOffset = btVector3(posOffset[0], posOffset[1], posOffset[2]);
	
	//DirectX::XMMatrixTranslation(this->Position.x+posOffset[0])
	this->CollisionShape->SetWorld(&this->getWorld());
	System::getDebugDraw()->addPrimitives(this->CollisionShape);
}

void GameObject::calcAABB(std::vector<Vertex3D> mesh)
{
	if (!this->hasLoadedAABB)
	{
		int minX = 0;
		int minY = 0;
		int minZ = 0;
		int maxX = 0;
		int maxY = 0;
		int maxZ = 0;
		for (int i = 1; i < mesh.size(); i++)
		{
			if (mesh.at(i).position.x < mesh.at(minX).position.x)
			{
				minX = i;
			}
			if (mesh.at(i).position.y < mesh.at(minY).position.y)
			{
				minY = i;
			}
			if (mesh.at(i).position.z < mesh.at(minZ).position.z)
			{
				minZ = i;
			}

			if (mesh.at(i).position.x > mesh.at(maxX).position.x)
			{
				maxX = i;
			}
			if (mesh.at(i).position.y > mesh.at(maxY).position.y)
			{
				maxY = i;
			}
			if (mesh.at(i).position.z > mesh.at(maxZ).position.z)
			{
				maxZ = i;
			}
		}


		DirectX::XMFLOAT3 max;
		DirectX::XMFLOAT3 min;
		max = DirectX::XMFLOAT3(mesh.at(maxX).position.x, mesh.at(maxY).position.y, mesh.at(maxZ).position.z);
		min = DirectX::XMFLOAT3(mesh.at(minX).position.x, mesh.at(minY).position.y, mesh.at(minZ).position.z);
		/*this->colBox.Max.x = max.x * this->Scale.x;
		this->colBox.Max.y = max.y * this->Scale.y;
		this->colBox.Max.z = max.z * this->Scale.z;
		this->colBox.Min.x = min.x * this->Scale.x;
		this->colBox.Min.y = min.y * this->Scale.y;
		this->colBox.Min.z = min.z * this->Scale.z;*/


		this->colBox.width = max.x;
		this->colBox.height = max.y;
		this->colBox.depth = max.z;
	}
	
	/*this->colBox.Min = min;
	this->colBox.Max = max;*/
}

void GameObject::addModel(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, bool hasSkeleton)
{
	this->theModel[nrOfModels] = new Model;
	this->theModel[nrOfModels]->setMesh(mesh, indices, numberOfIndices);
	//if(this->colBox.Max.x == 1.0f)
	this->calcAABB(mesh);
	//this->theModel[nrOfModels]->setSampler();
	this->theModel[nrOfModels]->setGotSkeleton(hasSkeleton);
	nrOfModels++;

}

void GameObject::setMesh(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, int id)
{
	this->theModel[id]->setMesh(mesh, indices, numberOfIndices);
}

void GameObject::setTexture(std::string file, int id, int mipLevels)
{									
	this->theModel[id]->setTexture(file, mipLevels);
}

void GameObject::setGlowMap(std::string file, int id)
{
	this->theModel[id]->setGlowMap(file);
}

void GameObject::draw()
{
	/*ForwardShader* ptr = nullptr;
	ptr = dynamic_cast<ForwardShader*>(this->theModel->getShader());
	if (ptr != nullptr)
	{
		ptr->setWorld(this->theTransforms.getWorld());
		this->theModel->draw();
	}*/
	/*for (int i = 0; i < this->nrOfModels; i++)
	{
		this->theModel[0]->getShader()->setWorld(this->theTransforms.getWorld());
		this->theModel[0]->draw();
	}*/



	this->theModel[0]->getShader()->setWorld(this->getWorld());
	this->theModel[0]->draw();

}

AABB GameObject::getCollisionBox()
{
	AABB tempBox;
	//updates the collision box
	/*tempBox.Max.x = tempBox.Max.x * this->Scale.x;
	tempBox.Max.y = tempBox.Max.y * this->Scale.y;
	tempBox.Max.z = tempBox.Max.z * this->Scale.z;

	tempBox.Min.x = tempBox.Min.x * this->Scale.x;
	tempBox.Min.y = tempBox.Min.y * this->Scale.y;
	tempBox.Min.z = tempBox.Min.z * this->Scale.z;

	tempBox.Max.x = this->colBox.Max.x + this->Position.x;
	tempBox.Max.y = this->colBox.Max.y + this->Position.y;
	tempBox.Max.z = this->colBox.Max.z + this->Position.z;

	tempBox.Min.x = this->colBox.Min.x + this->Position.x;
	tempBox.Min.y = this->colBox.Min.y + this->Position.y;
	tempBox.Min.z = this->colBox.Min.z + this->Position.z;
	*/  

	/*tempBox.Max.x = this->colBox.Max.x + this->Position.x;
	tempBox.Max.y = this->colBox.Max.y + this->Position.y;
	tempBox.Max.z = this->colBox.Max.z + this->Position.z;

	tempBox.Min.x = this->colBox.Min.x + this->Position.x;
	tempBox.Min.y = this->colBox.Min.y + this->Position.y;
	tempBox.Min.z = this->colBox.Min.z + this->Position.z;*/

	tempBox.width = this->colBox.width*this->Scale.x;
	tempBox.height = this->colBox.height*this->Scale.y;
	tempBox.depth = this->colBox.depth*this->Scale.z;
	return tempBox;
}

bool GameObject::haveAnimation() const
{
	bool result = false;
	if (anims.getName() != "Default")
	{
		result = true;
	}
	return result;
}

void GameObject::computeAnimationMatrix(float deltaTime)
{

	this->timePassed += deltaTime;
	if (this->timePassed >= anims.getDuration())
		this->timePassed = fmodf(this->timePassed,anims.getDuration());

	int k1 = (int)(this->timePassed* anims.getFPS());
	int k2 = std::min<int>(k1 + 1, anims.getKeyframes()[0].size());

	float k1_time = k1 / anims.getFPS();
	float k2_time = k2 / anims.getFPS();
	float t = (timePassed - k1_time) / (k2_time - k1_time);
	
	if (t > 1 || t < 0)
	{
		t = 0;
	}

	if (t != prevTimeIncrement)
	{
		prevTimeIncrement = t;
		
		std::vector<JointTransformation> pose;
		JointTransformation local1 = this->interpolate2(anims.getKeyframes()[0][0].getJointKeyFrames(), anims.getKeyframes()[0][1].getJointKeyFrames(),0.f);
		//JointTransformation test2 = temp.interpolate2(anims.getKeyframes()[k1][0].getJointKeyFrames(), anims.getKeyframes()[k2][0].getJointKeyFrames(), t);
		DirectX::XMMATRIX test1 = local1.getLocalTransform();
		pose.push_back(local1);
	

		std::vector<DirectX::XMMATRIX> jointTransforms;
		jointTransforms.resize(skeleton.size());
		DirectX::XMMATRIX test2 = pose[0].getLocalTransform();
		jointTransforms[0] = DirectX::XMMatrixMultiply(pose[0].getLocalTransform(), this->skeleton[0].getInverseBindTransform());
		for (int joint = 1; joint < skeleton.size(); joint++)
		{


			local1 = this->interpolate2(anims.getKeyframes()[joint][0].getJointKeyFrames(), anims.getKeyframes()[joint][1].getJointKeyFrames(), 0);
			pose.push_back(local1);

			pose[joint] = JointTransformation(DirectX::XMMatrixMultiply(pose[this->skeleton[joint].getParent()->getID()].getLocalTransform(), local1.getLocalTransform()));
			jointTransforms[joint] = DirectX::XMMatrixMultiply(pose[joint].getLocalTransform(), this->skeleton[joint].getInverseBindTransform());
		}

		DeferredShader* ptr = dynamic_cast<DeferredShader*>(System::shaderManager->getDefShader());
		if (ptr != nullptr)
		{
			ptr->setJointData(jointTransforms);

		}
		else
		{
			int i = 0;
			i = 1;
			OutputDebugStringA("== FAILED IN SET KEYFRAME to shader!! == ");
		}
	}

}

void GameObject::setNewAnimation(float fps, float duration, std::string name, std::vector<std::vector<Luna::Keyframe>> keyframePack)
{

	Animation newAnim(fps, duration, name, keyframePack.size(), keyframePack[0].size());
	newAnim.setKeyframes(keyframePack);
	this->anims = newAnim;

	//this->gotSkeleton = true;

	//DirectX::XMFLOAT4 pos1 = { 1,6,4,0 };
	//DirectX::XMFLOAT4 pos2 = { 1,9,6,0 };
	//DirectX::XMFLOAT4 rot1 = { 0.0507,-0.7052,-0.0507,0.7052 };
	//DirectX::XMFLOAT4 rot2 = { 0.0691,-0.7037,-0.0691,0.7037 };
	//DirectX::XMFLOAT4 scl1 = { 10,10,10,0 };
	//DirectX::XMFLOAT4 scl2 = { 10,10,10 ,0 };

	//JointTransformation temp1(pos1, rot1, scl1);
	//JointTransformation temp2(pos2, rot2, scl2);
		
	// keyframes involved.

	
	//DeferredShader* ptr;
	//if (ptr = dynamic_cast<DeferredShader*>(System::shaderManager->getDefShader()))
	//{
	//	//ptr->setJointData(jointTransforms, true);
	//}
	//else
	//{
	//	OutputDebugStringA("== FAILED IN SET KEYFRAME to shader!! == ");
	//}
	
}

void GameObject::setSkeleton(std::vector<Luna::Joint> theJoints)
{

	this->skeleton.resize(theJoints.size());
	for (int i = 0; i < theJoints.size(); i++)
	{
		this->skeleton[i]=theJoints[i];
		if (theJoints[i].parentID != -1)
			this->skeleton[i].setParent(&this->skeleton[theJoints[i].parentID]);
	}
	
}

DirectX::XMFLOAT3 GameObject::interpolate1(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression)
{
	DirectX::XMFLOAT3 interpol = { 0,0,0 };
	interpol.x = start.x + (end.x - start.x)*progression;
	interpol.y = start.y + (end.y - start.y)*progression;
	interpol.z = start.z + (end.z - start.z)*progression;
	return interpol;
}

DirectX::XMFLOAT3 GameObject::interpolate2(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, float progression)
{
	DirectX::XMFLOAT3 interpol = { 0,0,0 };
	interpol.x = start.x * (1 - progression) + end.x * progression;
	interpol.y = start.y * (1 - progression) + end.y * progression;
	interpol.z = start.z * (1 - progression) + end.z * progression;
	return interpol;
}

JointTransformation GameObject::interpolate1(JointTransformation frameA, JointTransformation frameB, float progression)
{
	DirectX::XMFLOAT3 pos = this->interpolate1(frameA.getPosition(), frameB.getPosition(), progression);
	DirectX::XMFLOAT4 pos4 = { pos.x, pos.y, pos.z, 0.f };
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.getRotation(), frameB.getRotation(), progression);
	//DirectX::XMFLOAT3 scale = this->interpolate(frameA.scale, frameB.scale, progression);
	DirectX::XMFLOAT3 scale = { 1.f,1.f,1.f };
	return JointTransformation(pos4, quaternion, scale);
}

JointTransformation GameObject::interpolate2(JointTransformation frameA, JointTransformation frameB, float progression)
{
	DirectX::XMFLOAT3 pos = this->interpolate2(frameA.getPosition(), frameB.getPosition(), progression);
	DirectX::XMFLOAT4 pos4 = { pos.x, pos.y, pos.z, 1.f };
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.getRotation(), frameB.getRotation(), progression);
	DirectX::XMFLOAT3 scale = this->interpolate2(frameA.getScale(), frameB.getScale(), progression);

	return JointTransformation(pos4, quaternion, scale);
}
