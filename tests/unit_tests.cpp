#define CATCH_CONFIG_MAIN

#include "catch/catch.hpp"

#include "../ass.hpp"

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