#ifndef RNPACKAGE_SAMPLE_CXX_MODULE_H_
#define RNPACKAGE_SAMPLE_CXX_MODULE_H_

#include "cxxreact/CxxModule.h"
class Sample {
public:
  std::string hello();
  double twice(double n);
  void call_later(int msec, std::function<void()> f);
  // void save(std::map<std::string, std::string> dict);
  // std::map<std::string, std::string> load();
  // std::string concat(const std::string &a, const std::string &b);
  // std::string repeat(int count, const std::string &str);

private:
 std::map<std::string, std::string> state_;
};

class CustomCxxModule : public facebook::xplat::module::CxxModule {
 public:
  CustomCxxModule(std::unique_ptr<Sample> sample);

  std::string getName() override;
  auto getConstants() -> std::map<std::string, folly::dynamic> override;
  auto getMethods() -> std::vector<Method> override;
private:
  // void save(folly::dynamic args);
  // void load(folly::dynamic args, Callback cb);
  std::unique_ptr<Sample> sample_;
};

#endif
