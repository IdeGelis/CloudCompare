//##########################################################################
//#                                                                        #
//#                               CCLIB                                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 of the License.  #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef MANUAL_SEGMENTATION_TOOLS_HEADER
#define MANUAL_SEGMENTATION_TOOLS_HEADER

//Local
#include "CCCoreLib.h"
#include "CCToolbox.h"
#include "Neighbourhood.h"

namespace CCLib
{

class GenericIndexedCloud;
class GenericIndexedCloudPersist;
class GenericIndexedMesh;
class GenericProgressCallback;
class ReferenceCloud;
class SimpleMesh;
class Polyline;

//! Manual segmentation algorithms (inside/outside a polyline, etc.)
class CC_CORE_LIB_API ManualSegmentationTools : public CCToolbox
{
public:

	//! Extracts the points that fall inside/outside of a 2D polyline once projected on the screen
	/** The camera parameters of the screen must be transmitted to this method,
		as well as the polyline (generally drawn on the screen by a user)
		expressed in the screen coordinates.
		\param aCloud the cloud to segment
		\param poly the polyline
		\param keepInside if true (resp. false), the points falling inside (resp. outside) the polyline will be extracted
		\param viewMat the optional 4x4 visualization matrix (OpenGL style)
		\return a cloud structure containing references to the extracted points (references to - no duplication)
	**/
	static ReferenceCloud* segment(GenericIndexedCloudPersist* aCloud, const Polyline* poly, bool keepInside, const float* viewMat=0);

	//! Extracts the points which associated scalar value fall inside a specified interval
	/** All the points with an associated scalar value comprised between minDist and maxDist
		will be extracted.
		Warning: be sure to activate an OUTPUT scalar field on the input cloud
		\param aCloud the cloud to segment
		\param minDist the lower boundary
		\param maxDist the upper boundary
		\return a new cloud structure containing the extracted points (references to - no duplication)
	**/
	static ReferenceCloud* segment(GenericIndexedCloudPersist* aCloud, ScalarType minDist, ScalarType maxDist);


	//! Tests if a point is inside a polygon (2D)
	/** \param P a 2D point
		\param polyVertices polygon vertices (considered as ordered 2D poyline vertices)
		\return true if P is inside poly
	**/
	static bool isPointInsidePoly(const CCVector2& P, const GenericIndexedCloud* polyVertices);

	//! Tests if a point is inside a polygon (2D)
	/** \param P a 2D point
		\param polyVertices polygon vertices (considered as ordered 2D poyline vertices)
		\return true if P is inside poly
	**/
	static bool isPointInsidePoly(const CCVector2& P, const std::vector<CCVector2>& polyVertices);

	//! Segments a mesh knowing which vertices should be kept or not
	/** This method takes as input a set of vertex indexes and creates a new mesh
	composed either of the triangles that have exactly those vertices as
	summits (pointsWillBeInside=true), or all the triangles from which none of
	the vertices is part of this subset (pointsWillBeInside=false). There is no
	re-triangulation on the border.
	\param theMesh a mesh
	\param pointsIndexes the vertices indexes as a set of references
	\param pointsWillBeInside specifies if the points corresponding to the input indexes should be the new mesh vertices, or the opposite
	\param progressCb the client application can get some notification of the process progress through this callback mechanism (see GenericProgressCallback)
	\param destCloud optionnaly, a cloud object can be specified to be associated to the new created mesh object, instead of the cloud associated to the ReferenceCloud "pointsIndexes"
	\param indexShift optionnaly, a shift can be added to all vertex indexes of the new mesh
	\return a new mesh structure, or 0 if something went wrong
	**/
	static GenericIndexedMesh* segmentMesh(GenericIndexedMesh* theMesh, ReferenceCloud* pointsIndexes, bool pointsWillBeInside, GenericProgressCallback* progressCb = 0, GenericIndexedCloud* destCloud = 0, unsigned indexShift = 0);

	//! Input/output parameters for the segmentMeshWitAAPlane method
	struct MeshCutterParams
	{
		SimpleMesh* insideMesh;
		SimpleMesh* outsideMesh;
		double epsilon;
		//for infinite plane intersection
		unsigned char planeOrthoDim;
		double planeCoord;
		//for box intersection
		CCVector3d bbMin, bbMax;

		MeshCutterParams()
			: insideMesh(0)
			, outsideMesh(0)
			, epsilon(ZERO_TOLERANCE)
			, planeOrthoDim(0)
			, planeCoord(0)
			, bbMin(0, 0, 0)
			, bbMax(0, 0, 0)
		{}
	};

	static bool segmentMeshWitAAPlane(GenericIndexedMesh* mesh,
		GenericIndexedCloudPersist* vertices,
		MeshCutterParams& ioParams,
		GenericProgressCallback* progressCb = 0);

	static bool segmentMeshWitAABox(GenericIndexedMesh* mesh,
		GenericIndexedCloudPersist* vertices,
		MeshCutterParams& ioParams,
		GenericProgressCallback* progressCb = 0);
};

}

#endif //MANUAL_SEGMENTATION_TOOLS_HEADER
