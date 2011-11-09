#ifndef STRUCT_H
#define STRUCT_H

struct point {
        float x, y, z;
};

struct Vector {
        float x, y, z;

    Vector& operator += (const Vector &v2){
            this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
            return *this;
    }
};

inline point operator + (const point&p, const Vector &v){
        point p2={p.x + v.x, p.y + v.y, p.z + v.z };
        return p2;
}

inline point operator - (const point&p, const Vector &v){
        point p2={p.x - v.x, p.y - v.y, p.z - v.z };
        return p2;
}

inline Vector operator + (const Vector&v1, const Vector &v2){
        Vector v={v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
        return v;
}

inline Vector operator - (const point&p1, const point &p2){
        Vector v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
        return v;
}

inline Vector operator * (float c, const Vector &v)
{
        Vector v2={v.x *c, v.y * c, v.z * c };
        return v2;
}

inline Vector operator - (const Vector&v1, const Vector &v2){
        Vector v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
        return v;
}

inline float operator * (const Vector&v1, const Vector &v2 ) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct color{
    float red, green, blue;

    inline color & operator += (const color &c2 ) {
            this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
            return *this;
    }
};

inline color operator * (const color&c1, const color &c2 ) {
        color c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
        return c;
}

inline color operator + (const color&c1, const color &c2 ) {
        color c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
        return c;
}

inline color operator * (float coef, const color &c ) {
        color c2 = {c.red * coef, c.green * coef, c.blue * coef};
        return c2;
}

struct sphere {
        point pos;
        float size;
        int textureID;
};

struct light {
        point pos;
        float red, green, blue;
};

struct ray {
        point start;
        Vector dir;
        bool in;
};

struct texture {
    float reflection;
    float refraction;
    float red, green, blue;
    float spec;
    float n;
};

struct plane {
    float D;
    Vector dir;
    int textureID;
};


#endif // STRUCT_H
