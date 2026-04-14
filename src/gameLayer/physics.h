#ifndef PHYSICS_H
#define PHYSICS_H

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <cmath>

#if defined(RAYMATH_DISABLE_CPP_OPERATORS)
// Vector2 operator overloads (only defined when raymath C++ operators are disabled)

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return { a.x + b.x, a.y + b.y };
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return { a.x - b.x, a.y - b.y };
}
inline Vector2 operator*(const Vector2& a, float scalar)
{
	return { a.x * scalar, a.y * scalar };
}

inline Vector2 operator/(const Vector2& a, float scalar)
{
	return { a.x / scalar, a.y / scalar };
}


inline Vector2& operator*=(Vector2& a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	return a;
}

inline Vector2& operator/=(Vector2& a, float scalar)
{
	a.x /= scalar;
	a.y /= scalar;
	return a;
}

inline Vector2& operator+=(Vector2& a, float scalar)
{
	a.x += scalar;
	a.y += scalar;
	return a;
}

inline Vector2& operator-=(Vector2& a, float scalar)
{
	a.x -= scalar;
	a.y -= scalar;
	return a;
}

inline bool operator==(const Vector2& a, const Vector2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vector2& a, const Vector2& b)
{
	return !(a == b);
}

inline Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vector2& operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Vector2& operator*=(Vector2& a, const Vector2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

inline Vector2& operator/=(Vector2& a, const Vector2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}
#endif // RAYMATH_DISABLE_CPP_OPERATORS

struct GameMap;

struct Transform2D
{
	Vector2 position = { 0, 0 };
	float w = 0;
	float h = 0;

	Vector2 getCenter()       const { return { position.x, position.y }; }
	Vector2 getTop()          const { return { position.x, position.y - h * 0.5f }; }
	Vector2 getBottom()       const { return { position.x, position.y + h * 0.5f }; }
	Vector2 getLeft()         const { return { position.x - w * 0.5f, position.y }; }
	Vector2 getRight()        const { return { position.x + w * 0.5f, position.y }; }
	Vector2 getTopLeft()      const { return { position.x - w * 0.5f, position.y - h * 0.5f }; }
	Vector2 getTopRight()     const { return { position.x + w * 0.5f, position.y - h * 0.5f }; }
	Vector2 getBottomLeft()   const { return { position.x - w * 0.5f, position.y + h * 0.5f }; }
	Vector2 getBottomRight()  const { return { position.x + w * 0.5f, position.y + h * 0.5f }; }

	Rectangle getAABB() const
	{
		return { position.x - w * 0.5f, position.y - h * 0.5f, w, h };
	}

	bool intersectPoint(Vector2 point, float delta = 0) const
	{
		Rectangle aabb = getAABB();
		aabb.x -= delta;
		aabb.y -= delta;
		aabb.width += delta * 2;
		aabb.height += delta * 2;

		return CheckCollisionPointRec(point, aabb);
	}

	bool intersectTransform(Transform2D other, float delta = 0) const
	{
		Rectangle a = getAABB();
		Rectangle b = other.getAABB();

		a.x -= delta;
		a.y -= delta;
		a.width += delta * 2;
		a.height += delta * 2;

		b.x -= delta;
		b.y -= delta;
		b.width += delta * 2;
		b.height += delta * 2;

		return CheckCollisionRecs(a, b);
	}

};

struct PhysicalEntity
{
	Transform2D transform = {0, 0, 0, 0};
	Texture2D texture = {};
	Vector2 lastPosition = {};

	Vector2 velocity = {};
	Vector2 acceleration = {};
	float moveSpeed = 1;

	bool upTouch = {};
	bool downTouch = {};
	bool leftTouch = {};
	bool rightTouch = {};

	void teleport(Vector2 newPosition)
	{
		transform.position = newPosition;
		lastPosition = newPosition;
	}

	// Force Functions
	void applyGravity()
	{
		acceleration += {0, 20};
	}

	void updateForces(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		transform.position += velocity * deltaTime;

		// Universal drag ( air resisitance, friction, etc. )
		Vector2 dragVector = Vector2{ velocity.x * std::abs(velocity.x), velocity.y * std::abs(velocity.y) };
		float drag = 0.01f; // Adjust this value to increase/decrease drag strength

		if (Vector2Length(dragVector) * drag * deltaTime > Vector2Length(velocity))
		{
			velocity = {};
		}
		else
		{
			velocity -= dragVector * drag * deltaTime;
		}

		if (Vector2Length(velocity) <= 0.01f)
		{
			velocity = {};
		}

		acceleration = {};
	}

	void updateFinal()
	{
		lastPosition = transform.position;
	}

	void jump(float force)
	{
		// Acceletation uses deltaTime
		// Add Force in one Frame Only
		if (downTouch)
		{
			velocity.y -= force;
		}
	}

	Vector2 &getPosition()
	{
		return transform.position;
	}

	// Collision Functions

	void resolveConstrains(GameMap& mapData);
	
	void checkCollisionOnce(Vector2& position, GameMap& mapData);

    Vector2 performCollisionOnOneAxis(GameMap& mapData, Vector2 position, Vector2 delta);
};

#endif // PHYSICS_H
