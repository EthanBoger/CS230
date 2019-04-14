//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
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

typedef struct AEGfxTexture AEGfxTexture;
typedef struct AEGfxTexture *AEGfxTexturePtr;

// Pointer to a SpriteSource structure.
typedef struct SpriteSource SpriteSource;
typedef struct SpriteSource * SpriteSourcePtr;

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in SpriteSource.c.

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
struct SpriteSource
{
private:
	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexturePtr	pTexture;
public:
	SpriteSource(int numCols, int numRows, AEGfxTexturePtr pTexture);
	~SpriteSource() = default;

	AEGfxTexturePtr getTexture() const;
	unsigned int getFrameCount() const;
	void getUV(unsigned int frameIndex, float * u, float * v);
};
