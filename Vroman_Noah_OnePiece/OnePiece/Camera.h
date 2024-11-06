#pragma once
class Camera final
{
public:
	enum class CameraState
	{
		moving,
		waiting
	};
	Camera(float width, float height);
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& rhs) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& rhs) = delete;

	void Transform(const Rectf& target)const;
	void SetLevelBoundries(const Rectf& levelBoundries);
	Point2f GetCameraTransform(const Rectf& target);


	float GetCameraWidth()const;
	float GetCameraHeight()const;

	void SetCurrentCameraStateMoving();
	void SetCurrentCameraStateWaiting();

private:
	const float m_Width;
	const float m_Height;
	Point2f		m_PosCamera;

	Rectf		m_LevelBoundries;
	CameraState m_StateCamera;

	Point2f Track(const Rectf& Target)const;
	void Clamp(Point2f& bottomleftPos)const;

};

