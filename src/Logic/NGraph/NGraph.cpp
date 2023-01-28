#include "NGraph.h"

namespace noveil {

}  // namespace noveil

void noveil::NNodeExecutor::Execute() { parent.lock()->Execute(); }
