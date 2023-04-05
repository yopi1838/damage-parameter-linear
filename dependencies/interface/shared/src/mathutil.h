#pragma once

// Mathematical utility functions

// Attempt to clear up nomenclature:
// a)  "Point" single location in space
// b)  "Line" is an *infinite* line, designated by either two points in space or a starting point and a direction.
// c)  "Ray" is a semi-infinite line, from an origin in a direction.
// d)  "Plane" is an *infinite* plane, designated generally by origin and normal.
// e)  "Segment" is a finite line between two endpoints
// f)  "Triangle" is three vertices.
// g)  "Polygon" is N vertices, where N is three or more, indicating a finite polygon.   
//               Theses area assumed to be coplanar and convex!
// All of these objects come in 2D and 3D versions.

#include "exportshared.h"
#include "base/src/farray.h"

namespace itasca {
    const double defRayTol = 1.0e-9; // Default tolerance used by ray-intersection function

    //
    // Tolerance calculation functions
    //
    // Helpful tolerance check aid - dTol returns a double precision tolerance based on
    // limits<double>::epsilon() and the target size.
    // Use mul to increase the tolerance from a relative value of about 1e-14.
    SHARED_EXPORT double dtol(const double &size,const double &mul=1.0);

    // Helpful tolerance check aid - dTol returns a double precision tolerance based on
    // limits<double>::epsilon() and the target size.
    // Use mul to increase the tolerance from a relative value of about 1e-14.
    SHARED_EXPORT double dtol(const DVect2 &size,const double &mul=1.0);

    // Helpful tolerance check aid - dTol returns a double precision tolerance based on
    // limits<double>::epsilon() and the target size.
    // Use mul to increase the tolerance from a relative value of about 1e-14.
    SHARED_EXPORT double dtol(const DVect3 &size,const double &mul=1.0);

    // Helpful tolerance check aid - dTol returns a double precision tolerance based on
    // limits<double>::epsilon() and the target size.
    // Use mul to increase the tolerance from a relative value of about 1e-14.
    SHARED_EXPORT double dtol(const DExtent2 &size,const double &mul=1.0);

    // Helpful tolerance check aid - dTol returns a double precision tolerance based on
    // limits<double>::epsilon() and the target size.
    // Use mul to increase the tolerance from a relative value of about 1e-14.
    SHARED_EXPORT double dtol(const DExtent3 &size,const double &mul=1.0);

    //
    // Tolerance Checking Functions
    //
    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const double &val1,const double &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const DVect2 &val1,const DVect2 &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const DVect3 &val1,const DVect3 &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const DExtent2 &val1,const DExtent2 &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const DExtent3 &val1,const DExtent3 &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns TRUE if val1 and val2 are equal within a tolerance determined by a
    // characteristic size charval.  If charval=0.0 (default) the charval is determined
    // by the absolute maximum of val1 and val2.   Mul multiplies the default tolerance
    // calculated with dtol().
    SHARED_EXPORT bool checktol(const std::vector<DVect3> &val1,const std::vector<DVect3> &val2,const double &charval=0.0,
                              const double &mul=1.0);

    // Returns the determinant of 3 vectors
    SHARED_EXPORT double determinant(const DVect3 &c1,const DVect3 &c2,const DVect3 &c3);

    // Returns the determinant of 2 2D vectors
    SHARED_EXPORT double determinant(const DVect2 &c1,const DVect2 &c2);

    // Returns the sign to within a tolerance
    SHARED_EXPORT int signum(const double &,const double &tol=0.0);


    //
    // Polygon analysis functions.  NOTE: Probably want to add convexity and planarity checks here.
    //
    // Returns the 2D area of the convex polygon poly.
    SHARED_EXPORT double polygonArea(const FArray<DVect2> &poly);

    // Returns the 3D area*normal in first, and the centroid in second.
    // Assumes poly is convex and planar.
    SHARED_EXPORT QPair<DAVect3,DVect3> polygonInfo(const FArray<DVect3> &poly);
    // Returns TRUE if 2d point pos is inside the 2D polygon poly.

    //
    // Inside checks
    //
    // Returns TRUE If the point projected onto the surface of the planar, closed, convex polygon 
    //   falls on the inside of the polygon.
    // The first part of the pair is true if the point is inside, 
    //   the second is the projected point location.
    // Tolerance is absolute, based on z-component of cross products.  
    // If zero it is set to a relative value, based on limits<float>::epsilon()*max absolute component of all poly vertices.
    // NOTE: No check is made to ensure poly is planar or convex.
    SHARED_EXPORT QPair<bool,DVect3> insidePolygonPoint(const FArray<DVect3> &poly,const DVect3 &point,const double &tolerance=limits<float>::epsilon());
    SHARED_EXPORT bool               insidePolygonPoint(const FArray<DVect2> &poly,const DVect2 &pos,const double &tolerance=0.0);

    // Returns whether or not a point is inside or on an edge of a 3D triangle. 
    // Return value 0 means no, return value 1 means yes, return value 2 means on an edge
    SHARED_EXPORT int insideTrianglePoint(const DVect3 &,const DVect3 &,const DVect3 &,const DVect3 &norm,const DVect3 &pt,double tol=0.0);
    SHARED_EXPORT int insideTrianglePoint(const DVect2 &,const DVect2 &,const DVect2 &,const DVect2 &pt,double tol=0.0);

    // Uses barycentric coordinates to return a Boolean indicating whether or not a point is inside a 3d triangle
    SHARED_EXPORT bool insideTrianglePointBarycentric(const DVect3 &,const DVect3 &,const DVect3 &,const DVect3 &norm,const DVect3 &pt,double tol=0.0);

    //
    // Intersection functions
    //

    // Returns TRUE in the first part of the return, if the segment v11-v12 intersects segement v21-v22.
    // Returns the location of the intersection in the second part of the return.
    // The tolerance is *absolute*, if zero it set to limits<float>::epsilon() times the maximum absolute component of the four input points.
    SHARED_EXPORT QPair<bool,DVect2> intersectSegmentSegment(const DVect2 &v11,const DVect2 &v12,const DVect2 &v21,const DVect2 &v22,const double &tolerance=0.0);

    // Returns TRUE if the ray (base,dir) intersects the point p, with *absolute* tolerance provided.
    // Dir is assumed to be a unit vector.
    // If zero the tolerance is defRayTol times the maximum absoute component of base and p.
    // If either base or p are identically zero then the tolerance is arbitrarily limits<float>::epsilon().
    // If pdist is non-null *and* an intersection occurs, then it is filled with the distance from base to the intersection.
    // If ptol is non-null, then it is filled with the tolerance used.
    SHARED_EXPORT bool intersectRayPoint(const DVect2 &base,const DVect2 &dir,const DVect2 &p,const double &tolerance=0.0,double *pdist=nullptr,double *ptol=nullptr);
    SHARED_EXPORT bool intersectRayPoint(const DVect3 &base,const DVect3 &dir,const DVect3 &p,const double &tolerance=0.0,double *pdist=nullptr,double *ptol=nullptr);

    // Returns TRUE if the ray (base,dir) intersects the segment (v11,v12), with *absolute* tolerance provided.
    // Dir is assumed to be a unit vector.
    // If zero the tolerance is defRayTol times the maximum absoute component of base, v11, and v12.
    // If pdist is non-null *and* an intersection occurs, then it is filled with the distance from base to the intersection.
    // If ptol is non-null, then it is filled with the tolerance used.
    SHARED_EXPORT bool intersectRaySegment(const DVect2 &base,const DVect2 &dir,const DVect2 &v11,const DVect2 &v12,const double &tolerance=0.0,double *pdist=nullptr,double *ptol=nullptr);
    SHARED_EXPORT bool intersectRaySegment(const DVect3 &base,const DVect3 &dir,const DVect3 &v11,const DVect3 &v12,const double &tolerance=0.0,double *pdist=nullptr,double *ptol=nullptr);

    // Returns TRUE and the point of intersection if the segment a1,a2 cross the infinite plane
    //   defined by origin,normal.
    SHARED_EXPORT QPair<bool,DVect3> intersectPlaneSegment(const DVect3 &a1,const DVect3 &a2,const DVect3 &origin,const DVect3 &normal,const double &tolerance=0.0);

    // Returns TRUE in the first part if the segment v1-v2 intersections the outside of the closed polygon poly, and
    //   returns the location of the intersection in the second part
    // Tolerance is *absolute*, if zero then is calculated based on maximum vertex abs component (poly+segment) * limits<float>::epsilon().
    SHARED_EXPORT QPair<bool,DVect2> intersectPolygonSegment(const FArray<DVect2> &poly,const DVect2 &v1,const DVect2 &v2,const double &tolerance=0.0);

    // Returns whether or not a triangle in 3D (tv1,tv2,tv3) intersects a ray (base,dir), with the *absolute* tolerance provided.
    // Dir is assumed to be a unit vector.
    // If zero the tolerance is defRayTol times the maximum absolute component of tv1,tv2,tv3, and base.
    // If pdist is non-null *and* an intersection occurs, then it is filled with the distance from base to the intersection.
    // If ptol is non-null, then it is filled with the tolerance used.
    // If mode is non-null *and* and intersection occurs then a return code specifies whether or not the 
    // intersection is not within ptol of and edge or vertex (0), within ptol of edge 1,2,3 (1,2,3 where edge 1 is between 
    // tv1 and tv2, edge 2 is between tv2 and tv3, and edge 3 is between tv3 and tv1), or within 
    // ptol of vertex 1,2,3 (4,5,6). In addition, as a special condition, the mode can be set to 101 delineating the case that 
    // the ray is parallel to the triangle. False is still returned, though.
    SHARED_EXPORT bool intersectTriangleRay(const DVect3 &tv1,const DVect3 &tv2,const DVect3 &tv3,const DVect3 &base,const DVect3 &dir,const double &tolerance=0.0,double *pdist=nullptr,double *ptol=nullptr,int *mode=nullptr);

    // Returns TRUE if two triangular polygons intersect each other.
    // NOTE: Optimized special case of intersectPolygonPolygon, with the same caveats. MOO
    SHARED_EXPORT bool intersectTriangleTriangle(const DVect3 &tv11,const DVect3 &tv12,const DVect3 &tv13,const DVect3 &tv21,const DVect3 &tv22,const DVect3 &tv23,const double &tolerance=limits<float>::epsilon());

    // Returns TRUE if the two planar, closed, convex polygons intersect each other.
    // NOTE: No check is made to ensure either polygon is planar or convex.
    // NOTE: Implemented by checking for edge intersections between all edges of poly1 with poly2, 
    //       and all edges of poly2 with poly1.  Hopefully that doesn't miss anything.  MOO
    SHARED_EXPORT bool intersectPolygonPolygon(const FArray<DVect3> &poly1,const FArray<DVect3> &poly2,const double &tolerance=limits<float>::epsilon());
    SHARED_EXPORT bool intersectPolygonPolygon(const FArray<DVect2> &poly1,const FArray<DVect2> &poly2,const double &tolerance=limits<float>::epsilon());

    // Returns the polygon representing the intersection between poly1 and poly2.
    // If the intersection does not exist then may return an empty array.
    // The tolerance is absolute - if not set the maximum absolute component of all vertices in poly1 and poly2 * limits<float>::epsilon() is used.
    SHARED_EXPORT bool intersectPolygonPolygon(const FArray<DVect2> &poly1,const FArray<DVect2> &poly2,FArray<DVect2> *inter=0,const double &tolerance=0.0);

    // Returns the area of overlap between poly1 and poly2, when poly2 is projected onto the plane of poly1.
    // Assumes both poly1 and poly2 are planar and convex.
    // Tolerance is absolute!  If not specified it is set to the maximum absolute component of poly1 and poly2 * limits<float>::epsilon().
    SHARED_EXPORT double projectedIntersectionAreaPolygonPolygon(const FArray<DVect3> &poly1,const FArray<DVect3> &poly2,const double &tolerance=0.0);

    //
    // Distance Functions
    //
    // Returns the minimum distance between two lines (each line is defined by two points a1,a2 and b1,b2)
    // NOTE: 2 added to end of name to disinguish between old distanceLineLine, which by the new terminology was actually distanceSegmentSegment.
    SHARED_EXPORT double distanceLineLine2(const DVect3 &a1,const DVect3 &a2,const DVect3 &b1,const DVect3 &b2);

    // Return the closest distance between a 3D line segment and a point in space.
    SHARED_EXPORT double distanceSegmentPoint(const DVect3 &e1,const DVect3 &e2,const DVect3 &pt);
    SHARED_EXPORT double distanceSegmentPoint(const DVect2 &e1,const DVect2 &e2,const DVect2 &pt);

    // Returns the minimum distance between two line segments (each segment is defined by two points a1,a2 and b1,b2)
    SHARED_EXPORT double distanceSegmentSegment(const DVect3 &a1,const DVect3 &a2,const DVect3 &b1,const DVect3 &b2);
    SHARED_EXPORT double distanceSegmentSegment(const DVect2 &a1,const DVect2 &a2,const DVect2 &b1,const DVect2 &b2);

    // Returns the minimum distance between a line segment and the surface of a triangle defined by three points.
    SHARED_EXPORT double distanceTriangleSegment(const DVect3 &tv1,const DVect3 &tv2,const DVect3 &tv3,const DVect3 &a1,const DVect3 &a2);
    SHARED_EXPORT double distanceTriangleSegment(const DVect2 &tv1,const DVect2 &tv2,const DVect2 &tv3,const DVect2 &a1,const DVect2 &a2);

    // Returns the minimum distance between two 3D trinagles
    SHARED_EXPORT double distanceTriangleTriangle(const DVect3 &tv11,const DVect3 &tv12,const DVect3 &tv13,const DVect3 &tv21,const DVect3 &tv22,const DVect3 &tv23);
    SHARED_EXPORT double distanceTriangleTriangle(const DVect2 &tv11,const DVect2 &tv12,const DVect2 &tv13,const DVect2 &tv21,const DVect2 &tv22,const DVect2 &tv23);

    SHARED_EXPORT double distancePolygonPoint(const FArray<DVect2> &poly,const DVect2 &a1);

    // Returns the minimum distance between closed, convex, planar polygon poly and the line
    //   segment from a1 to a2.
    // Note that no check is made to ensure the polygon is planar or convex.
    SHARED_EXPORT double distancePolygonSegment(const FArray<DVect3> &poly,const DVect3 &a1,const DVect3 &a2);

    //
    // Closest Point Functions
    //
    // Finds the closest point to v on the infinite line segment defined by p1 to p2.
    // The return value is a scalar where 0.0 is p1, and 1.0 is p2, that can be from -inf to inf.
    // If p1 and p2 are the same point, returns 0.0.
    SHARED_EXPORT double closestLinePoint(const DVect3 &v,const DVect3 &p1,const DVect3 &p2);
    SHARED_EXPORT double closestLinePoint(const DVect2 &v,const DVect2 &p1,const DVect2 &p2);

    // Finds the closest point on line p1 to p2 to line p3 to p4.
    // Will return p1 if lines are parallel.
    SHARED_EXPORT DVect3 closestLineLine(const DVect3 &p1,const DVect3 &p2,const DVect3 &p3,const DVect3 &p4,const double &tolerance=0.0);

    // Returns the closest point on a 3D line segment to a point in space - the first two arguments
    //   are the line segment ends, the third is the point, and the fourth is a tolerance used in the calculation
    SHARED_EXPORT DVect3 closestSegmentPoint(const DVect3 &v1,const DVect3 &v2,const DVect3 &P);
    SHARED_EXPORT DVect2 closestSegmentPoint(const DVect2 &v1,const DVect2 &v2,const DVect2 &P);


    // Returns the closest point on a 3D triangular facet to a point in space - the first three arguments
    //   are the facet vertices, the fourth is the point
    SHARED_EXPORT DVect3 closestTrianglePoint(const DVect3 &v1,const DVect3 &v2,const DVect3 &v3,const DVect3 &P);
    // another implementation that has some better properties
    void getMinEdge02(const double &a11,const double &b1,DVect2 &p);
    void getMinEdge12(const double &a01,const double &a11,const double &b1,const double &f10,const double &f01,DVect2 &p);
    void getMinInterior(const DVect2 &p0,const double &h0,const DVect2 &p1,const double &h1,DVect2 &p);
    SHARED_EXPORT DVect3 closestTrianglePoint2(const DVect3 &v1,const DVect3 &v2,const DVect3 &v3,const DVect3 &P,DVect2 *st=nullptr);
    SHARED_EXPORT DVect2 closestTrianglePoint(const DVect2 &v1,const DVect2 &v2,const DVect2 &v3,const DVect2 &P);

    // Returns the closest point of line segment a1-a2 to plane defined by origin,normal. 
    // Will always be either one of the endpoints, or the point where the segment actually intersects the plane.
    SHARED_EXPORT DVect3 closestPlaneSegment(const DVect3 &a1,const DVect3 &a2,const DVect3 &origin,const DVect3 &normal);

    // Returns the closest point of line segment a1-a2 to line segment a3-a4. 
    // Will always be either one of the endpoints, or the point where the segment actually intersects the segment.
    SHARED_EXPORT double closestSegmentSegment(const DVect2 &a1,const DVect2 &a2,const DVect2 &a3,const DVect2 &a4,DVect2 *ps1,DVect2 *ps2);
    SHARED_EXPORT double closestSegmentSegment(const DVect3 &a1,const DVect3 &a2,const DVect3 &a3,const DVect3 &a4,DVect3 *ps1,DVect3 *ps2);

    SHARED_EXPORT double closestSegmentRay(const DVect3 &s1,const DVect3 &s2,const DVect3 &raysource,const DVect3 &raydir,DVect3 *ps1,DVect3 *ps2);

    // Returns the closest point between a polygon and a segment.
    SHARED_EXPORT double closestPolygonSegment(const FArray<DVect3> &poly,const DVect3 &a1,const DVect3 &a2,DVect3 *ppoly,DVect3 *pseg);

    SHARED_EXPORT DVect2 closestExtentPoint(const DExtent2 &e,const DVect2 &v);
    SHARED_EXPORT DVect3 closestExtentPoint(const DExtent3 &e,const DVect3 &v);


    // Statically calculated integral power
    template <quint32 V,quint32 E> struct PowUInt32 {
        static const quint32 result = V*PowUInt32<V,E-1>::result;
    };
    template <quint32 V> struct PowUInt32<V,1> {
        static const quint32 result = V;
    };

    // Returns segment n-th segment, if segment [0,1] is divided in N zones according to geometric 
    //     progression with defined common ratio.
    // n >=0 and n<=N.
    //reference https://en.wikipedia.org/wiki/Geometric_progression
    SHARED_EXPORT double geomProgressSum(double ratio, quint32 N, quint32 n);
    
    //convert ratio to factor (ratio of the last to the first elements in geom.progression)
    SHARED_EXPORT double ratioToFactor(double ratio, quint32 N);
    
    // converts factor (ratio of the last to the first elements in geom.progression) to common 
    //     ratio of the geometric progression
    SHARED_EXPORT double factorToRatio(double factor, quint32 N);
    
    //// Checks if polygons is convex.   The polygon must have atleast 3 points.
    //// Also returns sharpest corner index.
    //SHARED_EXPORT bool poligonConvex(const QList<DVect2>  &poly, int &sharpestCornerIndex);
    
    //// makes list of convex polygon out of non-covex one. Uses ear clipping algorithm.
    ////If input polygon convex -- will return empy list.
    //SHARED_EXPORT QList<FArray<DVect2>> makePolygonConvex(const FArray<DVect2> &poly);

    
    //// Projects a point onto a 3D triangle if the point lies above/below the triangle. 
    //// returns false if the point is not above/below the triangle and the point is returned
    //// as well
    //SHARED_EXPORT bool projectPointInTriangle(const DVect3 &v1,const DVect3 &v2,const DVect3 &v3,const DVect3 &pt,DVect3 &ret);

    /// Helper function to create speheres: returns a point in the sphere.
    /// \param divisions Integer number of divisions.
    /// \param t Integer number of horizontal divisions of point to return (from 0 to 2*division).
    /// \param z Integer number of vertical divisions of point to return (from 0 to division).
    SHARED_EXPORT DVect3 pointOnSphere(int divisions,int t,int z);

    /// Use interpolation via a triangle to get a value at a new point. Returns 0 if not inside the triangle.
    /// \param p1 Triangle pt 
    /// \param v1 Value at first point 
    /// \param p2 Triangle pt 
    /// \param v2 Value at second point 
    /// \param p3 Triangle pt 
    /// \param v3 Value at second point
    /// \param pt Point for interpolation
    SHARED_EXPORT double triangleInterpolation(const DVect2 &p1,const double &v1,const DVect2 &p2,const double &v2,const DVect2 &p3,const double &v3,const DVect2 &pt);

    /// Returns a 'nice' valued interval representing the span between \a min and \a max.
    /// \a target is the number of intervals requested - the actual number of intervals will be
    /// from \a target to \a target*2.
    /// \a halfallowed allows you to disallow the factor 2.5 in the interval value.
    /// \ingroup plotitemsrc
    SHARED_EXPORT double getInterval(const double &min,const double &max,int target,bool halfallowed=true);

    /// Given two lines (\a p1 in \a n1 direction, \a p2 in \a n2 direction) returns (t,u) such that 
    /// \a p1 + \a n1*t  and \a p3 + \a n3*u represent the shorted possible line between these two lines.
    /// Returns DVect2(limits<double>::max()) if no solution.
    /// \ingroup plotitemsrc
    SHARED_EXPORT std::tuple<double,double> lineIntersection(const DVect3 &p1,const DVect3 &n1,
                                                             const DVect3 &p3,const DVect3 &n3);

    /// Returns the closest point on line lineStart + lineDir*t to point p.
    /// Returns both the point (first), and the parameter value t.
    SHARED_EXPORT std::tuple<DVect3,double> lineClosest(const DVect3 &p,const DVect3 &lineStart,
                                                        const DVect3 &lineDir);

    /// Given a line and a circle in 3D, return the closest point on the circle to the line.
    /// The line is defined by a point and a direction.
    /// The circle is defined by a center, and a vector indicating the normal to the plane
    ///    the circle lies in whose magitude is the circle direction.
    /// Note: Need to work on exact solution, right now return is approximate.
    SHARED_EXPORT DVect3 circleIntersection(const DVect3 &p1,const DVect3 &dir,
                                            const DVect3 &cen,const DVect3 &normRad);

    /// \a angle of rotation about \a point is Clockwise while looking down \a axis.
    /// \ingroup plotitemsrc
    SHARED_EXPORT DVect2 rotateAbout(const DAVect2 &axis,const DVect2 &point,const double &angle);
    SHARED_EXPORT DVect3 rotateAbout(const DAVect3 &axis,const DVect3 &point,const double &angle);
} // namespace itasca
// EoF
