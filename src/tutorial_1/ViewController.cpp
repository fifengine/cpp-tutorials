//*****************************************************************************
// FILE NAME:  ViewController.cpp
//
//*****************************************************************************
#include "view/camera.h"

#include "Game.h"
#include "ViewController.h"

#include <cassert>

//!***************************************************************
//! @details:
//! constructor
//!
//! @param[in]: parent
//! the Game object that owns this view controller
//!
//! @param[in]: camera
//! an optional camera that this view controller will manipulate
//!
//!***************************************************************
ViewController::ViewController(Game* parent, FIFE::Camera* camera)
: m_parent(parent), m_camera(camera), m_zoomIncrement(0.75), 
  m_maxZoom(4), m_minZoom(0.25), m_rotateIncrement(90)
{
	assert(m_parent);
}

//!***************************************************************
//! @details:
//! destructor
//!
//!***************************************************************
ViewController::~ViewController()
{

}

//!***************************************************************
//! @details:
//! associates this view controller with a camera
//!
//! @param[in]: camera
//! the camera class that will be controlled
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::AttachCamera(FIFE::Camera* camera)
{
	m_camera = camera;
}

//!***************************************************************
//! @details:
//! allows user the enable or disable the camera
//!
//! @param[in]: enable
//! true - enable camera
//! false - disables camera
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::EnableCamera(bool enable)
{
	if (m_camera)
	{
		m_camera->setEnabled(enable);
	}
}

//!***************************************************************
//! @details:
//! calculates the appropriate zoom level and increase the cameras
//! zoom
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::ZoomIn()
{
	if (m_camera)
	{
		// calculate the zoom in level
		double zoom = m_camera->getZoom() / m_zoomIncrement;

		if (zoom <= m_maxZoom)
		{
			m_camera->setZoom(zoom);
		}
	}
}

//!***************************************************************
//! @details:
//! calculates the appropriate zoom and zooms the camera out
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::ZoomOut()
{
	if (m_camera)
	{
		// calculate the zoom out level
		double zoom = m_camera->getZoom() * m_zoomIncrement;

		if (zoom >= m_minZoom)
		{
			m_camera->setZoom(zoom);
		}
	}
}

//!***************************************************************
//! @details:
//! rotates the camera left by a fixed rotation increment
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::RotateLeft()
{
	if (m_camera)
	{
		// calculate rotation
		double rotation = static_cast<int>((m_camera->getRotation() - m_rotateIncrement)) % 360;

		m_camera->setRotation(rotation);
	}
}

//!***************************************************************
//! @details:
//! rotates the camera right by a fixed rotation amount
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ViewController::RotateRight()
{
	if (m_camera)
	{
		// calculate rotation
		double rotation = static_cast<int>((m_camera->getRotation() + m_rotateIncrement)) % 360;

		m_camera->setRotation(rotation);
	}
}

