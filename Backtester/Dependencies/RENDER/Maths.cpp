#include "Maths.h"
#include <glm/gtc/quaternion.hpp>
#include <RandomHelper.h>

void Maths::Rotate(glm::vec3 & vec, float angle)
{
	glm::vec3 tmp_vec = vec;
	float rad = glm::radians(angle);
	float s = sin(rad);
	float c = cos(rad);

	vec.x = tmp_vec.x * c - tmp_vec.z * s;
	vec.z = tmp_vec.x * s + tmp_vec.z * c;
}

void Maths::Rotate(glm::vec2 & vec, float angle)
{
	float rad = glm::radians(angle);
	float s = sin(rad);
	float c = cos(rad);

	Rotate(vec, glm::vec2(c,s));
}

void Maths::Rotate(glm::vec2& vec, const glm::quat& rot)
{
	glm::vec3 frwd = rot * glm::vec3(0.f, 0.f, 1.f);

	Rotate(vec, frwd);
}

void Maths::Rotate(glm::vec2& vec, const glm::vec3& frwd)
{
	glm::vec2 tmp_vec = vec;

	vec.x = tmp_vec.x * frwd.x - tmp_vec.y * frwd.z;
	vec.y = tmp_vec.x * frwd.z + tmp_vec.y * frwd.x;
}

void Maths::Rotate(glm::vec2& vec, const glm::vec2& frwd)
{
	glm::vec2 tmp_vec = vec;

	vec.x = tmp_vec.x * frwd.x - tmp_vec.y * frwd.y;
	vec.y = tmp_vec.x * frwd.y + tmp_vec.y * frwd.x;
}

void Maths::ReverseVec(const glm::vec4& vecIn, glm::vec4& result)
{
	result.x = vecIn.y;
	result.y = vecIn.x;
	result.z = vecIn.w;
	result.w = vecIn.z;
}

void Maths::ExtractVec(const glm::vec4& vecInA, const glm::vec4& vecInB,  glm::vec4& result, uint8_t start)
{
    if(start < 4 && start >= 0)
    {
        for(int ctr = 0, i = start; ctr < 4; i++, ctr++)
            result[ctr] = (i < 4) ? vecInA[i] : vecInB[i - 4];
    }
}

glm::vec4 Maths::ReverseVec(const glm::vec4& vecIn)
{
	glm::vec4 result(0.f);

	ReverseVec(vecIn, result);

	return result;
}

glm::vec4 Maths::ExtractVec(const glm::vec4& vecInA, const glm::vec4& vecInB, uint8_t start)
{
	glm::vec4 result(0.f);

	ExtractVec(vecInA, vecInB, result, start);

	return result;
}

float Maths::Signed2DTriArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

bool Maths::IntersectSegments2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, glm::vec2& p, float* pt)
{
	// signs of areas correspond to which side of ab points c and d are
	float a1 = Signed2DTriArea(a,b,d); // Compute winding of abd (+ or -)
	float a2 = Signed2DTriArea(a,b,c); // To intersect, must have sign opposite of a1

	// If c and d are on different sides of ab, areas have different signs
	if( a1 * a2 < 0.0f ) // require unsigned x & y values.
	{
		float a3 = Signed2DTriArea(c,d,a); // Compute winding of cda (+ or -)
		float a4 = a3 + a2 - a1; // Since area is constant a1 - a2 = a3 - a4, or a4 = a3 + a2 - a1

		// Points a and b on different sides of cd if areas have different signs
		if( a3 * a4 < 0.0f )
		{
			float t = 0.f;
			// Segments intersect. Find intersection point along L(t) = a + t * (b - a).
			t = a3 / (a3 - a4);
			p = a + t * (b - a); // the point of intersection

			if(pt)
				*pt = t;

			return true;
		}
	}

	// Segments not intersecting or collinear
	return false;
}

namespace Maths
{
	float Lerp1D(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	
	glm::quat RandQuat() {
		

		glm::quat randQuat = glm::quat(
			glm::vec3(
				RandomHelper::getRandomInBtw(0.f, 360.f),
				RandomHelper::getRandomInBtw(0.f, 180.f),
				0.f
			)
		);

		return  glm::normalize(randQuat);
	}
}