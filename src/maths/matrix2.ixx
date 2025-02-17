module;
export module treklair:matrix2;
import std;
import :vec2;

export struct Matrix2
{
	float data[4];

	static const Matrix2 Identity;

	Matrix2()
	{
		std::memcpy(data, Identity.data, 4 * sizeof(float));
	}

	Matrix2(float a, float b, float c, float d)
	{
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
	}

	constexpr float& operator()(unsigned int row, unsigned int column)
	{
		if (row >= 2 || column >= 2)
			throw std::out_of_range(
				"Invalid index : " + std::to_string(row) + "," + std::to_string(column) + " is out of range");
		return data[row * 2 + column];
	};
	
	float& operator[](unsigned int i)
	{
		return data[i];
	}


	Matrix2& operator=(const Matrix2& other)
	{
		memcpy(this->data, other.data, 4 * sizeof(float));
		return *this;
	}

	bool operator==(const Matrix2& other)
	{
		return areEquals(*this, other);
	}

	Matrix2 operator+(const Matrix2& other) const
	{
		return add(*this, other);
	}

	Matrix2& operator+=(const Matrix2& other)
	{
		*this = add(*this, other);
		return *this;
	}

	Matrix2 operator-(const Matrix2& other) const
	{
		return subtract(*this, other);
	}

	Matrix2& operator-=(const Matrix2& other)
	{
		*this = subtract(*this, other);
		return *this;
	}

	Matrix2 operator*(float scalar) const
	{
		return multiply(*this, scalar);
	}

	Matrix2& operator*=(float scalar)
	{
		*this = multiply(*this, scalar);
		return *this;
	}

	Vec2 operator*(const Vec2& vector) const
	{
		return multiply(*this, vector);
	}

	Matrix2 operator*(const Matrix2& other) const
	{
		return multiply(*this, other);
	}

	Matrix2& operator*=(const Matrix2& other)
	{
		*this = multiply(*this, other);
		return *this;
	}

	Matrix2 operator/(float scalar) const
	{
		return divide(*this, scalar);
	}

	Matrix2 operator/=(float scalar)
	{
		*this = divide(*this, scalar);
		return *this;
	}
	/**
	Matrix2 operator/(const Matrix2& other) const
	{
		return divide(*this, other);
	}

	Matrix2& operator/=(const Matrix2& other)
	{
		*this = divide(*this, other);
		return *this;
	}
	*/
	static bool areEquals(const Matrix2& left, const Matrix2& right)
	{
		return std::memcmp(&left, &right, 9 * sizeof(float)) == 0;
	}

	static Matrix2 add(const Matrix2& left, float scalar)
	{
		Matrix2 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += scalar;
		return result;
	}

	static Matrix2 add(const Matrix2& left, const Matrix2& right)
	{
		Matrix2 result(left);
		for (uint8_t i = 0; i < 9; ++i)
			result.data[i] += right.data[i];
		return result;
	}

	static Matrix2 subtract(const Matrix2& left, float scalar)
	{
		Matrix2 result(left);
		for (float& element : result.data)
		{
			element -= scalar;
		}
		return result;
	}

	static Matrix2 subtract(const Matrix2& left, const Matrix2& right)
	{
		Matrix2 result(left);
		for (uint8_t i = 0; i < 4; ++i)
			result.data[i] -= right.data[i];
		return result;
	}

	static Matrix2 multiply(const Matrix2& left, float scalar)
	{
		Matrix2 result(left);
		for (float& element : result.data)
		{
			element *= scalar;
		}
		return result;
	}

	static Vec2 multiply(const Matrix2& matrix, const Vec2& vector)
	{
		Vec2 result;
		result.x = (matrix.data[0] * vector.x) + (matrix.data[1] * vector.y);
		result.y = (matrix.data[2] * vector.x) + (matrix.data[3] * vector.y);

		return result;
	}

	static Matrix2 multiply(const Matrix2& left, const Matrix2& right)
	{
		return Matrix2(
			(left.data[0] * right.data[0]) + (left.data[1] * right.data[2]) ,
			(left.data[0] * right.data[1]) + (left.data[1] * right.data[3]),

			(left.data[2] * right.data[0]) + (left.data[3] * right.data[2]),
			(left.data[2] * right.data[1]) + (left.data[3] * right.data[3]));
	};

	static Matrix2 divide(const Matrix2& left, float scalar)
	{
		Matrix2 result(left);
		for (float& element : result.data)
		{
			element /= scalar;
		}
		return result;
	}
	/*
	static Matrix2 divide(const Matrix2& left, const Matrix2& right)
	{
		return left * inverse(right);
	}
	*/
	static bool isIdentity(const Matrix2& matrix)
	{
		return std::memcmp(Identity.data, matrix.data, 9 * sizeof(float)) == 0;
	}
	/*
	static float determinant(const Matrix2& matrix)
	{
		return matrix.data[0] * (matrix.data[4] * matrix.data[8] - matrix.data[5] * matrix.data[7])
			- matrix.data[3] * (matrix.data[1] * matrix.data[8] - matrix.data[2] * matrix.data[7])
			+ matrix.data[6] * (matrix.data[1] * matrix.data[5] - matrix.data[2] * matrix.data[4]);
	}*/

	static Matrix2 transpose(const Matrix2& matrix)
	{
		Matrix2 result;

		result.data[0] = matrix.data[0];
		result.data[1] = matrix.data[2];

		result.data[2] = matrix.data[1];
		result.data[3] = matrix.data[3];

		return result;
	}
	/*
	static Matrix2 cofactor(const Matrix2& matrix)
	{
		return Matrix2(
			((matrix.data[4] * matrix.data[8]) - (matrix.data[5] * matrix.data[7])), //0
			-((matrix.data[3] * matrix.data[8]) - (matrix.data[5] * matrix.data[6])), //1
			((matrix.data[3] * matrix.data[7]) - (matrix.data[4] * matrix.data[6])), //2
			-((matrix.data[1] * matrix.data[8]) - (matrix.data[2] * matrix.data[7])), //3
			((matrix.data[0] * matrix.data[8]) - (matrix.data[2] * matrix.data[6])), //4
			-((matrix.data[0] * matrix.data[7]) - (matrix.data[1] * matrix.data[6])), //5
			((matrix.data[1] * matrix.data[5]) - (matrix.data[2] * matrix.data[4])), //6
			-((matrix.data[0] * matrix.data[5]) - (matrix.data[2] * matrix.data[3])), //7 
			((matrix.data[0] * matrix.data[4]) - (matrix.data[1] * matrix.data[3]))); //8
	}

	static Matrix2 minor(const Matrix2& matrix)
	{
		return Matrix2(
			((matrix.data[4] * matrix.data[8]) - (matrix.data[5] * matrix.data[7])), //0
			((matrix.data[3] * matrix.data[8]) - (matrix.data[5] * matrix.data[6])), //1
			((matrix.data[3] * matrix.data[7]) - (matrix.data[4] * matrix.data[6])), //2
			((matrix.data[1] * matrix.data[8]) - (matrix.data[2] * matrix.data[7])), //3
			((matrix.data[0] * matrix.data[8]) - (matrix.data[2] * matrix.data[6])), //4
			((matrix.data[0] * matrix.data[7]) - (matrix.data[1] * matrix.data[6])), //5
			((matrix.data[1] * matrix.data[5]) - (matrix.data[2] * matrix.data[4])), //6
			((matrix.data[0] * matrix.data[5]) - (matrix.data[2] * matrix.data[3])), //7 
			((matrix.data[0] * matrix.data[4]) - (matrix.data[1] * matrix.data[3]))); //8
	}

	static Matrix2 adjoint(const Matrix2& other)
	{
		return transpose(cofactor(other));
	}
	
	static Matrix2 inverse(const Matrix2& matrix)
	{
		const float det = determinant(matrix);
		if (det == 0)
			throw std::logic_error("Division by 0");

		return adjoint(matrix) / det;
	}
	*/

	static Matrix2 rotation(float rotation)
	{
		return Matrix2(
			std::cos(rotation), -std::sin(rotation),
			std::sin(rotation), std::cos(rotation));
	}

	static Matrix2 rotate(const Matrix2& matrix, float _rotation)
	{
		return matrix * rotation(_rotation);
	}

	static Matrix2 scaling(const Vec2& scale)
	{
		return Matrix2(
			scale.x, 0,
			0, scale.y);
	}

	static Matrix2 scale(const Matrix2& matrix, const Vec2& scale)
	{
		return matrix * scaling(scale);
	}

	static Matrix2 invInertiaTensorBox(float invMass, float width, float height)
	{
		Matrix2 mat;
		float scalar = 1.0 / 12.0 * invMass;
		mat[0] = scalar * (1.0 / height);
		mat[3] = scalar * (1.0 / width);
		return mat;
	}

	static Matrix2 invInertiaTensorCircle(float invMass, float radius)
	{
		Matrix2 mat;
		float scalar = 4 * invMass * (1.0 / (radius * radius));
		mat[0] = scalar;
		mat[3] = scalar;
		return mat;
	}
};

const Matrix2 Matrix2::Identity = Matrix2(
	1.0f, 0.0f,
	0.0f, 1.0f);