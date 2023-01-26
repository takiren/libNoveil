#include "NGraph.h"

void noveil::NNodeExecutor::Execute() { parent.lock()->Execute(); }
