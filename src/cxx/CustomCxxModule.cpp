
#include "CustomCxxModule.h"
#include <cxxreact/JsArgumentHelpers.h>
#include "cxxreact/Instance.h"

#include <folly/Memory.h>


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

double Sample::add(double a, double b) {
  return a + b;
}


void Sample::save(std::map<std::string, std::string> dict) {
  state_ = std::move(dict);
}

std::map<std::string, std::string> Sample::load() {
  return state_;
}

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
        Method("getEvent", [this]() { if (auto reactInstance = getInstance().lock()) {
         reactInstance->callJSFunction( "RCTDeviceEventEmitter", "emit",
                 folly::dynamic::array("appStateDidChange",folly::dynamic::object("app_state", "active")));
           }
         }),

        Method("getString", [] (folly::dynamic args, Callback callbacks) {
          callbacks({"Callback string"});
        }),

        Method("add", [this](folly::dynamic args, Callback callbacks) {
           callbacks({sample_->add(facebook::xplat::jsArgAsDouble(args, 0), facebook::xplat::jsArgAsDouble(args, 1))});
         }),
        Method("hello", [this] { sample_->hello(); }),

        Method("twice", [this] (folly::dynamic args) -> folly::dynamic {
          return sample_->twice(facebook::xplat::jsArgAsDouble(args, 0));
        } ,SyncTag),

        Method("syncHello", [this]() -> folly::dynamic {
          sample_->hello();
          return nullptr;
        },
        SyncTag),

        Method("call_later", [this](folly::dynamic args, Callback callbacks) {
           sample_->call_later((int)facebook::xplat::jsArgAsDouble(args, 0), [callbacks] { callbacks({"Callback later"}); });
         }),

        Method("addIfPositiveAsPromise", [] (folly::dynamic args, Callback callbacks, Callback callbacksError) {
          auto a = facebook::xplat::jsArgAsDouble(args, 0);
          auto b = facebook::xplat::jsArgAsDouble(args, 1);
          if (a < 0 || b < 0) {
            callbacksError({"Negative number!"});
          } else {
            callbacks({a + b});
          }
        }),

        Method("addIfPositiveAsAsync", [] (folly::dynamic args, Callback callbacks, Callback callbacksError) {
              auto a = facebook::xplat::jsArgAsDouble(args, 0);
              auto b = facebook::xplat::jsArgAsDouble(args, 1);
              if (a < 0 || b < 0) {
                callbacksError({"Negative number!"});
              } else {
                callbacks({a + b});
              }
            },
            AsyncTag),

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
// void CustomCxxModule::load(__unused folly::dynamic args, Callback callbacks) {
//   folly::dynamic d = folly::dynamic::object;
//   for (const auto &p : sample_->load()) {
//     d.insert(p.first, p.second);
//   }
//   callbacks({d});
// }

extern "C" CustomCxxModule* createSampleCxxModule() {
  return new CustomCxxModule(folly::make_unique<Sample>());
}
