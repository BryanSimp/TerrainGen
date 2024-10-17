////////////////////////////////////////////////////////////////////////////////
// Filename: perlinclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "perlinclass.h"

float PerlinClass::perlin(float x, float z)
{
	//Determine grid cell corner coordinates
	int x0 = (int)x;
	int z0 = (int)z;
	int x1 = x0 + 1;
	int z1 = z0 + 1;

	// Compute Interpolation weights
	float sx = x - (float)x0;
	float sz = z - (float)z0;

	// Compute and interpolate top two corners
	float n0 = dotGridGradient(x0, z0, x, z);
	float n1 = dotGridGradient(x1, z0, x, z);
	float ix0 = interpolate(n0, n1, sx);

	// Compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, z1, x, z);
	n1 = dotGridGradient(x1, z1, x, z
	);
	float ix1 = interpolate(n0, n1, sx);

	// Final step: interpolate between the two previously interpolated values, now in y
	float value = interpolate(ix0, ix1, sz);

	return value;
}

// Computes the dot product of the distance and gradient vectors.
float PerlinClass::dotGridGradient(int ix, int iz, float x, float z) {
	// Get gradient from integer coordinates
	vector2 gradient = randomGradient(ix, iz);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dz = z - (float)iz;

	// Compute the dot-product
	return (dx * gradient.x + dz * gradient.z);
}

vector2 PerlinClass::randomGradient(int ix, int iz) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iz;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    vector2 v;
    v.x = sin(random);
    v.z = cos(random);

    return v;
}

float PerlinClass::interpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}