//perlin.h
//Perlin Noise Generator
//Ported by ???
//Cleaned up by Kryssa@SWmud, September 2024

#ifndef _PERLIN_H
#define _PERLIN_H

int seed_next_perlin (int seed);
int srandom_perlin (int seed, int min, int max);
mixed *sshuffle_perlin (int seed, mixed *arr);
int *perlin_generate_permutation (int seed);
varargs float perlin_noise_2d (float x, float y, int *p, int octaves, float scale);


int seed_next_perlin (int seed) {
  return (seed * 1664525 + 1013904223) & 0x7fffffff;
}

int srandom_perlin (int seed, int min, int max) {
  return (seed % (max - min)) + min;
}

mixed *sshuffle_perlin (int seed, mixed *arr) {
  mixed *new_arr = ({ });
  
  int num_elements = sizeof(arr);
  
  for (int i = 0; i < num_elements; i++) {
    int j = srandom_perlin(seed, 0, sizeof(arr));
    seed = seed_next_perlin(seed);
    new_arr += ({ arr[j] });
    arr -= ({ arr[j] });
  }
  
  return new_arr;
}

private int *permutation = ({
  151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
  8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,
  117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,
  165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
  105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
  187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,
  3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,
  227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
  221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,
  185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
  81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,
  115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,
  195,78,66,215,61,156,180
});

private float fade(float t) {
  return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

private float lerp(float t, float a, float b) {
  return a + t * (b - a);
}

private float grad(int hash, float x, float y) {
  int h = hash & 15;
  float u = h < 8 ? x : y;
  float v = h < 4 ? y : h == 12 || h == 14 ? x : 0.0;
  
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

private float noise2D(float x, float y, int *p) {
  float u, v;
  int A, B, X, Y, A1, B1;
  X = to_int(floor(x)) & 255;
  Y = to_int(floor(y)) & 255;
  x -= floor(x);
  y -= floor(y);
  u = fade(x);
  v = fade(y);
  A = (p[X] + Y) & 255;
  B = (p[(X + 1) & 255] + Y) & 255;
  A1 = (A + 1) & 255;
  B1 = (B + 1) & 255;
  
  return lerp(v, lerp(u, grad(p[A], x, y), grad(p[B], x - 1.0, y)),
                 lerp(u, grad(p[A1], x, y - 1.0), grad(p[B1], x - 1.0, y - 1.0)));
}

int *perlin_generate_permutation (int seed) {
  return sshuffle_perlin(seed, permutation);
}

varargs float perlin_noise_2d (float x, float y, int *p, int octaves, float scale) {
  float total = 0.0;
  float amplitude = 1.0;

  if (!octaves) {
    octaves = 4;
  }

  if (!scale) {
    scale = 1.0;
  }

  for (int i = 0; i < octaves; i++) {
    total += noise2D(x * scale, y * scale, p) * amplitude;
    scale *= 2.0;
    amplitude *= 0.5;
  }

  return total;
}

#endif