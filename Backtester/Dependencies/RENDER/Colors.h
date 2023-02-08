#pragma once

#include <glm\glm.hpp>

namespace Colors {
	const glm::vec4 cWhite = 	glm::vec4 ( 1.f, 1.f, 1.f, 1.f);
	const glm::vec4 cBlack = 	glm::vec4 ( 0.01f,0.01f, 0.01f, 1.f );
	const glm::vec4 cRed = 		glm::vec4 ( 1.f, 0.f, 0.f, 1.f );
	const glm::vec4 cGreen = 	glm::vec4 ( 0.f, 1.f, 0.f, 1.f );
	const glm::vec4 cBlue = 	glm::vec4 ( 0.f, 0.f, 1.f, 1.f );
	const glm::vec4 cPurple = 	glm::vec4 ( 1.f, 0.f, 1.f, 1.f );
	const glm::vec4 cCyan = 	glm::vec4 ( 0.f, 1.f, 1.f, 1.f);
	const glm::vec4 cOrange =	glm::vec4( 1.f, 0.64705882352f, 0.f, 1.f);
	const glm::vec4 cGray = 	glm::vec4 ( 5.f, .5f, .5f, 1.f);

	inline glm::vec4 Alpha(const glm::vec4& c, float alpha)
	{
		return glm::vec4(c.x, c.y, c.z, alpha);
	}
}