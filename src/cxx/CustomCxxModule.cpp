
#include "CustomCxxModule.h"
#include <cxxreact/JsArgumentHelpers.h>
#include "cxxreact/Instance.h"

#include <folly/Memory.h>
#include <glog/logging.h>


#include <thread>


CustomCxxModule::CustomCxxModule(std::unique_ptr<Sample> sample): sample_(std::move(sample)) {}

std::string CustomCxxModule::getName() {
  return "CustomCxxModule";
}

double Sample::twice(double n) {
  return n * 2;
}

auto CustomCxxModule::getConstants() -> std::map<std::string, folly::dynamic> {
  return {
      {"constId", 12345}, {"constText", "sample string"},
  };
}

auto CustomCxxModule::getMethods() -> std::vector<Method> {
  return {
        Method("getEvent",
             [this]() {
               if (auto reactInstance = getInstance().lock()) {
                 reactInstance->callJSFunction(
                     "RCTDeviceEventEmitter", "emit",
                     folly::dynamic::array(
                         "appStateDidChange",
                         folly::dynamic::object("app_state", "active")));
               }
             }),
        Method("getString", []     (folly::dynamic args, Callback cb){cb({"Callback string"});}),
        Method("twice",     [this] (folly::dynamic args) -> folly::dynamic {
                            return sample_->twice(facebook::xplat::jsArgAsDouble(args, 0));
                            },SyncTag),
      };
}

extern "C" CustomCxxModule* createSampleCxxModule() {
  return new CustomCxxModule(folly::make_unique<Sample>());
}
