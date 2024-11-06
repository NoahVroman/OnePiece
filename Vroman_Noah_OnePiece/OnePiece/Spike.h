#pragma once
class Spike final
{
public:
	Spike(const Rectf& shape);
	~Spike();

	Spike(const Point2f& pos, const Spike& other) = delete;
	Spike& operator=(const Spike& rhs) = delete;
	Spike(Spike&& other) = delete;
	Spike& operator=(Spike&& rhs) = delete;
	bool IsOverlapping(const Rectf& otherShape);
	const Rectf& GetShape()const;
	const int GetDamage()const;
	void SetShape(const Rectf& other);
private:

	Rectf m_Shape;
	const int m_Damage;


};

