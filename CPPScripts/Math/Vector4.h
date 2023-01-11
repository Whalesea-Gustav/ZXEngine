#pragma once
#include <string>

namespace ZXEngine
{
	class Vector2;
	class Vector3;
	class Matrix4;
	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;
		float& r = x;
		float& g = y;
		float& b = z;
		float& a = w;

		Vector4();
		Vector4(float n);
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector2& v2);
		Vector4(const Vector2& v2l, const Vector2& v2r);
		Vector4(const Vector2& v2, float z, float w);
		Vector4(const Vector3& v3);
		Vector4(const Vector3& v3, float w);
		Vector4(const Vector4& v);

		Vector4 Normalize() const;
		void ToArray(float* array) const;
		std::string ToString() const;
		float GetMagnitude() const;

		float& operator[] (int i);
		const float& operator[] (int i) const;
		bool operator== (const Vector4& v) const;
		bool operator!= (const Vector4& v) const;
		Vector4& operator= (const Vector4& v);
		Vector4 operator+ (const Vector4& v) const;
		Vector4 operator- (const Vector4& v) const;
		Vector4 operator* (const Vector4& v) const;
		Vector4 operator/ (const Vector4& v) const;
		Vector4& operator+= (const Vector4& v);
		Vector4& operator-= (const Vector4& v);
		Vector4& operator*= (const Vector4& v);
		Vector4& operator/= (const Vector4& v);
		Vector4 operator+ (float n) const;
		Vector4 operator- (float n) const;
		Vector4 operator* (float n) const;
		Vector4 operator/ (float n) const;
		Vector4& operator+= (float n);
		Vector4& operator-= (float n);
		Vector4& operator*= (float n);
		Vector4& operator/= (float n);
		friend Vector4 operator+ (float n, const Vector4& v);
		friend Vector4 operator- (float n, const Vector4& v);
		friend Vector4 operator* (float n, const Vector4& v);
		friend Vector4 operator/ (float n, const Vector4& v);
		Vector4 operator* (const Matrix4& mat) const;
	};
}