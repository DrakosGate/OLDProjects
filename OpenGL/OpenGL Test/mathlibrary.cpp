
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mathlibrary.h"


namespace Math
{

	Matrix Translate( const Matrix& input, const Vector3& position )
	{
		return glm::translate( input, position );
	}
	
	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis )
	{
		return glm::rotate( input, angle, axis );
	}
	
	Matrix Scale( const Matrix& input, const Vector3& scale )
	{
		return glm::scale( input, scale );
	}

}