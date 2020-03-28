
#include "CustomCxxModule.h"
#include <cxxreact/JsArgumentHelpers.h>
#include "cxxreact/Instance.h"

#include <folly/Memory.h>
//#include <glog/logging.h>


#include <thread>


CustomCxxModule::CustomCxxModule(std::unique_ptr<Sample> sample): sample_(std::move(sample)) {}

std::string Sample::hello() {
  return "hello";
}

std::string CustomCxxModule::getName() {
  return "CustomCxxModule";
}

double Sample::twice(double n) {
  return n * 2;
}

// std::string Sample::concat(const std::string &a, const std::string &b) {
//   return a + b;
// }

// std::string Sample::repeat(int count, const std::string &str) {
//   std::string ret;
//   for (int i = 0; i < count; i++) {
//     ret += str;
//   }
//
//   return ret;
// }

// void Sample::save(std::map<std::string, std::string> dict) {
//   state_ = std::move(dict);
// }
//
// std::map<std::string, std::string> Sample::load() {
//   return state_;
// }

void Sample::call_later(int msec, std::function<void()> f) {
  std::thread t([=] {
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    f();
  });
  t.detach();
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
        Method("hello", [this] { sample_->hello(); }),
        Method(
        "syncHello",
        [this]() -> folly::dynamic {
          sample_->hello();
          return nullptr;
        },
        SyncTag),
        Method("call_later", [this](folly::dynamic args, Callback cb) {
           sample_->call_later((int)facebook::xplat::jsArgAsDouble(args, 0), [cb] { cb({"Callback later"}); });
         }),
         Method(
        "addIfPositiveAsPromise",
        [](folly::dynamic args, Callback cb, Callback cbError) {
          auto a = facebook::xplat::jsArgAsDouble(args, 0);
          auto b = facebook::xplat::jsArgAsDouble(args, 1);
          if (a < 0 || b < 0) {
            cbError({"Negative number!"});
          } else {
            cb({a + b});
          }
        }),
    Method(
        "addIfPositiveAsAsync",
        [](folly::dynamic args, Callback cb, Callback cbError) {
          auto a = facebook::xplat::jsArgAsDouble(args, 0);
          auto b = facebook::xplat::jsArgAsDouble(args, 1);
          if (a < 0 || b < 0) {
            cbError({"Negative number!"});
          } else {
            cb({a + b});
          }
        },
        AsyncTag),
         // Method(
         //  "concat",
         //  [this](folly::dynamic args, Callback cb) {
         //    cb({sample_->concat(
         //        facebook::xplat::jsArgAsString(args, 0), facebook::xplat::jsArgAsString(args, 1))});
         //  }),
         // Method(
         //  "repeat",
         //  [this](folly::dynamic args, Callback cb) {
         //    cb({sample_->repeat(
         //        (int)facebook::xplat::jsArgAsInt(args, 0), facebook::xplat::jsArgAsString(args, 1))});
         //  }),
         // Method("save", this, &CustomCxxModule::save),
         // Method("load", this, &CustomCxxModule::load),
      };
}

// void CustomCxxModule::save(folly::dynamic args) {
//   std::map<std::string, std::string> m;
//   for (const auto &p : facebook::xplat::jsArgN(args, 0, &folly::dynamic::items)) {
//     m.emplace(
//         facebook::xplat::jsArg(p.first, &folly::dynamic::asString, "map key"),
//         facebook::xplat::jsArg(p.second, &folly::dynamic::asString, "map value"));
//   }
//   sample_->save(std::move(m));
// }
//
// void CustomCxxModule::load(__unused folly::dynamic args, Callback cb) {
//   folly::dynamic d = folly::dynamic::object;
//   for (const auto &p : sample_->load()) {
//     d.insert(p.first, p.second);
//   }
//   cb({d});
// }

extern "C" CustomCxxModule* createSampleCxxModule() {
  return new CustomCxxModule(folly::make_unique<Sample>());
}
