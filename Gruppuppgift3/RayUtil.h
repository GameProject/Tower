struct Vec
{
	float x, y, z;

	Vec() {};
	Vec(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vec operator-(Vec& v2)
	{
		return Vec(x-v2.x, y-v2.y, z-v2.z);
	}

	Vec operator+(Vec& v2)
	{
		return Vec(x+v2.x, y+v2.y, z+v2.z);
	}

	Vec operator/(float n)
	{
		return Vec(x/n, y/n, z/n);
	}

	Vec operator*(float n)
	{
		return Vec(x*n, y*n, z*n);
	}

	float Dot(Vec& v2)
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z);
	}

	Vec Cross(Vec& v2)
	{
		return Vec((y*v2.z)-(z*v2.y), (z*v2.x)-(x*v2.z), (x*v2.y)-(y*v2.x));
	}

	float Length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	void Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
		z /= l;
	}
};

struct Ray
{
	Vec	o;
	Vec	d;

	Ray(Vec origin, Vec direction)
	{
		o = origin;
		d = direction;
	}
};

struct Plane
{
	Vec		n;
	float	d;

	Plane(){}
	Plane(Vec normal, float _d)
	{
		n = normal;
		d = _d;
	}
};

struct Slab
{
	Plane p1;
	Plane p2;
	Slab(){}
	Slab(Plane plane1, Plane plane2)
	{
		p1 = plane1;
		p2 = plane2;
	}
};

struct Box
{	
	Slab s[3];
	Box(Slab s1, Slab s2, Slab s3)
	{
		s[0] = s1;
		s[1] = s2;
		s[2] = s3;
	}
};

struct HitData
{
	float	t;

	HitData()
	{
		t = -1.0f;
	}
};