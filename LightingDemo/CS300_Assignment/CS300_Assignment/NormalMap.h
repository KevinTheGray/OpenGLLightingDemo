#pragma once
#include "Utilities.h"
#include "tgaimage.h"
#include "Vector4.h"
class NormalMap
{
public:
  static NormalMap*	Generate (Image* height_map, float scale);
  static void		Free	(NormalMap *normal_map);
  static void   Regenerate(NormalMap *map, float scale);

  const u8*		NormalData	() const	{	return mpNormalData;	}
  u8*			    NormalData	()			{	return mpNormalData;	}
  const u8*		ImageData	() const	{	return mpImageData;	}
  u8*			    ImageData	()			{	return mpImageData;	}
  u32				      SizeX	() const	{	return mSizeX;	}
  u32				      SizeY	() const	{	return mSizeY;	}

private:
  u8*		mpNormalData;
  u8*   mpImageData;
  u32				mSizeX;
  u32				mSizeY;

  // ---------------------------------------------------------------------------

  NormalMap();
  NormalMap(const NormalMap& rhs);
  //Returns the data for the Normal Map
  u8* CreateNormalMap(float scale);
  u8* StoreImageData(Image *height_map);
  ~NormalMap();

  // ---------------------------------------------------------------------------
};
