//*****************************************************************************
// FILE NAME:  ViewController.h
//
//*****************************************************************************
#ifndef VIEW_CONTROLLER_H_
#define VIEW_CONTROLLER_H_

namespace FIFE
{
	class Camera;
}

//! provides manipulation of camera and view
class ViewController
{
public:
	ViewController(FIFE::Camera* camera=0);
	~ViewController();
	void AttachCamera(FIFE::Camera* camera);
	void EnableCamera(bool enable);
	void ZoomIn();
	void ZoomOut();
	void RotateLeft();
	void RotateRight();
private:
	Game* m_parent;
	FIFE::Camera* m_camera;
	double m_zoomIncrement;
	double m_maxZoom;
	double m_minZoom;
	double m_rotateIncrement;
};

#endif
