# React native C++ Native Modules (Android version)

[![N|Solid](https://hackernoon.com/hn-images/1*q16vcoe7CRolZLOCoG5_Pg.png)](https://nodesource.com/products/nsolid)

[![Android 10](https://github.com/dev-event/react-native-ci/blob/master/screenshot/src1.jpg)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://github.com/dev-event/react-native-ci)

There is a more straitforward way to connect C++ in React Native.
The CxxModule is it, and there is a sample as well.
Unfortunately, setup CxxModule on Android right now is getting a little difficult.
Modules allow you to do:
  - Encryption
  - Image Processing
  - and other...

And you are willing to do a little extra work you can use [Djinni](https://github.com/sulewicz/djinni-react-native).
Djinni is a codegen for different languages, but there still some overhead.

Especially on Android, a JS call might be in such flow:
JS -> RNBridge in JS VM -> JNI -> Android VM -> Java NativeModule -> JNI -> C++
The back and forth between JNI is redundant and wasted in command serialization/deserialzation.

This project generates interface bindings for Objective-C and Java. Once you create the wrapper code for C++, you can create Java and Objective-C classes that integrate with React Native. Thus React will call Obj-C/Java which will call C++. Profit.


### Installation

React Native version  > 0.60

```sh
$ yarn install
```
### Documentation 

Use data from a table to convert values.

| Plugin | README |
| ------ | ------ |
| jsArgAsBool |(args, 0) |
| jsArgAsInt | (args, 1) |
| jsArgAsDouble | (args, 2) |
| jsArgAsString | (args, 3) |
| jsArgAsArray | (args, 4) |
|jsArgAsObject | (args, 5) |



#### Telegram : dev_event


License
----

MIT



