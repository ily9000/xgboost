/*!
 * Copyright 2015 by Contributors
 * \file global.cc
 * \brief Enable all kinds of global static registry and variables.
 */
#include <xgboost/objective.h>
#include <xgboost/metric.h>
#include <xgboost/tree_updater.h>

namespace dmlc {
DMLC_REGISTRY_ENABLE(::xgboost::ObjFunctionReg);
DMLC_REGISTRY_ENABLE(::xgboost::MetricReg);
DMLC_REGISTRY_ENABLE(::xgboost::TreeUpdaterReg);
}  // namespace dmlc

namespace xgboost {
// implement factory functions
ObjFunction* ObjFunction::Create(const char* name) {
  auto *e = ::dmlc::Registry< ::xgboost::ObjFunctionReg>::Get()->Find(name);
  if (e == nullptr) {
    LOG(FATAL) << "Unknown objective function " << name;
  }
  return (e->body)();
}

Metric* Metric::Create(const char* name) {
  std::string buf = name;
  std::string prefix = name;
  auto pos = buf.find('@');
  if (pos == std::string::npos) {
    auto *e = ::dmlc::Registry< ::xgboost::MetricReg>::Get()->Find(name);
    if (e == nullptr) {
      LOG(FATAL) << "Unknown objective function " << name;
    }
    return (e->body)(nullptr);
  } else {
    std::string prefix = buf.substr(0, pos);
    auto *e = ::dmlc::Registry< ::xgboost::MetricReg>::Get()->Find(prefix.c_str());
    if (e == nullptr) {
      LOG(FATAL) << "Unknown objective function " << name;
    }
    return (e->body)(buf.substr(pos + 1, buf.length()).c_str());
  }
}

// implement factory functions
TreeUpdater* TreeUpdater::Create(const char* name) {
  auto *e = ::dmlc::Registry< ::xgboost::TreeUpdaterReg>::Get()->Find(name);
  if (e == nullptr) {
    LOG(FATAL) << "Unknown tree updater " << name;
  }
  return (e->body)();
}

}  // namespace xgboost

