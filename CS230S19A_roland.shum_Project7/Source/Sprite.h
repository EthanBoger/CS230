//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxVertexList AEGfxVertexList;
typedef struct SpriteSource * SpriteSourcePtr;
typedef struct Sprite Sprite;
typedef struct Sprite * SpritePtr;
typedef struct Transform * TransformPtr;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
struct Sprite
{
private:
	// The name of the sprite.
	const char * name;

	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	SpriteSourcePtr spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList * mesh;

	// Zero-terminated string used to display sprite text.
	const char * text;

public:
	Sprite(const char * name);
	Sprite(const Sprite& other);
	~Sprite() = default;

	void setAlpha(float newAlpha);
	float getAlpha() const;

	void setFrame(unsigned frameIndex);
	void setMesh(AEGfxVertexList *mesh);
	void setSpriteSource(SpriteSourcePtr spriteSource);
	void setText(const char * text);

	void Draw(TransformPtr transform);
};


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------