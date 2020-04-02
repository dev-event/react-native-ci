
#import <Foundation/Foundation.h>
#import "RCTCustomCxxModule.h"
#import "CustomCxxModule.h"

@implementation RCTCustomCxxModule

RCT_EXPORT_MODULE()

- (std::unique_ptr<facebook::xplat::module::CxxModule>)createModule
{
  return std::make_unique<CustomCxxModule>();
}

@end
