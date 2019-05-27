#include "GameObject.h"
#include "System.h"

GameObject::GameObject()
{
	this->hasLoadedAABB = false;
	this->cap=5;
	this->nrOfModels = 0;
	shared_ptr<Model> m;
	this->theModel = m;
	this->colBox = AABB();
	//this->gotSkeleton = false;
	this->repeat = DirectX::XMFLOAT4(1, 1, 1, 1);
	this->timePassed = 0.f;
	this->frameCounter = 0;
	this->activeDraw = true;
	this->hitboxJointID = -1;
	this->hitboxJointID = 27;
}

GameObject::GameObject(Shader * shader)
{
	this->hasLoadedAABB = false;
	this->activeDraw = true;
	this->cap = 5;
	this->nrOfModels = 0;
	shared_ptr<Model> m;
	this->theModel = m;
	this->colBox = AABB();
	this->repeat = DirectX::XMFLOAT4(1, 1, 1, 1);
	this->timePassed = 0.f;
	//this->gotAnimation = false;
	this->frameCounter=0;
	this->hitboxJointID = -1;
	this->hitboxJointID = 27;

	//this->gotSkeleton = false;
	/*this->theModel[0] = new Model;
	this->theModel[0]->setShader(shader);
	this->nrOfModels++;*/

	//this->worldConstBuffer.initialize();
}


GameObject::~GameObject()
{

	if (theModel != NULL)
		this->theModel = nullptr;
}


int GameObject::getNrOfModels()
{
	return this->nrOfModels;
}

shared_ptr<Model>  GameObject::getModel()
{
	return this->theModel;
}

//Model **& GameObject::getTheModelPtr()
//{
//	return this->theModel;
//}

void GameObject::setHalfSize(float halfSize[3], float posOffset[3])
{
	
	this->hasLoadedAABB = true;
	this->colBox.width = halfSize[0];
	this->colBox.height = halfSize[1];
	this->colBox.depth = halfSize[2];
	this->colBox.offset = XMFLOAT3(posOffset[0], posOffset[1], posOffset[2]);
	
	/*check if collisionshape exist*/
	if (this->CollisionShape != nullptr)
	{
		System::getDebugDraw()->DeletePrimitiv(this->CollisionShape);
		delete	this->CollisionShape;
	}

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

void GameObject::addModel(shared_ptr<Model> m, bool gotSkeleton)
{
	this->theModel.reset();
	this->theModel = m;
	if (this->theModel->getShader() == nullptr)
	{
		if (this->theModel->getOpacity() == Transparent)
			this->theModel->setShader(System::shaderManager->getForwardShader());
		else
			this->theModel->setShader(System::shaderManager->getDefShader());
	}
	this->theModel->setGotSkeleton(gotSkeleton);
	nrOfModels = 1;
}

//void GameObject::addModel(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, bool hasSkeleton)
//{ //not using this???
//	shared_ptr<Model> m;
//	this->theModel = m;
//	this->theModel->setMesh(mesh, indices, numberOfIndices);
//	//if(this->colBox.Max.x == 1.0f)
//	this->calcAABB(mesh);
//	//this->theModel[nrOfModels]->setSampler();
//	this->theModel->setGotSkeleton(hasSkeleton);
//	nrOfModels = 1;
//
//}

//void GameObject::setMesh(std::vector<Vertex3D> mesh, DWORD * indices, int numberOfIndices, int id)
//{
//	this->theModel->setMesh(mesh);
//}

void GameObject::setTexture(std::string file, int id, int mipLevels)
{									
	this->theModel->setTexture(file, mipLevels);
}

void GameObject::setGlowMap(std::string file, int id)
{
	this->theModel->setGlowMap(file);
}

void GameObject::setMask(std::string file, int id)
{
	this->theModel->setMask(file);
}

void GameObject::setRepeat(float x, float y)
{
	this->repeat.x = x;
	this->repeat.y = y;
}

void GameObject::setColorMask(DirectX::XMFLOAT4 colorMask)
{
	this->colorMask = colorMask;
}

DirectX::XMFLOAT4 & GameObject::getColorMask()
{
	return this->colorMask;
}

DirectX::XMFLOAT4 & GameObject::getRepeat()
{
	return this->repeat;
}

//void GameObject::draw()
//{
//	/*ForwardShader* ptr = nullptr;
//	ptr = dynamic_cast<ForwardShader*>(this->theModel->getShader());
//	if (ptr != nullptr)
//	{
//		ptr->setWorld(this->theTransforms.getWorld());
//		this->theModel->draw();
//	}*/
//	/*for (int i = 0; i < this->nrOfModels; i++)
//	{
//		this->theModel[0]->getShader()->setWorld(this->theTransforms.getWorld());
//		this->theModel[0]->draw();
//	}*/
//
//
//
//	this->theModel->getShader()->setWorld(this->getWorld());
//	this->theModel->draw();
//
//}

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

void GameObject::setActiveDraw(bool arg)
{
	this->activeDraw = arg;
}

bool GameObject::getActiveDraw()
{
	return this->activeDraw;
}

bool GameObject::haveAnimation() const
{
	bool result = false;
	if (anims.empty() != true)
	{
		result = true;
	}
	return result;
}

void GameObject::computeAnimationMatrix(float deltaTime, std::string animName) //include float to multiply timePassed to make speed of animation (den ska gå från 0 - 1)
{
	this->timePassed += deltaTime;
	if (this->lastAnimation != animName)
	{
		this->timePassed = 0;
		this->lastAnimation = animName;
	}
	if (this->timePassed >= anims[animName].getDuration())
		this->timePassed = fmodf(this->timePassed,anims[animName].getDuration());

	this->frameCounter++;

	if (this->frameCounter >= 1) //frameskip (must never be zero, higher for lower quality)
	{
		int k1 = (int)(this->timePassed* anims[animName].getFPS());
		int k2 = std::min<int>(k1 + 1, anims[animName].getKeyframes()[0].size());

		float k1_time = k1 / anims[animName].getFPS();
		float k2_time = k2 / anims[animName].getFPS();
		float t = (timePassed - k1_time) / (k2_time - k1_time);

		//int timeStamp = int(10*(k1 + t)); //accuracy of deltaTime saved (lower for low quality, high for higher quality)

		//if (calculatedFrames[timeStamp].empty())
		//{

			DirectX::XMMATRIX sclMtx = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
			JointTransformation local;
			JointTransformation local_root = this->interpolate1(anims[animName].getKeyframes()[0][k1].getJointKeyFrames(), anims[animName].getKeyframes()[0][k2].getJointKeyFrames(), t);

			pose_global[0] = local_root.getLocalTransform();

			pose_global[0] = DirectX::XMMatrixMultiply(sclMtx, DirectX::XMMatrixTranspose(pose_global[0]));
			this->matrixPallete[0] = DirectX::XMMatrixMultiplyTranspose(pose_global[0], DirectX::XMMatrixTranspose(this->skeleton[0].getInverseBindTransform()));
			for (int joint = 1; joint < skeleton.size(); joint++)
			{

				local = this->interpolate1(anims[animName].getKeyframes()[joint][k1].getJointKeyFrames(), anims[animName].getKeyframes()[joint][k2].getJointKeyFrames(), t);

				pose_global[joint] = DirectX::XMMatrixMultiply(pose_global[this->skeleton[joint].getParent()->getID()], DirectX::XMMatrixTranspose(local.getLocalTransform()));
				this->matrixPallete[joint] = DirectX::XMMatrixMultiplyTranspose(pose_global[joint], DirectX::XMMatrixTranspose(this->skeleton[joint].getInverseBindTransform()));
			}

			//this->calculatedFrames[timeStamp] = matrixPallete;

	

			if(hitboxJointID!=-1)
				this->hitboxJointPos = pose_global[hitboxJointID];
			System::shaderManager->getDefShader()->setJointData(matrixPallete);
		//}
		//else //if the frames is already calculated
		//{
		//	System::shaderManager->getDefShader()->setJointData(calculatedFrames[timeStamp]);
		//}
		this->frameCounter = 0;
	}
}

void GameObject::setNewAnimation(float fps, float duration, std::string name, std::vector<std::vector<Luna::Keyframe>> keyframePack)
{

	Animation newAnim(fps, duration, name, keyframePack.size(), keyframePack[0].size());
	newAnim.setKeyframes(keyframePack);
	this->anims[name] = newAnim;

}

void GameObject::setSkeleton(std::vector<Luna::Joint> theJoints)
{
	//this->
	this->skeleton.resize(theJoints.size());
	for (int i = 0; i < theJoints.size(); i++)
	{
		this->skeleton[i]=theJoints[i];
		if (theJoints[i].parentID != -1)
			this->skeleton[i].setParent(&this->skeleton[theJoints[i].parentID]);

		if (this->skeleton[i].getName() == "name_of_joint") //get animation hitbox joint
			this->hitboxJointID = i;
	
	}
	//this->gotAnimation = true;
	this->pose_global.resize(skeleton.size());
	this->matrixPallete.resize(skeleton.size());
	
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
	DirectX::XMFLOAT4 pos4 = { pos.x, pos.y, pos.z, 1.f };
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionSlerp(frameA.getRotation(), frameB.getRotation(), progression);
	DirectX::XMFLOAT3 scale = this->interpolate1(frameA.getScale(), frameB.getScale(), progression);
	
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

bool GameObject::checkIfAnimExist(std::string animName)
{
	bool result = false;
	if (anims.find(animName)!=anims.end())
		result = true;
	return result;
}

DirectX::XMMATRIX GameObject::getJointPos() const
{
	return this->hitboxJointPos;
}
