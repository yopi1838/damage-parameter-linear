#pragma once
/**
    * \file
    * \brief Interface for accessing informations in the three drawable "interactive" filter elements in kernel/src/rangeelementinteractive.h
    *   (polygon,rectangle,ellipse)
    */

#include "module/interface/irangeelement.h"

namespace itascaxd {
    /// \brief Interface for accessing informations in the three drawable "interactive" filter elements in kernel/src/rangeelementinteractive.h
    ///   (polygon,rectangle,ellipse)
    /** These elements are defined by a list of vertices (DVect3), a projection mode (parallel/perspective), 
      *  and, if the projection is in perspective mode, by an origin point (DVect3).\n
      * This projection mode correspond to the one which was active WHEN THE RANGE WAS CREATED.\n
      * Therefore, if in perspective projection, the shape of the filter element region
      * is a cone with its top at the origin point.\n
      * \b Note - an ellipse is stored internally via its circumscribed rectangle
      * \ingroup kernel
      */
    class IRangeElementPolygonBased { // name = "interactivepolygon"
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815bbe;

        /// returns true if the projection mode was in perspective when the filter element was created
        virtual bool isPerspective() const=0; 

        /// returns the list of vertices that delimitate a polygonal shape in 3D "real" coordinates
        virtual QVector<DVect> getVertices()const=0;
    #ifdef THREED
        /// returns the origin (top) of the pyramidal region - usefull when isPerspective() returns true
        virtual DVect getOrigin() const=0;
    #endif

        /// clears the list of vertices that delimitate a polygonal shape in 3D "real" coordinates
        virtual void clearVertices()=0;
        /// Checks internally if list's vertices define a plane. Returns false without any copy if not, 
        /// otherwise returns true and sets (by copy) the list of vertices that delimitate a polygonal 
        /// shape in 3D "real" coordinates
        virtual bool setVertices(const QVector<DVect> &list)=0;
        /// Returns \b true and adds a new vertex to the list of vertices (push_back) if this new vertex is 
        /// in the plane defined by previous vertices in the list.\n
        /// Returns \b false without adding the vertex if not.
        virtual bool addVertex(const DVect &vec)=0;
    #ifdef THREED
        /// In perspective mode, sets the origin point (top of the pyramidal region)
        virtual void setOrigin(const DVect &vec)=0;
        /// Sets the projection mode to be parallel(b=false)/perspective(b=true)
        virtual void setProjectionType(bool b)=0;
    #endif

        /// Returns a pointer to the IRangeElement object.
        virtual IRangeElement *getIRangeElement()=0;

        //SIGNALS
        /// Signal emitted when vertices have changed.
        virtual void verticesChanged()=0;
    };
} // namespace itascaxd
// EoF