/*
 * Matrix.h
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector3.h"
#include "Vector4.h"

namespace gameplay
{

/**
 * Defines a 4 x 4 floating point matrix representing a 3D transformation.
 *
 * Vectors are treated as columns, resulting in a matrix that is represented as follows,
 * where x, y and z are the translation components of the matrix:
 *
 * 1  0  0  x
 * 0  1  0  y
 * 0  0  1  z
 * 0  0  0  1
 *
 * This matrix class is directly compatible with OpenGL since its elements are
 * laid out in memory exactly as they are expected by OpenGL.
 * The matrix uses column-major format such that array indices increase down column first.
 * Since matrix multiplication is not commutative, multiplication must be done in the
 * correct order when combining transformations. Suppose we have a translation
 * matrix T and a rotation matrix R. To first rotate an object around the origin
 * and then translate it, you would multiply the two matrices as TR.
 *
 * Likewise, to first translate the object and then rotate it, you would do RT.
 * So generally, matrices must be multiplied in the reverse order in which you
 * want the transformations to take place (this also applies to
 * the scale, rotate, and translate methods below; these methods are convenience
 * methods for post-multiplying by a matrix representing a scale, rotation, or translation).
 *
 * In the case of repeated local transformations (i.e. rotate around the Z-axis by 0.76 radians,
 * then translate by 2.1 along the X-axis, then ...), it is better to use the Transform class
 * (which is optimized for that kind of usage).
 *
 * @see Transform
 */
class Matrix
{
public:

    /**
     * Stores the columns of this 4x4 matrix.
     * */
    float m[16];

    /**
     * Constructs a matrix initialized to the identity matrix:
     *
     * 1  0  0  0
     * 0  1  0  0
     * 0  0  1  0
     * 0  0  0  1
     */
    Matrix();

    /**
     * Constructs a matrix initialized to the specified value.
     *
     * @param m11 The first element of the first row.
     * @param m12 The second element of the first row.
     * @param m13 The third element of the first row.
     * @param m14 The fourth element of the first row.
     * @param m21 The first element of the second row.
     * @param m22 The second element of the second row.
     * @param m23 The third element of the second row.
     * @param m24 The fourth element of the second row.
     * @param m31 The first element of the third row.
     * @param m32 The second element of the third row.
     * @param m33 The third element of the third row.
     * @param m34 The fourth element of the third row.
     * @param m41 The first element of the fourth row.
     * @param m42 The second element of the fourth row.
     * @param m43 The third element of the fourth row.
     * @param m44 The fourth element of the fourth row.
     */
    Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31,
           float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    /**
     * Creates a matrix initialized to the specified column-major array.
     *
     * The passed-in array is in column-major order, so the memory layout of the array is as follows:
     *
     *     0   4   8   12
     *     1   5   9   13
     *     2   6   10  14
     *     3   7   11  15
     *
     * @param m An array containing 16 elements in column-major order.
     */
    Matrix(const float* m);

    /**
     * Constructs a new matrix by copying the values from the specified matrix.
     *
     * @param copy The matrix to copy.
     */
    Matrix(const Matrix& copy);

    /**
     * Destructor.
     */
    ~Matrix();

    /**
     * Returns the identity matrix:
     *
     * 1  0  0  0
     * 0  1  0  0
     * 0  0  1  0
     * 0  0  0  1
     *
     * @return The identity matrix.
     */
    static const Matrix& identity();

    /**
     * Returns the matrix with all zeros.
     *
     * @return The matrix with all zeros.
     */
    static const Matrix& zero();

    /**
     * Creates a view matrix based on the specified input parameters.
     *
     * @param eyePosition The eye position.
     * @param targetPosition The target's cener position.
     * @param up The up vector.
     * @param dst A matrix to store the result in.
     */
    static void createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix* dst);

    /**
     * Creates a view matrix based on the specified input parameters.
     *
     * @param eyePositionX The eye x-coordinate position.
     * @param eyePositionY The eye y-coordinate position.
     * @param eyePositionZ The eye z-coordinate position.
     * @param targetCenterX The target's center x-coordinate position.
     * @param targetCenterY The target's center y-coordinate position.
     * @param targetCenterZ The target's center z-coordinate position.
     * @param upX The up vector x-coordinate value.
     * @param upY The up vector y-coordinate value.
     * @param upZ The up vector z-coordinate value.
     * @param dst A matrix to store the result in.
     */
    static void createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ, float targetCenterX,
                             float targetCenterY, float targetCenterZ, float upX, float upY, float upZ, Matrix* dst);

    /**
     * Builds a perspective projection matrix based on a field of view and returns by value.
     *
     * Projection space refers to the space after applying projection transformation from view space.
     * After the projection transformation, visible content has x- and y-coordinates ranging from -1 to 1,
     * and a z-coordinate ranging from 0 to 1. To obtain the viewable area (in world space) of a scene,
     * create a BoundingFrustum and pass the combined view and projection matrix to the constructor.
     *
     * @param fieldOfView The field of view in the y direction (in radians).
     * @param aspectRatio The aspect ratio, defined as view space width divided by height.
     * @param zNearPlane The distance to the near view plane.
     * @param zFarPlane The distance to the far view plane.
     * @param dst A matrix to store the result in.
     */
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Matrix* dst);

    /**
     * Creates an orthographic projection matrix.
     *
     * @param width The width of the view.
     * @param height The height of the view.
     * @param zNearPlane The minimum z-value of the view volume.
     * @param zFarPlane The maximum z-value of the view volume.
     * @param dst A matrix to store the result in.
     */
    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix* dst);

    /**
     * Creates an orthographic projection matrix.
     *
     * Projection space refers to the space after applying
     * projection transformation from view space. After the
     * projection transformation, visible content has
     * x and y coordinates ranging from -1 to 1, and z coordinates
     * ranging from 0 to 1.
     *
     * Unlike perspective projection, in orthographic projection
     * there is no perspective foreshortening.
     *
     * The viewable area of this orthographic projection extends
     * from left to right on the x-axis, bottom to top on the y-axis,
     * and zNearPlane to zFarPlane on the z-axis. These values are
     * relative to the position and x, y, and z-axes of the view.
     * To obtain the viewable area (in world space) of a scene,
     * create a BoundingFrustum and pass the combined view and
     * projection matrix to the constructor.
     *
     * @param left The minimum x-value of the view volume.
     * @param right The maximum x-value of the view volume.
     * @param bottom The minimum y-value of the view volume.
     * @param top The maximum y-value of the view volume.
     * @param zNearPlane The minimum z-value of the view volume.
     * @param zFarPlane The maximum z-value of the view volume.
     * @param dst A matrix to store the result in.
     */
    static void createOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane,
                                            float zFarPlane, Matrix* dst);

//    /*
//     * Creates a spherical billboard that rotates around a specified object position.
//     *
//     * This method computes the facing direction of the billboard from the object position
//     * and camera position. When the object and camera positions are too close, the matrix
//     * will not be accurate. To avoid this problem, the method uses the optional camera
//     * forward vector if the positions are too close.
//     *
//     * @param objectPosition The position of the object the billboard will rotate around.
//     * @param cameraPosition The position of the camera.
//     * @param cameraUpVector The up vector of the camera.
//     * @param dst A matrix to store the result in.
//     */
//    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector, Matrix* dst);
//
//    /*
//     * Fills in an existing Matrix so that it reflects the coordinate system about a specified Plane.
//     *
//     * @param plane The Plane about which to create a reflection.
//     * @param dst A matrix to store the result in.
//     */
//    static void createReflection(const Plane& plane, Matrix* dst);

    /**
     * Creates a scale matrix.
     *
     * @param scale The amount to scale.
     * @param dst A matrix to store the result in.
     */
    static void createScale(const Vector3& scale, Matrix* dst);

    /**
     * Creates a scale matrix.
     *
     * @param xScale The amount to scale along the x-axis.
     * @param yScale The amount to scale along the y-axis.
     * @param zScale The amount to scale along the z-axis.
     * @param dst A matrix to store the result in.
     */
    static void createScale(float xScale, float yScale, float zScale, Matrix* dst);

    /**
     * Creates a rotation matrix from the specified quaternion.
     *
     * @param quat A quaternion describing a 3D orientation.
     * @param dst A matrix to store the result in.
     */
    static void createRotation(const Quaternion& quat, Matrix* dst);

    /**
     * Creates a rotation matrix from the specified axis and angle.
     *
     * @param axis A vector describing the axis to rotate about.
     * @param angle The angle (in radians).
     * @param dst A matrix to store the result in.
     */
    static void createRotation(const Vector3& axis, float angle, Matrix* dst);

    /**
     * Creates a matrix describing a rotation around the x-axis.
     *
     * @param angle The angle of rotation (in radians).
     * @param dst A matrix to store the result in.
     */
    static void createRotationX(float angle, Matrix* dst);

    /**
     * Creates a matrix describing a rotation around the y-axis.
     *
     * @param angle The angle of rotation (in radians).
     * @param dst A matrix to store the result in.
     */
    static void createRotationY(float angle, Matrix* dst);

    /**
     * Creates a matrix describing a rotation around the z-axis.
     *
     * @param angle The angle of rotation (in radians).
     * @param dst A matrix to store the result in.
     */
    static void createRotationZ(float angle, Matrix* dst);

    /**
     * Creates a translation matrix.
     *
     * @param translation The translation.
     * @param dst A matrix to store the result in.
     */
    static void createTranslation(const Vector3& translation, Matrix* dst);

    /**
     * Creates a translation matrix.
     *
     * @param xTranslation The translation on the x-axis.
     * @param yTranslation The translation on the y-axis.
     * @param zTranslation The translation on the z-axis.
     * @param dst A matrix to store the result in.
     */
    static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix* dst);

    /**
     * Adds a scalar value to each component of this matrix.
     *
     * @param scalar The scalar to add.
     */
    void add(float scalar);

    /**
     * Adds a scalar value to each component of this matrix and stores the result in dst.
     *
     * @param scalar The scalar value to add.
     * @param dst A matrix to store the result in.
     */
    void add(float scalar, Matrix* dst);

    /**
     * Adds the specified matrix to this matrix.
     *
     * @param m The matrix to add.
     */
    void add(const Matrix& m);

    /**
     * Adds the specified matrices and stores the result in dst.
     *
     * @param m1 The first matrix.
     * @param m2 The second matrix.
     * @param dst The destination matrix to add to.
     */
    static void add(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Decomposes the scale, rotation and translation components of this matrix.
     *
     * @param scale The scale.
     * @param rotation The rotation.
     * @param translation The translation.
     */
    bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

    /**
     * Computes the determinant of this matrix.
     *
     * @return The determinant.
     */
    float determinant() const;

    /**
     * Gets the scalar component of this matrix in the specified vector.
     *
     * If the scalar component of this matrix has negative parts,
     * it is not possible to always extract the exact scalar component;
     * instead, a scale vector that is mathematically equivalent to the
     * original scale vector is extracted and returned.
     *
     * @param scale A vector to receive the scale.
     */
    void getScale(Vector3* scale) const;

    /**
     * Gets the rotational component of this matrix in the specified quaternion.
     *
     * @param rotation A quaternion to receive the rotation.
     * 
     * @return true if the rotation is successfully extracted, false otherwise.
     */
    bool getRotation(Quaternion* rotation) const;

    /**
     * Gets the translational component of this matrix in the specified vector.
     *
     * @param translation A vector to receive the translation.
     */
    void getTranslation(Vector3* translation) const;

    /**
     * Gets the up vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getUpVector(Vector3* dst) const;

    /**
     * Gets the down vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getDownVector(Vector3* dst) const;

    /**
     * Gets the left vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getLeftVector(Vector3* dst) const;

    /**
     * Gets the right vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getRightVector(Vector3* dst) const;

    /**
     * Gets the forward vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getForwardVector(Vector3* dst) const;

    /**
     * Gets the backward vector of this matrix.
     *
     * @param dst The destination vector.
     */
    void getBackVector(Vector3* dst) const;

    /**
     * Inverts this matrix.
     *
     * @return true if the the matrix can be inverted, false otherwise.
     */
    bool invert();

    /**
     * Stores the inverse of this matrix in the specified matrix.
     *
     * @param dst A matrix to store the invert of this matrix in.
     * 
     * @return true if the the matrix can be inverted, false otherwise.
     */
    bool invert(Matrix* dst) const;

    /**
     * Determines if this matrix is equal to the identity matrix.
     *
     * @return true if the matrix is an identity matrix, false otherwise.
     */
    bool isIdentity() const;

    /**
     * Multiplies the components of this matrix by the specified scalar.
     *
     * @param scalar The scalar value.
     */
    void multiply(float scalar);

    /**
     * Multiplies the components of this matrix by a scalar and stores the result in dst.
     *
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    void multiply(float scalar, Matrix* dst) const;

    /**
     * Multiplies the components of the specified matrix by a scalar and stores the result in dst.
     *
     * @param m The matrix.
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Matrix& m, float scalar, Matrix* dst);

    /**
     * Multiplies this matrix by the specified one.
     *
     * @param m The matrix to multiply.
     */
    void multiply(const Matrix& m);

    /**
     * Multiplies m1 by m2 and stores the result in dst.
     *
     * @param m1 The first matrix to multiply.
     * @param m2 The second matrix to multiply.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Negates this matrix.
     */
    void negate();

    /**
     * Negates this matrix and stores the result in dst.
     *
     * @param dst A matrix to store the result in.
     */
    void negate(Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified quaternion rotation.
     *
     * @param q The quaternion to rotate by.
     */
    void rotate(const Quaternion& q);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified quaternion rotation and stores the result in dst.
     *
     * @param q The quaternion to rotate by.
     * @param dst A matrix to store the result in.
     */
    void rotate(const Quaternion& q, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation about the specified axis.
     *
     * @param axis The axis to rotate about.
     * @param angle The angle (in radians).
     */
    void rotate(const Vector3& axis, float angle);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the specified
     * rotation about the specified axis and stores the result in dst.
     *
     * @param axis The axis to rotate about.
     * @param angle The angle (in radians).
     * @param dst A matrix to store the result in.
     */
    void rotate(const Vector3& axis, float angle, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the x-axis.
     *
     * @param angle The angle (in radians).
     */
    void rotateX(float angle);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the x-axis and stores the result in dst.
     *
     * @param angle The angle (in radians).
     * @param dst A matrix to store the result in.
     */
    void rotateX(float angle, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the y-axis.
     *
     * @param angle The angle (in radians).
     */
    void rotateY(float angle);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the y-axis and stores the result in dst.
     *
     * @param angle The angle (in radians).
     * @param dst A matrix to store the result in.
     */
    void rotateY(float angle, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the z-axis.
     *
     * @param angle The angle (in radians).
     */
    void rotateZ(float angle);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified rotation around the z-axis and stores the result in dst.
     *
     * @param angle The angle (in radians).
     * @param dst A matrix to store the result in.
     */
    void rotateZ(float angle, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation.
     *
     * @param value The amount to scale along all axes.
     */
    void scale(float value);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation and stores the result in dst.
     *
     * @param value The amount to scale along all axes.
     * @param dst A matrix to store the result in.
     */
    void scale(float value, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation.
     *
     * @param xScale The amount to scale along the x-axis.
     * @param yScale The amount to scale along the y-axis.
     * @param zScale The amount to scale along the z-axis.
     */
    void scale(float xScale, float yScale, float zScale);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation and stores the result in dst.
     *
     * @param xScale The amount to scale along the x-axis.
     * @param yScale The amount to scale along the y-axis.
     * @param zScale The amount to scale along the z-axis.
     * @param dst A matrix to store the result in.
     */
    void scale(float xScale, float yScale, float zScale, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation.
     *
     * @param s The scale values along the x, y and z axes.
     */
    void scale(const Vector3& s);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation and stores the result in dst.
     *
     * @param s The scale values along the x, y and z axes.
     * @param dst A matrix to store the result in.
     */
    void scale(const Vector3& s, Matrix* dst) const;

    /**
     * Sets the values of this matrix.
     *
     * @param m11 The first element of the first row.
     * @param m12 The second element of the first row.
     * @param m13 The third element of the first row.
     * @param m14 The fourth element of the first row.
     * @param m21 The first element of the second row.
     * @param m22 The second element of the second row.
     * @param m23 The third element of the second row.
     * @param m24 The fourth element of the second row.
     * @param m31 The first element of the third row.
     * @param m32 The second element of the third row.
     * @param m33 The third element of the third row.
     * @param m34 The fourth element of the third row.
     * @param m41 The first element of the fourth row.
     * @param m42 The second element of the fourth row.
     * @param m43 The third element of the fourth row.
     * @param m44 The fourth element of the fourth row.
     */
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31,
             float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    /**
     * Sets the values of this matrix to those in the specified column-major array.
     *
     * @param m An array containing 16 elements in column-major format.
     */
    void set(const float* m);

    /**
     * Sets the values of this matrix to those of the specified matrix.
     *
     * @param m The source matrix.
     */
    void set(const Matrix& m);

    /**
     * Sets this matrix to the identity matrix.
     */
    void setIdentity();

    /**
     * Sets all elements of the current matrix to zero.
     */
    void setZero();

    /**
     * Subtracts the specified matrix from the current matrix.
     *
     * @param m The matrix to subtract.
     */
    void subtract(const Matrix& m);

    /**
     * Subtracts the specified matrix from the current matrix.
     *
     * @param m1 The first matrix.
     * @param m2 The second matrix.
     * @param dst A matrix to store the result in.
     */
    static void subtract(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Transforms the specified point by this matrix.
     *
     * The result of the transformation is stored directly into point.
     *
     * @param point The point to transform and also a vector to hold the result in.
     */
    void transformPoint(Vector3* point) const;

    /**
     * Transforms the specified point by this matrix, and stores
     * the result in dst.
     *
     * @param point The point to transform.
     * @param dst A vector to store the transformed point in.
     */
    void transformPoint(const Vector3& point, Vector3* dst) const;

    /**
     * Transforms the specified vector by this matrix by
     * treating the fourth (w) coordinate as zero.
     *
     * The result of the transformation is stored directly into vector.
     *
     * @param vector The vector to transform and also a vector to hold the result in.
     */
    void transformVector(Vector3* vector) const;

    /**
     * Transforms the specified vector by this matrix by
     * treating the fourth (w) coordinate as zero, and stores the
     * result in dst.
     *
     * @param vector The vector to transform.
     * @param dst A vector to store the transformed vector in.
     */
    void transformVector(const Vector3& vector, Vector3* dst) const;

    /**
     * Transforms the specified vector by this matrix.
     *
     * @param x The vector x-coordinate to transform by.
     * @param y The vector y-coordinate to transform by.
     * @param z The vector z-coordinate to transform by.
     * @param w The vector w-coordinate to transform by.
     * @param dst A vector to store the transformed point in.
     */
    void transformVector(float x, float y, float z, float w, Vector3* dst) const;

    /**
     * Transforms the specified vector by this matrix.
     *
     * The result of the transformation is stored directly into vector.
     *
     * @param vector The vector to transform.
     */
    void transformVector(Vector4* vector) const;

    /**
     * Transforms the specified vector by this matrix.
     *
     * @param vector The vector to transform.
     * @param dst A vector to store the transformed point in.
     */
    void transformVector(const Vector4& vector, Vector4* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation.
     *
     * @param x The amount to translate along the x-axis.
     * @param y The amount to translate along the y-axis.
     * @param z The amount to translate along the z-axis.
     */
    void translate(float x, float y, float z);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation and stores the result in dst.
     *
     * @param x The amount to translate along the x-axis.
     * @param y The amount to translate along the y-axis.
     * @param z The amount to translate along the z-axis.
     * @param dst A matrix to store the result in.
     */
    void translate(float x, float y, float z, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation.
     *
     * @param t The translation values along the x, y and z axes.
     */
    void translate(const Vector3& t);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation and stores the result in dst.
     *
     * @param t The translation values along the x, y and z axes.
     * @param dst A matrix to store the result in.
     */
    void translate(const Vector3& t, Matrix* dst) const;

    /**
     * Transposes this matrix.
     */
    void transpose();

    /**
     * Transposes this matrix and stores the result in dst.
     *
     * @param dst A matrix to store the result in.
     */
    void transpose(Matrix* dst) const;

    /**
     * Calculates the sum of this matrix with the given matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @param m The matrix to add.
     * @return The matrix sum.
     */
    inline Matrix operator+(const Matrix& m);
    
    /**
     * Adds the given matrix to this matrix.
     * 
     * @param m The matrix to add.
     * @return This matrix, after the addition occurs.
     */
    inline Matrix& operator+=(const Matrix& m);

    /**
     * Calculates the sum of this matrix with the given matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @param m The matrix to add.
     * @return The matrix sum.
     */
    inline Matrix operator-(const Matrix& m);

    /**
     * Subtracts the given matrix from this matrix.
     * 
     * @param m The matrix to subtract.
     * @return This matrix, after the subtraction occurs.
     */
    inline Matrix& operator-=(const Matrix& m);

    /**
     * Calculates the negation of this matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @return The negation of this matrix.
     */
    inline Matrix operator-();

    /**
     * Calculates the matrix product of this matrix with the given matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @param m The matrix to multiply by.
     * @return The matrix product.
     */
    inline Matrix operator*(const Matrix& m);

    /**
     * Right-multiplies this matrix by the given matrix.
     * 
     * @param m The matrix to multiply by.
     * @return This matrix, after the multiplication occurs.
     */
    inline Matrix& operator*=(const Matrix& m);
};

/**
 * Transforms the given vector by the given matrix.
 * 
 * Note: this treats the given vector as a vector and not as a point.
 * 
 * @param v The vector to transform.
 * @param m The matrix to transform by.
 * @return This vector, after the transformation occurs.
 */
inline Vector3& operator*=(Vector3& v, const Matrix& m);

/**
 * Transforms the given vector by the given matrix.
 * 
 * Note: this treats the given vector as a vector and not as a point.
 * 
 * @param m The matrix to transform by.
 * @param v The vector to transform.
 * @return The resulting transformed vector.
 */
inline Vector3 operator*(const Matrix& m, const Vector3& v);

/**
 * Transforms the given vector by the given matrix.
 * 
 * Note: this treats the given vector as a vector and not as a point.
 * 
 * @param v The vector to transform.
 * @param m The matrix to transform by.
 * @return This vector, after the transformation occurs.
 */
inline Vector4& operator*=(Vector4& v, const Matrix& m);

/**
 * Transforms the given vector by the given matrix.
 * 
 * Note: this treats the given vector as a vector and not as a point.
 * 
 * @param m The matrix to transform by.
 * @param v The vector to transform.
 * @return The resulting transformed vector.
 */
inline Vector4 operator*(const Matrix& m, const Vector4& v);

}

//#include "Matrix.inl"

#endif
