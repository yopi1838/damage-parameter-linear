#include "modelweakplanetype.h"

//static const Double pi     = 3.14159265358979323846264338327950;
static const Double degrad = 0.01745329251994329576923690768488;

namespace models {

ModelWeakplaneType::ModelWeakplaneType() : 
    dip_(0.0),dd_(0.0),angle_(0.0) {
}

Variant ModelWeakplaneType::getProperty(UInt index) const {
    switch (index)  {
    case 1: return(dip_);
    case 2: return(dd_);
    case 3: return norma_.norm().x(); 
    case 4: return norma_.norm().y(); 
    case 5: return norma_.norm().z();  
    case 6: return norma_.norm(); 
    case 7: return(angle_);  
    }  
    return 0.0;
}

void ModelWeakplaneType::setProperty(UInt index, const Variant &p,UInt restoreVersion) {
    switch (index) {
    case 1: dip_ = p.toDouble();
        if (!restoreVersion) setDip(dip_);
        break;
    case 2: dd_ = p.toDouble();
        if (!restoreVersion) setDd(dd_);
        break;
    case 3: setNx(p.toDouble());
        break;
    case 4: setNy(p.toDouble());
        break;
    case 5: setNz(p.toDouble());
        break;
    case 6: setNorm(p.toDVect3().unit());
        break;
    case 7: angle_ = p.toDouble();
        if (!restoreVersion) setAngle(angle_);
        break;
    }
}

void ModelWeakplaneType::setDip(const Double& dip) {
    DVect3 norm = Orientation3::getNormFromDipDD(dip*degrad,dd_*degrad);
    norma_.set_norm(norm);
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;    
}

void ModelWeakplaneType::setDd(const Double& dd) {
    DVect3 norm = Orientation3::getNormFromDipDD(dip_*degrad,dd*degrad);
    norma_.set_norm(norm);
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
}

void ModelWeakplaneType::setNx (const Double& nx) {
    DVect3 norm = norma_.norm();
    norm.rx() = nx;
    norma_.set_norm(norm);
    DVect2 dv2 = Orientation3::getDipDDFromNorm(norm);
    dip_ = dv2.x() / degrad;
    dd_  = dv2.y() / degrad;
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
}

void ModelWeakplaneType::setNy(const Double& ny) {
    DVect3 norm = norma_.norm();
    norm.ry() = ny;
    norma_.set_norm(norm);
    DVect2 dv2 = Orientation3::getDipDDFromNorm(norm);
    dip_ = dv2.x() / degrad;
    dd_  = dv2.y() / degrad;
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
}

void ModelWeakplaneType::setNz(const Double& nz) {
    DVect3 norm = norma_.norm();
    norm.rz() = nz;
    norma_.set_norm(norm);
    DVect2 dv2 = Orientation3::getDipDDFromNorm(norm);
    dip_ = dv2.x() / degrad;
    dd_  = dv2.y() / degrad;
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
}

void ModelWeakplaneType::setNorm(const DVect3& norm) {
    DVect2 dv2 = Orientation3::getDipDDFromNorm(norm);
    norma_.set_norm(norm);
    dip_ = dv2.x() / degrad;
    dd_  = dv2.y() / degrad;
    angle_ = Orientation3::getJointAngleFromNorm(norm) / degrad;
}

void ModelWeakplaneType::setAngle(const Double &ang) {
    // Method #1
    if ( ang > 0.0) 
        dd_ = 180.0 - ang;
    else
        dd_ = - ang;

    dip_ = 90.0;
    DVect3 norm = Orientation3::getNormFromDipDD(dip_*degrad,dd_*degrad);

    // Method #2
    //Orientation3 orient;
    //orient.setJointAngle(ang*degrad);
    //dip_ = orient.getDip()/degrad;
    //dd_ = orient.getDipDirection()/degrad;
    //DVect3 norm = orient.getNormal();
    norma_.set_norm(norm);
}
}//namespace models
// EOF
