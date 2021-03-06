# Another Signal Slot
> Signal/Slot implementation with a focus on automatic connection handling

[![Try online](https://img.shields.io/badge/try-online-blue.svg)](https://wandbox.org/permlink/2K1A2nrFx2i5I3II)
[![GitHub](https://img.shields.io/github/license/michaelcowan/ass)](https://github.com/michaelcowan/ass/blob/master/LICENSE.txt)
[![Github Releases](https://img.shields.io/github/release-pre/michaelcowan/ass)](https://github.com/michaelcowan/ass/releases)
[![Build Status](https://travis-ci.org/michaelcowan/ass.svg?branch=master)](https://travis-ci.org/michaelcowan/ass)
[![Coverage Status](https://coveralls.io/repos/github/michaelcowan/ass/badge.svg?branch=master)](https://coveralls.io/github/michaelcowan/ass?branch=master)

## Features
* Automatic connection handling
  * when a `Signal` or `Slot` goes out of scope, the connection is severed
  * no need to implement an interface or inherit a base class
* Type-safe
* Header only

## Limitations
* Not thread-safe
* Not reentrant-safe

## Usage Examples
### Observer Pattern Example
```cpp
Slot<std::string> observer([](std::string s) {
    std::cout << s << std::endl;
});

Signal<std::string> subject;

subject.connect(observer);

subject.emit("Hello World");
```

### Class Decoupling Example
```cpp
class Button {
public:

    Signal<> pressed;

private:
    void onMouseButtonDownInsideButtonArea() {
        pressed.emit();
    }
};

class Popup {
public:

    const Slot<> show = Slot<>(this, &Popup::render);

private:
    void render() { }
};


Button button;
Popup popup;

button.pressed.connect(popup.show);
```
