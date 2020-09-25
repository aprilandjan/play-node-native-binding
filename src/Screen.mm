#import <iostream>
#import <string>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <IOKit/graphics/IOGraphicsLib.h>
#include "./Screen.h"

void listScreens() {
  @autoreleasepool {
    NSArray *screenArray = [NSScreen screens];
    int count = (int)[screenArray count];
    for (int i=0; i < count; i++){
      NSScreen *screen = screenArray[i];

      NSDictionary *s = [screen deviceDescription];
      NSNumber *n = [s objectForKey:@"NSScreenNumber"];
      NSString *id = [n stringValue];

      CGDirectDisplayID displayID = [n intValue];
      NSString *screenName = @"default";
      NSDictionary *deviceInfo = (NSDictionary *)CFBridgingRelease(IODisplayCreateInfoDictionary(CGDisplayIOServicePort(displayID), kIODisplayOnlyPreferredName));
      NSDictionary *localizedNames = [deviceInfo objectForKey:[NSString stringWithUTF8String:kDisplayProductName]];

      if ([localizedNames count] > 0) {
        screenName = [localizedNames objectForKey:[[localizedNames allKeys] objectAtIndex:0]];
      }

      std::string deviceId = std::string([id UTF8String]);
      std::string deviceName = std::string([screenName UTF8String]);
      std::cout << "[" << (i + 1) << "/" << count << "] deviceId = " << deviceId << "; deviceName = " << deviceName << std::endl;
    }
  }
}