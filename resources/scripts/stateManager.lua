StateManger = {}

StateManager.states = {}

function StateManager:Update(entitySize)
	StateManager:LoadStates()
	for i = 0, entitySize - 1 do
		id = assert(Get(i))
		
		if (HasState(id)) then
			type = GetStateType(id)
			State = GetState(id)
			
			if (GetStart(id)) then
				self.states[type][state].enter(id)
				SetStartState(id, false)
			end
			
			if (IsSwitchingState(id)) then
				StateManager:ChangeState(id, GetState(id))
			end
			
			self.states[type][state].execute(id)
		end
	end
end

function StateManager:LoadState()
	local stateTable = assert(dofile("./resources/scripts/states.lua"))
	for k, v, in pairs(stateTable) do
		self.states[k] = v
	end
end

function StateManager:ChangeState(id, newState)
	type = GetStateType(id)
	state = GetState(id)
	self.states[type][state].exit(id)
	SetState(id, newState)
	SetStartState(id, true)
end

CallUpdate = function(entitySize)
	StateManager:Update(entitySize)
end