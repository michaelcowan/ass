#define CATCH_CONFIG_MAIN

#include "catch/catch.hpp"

#include "../ass.hpp"

using namespace ass;

class CountingCallable {
public:

    CountingCallable() : m_count(0), func([&]() { ++m_count; }) {};

    void operator()() {
        func();
    }

    int count() const {
        return m_count;
    }

private:

    std::function<void()> func;
    int m_count;
};

TEST_CASE("a new Signal should have no connections") {
    Signal<> signal;

    REQUIRE(signal.connectionCount() == 0);
}

TEST_CASE("a new Slot should have no connections") {
    Slot<> slot([]() {});

    REQUIRE(slot.connectionCount() == 0);
}

TEST_CASE("Signal and Slot can be connected") {
    Signal<> signal;
    Slot<> slot([]() {});

    signal.connect(slot);

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("Signal should not be connected to another Slot") {
        Slot<> anotherSlot([]() {});

        REQUIRE_FALSE(signal.isConnectedTo(anotherSlot));
    }

    SECTION("Slot should not be connected to another Signal") {
        Signal<> anotherSignal;

        REQUIRE_FALSE(slot.isConnectedTo(anotherSignal));
    }

    SECTION("Signal and Slot should only be able to connect once") {
        signal.connect(slot);

        SECTION("Signal should have a single connection") {
            REQUIRE(signal.connectionCount() == 1);
        }

        SECTION("Slot should have a single connection") {
            REQUIRE(slot.connectionCount() == 1);
        }
    }
}

TEST_CASE("Signal can be connected to multiple Slots") {
    Signal<> signal;
    Slot<> slot1([]() {});
    Slot<> slot2([]() {});
    Slot<> slot3([]() {});

    signal.connect(slot1);
    signal.connect(slot2);
    signal.connect(slot3);

    SECTION("Signal should have three connections") {
        REQUIRE(signal.connectionCount() == 3);
    }

    SECTION("each Slot should have a single connection") {
        REQUIRE(slot1.connectionCount() == 1);
        REQUIRE(slot2.connectionCount() == 1);
        REQUIRE(slot3.connectionCount() == 1);
    }

    SECTION("Signal should be connected to each Slot") {
        REQUIRE(signal.isConnectedTo(slot1));
        REQUIRE(signal.isConnectedTo(slot2));
        REQUIRE(signal.isConnectedTo(slot3));
    }

    SECTION("each Slot should be connected to Signal") {
        REQUIRE(slot1.isConnectedTo(signal));
        REQUIRE(slot2.isConnectedTo(signal));
        REQUIRE(slot3.isConnectedTo(signal));
    }

    SECTION("Signal should not be connected to another Slot") {
        Slot<> anotherSlot([]() {});

        REQUIRE_FALSE(signal.isConnectedTo(anotherSlot));
    }

    SECTION("each Slot should not be connected to another Signal") {
        Signal<> anotherSignal;

        REQUIRE_FALSE(slot1.isConnectedTo(anotherSignal));
        REQUIRE_FALSE(slot2.isConnectedTo(anotherSignal));
        REQUIRE_FALSE(slot3.isConnectedTo(anotherSignal));
    }
}

TEST_CASE("Slot can be connected to multiple Signals") {
    Signal<> signal1;
    Signal<> signal2;
    Signal<> signal3;
    Slot<> slot([]() {});

    signal1.connect(slot);
    signal2.connect(slot);
    signal3.connect(slot);

    SECTION("each Signal should have a single connection") {
        REQUIRE(signal1.connectionCount() == 1);
        REQUIRE(signal2.connectionCount() == 1);
        REQUIRE(signal3.connectionCount() == 1);
    }

    SECTION("Slot should have a three connections") {
        REQUIRE(slot.connectionCount() == 3);
    }

    SECTION("each Signal should be connected to Slot") {
        REQUIRE(signal1.isConnectedTo(slot));
        REQUIRE(signal2.isConnectedTo(slot));
        REQUIRE(signal3.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to each Signal") {
        REQUIRE(slot.isConnectedTo(signal1));
        REQUIRE(slot.isConnectedTo(signal2));
        REQUIRE(slot.isConnectedTo(signal3));
    }

    SECTION("each Signal should not be connected to another Slot") {
        Slot<> anotherSlot([]() {});

        REQUIRE_FALSE(signal1.isConnectedTo(anotherSlot));
        REQUIRE_FALSE(signal2.isConnectedTo(anotherSlot));
        REQUIRE_FALSE(signal3.isConnectedTo(anotherSlot));
    }

    SECTION("Slot should not be connected to another Signal") {
        Signal<> anotherSignal;

        REQUIRE_FALSE(slot.isConnectedTo(anotherSignal));
    }
}

TEST_CASE("Signal and Slot can be disconnected") {
    Signal<> signal;
    Slot<> slot([]() {});

    signal.connect(slot);
    signal.disconnect(slot);

    SECTION("Signal should have zero connections") {
        REQUIRE(signal.connectionCount() == 0);
    }

    SECTION("Slot should have zero connections") {
        REQUIRE(slot.connectionCount() == 0);
    }

    SECTION("Signal should not be connected to Slot") {
        REQUIRE_FALSE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should not be connected to Signal") {
        REQUIRE_FALSE(slot.isConnectedTo(signal));
    }
}

TEST_CASE("Signal can be disconnected from a single Slot") {
    Signal<> signal;
    Slot<> connectedSlot1([]() {});
    Slot<> disconnectedSlot([]() {});
    Slot<> connectedSlot2([]() {});

    signal.connect(connectedSlot1);
    signal.connect(disconnectedSlot);
    signal.connect(connectedSlot2);

    signal.disconnect(disconnectedSlot);

    SECTION("Signal should have two connections") {
        REQUIRE(signal.connectionCount() == 2);
    }

    SECTION("each Slot not disconnected should have a single connection") {
        REQUIRE(connectedSlot1.connectionCount() == 1);
        REQUIRE(connectedSlot2.connectionCount() == 1);
    }

    SECTION("the disconnected Slot should have zero connections") {
        REQUIRE(disconnectedSlot.connectionCount() == 0);
    }

    SECTION("Signal should be connected to each Slot not disconnected") {
        REQUIRE(signal.isConnectedTo(connectedSlot1));
        REQUIRE(signal.isConnectedTo(connectedSlot2));
    }

    SECTION("Signal should not be connected to the disconnected Slot") {
        REQUIRE_FALSE(signal.isConnectedTo(disconnectedSlot));
    }

    SECTION("each not disconnected Slot should be connected to Signal") {
        REQUIRE(connectedSlot1.isConnectedTo(signal));
        REQUIRE(connectedSlot2.isConnectedTo(signal));
    }

    SECTION("the disconnected Slot should not be connected to Signal") {
        REQUIRE_FALSE(disconnectedSlot.isConnectedTo(signal));
    }
}

TEST_CASE("Slot can be disconnected from a single Signal") {
    Signal<> connectedSignal1;
    Signal<> disconnectedSignal;
    Signal<> connectedSignal2;
    Slot<> slot([]() {});

    connectedSignal1.connect(slot);
    disconnectedSignal.connect(slot);
    connectedSignal2.connect(slot);

    disconnectedSignal.disconnect(slot);

    SECTION("each Signal not disconnected should have a single connection") {
        REQUIRE(connectedSignal1.connectionCount() == 1);
        REQUIRE(connectedSignal2.connectionCount() == 1);
    }

    SECTION("the disconnected Signal should have zero connections") {
        REQUIRE(disconnectedSignal.connectionCount() == 0);
    }

    SECTION("Slot should have a two connections") {
        REQUIRE(slot.connectionCount() == 2);
    }

    SECTION("each Signal not disconnected should be connected to Slot") {
        REQUIRE(connectedSignal1.isConnectedTo(slot));
        REQUIRE(connectedSignal2.isConnectedTo(slot));
    }

    SECTION("the disconnected Signal should not be connected to Slot") {
        REQUIRE_FALSE(disconnectedSignal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to each Signal not disconnected") {
        REQUIRE(slot.isConnectedTo(connectedSignal1));
        REQUIRE(slot.isConnectedTo(connectedSignal2));
    }

    SECTION("Slot should not be connected to the disconnected Signal") {
        REQUIRE_FALSE(slot.isConnectedTo(disconnectedSignal));
    }
}

TEST_CASE("Signal can disconnect from all Slots") {
    Signal<> signal;
    Slot<> slot1([]() {});
    Slot<> slot2([]() {});
    Slot<> slot3([]() {});

    signal.connect(slot1);
    signal.connect(slot2);
    signal.connect(slot3);

    signal.disconnectAll();

    SECTION("Signal should have zero connections") {
        REQUIRE(signal.connectionCount() == 0);
    }

    SECTION("each Slot should have zero connections") {
        REQUIRE(slot1.connectionCount() == 0);
        REQUIRE(slot2.connectionCount() == 0);
        REQUIRE(slot3.connectionCount() == 0);
    }

    SECTION("Signal should not be connected to any of the Slots") {
        REQUIRE_FALSE(signal.isConnectedTo(slot1));
        REQUIRE_FALSE(signal.isConnectedTo(slot2));
        REQUIRE_FALSE(signal.isConnectedTo(slot3));
    }

    SECTION("each Slot should not be connected to Signal") {
        REQUIRE_FALSE(slot1.isConnectedTo(signal));
        REQUIRE_FALSE(slot2.isConnectedTo(signal));
        REQUIRE_FALSE(slot3.isConnectedTo(signal));
    }
}

TEST_CASE("Signal should disconnect when destructed") {
    Slot<> slot([]() {});
    {
        Signal<> signal;
        signal.connect(slot);
    }

    REQUIRE(slot.connectionCount() == 0);
}

TEST_CASE("Slot should disconnect when destructed") {
    Signal<> signal;
    {
        Slot<> slot([]() {});
        signal.connect(slot);
    }

    REQUIRE(signal.connectionCount() == 0);
}

TEST_CASE("Signal can be copy constructed") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Signal<> copy(signal);

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("copied Signal should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have two connections") {
        REQUIRE(slot.connectionCount() == 2);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("copied Signal should be connected to Slot") {
        REQUIRE(copy.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to copied Signal") {
        REQUIRE(slot.isConnectedTo(copy));
    }
}

TEST_CASE("Slot can be copy constructed") {
    CountingCallable callable;
    Signal<> signal;
    Slot<> slot(callable);
    signal.connect(slot);

    Slot<> copy(slot);

    SECTION("Signal should have two connections") {
        REQUIRE(signal.connectionCount() == 2);
    }

    SECTION("copied Slot should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to copied Slot") {
        REQUIRE(signal.isConnectedTo(copy));
    }

    SECTION("copied Slot should be connected to Signal") {
        REQUIRE(copy.isConnectedTo(signal));
    }

    SECTION("Signal should call Slot and copied Slot") {
        signal.emit();
        REQUIRE(callable.count() == 2);
    }
}

TEST_CASE("Signal can be copy assigned") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Signal<> copy;
    copy = signal;

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("copied Signal should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have two connections") {
        REQUIRE(slot.connectionCount() == 2);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("copied Signal should be connected to Slot") {
        REQUIRE(copy.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to copied Signal") {
        REQUIRE(slot.isConnectedTo(copy));
    }
}

TEST_CASE("Signal can be copy assigned after being previously connected") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Signal<> copy;
    Slot<> previousSlot([]() {});
    copy.connect(previousSlot);

    copy = signal;

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("copied Signal should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have two connections") {
        REQUIRE(slot.connectionCount() == 2);
    }

    SECTION("previous Slot should have zero connections") {
        REQUIRE(previousSlot.connectionCount() == 0);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("copied Signal should be connected to Slot") {
        REQUIRE(copy.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to copied Signal") {
        REQUIRE(slot.isConnectedTo(copy));
    }

    SECTION("previous Slot should not be connected to copied Signal") {
        REQUIRE_FALSE(previousSlot.isConnectedTo(copy));
    }

    SECTION("copied Signal should not be connected to the previous Slot") {
        REQUIRE_FALSE(copy.isConnectedTo(previousSlot));
    }
}

TEST_CASE("Slot can be copy assigned") {
    CountingCallable callable;
    Signal<> signal;
    Slot<> slot(callable);
    signal.connect(slot);

    Slot<> copy;
    copy = slot;

    SECTION("Signal should have two connections") {
        REQUIRE(signal.connectionCount() == 2);
    }

    SECTION("copied Slot should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to copied Slot") {
        REQUIRE(signal.isConnectedTo(copy));
    }

    SECTION("copied Slot should be connected to Signal") {
        REQUIRE(copy.isConnectedTo(signal));
    }

    SECTION("Signal should call Slot and copied Slot") {
        signal.emit();
        REQUIRE(callable.count() == 2);
    }
}

TEST_CASE("Slot can be copy assigned after being previously connected") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Slot<> copy([]() {});
    Signal<> previousSignal;
    previousSignal.connect(copy);

    copy = slot;

    SECTION("Signal should have two connections") {
        REQUIRE(signal.connectionCount() == 2);
    }

    SECTION("copied Slot should have a single connection") {
        REQUIRE(copy.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("previous Signal should have zero connections") {
        REQUIRE(previousSignal.connectionCount() == 0);
    }

    SECTION("Signal should be connected to Slot") {
        REQUIRE(signal.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to Signal") {
        REQUIRE(slot.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to copied Slot") {
        REQUIRE(signal.isConnectedTo(copy));
    }

    SECTION("copied Slot should be connected to Signal") {
        REQUIRE(copy.isConnectedTo(signal));
    }

    SECTION("previous Signal should not be connected to copied Slot") {
        REQUIRE_FALSE(previousSignal.isConnectedTo(copy));
    }

    SECTION("copied Slot should not be connected to the previous Signal") {
        REQUIRE_FALSE(copy.isConnectedTo(previousSignal));
    }
}

TEST_CASE("Signal can be move constructed") {
    Slot<> slot([]() {});
    Signal<> signal;
    signal.connect(slot);

    Signal<> moved(std::move(signal));

    SECTION("moved Signal should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("moved Signal should be connected to Slot") {
        REQUIRE(moved.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to moved Signal") {
        REQUIRE(slot.isConnectedTo(moved));
    }
}

TEST_CASE("Slot can be move constructed") {
    CountingCallable callable;
    Slot<> slot(callable);
    Signal<> signal;
    signal.connect(slot);

    Slot<> moved(std::move(slot));

    SECTION("moved Slot should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("moved Slot should be connected to Signal") {
        REQUIRE(moved.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to moved Slot") {
        REQUIRE(moved.isConnectedTo(signal));
    }

    SECTION("Signal should call only moved Slot") {
        signal.emit();
        REQUIRE(callable.count() == 1);
    }
}

TEST_CASE("Signal can be move assigned") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Signal<> moved;

    moved = std::move(signal);

    SECTION("moved Signal should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("moved Signal should be connected to Slot") {
        REQUIRE(moved.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to moved Signal") {
        REQUIRE(slot.isConnectedTo(moved));
    }
}

TEST_CASE("Signal can be move assigned after being previously connected") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Signal<> moved;
    Slot<> previousSlot([]() {});
    moved.connect(previousSlot);

    moved = std::move(signal);

    SECTION("moved Signal should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Slot should have a single connection") {
        REQUIRE(slot.connectionCount() == 1);
    }

    SECTION("previous Slot should have zero connections") {
        REQUIRE(previousSlot.connectionCount() == 0);
    }

    SECTION("moved Signal should be connected to Slot") {
        REQUIRE(moved.isConnectedTo(slot));
    }

    SECTION("Slot should be connected to moved Signal") {
        REQUIRE(slot.isConnectedTo(moved));
    }

    SECTION("previous Slot should not be connected to moved Signal") {
        REQUIRE_FALSE(previousSlot.isConnectedTo(moved));
    }

    SECTION("moved Signal should not be connected to the previous Slot") {
        REQUIRE_FALSE(moved.isConnectedTo(previousSlot));
    }
}

TEST_CASE("Slot can be move assigned") {
    CountingCallable callable;
    Signal<> signal;
    Slot<> slot(callable);
    signal.connect(slot);

    Slot<> moved;
    moved = std::move(slot);

    SECTION("moved Slot should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("moved Slot should be connected to Signal") {
        REQUIRE(moved.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to moved Slot") {
        REQUIRE(signal.isConnectedTo(moved));
    }

    SECTION("Signal should call only moved Slot") {
        signal.emit();
        REQUIRE(callable.count() == 1);
    }
}

TEST_CASE("Slot can be move assigned after being previously connected") {
    Signal<> signal;
    Slot<> slot([]() {});
    signal.connect(slot);

    Slot<> moved([]() {});
    Signal<> previousSignal;
    previousSignal.connect(moved);

    moved = std::move(slot);

    SECTION("moved Slot should have a single connection") {
        REQUIRE(moved.connectionCount() == 1);
    }

    SECTION("Signal should have a single connection") {
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("previous Signal should have zero connections") {
        REQUIRE(previousSignal.connectionCount() == 0);
    }

    SECTION("moved Slot should be connected to Signal") {
        REQUIRE(moved.isConnectedTo(signal));
    }

    SECTION("Signal should be connected to moved Slot") {
        REQUIRE(signal.isConnectedTo(moved));
    }

    SECTION("previous Signal should not be connected to moved Slot") {
        REQUIRE_FALSE(previousSignal.isConnectedTo(moved));
    }

    SECTION("moved Slot should not be connected to the previous Signal") {
        REQUIRE_FALSE(moved.isConnectedTo(previousSignal));
    }
}

TEST_CASE("Signal should call Slot function for each emit") {
    int timesToEmit = GENERATE(1, 2, 3, 4, 5);

    int called = 0;
    Signal<> signal;
    Slot<> slot([&]() { ++called; });

    signal.connect(slot);

    for (int i = 0; i < timesToEmit; i++) {
        signal.emit();
    }

    REQUIRE(called == timesToEmit);
}

TEST_CASE("Signal should forward argument to Slot") {
    std::string string;

    Slot<std::string> slot([&](std::string s) {
        string = std::move(s);
    });

    Signal<std::string> signal;
    signal.connect(slot);
    signal.emit("hello");

    REQUIRE(string == "hello");
}

TEST_CASE("Signal should forward multiple arguments to Slot") {
    std::string string;
    int integer;

    Slot<std::string, int> slot([&](std::string s, int i) {
        string = std::move(s);
        integer = i;
    });

    Signal<std::string, int> signal;
    signal.connect(slot);
    signal.emit("hello", 5);

    REQUIRE(string == "hello");
    REQUIRE(integer == 5);
}

TEST_CASE("Slot can be const") {
    CountingCallable callable;
    Signal<> signal;
    const Slot<> slot(callable);

    SECTION("Slot should be able to connect to Signal") {
        signal.connect(slot);
        REQUIRE(slot.isConnectedTo(signal));
        REQUIRE(slot.connectionCount() == 1);
        REQUIRE(signal.isConnectedTo(slot));
        REQUIRE(signal.connectionCount() == 1);
    }

    SECTION("Slot should be able to connect to Signal and be called") {
        signal.connect(slot);
        signal.emit();

        REQUIRE(callable.count() == 1);
    }

    SECTION("Slot should be able to disconnect from Signal") {
        signal.connect(slot);
        signal.disconnect(slot);
        REQUIRE_FALSE(slot.isConnectedTo(signal));
        REQUIRE(slot.connectionCount() == 0);
        REQUIRE_FALSE(signal.isConnectedTo(slot));
        REQUIRE(signal.connectionCount() == 0);
    }
}

TEST_CASE("Slot can callback on an instance function") {
    class Triggerable {
    public:
        int count = 0;

        void trigger(int n) {
            count += n;
        }
    } triggerable;

    Slot<int> slot(&triggerable, &Triggerable::trigger);

    Signal<int> signal;
    signal.connect(slot);

    signal.emit(10);

    REQUIRE(triggerable.count == 10);
}

TEST_CASE("Slot can callback on a class function") {
    static int count = 0;

    class Triggerable {
    public:
        static void trigger(int n) {
            count += n;
        }
    };

    Slot<int> slot(&Triggerable::trigger);

    Signal<int> signal;
    signal.connect(slot);

    signal.emit(5);

    REQUIRE(count == 5);
}
