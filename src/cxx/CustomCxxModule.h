#ifndef RNPACKAGE_SAMPLE_CXX_MODULE_H_
#define RNPACKAGE_SAMPLE_CXX_MODULE_H_

#include "cxxreact/CxxModule.h"
class Sample {
public:
  double twice(double n);
};

class CustomCxxModule : public facebook::xplat::module::CxxModule {
 public:
  CustomCxxModule(std::unique_ptr<Sample> sample);

  std::string getName() override;
  auto getConstants() -> std::map<std::string, folly::dynamic> override;
  auto getMethods() -> std::vector<Method> override;
private:
  std::unique_ptr<Sample> sample_;
};

#endif
