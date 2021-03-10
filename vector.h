#pragma once

//
// 4-dimensional vector implementatioon
//
class Vector4 {
public:
    // vector components
    float x, y, z, w;

    // ctor(s)
    FORCEINLINE Vector4() : x{}, y{}, z{}, w{} {

    }

    FORCEINLINE Vector4( const Vector4 &other ) : x{ other.x }, y{ other.y }, z{ other.z }, w{ other.w } {

    }

    FORCEINLINE Vector4( float x, float y, float z, float w ) : x{ x }, y{ y }, z{ z }, w{ w } {

    }

    FORCEINLINE void init( float x0, float y0, float z0, float w0 ) {
        x = x0;
        y = y0;
        z = z0;
        w = w0;
    }

    // 
    // operators
    //
    // access
    FORCEINLINE float operator []( uint8_t i ) const {
        return ( ( float * ) this )[ i ];
    }

    FORCEINLINE float &operator []( uint8_t i ) {
        return ( ( float * ) this )[ i ];
    }

    // assignment
    FORCEINLINE Vector4 &operator =( const Vector4 &other ) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;

        return *this;
    }

    // equality
    FORCEINLINE bool operator ==( const Vector4 &other ) const {
        return ( other.x == x ) && ( other.y == y ) && ( other.z == z ) && ( other.w == w );
    }

    FORCEINLINE bool operator !=( const Vector4 & other ) const {
        return ( other.x != x ) || ( other.y != y ) || ( other.z != z ) && ( other.w != w );
    }

    // arithmetic operations
    // copy
    FORCEINLINE Vector4 operator +( const Vector4 & other ) const {
        return Vector4( x + other.x, y + other.y, z + other.z, w + other.w );
    }

    FORCEINLINE Vector4 operator -( const Vector4 & other ) const {
        return Vector4( x - other.x, y - other.y, z - other.z, w - other.w );
    }

    FORCEINLINE Vector4 operator *( const Vector4 & other ) const {
        return Vector4( x * other.x, y * other.y, z * other.z, w * other.w );
    }

    FORCEINLINE Vector4 operator /( const Vector4 & other ) const {
        return Vector4( x / other.x, y / other.y, z / other.z, w / other.w );
    }

    FORCEINLINE Vector4 operator +( float scalar ) const {
        return Vector4( x + scalar, y + scalar, z + scalar, w + scalar );
    }

    FORCEINLINE Vector4 operator -( float scalar ) const {
        return Vector4( x - scalar, y - scalar, z - scalar, w - scalar );
    }

    FORCEINLINE Vector4 operator *( float scalar ) const {
        return Vector4( x * scalar, y * scalar, z * scalar, w * scalar );
    }

    FORCEINLINE Vector4 operator /( float scalar ) const {
        return Vector4( x / scalar, y / scalar, z / scalar, w / scalar );
    }

    // reference
    FORCEINLINE Vector4 &operator +=( const Vector4 & other ) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;

        return *this;
    }

    FORCEINLINE Vector4 &operator -=( const Vector4 &other ) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;

        return *this;
    }

    FORCEINLINE Vector4 &operator *=( const Vector4 &other ) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;

        return *this;
    }

    FORCEINLINE Vector4 &operator /=( const Vector4 &other ) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;

        return *this;
    }

    FORCEINLINE Vector4 &operator *=( float scalar ) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;

        return *this;
    }

    FORCEINLINE Vector4 &operator /=( float scalar ) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;

        return *this;
    }

    FORCEINLINE Vector4 &operator +=( float scalar ) {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;

        return *this;
    }

    FORCEINLINE Vector4 &operator -=( float scalar ) {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;

        return *this;
    }

    // util
    FORCEINLINE void fill( float scalar ) {
        x = y = z = w = scalar;
    }

    FORCEINLINE float len() {
        return sqrtf( x * x + y * y + z * z + w * w );
    }

    FORCEINLINE float len_3d() {
        return sqrtf( x * x + y * y + z * z );
    }

    FORCEINLINE float len_2d() {
        return sqrtf( x * x + y * y );
    }

    FORCEINLINE float len_sqr() {
        return ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
    }

    FORCEINLINE float dot( const Vector4 & other ) {
        return ( x * other.x ) + ( y * other.y ) + ( z * other.z ) + ( w * other.w );
    }

    FORCEINLINE float dist( const Vector4 & other ) {
        return ( *this - other ).len();
    }

    FORCEINLINE float normalize() {
        float mag;

        // get vector magnitude
        mag = len();

        // create unit vector, divide each component by magnitude
        if( mag != 0.f ) {
            x /= mag;
            y /= mag;
            z /= mag;
            w /= mag;
        }

        return mag;
    }

    FORCEINLINE Vector4 normalized() {
        Vector4 vec;
        float mag;

        // make copy of current vector
        vec = ( *this );

        // normalize vector
        mag = vec.normalize();

        return vec;
    }
};

class Vector2 {
public:
    // vector components
    float x, y;

    // ctor(s)
    FORCEINLINE Vector2() : x{}, y{} {

    }

    FORCEINLINE Vector2( const Vector2 &other ) : x{ other.x }, y{ other.y } {

    }

    FORCEINLINE Vector2( float x, float y ) : x{ x }, y{ y } {

    }

    FORCEINLINE void init( float x0, float y0 ) {
        x = x0;
        y = y0;
    }

    // 
    // operators
    //
    // access
    FORCEINLINE float operator []( uint8_t i ) const {
        return ( ( float * ) this )[ i ];
    }

    FORCEINLINE float &operator []( uint8_t i ) {
        return ( ( float * ) this )[ i ];
    }

    // assignment
    FORCEINLINE Vector2 &operator =( const Vector2 &other ) {
        x = other.x;
        y = other.y;

        return *this;
    }

    // equality
    FORCEINLINE bool operator ==( const Vector2 &other ) const {
        return ( other.x == x ) && ( other.y == y );
    }

    FORCEINLINE bool operator !=( const Vector2 &other ) const {
        return ( other.x != x ) || ( other.y != y );
    }

    // arithmetic operations
    // copy
    FORCEINLINE Vector2 operator +( const Vector2 & other ) const {
        return Vector2( x + other.x, y + other.y);
    }

    FORCEINLINE Vector2 operator -( const Vector2 & other ) const {
        return Vector2( x - other.x, y - other.y );
    }

    FORCEINLINE Vector2 operator *( const Vector2 &other ) const {
        return Vector2( x * other.x, y * other.y );
    }

    FORCEINLINE Vector2 operator /( const Vector2 &other ) const {
        return Vector2( x / other.x, y / other.y );
    }

    FORCEINLINE Vector2 operator +( float scalar ) const {
        return Vector2( x + scalar, y + scalar );
    }

    FORCEINLINE Vector2 operator -( float scalar ) const {
        return Vector2( x - scalar, y - scalar  );
    }

    FORCEINLINE Vector2 operator *( float scalar ) const {
        return Vector2( x * scalar, y * scalar );
    }

    FORCEINLINE Vector2 operator /( float scalar ) const {
        return Vector2( x / scalar, y / scalar );
    }

    // reference
    FORCEINLINE Vector2 &operator +=( const Vector2 &other ) {
        x += other.x;
        y += other.y;

        return *this;
    }

    FORCEINLINE Vector2 &operator -=( const Vector2 &other ) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    FORCEINLINE Vector2 &operator *=( const Vector2 &other ) {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    FORCEINLINE Vector2 &operator /=( const Vector2 &other ) {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    FORCEINLINE Vector2 &operator *=( float scalar ) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    FORCEINLINE Vector2 &operator /=( float scalar ) {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    FORCEINLINE Vector2 &operator +=( float scalar ) {
        x += scalar;
        y += scalar;

        return *this;
    }

    FORCEINLINE Vector2 &operator -=( float scalar ) {
        x -= scalar;
        y -= scalar;

        return *this;
    }

    // util
    FORCEINLINE void fill( float scalar ) {
        x = y =scalar;
    }

    FORCEINLINE float len_2d() {
        return sqrtf( x * x + y * y );
    }

    FORCEINLINE float len_2d_sqr() {
        return ( x * x ) + ( y * y );
    }

    FORCEINLINE float dot( const Vector2 &other ) {
        return ( x * other.x ) + ( y * other.y );
    }

    FORCEINLINE float cross( const Vector2 &other ) {
        return ( x * other.y ) - ( y * other.x );
    }

    FORCEINLINE float dist( const Vector2 & other ) {
        return ( *this - other ).len_2d();
    }

    FORCEINLINE float normalize() {
        float mag;

        // get vector magnitude
        mag = len_2d();

        // create unit vector, divide each component by magnitude
        if( mag != 0.f ) {
            x /= mag;
            y /= mag;
        }

        return mag;
    }

    FORCEINLINE Vector2 normalized() {
        Vector2 vec;
        float mag;

        // make copy of current vector
        vec = ( *this );

        // normalize vector
        mag = vec.normalize();

        return vec;
    }
};

//
// 3-dimensional vector implementatioon
//
class Vector3 {
public:
    // vector components
    float x, y, z;

    // ctor(s)
    FORCEINLINE Vector3() : x{}, y{}, z{} {

    }

    FORCEINLINE Vector3( const Vector3 &other ) : x{ other.x }, y{ other.y }, z{ other.z }{

    }

    FORCEINLINE Vector3( float x, float y, float z ) : x{ x }, y{ y }, z{ z } {

    }

    FORCEINLINE void init( float x0, float y0, float z0 ) {
        x = x0;
        y = y0;
        z = z0;
    }

    // 
    // operators
    //
    // access
    FORCEINLINE float operator []( uint8_t i ) const {
        return ( ( float * ) this )[ i ];
    }

    FORCEINLINE float &operator []( uint8_t i ) {
        return ( ( float * ) this )[ i ];
    }

    // assignment
    FORCEINLINE Vector3 &operator =( const Vector3 &other ) {
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }

    // equality
    FORCEINLINE bool operator ==( const Vector3 &other ) const {
        return ( other.x == x ) && ( other.y == y ) && ( other.z == z );
    }

    FORCEINLINE bool operator !=( const Vector3 & other ) const {
        return ( other.x != x ) || ( other.y != y ) || ( other.z != z );
    }

    // arithmetic operations
    // copy
    FORCEINLINE Vector3 operator +( const Vector3 & other ) const {
        return Vector3( x + other.x, y + other.y, z + other.z );
    }

    FORCEINLINE Vector3 operator -( const Vector3 & other ) const {
        return Vector3( x - other.x, y - other.y, z - other.z );
    }

    FORCEINLINE Vector3 operator *( const Vector3 & other ) const {
        return Vector3( x * other.x, y * other.y, z * other.z );
    }

    FORCEINLINE Vector3 operator /( const Vector3 & other ) const {
        return Vector3( x / other.x, y / other.y, z / other.z );
    }

    FORCEINLINE Vector3 operator +( float scalar ) const {
        return Vector3( x + scalar, y + scalar, z + scalar );
    }

    FORCEINLINE Vector3 operator -( float scalar ) const {
        return Vector3( x - scalar, y - scalar, z - scalar );
    }

    FORCEINLINE Vector3 operator *( float scalar ) const {
        return Vector3( x * scalar, y * scalar, z * scalar );
    }

    FORCEINLINE Vector3 operator /( float scalar ) const {
        return Vector3( x / scalar, y / scalar, z / scalar );
    }

    // reference
    FORCEINLINE Vector3 &operator +=( const Vector3 & other ) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    FORCEINLINE Vector3 &operator -=( const Vector3 & other ) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    FORCEINLINE Vector3 &operator *=( const Vector3 & other ) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    FORCEINLINE Vector3 &operator /=( const Vector3 & other ) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    FORCEINLINE Vector3 &operator *=( float scalar ) {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    FORCEINLINE Vector3 &operator /=( float scalar ) {
        x /= scalar;
        y /= scalar;
        z /= scalar;

        return *this;
    }

    FORCEINLINE Vector3 &operator +=( float scalar ) {
        x += scalar;
        y += scalar;
        z += scalar;

        return *this;
    }

    FORCEINLINE Vector3 &operator -=( float scalar ) {
        x -= scalar;
        y -= scalar;
        z -= scalar;

        return *this;
    }

    // util
    FORCEINLINE void fill( float scalar ) {
        x = y = z = scalar;
    }

    FORCEINLINE float len() {
        return sqrtf( x * x + y * y + z * z );
    }

    FORCEINLINE float len_2d() {
        return sqrtf( x * x + y * y );
    }

    FORCEINLINE float len_sqr() {
        return ( x * x ) + ( y * y ) + ( z * z );
    }

    FORCEINLINE float dot( const Vector3 & other ) {
        return ( x * other.x ) + ( y * other.y ) + ( z * other.z );
    }

    FORCEINLINE Vector3 cross( const Vector3 & other ) {
        return Vector3( ( y * other.z ) - ( z * other.y ), ( z * other.x ) - ( x * other.z ), ( x * other.y ) - ( y * other.x ) );
    }

    FORCEINLINE float dist( const Vector3 & other ) {
        return ( *this - other ).len();
    }

    FORCEINLINE float normalize() {
        float mag;

        // get vector magnitude
        mag = len();

        // create unit vector, divide each component by magnitude
        if( mag != 0.f ) {
            x /= mag;
            y /= mag;
            z /= mag;
        }

        return mag;
    }

    FORCEINLINE Vector3 normalized() {
        Vector3 vec;
        float mag;

        // make copy of current vector
        vec = ( *this );

        // normalize vector
        mag = vec.normalize();

        return vec;
    }
};
