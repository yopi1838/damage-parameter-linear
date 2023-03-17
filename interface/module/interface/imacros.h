#pragma once
//
//#include "base/src/baseqt.h"
//
///**
//    * \file
//    * \brief Interface to the list of Macros used during command processing.
//    *  Access only - to change use the command line.
//    */
//
//namespace itasca {
//    /// \brief Interface to the list of Macros used during command processing.
//    /// \ingroup module
//    class IMacros {
//    public:
//        /// Returns an StringList that contains all of the macro keys that are currently defined.
//        virtual QStringList getKeys() const=0;
//
//        /// Returns an StringList that containsthe list of tokens that are substituted for key.
//        virtual QStringList getValue(const QString &key) const=0;
//
//        /// Returns an StringList that containsthe list of delimiters that were removed during tokenization.
//        virtual QStringList getSkipped(const QString &key) const=0;
//
//        /// Returns a QObject interface. 
//        virtual QObject *getQObject()=0;
//
//        // signals
//        //void changed();
//    };
//} // namespace itasca
//// EoF
