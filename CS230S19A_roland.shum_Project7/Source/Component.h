#pragma once

typedef class Component* ComponentPtr;
typedef struct GameObject* GameObjectPtr;

class Component
{
public:
	enum TypeEnum
	{
		Transform,
		Physics,
		Behavior,
		Sprite,
		Collider,
		Animation,

		Max_Component_Type
	};
public:
	Component() = delete;
	Component(TypeEnum component, GameObjectPtr parent);
	virtual ~Component() = default;
	virtual ComponentPtr Clone(void) const = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

public:
	void SetParent(const GameObjectPtr parent);
	GameObjectPtr GetParent() const;
	TypeEnum GetComponent() const;
protected:
	TypeEnum componentType;
	GameObjectPtr parent;
};

