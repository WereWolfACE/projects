#pragma once

inline void sort2(float& min, float& max){
	if ( min > max ){
		float tmp = min;
		min = max;
		max = tmp;
	}
}

inline void expand(float& min, float& max, float newMin, float newMax){
	if ( newMin < min )
		min = newMin;
	if ( newMax > max )
		max = newMax;
}

inline void expand1(float& min, float& max, float x){
	if ( x < min )
		min = x;
	if ( x > max )
		max = x;
}

inline void sort2AndSet(float& min, float& max, float x, float y){
	min = x;
	max = y;
	sort2(min, max);
}

struct MatrixDecomposition{
	float shear;
	float scaleX, scaleY;
	float rotation;
};

extern float identityMatrix[6];

void multMatrix3x3(float dest[6], const float src1[6], const float src2[6]);
bool matrixIdentity(const float m[6]);

MatrixDecomposition decompose(const float* matrix);
void recompose(float* matrix, const MatrixDecomposition& decomp);
