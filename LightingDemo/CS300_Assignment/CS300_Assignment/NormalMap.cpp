#include "NormalMap.h"

NormalMap* NormalMap::Generate(Image *height_map, float scale)
{

  //Getting a gray scale height map, lets save the height map, and generate the normal map
  NormalMap *map = new NormalMap;
  map->mSizeX = height_map->SizeX();
  map->mSizeY = height_map->SizeY();
  map->mpImageData  = map->StoreImageData(height_map);
  map->mpNormalData = map->CreateNormalMap(scale);
  return map;
}

void NormalMap::Regenerate(NormalMap *map, float scale)
{
  if (map->mpNormalData)
    delete [] map->mpNormalData;
  map->mpNormalData = map->CreateNormalMap(scale);
}
u8* NormalMap::StoreImageData(Image *height_map)
{
  unsigned height_map_size = height_map->SizeX() * height_map->SizeY() * 3;
  u8 *image_data = new u8[height_map_size];
  //memcpy(image_data, height_map->Data(), height_map_size);
  u32 width = height_map->SizeX();
  u32 height = height_map->SizeY();
  u8 *data = height_map->Data();
  unsigned count = 0;
  for(unsigned i = 0; i < width; i++)
  {
    for(unsigned j = 0; j < (height * 3); j+=3)
    {
      int cur_pixel = (i * width * 3) + j + 0;
      image_data[cur_pixel + 0] = data[cur_pixel + 0];
      image_data[cur_pixel + 1] = data[cur_pixel + 1];
      image_data[cur_pixel + 2] = data[cur_pixel + 2];
      count += 3;
    }
  }
  std::cout << count << std::endl;
  return image_data;
}


u8* NormalMap::CreateNormalMap(float scale)
{
  unsigned height_map_size = mSizeX * mSizeY * 3;
  u8 *map_data = new u8[height_map_size];

  //Generate normal map
  u32 width = mSizeX;
  u32 height = mSizeY;
  u8 *data = mpImageData;
  unsigned lc = 0,rc = 0,ac = 0,bc = 0;
  for(unsigned i = 0; i < width; i++)
  {
    for(unsigned j = 0; j < (height * 3); j+=3)
    {
      //Access texel (i,j/3)
      int cur_pixel = (i * width * 3) + j + 0;
      //Above
      int above_pixel = ((i-1) * width * 3) + j;
      if(i == 0)
      {
        above_pixel = cur_pixel;
        ac++;
      }
      //Below
      int below_pixel = ((i+1) * width * 3) + j;
      if(i == width - 1)
      {
        below_pixel = cur_pixel;
        bc++;
      }
      //Right
      int right_pixel = (i * width * 3) + (j + 3);
      if(j + 2 == (height * 3 - 1))
      {
        right_pixel = cur_pixel;
        rc++;
      }
      //Left;
      int left_pixel = (i * width * 3) + (j - 3);
      if(j == 0)
      {
        left_pixel = cur_pixel;
        lc++;
      }

      Vector4 s(1.0f, 0.0f, scale * (data[right_pixel] - data[left_pixel]));
      Vector4 t(0.0f, 1.0f, scale * (data[below_pixel] - data[above_pixel]));
      Vector4 N = s.Cross(t);
      N.Normalize();
      N.r = 255.0 * 0.5 * (N.x + 1);
      N.g = 255.0 * 0.5 * (N.y + 1);
      N.b = 255.0 * 0.5 * (N.z + 1);
      map_data[cur_pixel + 0] = N.r;
      map_data[cur_pixel + 1] = N.g;
      map_data[cur_pixel + 2] = N.b;
      //map_data[cur_pixel + 0] = 0;
      //map_data[cur_pixel + 1] = 0;
      ///map_data[cur_pixel + 2] = 0;
      //std::cout << (int)height_map->Data()[cur_pixel] << std::endl;
    }

  }
  //memcpy(map_data, mpImageData, height_map_size);
  return map_data;
  //return mpImageData; 
}
void NormalMap::Free(NormalMap* normal_map)
{
  if (!normal_map)
    return;

  // deallocate the image
  delete normal_map;
}

NormalMap::NormalMap() :
mpNormalData(0),
mpImageData(0),
mSizeX(0),
mSizeY(0)
{
}

NormalMap::~NormalMap()
{
  if (mpNormalData)
    delete [] mpNormalData;
  if(mpImageData)
    delete[] mpImageData;
}