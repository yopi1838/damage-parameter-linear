#include "modelnull.h"
#include "base/src/version.h"

namespace models {
    ModelNull::ModelNull() {}
    UInt ModelNull::getMinorVersion() const { return version::update; }
    String ModelNull::getProperties(void) const { return L""; }
    Variant ModelNull::getProperty(UInt) const { return 0; }
    void ModelNull::setProperty(UInt, const Variant &,UInt)  { return; }
    void ModelNull::initialize(UByte ,State *) {return;}
    void ModelNull::run(UByte, State *) {return;} 
    String ModelNull::getStates(void) const { return L""; }
    void ModelNull::copy(const ConstitutiveModel *m) { return ConstitutiveModel::copy(m); }
}
// EOF