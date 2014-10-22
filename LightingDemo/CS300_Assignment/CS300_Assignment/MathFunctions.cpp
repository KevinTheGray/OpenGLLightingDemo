float get_max(float *values, unsigned size)
{
  float max = -100000000000.0f;
	for(unsigned i = 0; i < size; i++)
	{
		if(values[i] > max)
			max = values[i];
	}
	return max;
}

float get_min(float *values, unsigned size)
{
  float min = 100000000000.0f;
	for(unsigned i = 0; i < size; i++)
	{
		if(values[i] < min)
			min = values[i];
	}
	return min;
}