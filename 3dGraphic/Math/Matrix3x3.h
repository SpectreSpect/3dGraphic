#pragma once
#pragma once
#include "float3.h"



struct Matrix3x3
{
public:
	float a11;
	float a12;
	float a13;

	float a21;
	float a22;
	float a23;

	float a31;
	float a32;
	float a33;

	void MakeUnit()
	{
		a11 = 1;
		a12 = 0;
		a13 = 0;

		a21 = 0;
		a22 = 1;
		a23 = 0;

		a31 = 0;
		a32 = 0;
		a33 = 1;
	}

	float3 operator *(float3 vector)
	{
		float3 vec;

		vec.x = a11 * vector.x + a12 * vector.y + a13 * vector.z;
		vec.y = a21 * vector.x + a22 * vector.y + a23 * vector.z;
		vec.z = a31 * vector.x + a32 * vector.y + a33 * vector.z;

		return vec;
	}
	Matrix3x3 operator *(Matrix3x3 matArg)
	{
		Matrix3x3 mat;

		mat.a11 = a11 * matArg.a11 + a12 * matArg.a21 + a13 * matArg.a31;
		mat.a12 = a11 * matArg.a12 + a12 * matArg.a22 + a13 * matArg.a32;
		mat.a13 = a11 * matArg.a13 + a12 * matArg.a23 + a13 * matArg.a33;


		mat.a21 = a21 * matArg.a11 + a22 * matArg.a21 + a23 * matArg.a31;
		mat.a22 = a21 * matArg.a12 + a22 * matArg.a22 + a23 * matArg.a32;
		mat.a23 = a21 * matArg.a13 + a22 * matArg.a23 + a23 * matArg.a33;

		mat.a31 = a31 * matArg.a11 + a32 * matArg.a21 + a33 * matArg.a31;
		mat.a32 = a31 * matArg.a12 + a32 * matArg.a22 + a33 * matArg.a32;
		mat.a33 = a31 * matArg.a13 + a32 * matArg.a23 + a33 * matArg.a33;

		return mat;
	}

	void RotateX(Matrix3x3* matrix, float angle)
	{
		Matrix3x3 tempMatrix{};
		tempMatrix.a11 = 1;
		tempMatrix.a12 = 0;
		tempMatrix.a13 = 0;

		tempMatrix.a21 = 0;
		tempMatrix.a22 = cos(angle);
		tempMatrix.a23 = -sin(angle);

		tempMatrix.a31 = 0;
		tempMatrix.a32 = sin(angle);
		tempMatrix.a33 = cos(angle);

		*matrix = tempMatrix * (*matrix);
	}

	void RotateY(Matrix3x3* matrix, float angle)
	{
		Matrix3x3 tempMatrix{};
		tempMatrix.a11 = cos(angle);
		tempMatrix.a12 = 0;
		tempMatrix.a13 = sin(angle);

		tempMatrix.a21 = 0;
		tempMatrix.a22 = 1;
		tempMatrix.a23 = 0;

		tempMatrix.a31 = -sin(angle);
		tempMatrix.a32 = 0;
		tempMatrix.a33 = cos(angle);

		*matrix = tempMatrix * (*matrix);
	}

	void RotateZ(Matrix3x3* matrix, float angle)
	{
		Matrix3x3 tempMatrix{};
		tempMatrix.a11 = cos(angle);
		tempMatrix.a12 = -sin(angle);
		tempMatrix.a13 = 0;

		tempMatrix.a21 = sin(angle);
		tempMatrix.a22 = cos(angle);
		tempMatrix.a23 = 0;

		tempMatrix.a31 = 0;
		tempMatrix.a32 = 0;
		tempMatrix.a33 = 1;

		*matrix = tempMatrix * (*matrix);
	}
};

