#ifndef STATES_H
#define STATES_H
#include <memory>
#include <stack>

class State
{
public:
	virtual void Initialize() = 0;
	virtual void HandleInput() = 0;
	virtual void update(float dt) = 0;
	virtual void Draw(float dt) = 0;
	virtual void Pause() {}
	virtual void Resume() {}
private:
};

typedef std::unique_ptr<State> StateRef;

class StateMachine
{
public:
	StateMachine() {}
	~StateMachine() {}
	void addState(StateRef newState, bool isReplacing = true);
	void RemoveState();
	StateRef &GetActiveState();

private:
	std::stack<StateRef> _states;
	StateRef _newState;
};


#endif // !STATES_H
