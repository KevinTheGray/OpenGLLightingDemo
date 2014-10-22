#ifndef TGAIMAGE_H
#define TGAIMAGE_H

// ---------------------------------------------------------------------------
// typedef for basic variable types
/////////////////////////////////////////////////////////////////////////////
#include "Utilities.h"
class Image
{
public:
  static Image*	Load	(const char* pName);
  static void		Free	(Image* pImage);

  const u8*		Data	() const	{	return mpData;	}
  u8*				Data	()			{	return mpData;	}
  u32				BPP		() const	{	return mBPP;	}
  u32				SizeX	() const	{	return mSizeX;	}
  u32				SizeY	() const	{	return mSizeY;	}

private:
  u8*				mpData;
  u32				mBPP;
  u32				mSizeX;
  u32				mSizeY;

  // ---------------------------------------------------------------------------

  Image	();
  Image	(const Image& rhs);
  ~Image	();

  // ---------------------------------------------------------------------------

  bool			loadTGA	(u8* pData, u32 dataSize);
};

#endif // TGAIMAGE_H
