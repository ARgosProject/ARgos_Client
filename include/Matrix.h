#ifndef MATRIX_H
#define MATRIX_H

/**
 ** A C++ port of Java android.opengl.Matrix.
 ** All offsets have been removed in order to simplify.
 */
class Matrix {
public:
  static void frustumM(float* m, float left, float right, float bottom, float top, float near, float far);
  static bool invertM(float* mInv, float* m);
  static float length(float x, float y, float z);
  static void multiplyMM(float* result, float* lhs, float* rhs);
  static void multiplyMV(float* resultVec, float* lhsMat, float* rhsVec);
  static void orthoM(float* m, float left, float right, float bottom, float top, float near, float far);
  static void perspectiveM(float* m, float fovy, float aspect, float zNear, float zFar);
  static void rotateM(float* m, float a, float x, float y, float z);
  static void rotateM(float* rm, float* m, float a, float x, float y, float z);
  static void scaleM(float* sm, float* m, float x, float y, float z);
  static void scaleM(float* m, float x, float y, float z);
  static void setIdentityM(float* m);
  static void setLookAtM(float* m, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
  static void setRotateEulerM(float* rm, float x, float y, float z);
  static void translateM(float* m, float x, float y, float z);
  static void translateM(float* tm, float* m, float x, float y, float z);
  static void transposeM(float* mTrans, float* m);
};

#endif
