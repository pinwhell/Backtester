#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Maths
{
	void Rotate(glm::vec3& vec, float angle);
	void Rotate(glm::vec2& vec, float angle);
	void Rotate(glm::vec2& vec, const glm::quat& rot);
	void Rotate(glm::vec2& vec, const glm::vec3& frwd);
	void Rotate(glm::vec2& vec, const glm::vec2& frwd);
	void ReverseVec(const glm::vec4& vecIn, glm::vec4& result);
	void ExtractVec(const glm::vec4& vecInA, const glm::vec4& vecInB,  glm::vec4& result, uint8_t start);
	glm::vec4 ReverseVec(const glm::vec4& vecIn);
	glm::vec4 ExtractVec(const glm::vec4& vecInA, const glm::vec4& vecInB, uint8_t start);
	float Signed2DTriArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	bool IntersectSegments2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d, glm::vec2& p, float* pt = nullptr);
	float Lerp1D(float a, float b, float f);

	glm::quat RandQuat();

	class BezierManager {
		private:

		glm::quat mlastP2Rand;
		glm::quat mlastP3Rand;

		glm::quat mStartQuat;
		glm::quat p2;
		glm::quat p3;
		glm::quat mEndQuat;

		bool bHadAppliedQuats;
		
		public:
		
		BezierManager()
			: bHadAppliedQuats(false)
		{
			RandomizeQuatSeed();
		}

		bool getHadAppliedQuats()
		{
			return bHadAppliedQuats;
		}

		void setHadAppliedQuats(bool b)
		{
			bHadAppliedQuats = b;
		}

		void RandomizeQuatSeed()
		{
			mlastP2Rand = RandQuat();
			mlastP3Rand = RandQuat();
		}

		void setStartQuat(const glm::quat& q)
		{
			mStartQuat = q;
		}

		void RandomizeBasedOnRot(const glm::quat& rot, const float t)
		{
			p2 = glm::normalize(glm::slerp(rot, mlastP2Rand, t));
			p3 = glm::normalize(glm::slerp(rot, mlastP3Rand, t));
			mEndQuat = rot;
		}

		glm::quat getPointAt(float t)
		{
			glm::quat p1p2 = glm::slerp(mStartQuat, p2, t);
			glm::quat p2p3 = glm::slerp(p2, p3, t);
			glm::quat p3p4 = glm::slerp(p3, mEndQuat, t);
			glm::quat p1p2p2p3 = glm::slerp(p1p2, p2p3, t);
			glm::quat p2p3p3p4 = glm::slerp(p2p3, p3p4, t);

			return glm::slerp(p1p2p2p3, p2p3p3p4, t);
		}
	};

	class PlayerBezierManager : public BezierManager{
		glm::quat mAimingPlayerHeadRot;
		float mNoiseFromHeadRot; 	// Goes from 0.f to 1.f, where 0.f
									// start at aimbot enemy head, and 1.f
									// is the randomly generated quaternion

		public: 
		PlayerBezierManager()
			: mNoiseFromHeadRot(0.2f)
		{}

		void setPlayerHeadRot(const glm::quat& aimingPlayerHead)
		{
			mAimingPlayerHeadRot = aimingPlayerHead;
		}
		
		void setNoiseFromHeadRot(const float noise)
		{
			mNoiseFromHeadRot = noise;
		}

		void Update()
		{
			RandomizeBasedOnRot(mAimingPlayerHeadRot, mNoiseFromHeadRot);
		}
	};
}
