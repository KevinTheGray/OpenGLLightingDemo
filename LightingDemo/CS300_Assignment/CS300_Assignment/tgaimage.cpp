#include "tgaimage.h"
#include <stdio.h>
#include "string.h"

Image* Image::Load(const char* pName)
{
  //SFW_ASSERT_PARM(pName);

  Image* pImage = new Image;
  //SFW_ASSERT_ALLOC(pImage);

  // check the extension
  const char* pExt = pName + strlen(pName) - 4;
  //SFW_ASSERT_MESG(stricmp(pExt, ".TGA") == 0, "Can only load .TGA file!!");

  FILE* pFile;
  fopen_s(&pFile, pName, "rb");
  //SFW_ASSERT_MESG(pFile, "Cannot open '%s'\n", pName);

  // get the file size
  fseek(pFile, 0, SEEK_END);
  u32 fileSize = ftell(pFile);
  fseek(pFile, 0, SEEK_SET);

  u8* pFileData = new u8 [fileSize];
  //SFW_ASSERT_ALLOC(pFileData);

  // read in the data
  u32 readSize = fread(pFileData, 1, fileSize, pFile);
  //SFW_ASSERT_MESG(readSize == fileSize, "Error in reading '%s'\n", pName);

  if (pImage->loadTGA(pFileData, fileSize))
    return pImage;

  // deallocate the image
  delete pImage;

  return 0;
}

// ---------------------------------------------------------------------------

void Image::Free(Image* pImage)
{
  if (!pImage)
    return;

  // deallocate the image
  delete pImage;
}

// ---------------------------------------------------------------------------

Image::Image() :
mpData(0),
mBPP(0),
mSizeX(0),
mSizeY(0)
{
}

// ---------------------------------------------------------------------------

Image::~Image()
{
  if (mpData)
    delete [] mpData;
}

// ---------------------------------------------------------------------------

bool Image::loadTGA(u8* pData, u32 dataSize)
{
  //SFW_ASSERT_PARM(pData && dataSize);

  // get the image type
  u8 imageType;
  imageType = pData[2];
  //SFW_ASSERT_MESG(imageType == 2, "Only support uncompressed, true-color image");

  // get the bits per pixel
  mBPP = pData[16];
  //SFW_ASSERT_MESG((mBPP == 24) || (mBPP == 32), "Only support 24 or 32 bits image");

  // get image size
  mSizeX = (pData[13] << 8) | pData[12];
  mSizeY = (pData[15] << 8) | pData[14];

  // get the pointer to the image data area
  // * 18 is the header size
  // * the '0' entry is the number of bytes in the image id field (ignored!)
  u8* pImageData = pData + 18 + pData[0];

  // allocate memory for the data
  mpData = new u8 [mSizeX * mSizeY * mBPP / 8];
  //SFW_ASSERT_ALLOC(mpData);

  // get the image descriptor to get the orientation
  // * bit 5 (0 = bottom, 1 = top)
  // * bit 4 (0 = left    1 = right)
  u8  desc    = pData[17];
  u32 rowSize = mSizeX * mBPP / 8;

  // check if need to mirror the image vertically
  if ((desc & 0x20) == 0)
  {
    // mirror data upside down
    for (s32 y = 0; y < (int)mSizeY; ++y)
    {
      u32* pSrc = (u32*)(pImageData + y * rowSize);
      u32* pDst = (u32*)(mpData     + (mSizeY - 1 - y) * rowSize);

      memcpy(pDst, pSrc, rowSize);
    }
  }
  else
  {
    memcpy(mpData, pImageData, mSizeY * rowSize);
  }

  // check if need to mirror the image horizontally
  if (desc & 0x10)
  {
    for (s32 y = 0; y < (int)mSizeY; ++y)
    {
      for (s32 x = 0; x < (int)mSizeX / 2; ++x)
      {
        u8* pSrc = mpData + y * rowSize + x * mBPP / 8;
        u8* pDst = mpData + y * rowSize + (mSizeX - 1 - x) * mBPP / 8;

        pSrc[0] ^= pDst[0]; pDst[0] ^= pSrc[0]; pSrc[0] ^= pDst[0];
        pSrc[1] ^= pDst[1]; pDst[1] ^= pSrc[1]; pSrc[1] ^= pDst[1];
        pSrc[2] ^= pDst[2]; pDst[2] ^= pSrc[2]; pSrc[2] ^= pDst[2];

        if (mBPP == 32)
          pSrc[3] ^= pDst[3]; pDst[3] ^= pSrc[3]; pSrc[3] ^= pDst[3];
      }
    }
  }

  return true;
}
