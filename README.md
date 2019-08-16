# Another Signal Slot
> Signal/Slot implementation with a focus on automatic connection handling

[![Github Releases](https://img.shields.io/github/release/michaelcowan/ass.svg)](https://github.com/michaelcowan/ass/releases)
[![Build Status](https://travis-ci.org/michaelcowan/ass.svg?branch=master)](https://travis-ci.org/michaelcowan/ass)
[![Coverage Status](https://coveralls.io/repos/github/michaelcowan/ass/badge.svg?branch=master)](https://coveralls.io/github/michaelcowan/ass?branch=master)

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

    const Slot<> show = Slot(this, &Popup::render);

private:
    void render() { }
};


Button button;
Popup popup;

button.pressed.connect(popup.show);
```
