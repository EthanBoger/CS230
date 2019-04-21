#include "stdafx.h"
#include "Component.h"


Component::Component(TypeEnum component, GameObjectPtr parent) :
	componentType(component), parent(parent)
{
}

void Component::SetParent(const GameObjectPtr parent)
{
	this->parent = parent;
}

GameObjectPtr Component::GetParent() const
{
	return parent;
}

Component::TypeEnum Component::GetComponent() const
{
	return componentType;
}