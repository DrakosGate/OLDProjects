#pragma once

// Library Includes
#include <glm/glm.hpp>

namespace Math
{

	// Type definitions
	typedef glm::mat4 Matrix;
	typedef glm::quat Quat;
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	// Function declarations
	Matrix Translate( const Matrix& input, const Vector3& position );
	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis );
	Matrix Scale( const Matrix& input, const Vector3& scale );

}