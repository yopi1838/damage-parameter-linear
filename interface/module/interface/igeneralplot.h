#pragma once

#include "dim.h"

/**
    * \file
    * \brief This class allows user to draw basic geometric plots using FISH functions.
    */

namespace itasca {
    class IFish;
}

namespace itascaxd {
    /// \brief This class allows user to draw basic geometric plots using FISH functions.

    /// This interface is passed to FISH to allow implementation of FISH plotting.\n
    /// In the future, this same interface may be used for general plugin plotting purposes.
    /// \ingroup module
    class IGeneralPlot {
    public:
        /// Draw a circle with a given center and radius. 
        virtual void addCircle(bool solid,const DVect &cent, const double & rad)=0;
        /// Draw a sphere with a given center and radius. 
        virtual void addSphere(bool solid,const DVect &cent, const double & rad)=0;
        /// Draw a line beteween two points
        virtual void addLine(const DVect &p1, const DVect &p2)=0;
        /// Draw a polygon. Points are stored on vector 'points' 
        virtual void addPoly(bool solid,const QVector<DVect> & points,bool cl)=0;
        /// Draw a rectangle 
        virtual void adDExtent2(bool solid,const DExtent2 &r)=0;
        /// Write out a string on a given coordinate (point v). 
        virtual void addString(const DVect &v, const QString &s)=0;
        /// Move to a point in the screen coordinates
        virtual void addMoveTo(const DVect &p1)=0;
        /// Draw a line (used in conjuction with function addMoveTo)
        virtual void addLineTo(const DVect &p1)=0;
        /// Set the current plot color. Objects draw after this command will be created with this color.
        virtual void setColor(int c)=0;
        /// Given a label, sets the current plot color. Objects draw after this command will 
        ///  be created with this color.
        virtual void setColor(const QString & c)=0;
        /// Set dash patterns for lines. Zero is solid.
        virtual void setDashPattern(int lp)=0;
        /// Set line width
        virtual void setLineWidth(int w)=0;
        /// Set font size
        virtual void setFontData(const double & fz)=0;
        /// Set transparency. Zero is opaque. Valid values between 0 and 100.
        virtual void setTransparencyFactor(int c)=0;
    };
} // namespace itascaxd
// EoF
