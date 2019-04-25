

//void InputHandler::inPauseMode(int id)
//{
//	//only one player pauses and controlls the screen at a time
// 
//
//	//move camera
//	float stickAbsL = abs(this->theGamePad.GetState(id).thumbSticks.leftX);
//	if (stickAbsL > 0.f)
//	{
//		float dir = this->theGamePad.GetState(id).thumbSticks.leftX / stickAbsL;
//		//this->obj->move(dir * deltaTime, 0, 0);
//	}
//	else if (this->theGamePad.GetState(id).dpad.right || this->theGamePad.GetState(id).dpad.left)
//	{
//		//this->obj->move((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
//	}
//
//	//rotate camera
//	float rightStickX = this->theGamePad.GetState(id).thumbSticks.rightX;
//	float rightStickY = this->theGamePad.GetState(id).thumbSticks.rightY;
//	this->cameraRotation.x += -rightStickY;//* deltaTime;
//	this->cameraRotation.y += rightStickX;//* deltaTime;
//
//	//move selection
//
//	//zoom
//	if (theGamePad.GetState(id).buttons.x|| theGamePad.GetState(id).buttons.y)
//	{
//		//cameraPos
//
//		//this->obj->move((state.dpad.right - state.dpad.left) * deltaTime, 0, 0);
//	}
//
//	//unpause
//	if (this->theGamePad.GetState(id).buttons.menu || theGamePad.GetState(id).buttons.back)
//	{
//
//	}
//
//	//L+R+A+Start
//	//if (this->checkReset(state))
//	//{
//	//	theGamePad.SetVibration(0, 1.f, 1.f);
//	//}
//}
//
//
//
