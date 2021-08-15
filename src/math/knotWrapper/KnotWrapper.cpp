#include "KnotWrapper.h"

namespace KE { namespace ThreeD {

KnotWrapper::KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(diagram, width, height) {
}
KnotWrapper::KnotWrapper(const rapidjson::Document &doc) : knot(doc) {
}

}}
