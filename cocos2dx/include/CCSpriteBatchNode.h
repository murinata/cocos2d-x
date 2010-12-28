/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_SPRITE_BATCH_NODE_H__
#define __CC_SPRITE_BATCH_NODE_H__

#include "CCNode.h"
#include "CCProtocols.h"
#include "CCTextureAtlas.h"
#include "ccMacros.h"
#include "NSMutableArray.h"
#include "CCXCocos2dDefine.h"

namespace cocos2d
{
	class CCSprite;

	/** CCSpriteBatchNode is like a batch node: if it contains children, it will draw them in 1 single OpenGL call
	* (often known as "batch draw").
	*
	* A CCSpriteBatchNode can reference one and only one texture (one image file, one texture atlas).
	* Only the CCSprites that are contained in that texture can be added to the CCSpriteBatchNode.
	* All CCSprites added to a CCSpriteBatchNode are drawn in one OpenGL ES draw call.
	* If the CCSprites are not added to a CCSpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient.
	*
	*
	* Limitations:
	*  - The only object that is accepted as child (or grandchild, grand-grandchild, etc...) is CCSprite or any subclass of CCSprite. eg: particles, labels and layer can't be added to a CCSpriteBatchNode.
	*  - Either all its children are Aliased or Antialiased. It can't be a mix. This is because "alias" is a property of the texture, and all the sprites share the same texture.
	* 
	* @since v0.7.1
	*/
	class CCX_DLL CCSpriteBatchNode : public CCNode, public CCTextureProtocol
	{
	public:
		~CCSpriteBatchNode();

		// property
        
		// retain
		inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; }
		inline void setTextureAtlas(CCTextureAtlas* textureAtlas) 
		{ 
			if (textureAtlas != m_pobTextureAtlas)
			{
				CCX_SAFE_RETAIN(textureAtlas);
				CCX_SAFE_RELEASE(m_pobTextureAtlas);
				m_pobTextureAtlas = textureAtlas;
			}
		}

		inline NSArray<CCSprite*>* getDescendants(void) { return m_pobDescendants; }

		/** creates a CCSpriteBatchNode with a texture2d and a default capacity of 29 children.
		The capacity will be increased in 33% in runtime if it run out of space.
		*/
		static CCSpriteBatchNode* batchNodeWithTexture(CCTexture2D *tex);
		static CCSpriteBatchNode* spriteSheetWithTexture(CCTexture2D *tex); // deprecated

		/** creates a CCSpriteBatchNode with a texture2d and capacity of children.
		The capacity will be increased in 33% in runtime if it run out of space.
		*/
		static CCSpriteBatchNode* batchNodeWithTexture(CCTexture2D* tex, unsigned int capacity);
		static CCSpriteBatchNode* spriteSheetWithTexture(CCTexture2D *tex, unsigned int capacity); // deprecated

		/** creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) with a default capacity of 29 children.
		The capacity will be increased in 33% in runtime if it run out of space.
		The file will be loaded using the TextureMgr.
		*/
		static CCSpriteBatchNode* batchNodeWithFile(const char* fileImage);
		static CCSpriteBatchNode* spriteSheetWithFile(const char* fileImage); // deprecated

		/** creates a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and capacity of children.
		The capacity will be increased in 33% in runtime if it run out of space.
		The file will be loaded using the TextureMgr.
		*/
		static CCSpriteBatchNode* batchNodeWithFile(const char* fileImage, unsigned int capacity);
		static CCSpriteBatchNode* spriteSheetWithFile(const char* fileImage, unsigned int capacity); // deprecated

		/** initializes a CCSpriteBatchNode with a texture2d and capacity of children.
		The capacity will be increased in 33% in runtime if it run out of space.
		*/
		bool initWithTexture(CCTexture2D *tex, unsigned int capacity);
		/** initializes a CCSpriteBatchNode with a file image (.png, .jpeg, .pvr, etc) and a capacity of children.
		The capacity will be increased in 33% in runtime if it run out of space.
		The file will be loaded using the TextureMgr.
		*/
		bool initWithFile(const char* fileImage, unsigned int capacity);

		void increaseAtlasCapacity();

		/** creates an sprite with a rect in the CCSpriteBatchNode.
		It's the same as:
		- create an standard CCSsprite
		- set the usingSpriteSheet = YES
		- set the textureAtlas to the same texture Atlas as the CCSpriteBatchNode
		@deprecated Use [CCSprite spriteWithBatchNode:rect:] instead;
		*/
		CCSprite* createSpriteWithRect(CGRect rect);

		/** initializes a previously created sprite with a rect. This sprite will have the same texture as the CCSpriteBatchNode.
		It's the same as:
		- initialize an standard CCSsprite
		- set the usingBatchNode = YES
		- set the textureAtlas to the same texture Atlas as the CCSpriteBatchNode
		@since v0.99.0
		@deprecated Use [CCSprite initWithBatchNode:rect:] instead;
		*/ 
		void initSprite(CCSprite *sprite, CGRect rect);

		/** removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
		@warning Removing a child from a CCSpriteBatchNode is very slow
		*/
		void removeChildAtIndex(unsigned int index, bool doCleanup);

		/** removes a child given a reference. It will also cleanup the running actions depending on the cleanup parameter.
		@warning Removing a child from a CCSpriteBatchNode is very slow
		*/
		void removeChild(CCSprite *sprite, bool doCleanup);

		void insertChild(CCSprite *child, unsigned int index);
		void removeSpriteFromAtlas(CCSprite *sprite);

		unsigned int rebuildIndexInOrder(CCSprite *parent, unsigned int index);
		unsigned int highestAtlasIndexInChild(CCSprite *sprite);
		unsigned int lowestAtlasIndexInChild(CCSprite *sprite);
		unsigned int atlasIndexForChild(CCSprite *sprite, int z);
		
		// CCTextureProtocol
	    virtual CCTexture2D* getTexture(void);
	    virtual void setTexture(CCTexture2D *texture);
	    virtual void setBlendFunc(ccBlendFunc blendFunc);
	    virtual ccBlendFunc getBlendFunc(void);

	    virtual void visit(void);
	    virtual void addChild(CCNode * child);
	    virtual void addChild(CCNode * child, int zOrder);
	    virtual void addChild(CCNode * child, int zOrder, int tag);
	    virtual void reorderChild(CCNode * child, int zOrder);
	        
	    virtual void removeChild(CCNode* child, bool cleanup);
	    virtual void removeAllChildrenWithCleanup(bool cleanup);
	    virtual void draw(void);

	private:
		void updateBlendFunc();

	private:
		CCTextureAtlas *m_pobTextureAtlas;
		ccBlendFunc m_blendFunc;

		// all descendants: chlidren, gran children, etc...
		NSArray<CCSprite*>* m_pobDescendants;
	};
}

#endif // __CC_SPRITE_BATCH_NODE_H__
