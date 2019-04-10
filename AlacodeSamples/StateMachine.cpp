#include "StateMachine.h"

void StateMachine::addState(StateRef newState, bool isReplacing)
{
	if (!this->_states.empty())
	{
		if (isReplacing)
		{
			this->_states.pop();
		}
		else
		{
			this->_states.top()->Pause();
		}
	}
	this->_newState = std::move(newState);
	this->_states.push(std::move(this->_newState));
	this->_states.top()->Initialize();
}

void StateMachine::RemoveState()
{
	if (!this->_states.empty())
	{
		this->_states.pop();
		if (!this->_states.empty())
		{
			this->_states.top()->Resume();
		}
	}
}

StateRef & StateMachine::GetActiveState()
{
	return this->_states.top();
}
