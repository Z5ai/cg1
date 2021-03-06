/*
	This file is part of NSEssentials.

	Use of this source code is granted via a BSD-style license, which can be found
	in License.txt in the repository root.

	@author Wenzel Jakob
	@author Nico Schertler
*/

#pragma once

#include <nanogui/glutil.h>
#include <nanogui/widget.h>

#include "math/BoundingBox.h"

namespace nse {
	namespace gui
	{

		//Provides a 3D camera with arcball interaction capabilities.
		class Camera
		{
		public:

			//Initialize the camera. parent is the widget onto which the scene is rendered and is only used to query its size.
			Camera(const nanogui::Widget& parent);

			//Computes view and projection matrix for the current camera state.
			void ComputeCameraMatrices(
				Eigen::Matrix4f &view,
				Eigen::Matrix4f &proj,
				float customAspectRatio = 0) const;

			//Applies a zoom by changing the view distance. Positive values of amount decrease the distance.
			void Zoom(float amount);

			//Sets the extent of the scene, which is kept between znear/zfar.
			void SetSceneExtent(const nse::math::BoundingBox<float, 3>& bbox);

			//Translates and zooms the camera in a way that it shows the entire bounding box while keeping the orientation.
			//The parameters are only estimated; it is not guaranteed that the bounding box actually fits in the viewport.
			void FocusOnBBox(const nse::math::BoundingBox<float, 3>& bbox);

			//Keeps the camera's rotation and viewing distance and sets the focus point to the provided location.
			void FocusOnPoint(const Eigen::Vector3f& point);

			//Rotates the camera, such that it becomes horizontally aligned
			void MakeHorizontal();

			//Rotates the camera around the current focus point by rotation. The rotation is expressed in the
			//camera's local coordinate system.
			void RotateAroundFocusPointLocal(const Eigen::Quaternionf& rotation);

			//Rotates the camera around the current focus point by rotation. The rotation is expressed in the
			//global coordinate system.
			void RotateAroundFocusPointGlobal(const Eigen::Quaternionf& rotation);

			//Forwarded mouse button event.
			bool HandleMouseButton(const Eigen::Vector2i &p, int button, bool down, int modifiers);
			//Forwarded mouse move event.
			bool HandleMouseMove(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers);
			//Forwarded resize event.
			void resize(const Eigen::Vector2i & s);

			void FixClippingPlanes(float znear, float zfar);

			//Returns the point that the camera focuses on
			const Eigen::Vector3f& GetFocusPoint() const;

			struct CamParams
			{
				nanogui::Arcball arcball;

				//bounding sphere of scene
				Eigen::Vector3f sceneCenter;
				float sceneRadius;

				float fovy = 45.0f;
				Eigen::Vector3f focusPoint = Eigen::Vector3f::Zero();
				float viewDistance = 5;
			};

			//Returns the current camera parameters
			const CamParams& saveParams() const { return params; }
			//Restores the camera parameters that have been previously saved.
			void restoreParams(const CamParams& params) { this->params = params; }

		private:
			const nanogui::Widget& parent;

			enum InteractionMode
			{
				None,
				Translate,
				Rotate
			} interactionMode = None;

			CamParams params;

			Eigen::Vector3f modelTranslation_start = Eigen::Vector3f::Zero();
			Eigen::Vector2i translation_start; //mouse position on the screen where translation started	

			float fixedZNear, fixedZFar;
		};
	}
}