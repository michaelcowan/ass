/**
 * MIT License
 *
 * Copyright (c) 2019 Michael Cowan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace ass {

    template<typename... Args>
    class Signal;

    template<typename... Args>
    class Slot {

        friend class Signal<Args...>;

    public:

        Slot() = default;

        explicit Slot(std::function<void(Args...)> callback)
                : callback(std::move(callback)) {}

        template<typename T>
        Slot(T *instance, void (T::*function)(Args...))
                : Slot([=](Args... args) { (instance->*function)(args...); }) {}

        ~Slot() {
            disconnectAll();
        }

        /**
         * Copies all connections of other Slot to this Slot.
         * @param other Slot to copy connections from.
         */
        Slot(const Slot &other) {
            copyConnectionsFrom(other);
            this->callback = other.callback;
        }

        /**
         * Replaces connections of this Slot with connections of other Slot.
         * @param other Slot to copy connections from.
         * @return Copy assigned instance.
         */
        Slot &operator=(const Slot &other) {
            disconnectAll();
            copyConnectionsFrom(other);
            this->callback = other.callback;
            return *this;
        };

        /**
         * Copies all connections of other Slot to this Slot then disconnects other Slot.
         * @param other Slot to copy connections from.
         */
        Slot(Slot &&other) noexcept {
            copyConnectionsFrom(other);
            other.disconnectAll();
            std::swap(this->callback, other.callback);
        }

        /**
         * Replaces connections of this Slot with connections of other Slot then disconnects other
         * Slot.
         * @param other Slot to copy connections from.
         * @return Move assigned instance.
         */
        Slot &operator=(Slot &&other) noexcept {
            disconnectAll();
            copyConnectionsFrom(other);
            other.disconnectAll();
            std::swap(this->callback, other.callback);
            return *this;
        }

        /**
         * Returns the number of connections for this Slot.
         *
         * @return Number of connections for this Slot.
         */
        int connectionCount() const {
            return signals.size();
        }

        /**
         * Returns true if this Slot is connected to the provided Signal.
         *
         * @param signal Signal to test connection against.
         * @return true if connected.
         */
        bool isConnectedTo(const Signal<Args...> &signal) const {
            return std::find(signals.begin(), signals.end(), &signal) != signals.end();
        }

    private:

        void addSignal(Signal<Args...> &signal) const {
            signals.push_back(&signal);
        }

        void removeSignal(Signal<Args...> &signal) const {
            signals.erase(std::remove(signals.begin(), signals.end(), &signal), signals.end());
        }

        void disconnectAll() {
            for (auto *signal : signals) {
                signal->removeSlot(*this);
            }
            signals.clear();
        }

        void copyConnectionsFrom(const Slot<Args...> &other) {
            for (auto *signal : other.signals) {
                signal->connect(*this);
            }
        }

    protected:

        std::function<void(Args...)> callback;

    private:

        mutable std::vector<Signal<Args...> *> signals;

    };

    template<typename... Args>
    class Signal final {

        friend class Slot<Args...>;

    public:

        Signal() = default;

        ~Signal() {
            disconnectAll();
        }

        /**
         * Copies all connections of other Signal to this Signal.
         * @param other Signal to copy connections from.
         */
        Signal(const Signal &other) {
            copyConnectionsFrom(other);
        }

        /**
         * Replaces connections of this Signal with connections of other Signal.
         * @param other Signal to copy connections from.
         * @return Copy assigned instance.
         */
        Signal &operator=(const Signal &other) {
            disconnectAll();
            copyConnectionsFrom(other);
            return *this;
        };

        /**
         * Copies all connections of other Signal to this Signal then disconnects other Signal.
         * @param other Signal to copy connections from.
         */
        Signal(Signal &&other) noexcept {
            copyConnectionsFrom(other);
            other.disconnectAll();
        }

        /**
         * Replaces connections of this Signal with connections of other Signal then disconnects other
         * Signal.
         * @param other Signal to copy connections from.
         * @return Move assigned instance.
         */
        Signal &operator=(Signal &&other) noexcept {
            disconnectAll();
            copyConnectionsFrom(other);
            other.disconnectAll();
            return *this;
        }

        /**
         * Calls function(s) of the connected Slot(s).
         * @param args Arguments to pass to the Slot functions.
         */
        void emit(Args... args) {
            for (auto *slot : slots) {
                slot->callback(std::forward<Args>(args)...);
            }
        }

        /**
         * Connects this Signal to the provided Slot unless already connected.
         *
         * @param slot Slot to connect this Signal to.
         */
        void connect(const Slot<Args...> &slot) {
            if (!isConnectedTo(slot)) {
                slots.push_back(&slot);
                slot.addSignal(*this);
            }
        }

        /**
         * Disconnects this Signal from the provided Slot if connected.
         *
         * @param slot Slot to disconnect this Signal from.
         */
        void disconnect(const Slot<Args...> &slot) {
            this->removeSlot(slot);
            slot.removeSignal(*this);
        }

        /**
         * Disconnects this Signal from all connected Slot.
         */
        void disconnectAll() {
            for (auto *slot : slots) {
                slot->removeSignal(*this);
            }
            slots.clear();
        }

        /**
         * Returns the number of connections for this Signal.
         *
         * @return Number of connections for this Signal.
         */
        int connectionCount() const {
            return slots.size();
        }

        /**
         * Returns true if this Signal is connected to the provided Slot.
         *
         * @param slot Slot to test connection against.
         * @return true if connected.
         */
        bool isConnectedTo(const Slot<Args...> &slot) const {
            return std::find(slots.begin(), slots.end(), &slot) != slots.end();
        }

    private:

        void removeSlot(const Slot<Args...> &slot) {
            slots.erase(std::remove(slots.begin(), slots.end(), &slot), slots.end());
        }

        void copyConnectionsFrom(const Signal<Args...> &other) {
            for (auto *slot : other.slots) {
                this->connect(*slot);
            }
        }

    private:

        std::vector<const Slot<Args...> *> slots;

    };

    template<typename T, typename... Args>
    class MemberSlot : public Slot<Args...> {
    public:

        MemberSlot(T *instance, void (T::*function)(Args...))
                : Slot<Args...>(buildCallback()), instance(instance), function(function) {}

        MemberSlot(const MemberSlot &other) : Slot<Args...>(other) {
            this->callback = std::move(buildCallback(&other));
        }

        MemberSlot &operator=(const MemberSlot &other) {
            Slot<Args...>::operator=(other);
            this->callback = std::move(buildCallback(&other));
            return *this;
        }

        MemberSlot(MemberSlot &&other) noexcept : Slot<Args...>(std::move(other)) {
            this->callback = std::move(buildCallback(&other));
        }

        MemberSlot &operator=(MemberSlot &&other) noexcept {
            Slot<Args...>::operator=(std::move(other));
            this->callback = std::move(buildCallback(&other));
            return *this;
        }

    private:

        std::function<void(Args...)> buildCallback(const MemberSlot *other = nullptr) {
            if (other) { calculateCallbackPointersFrom(other); }
            return [=](Args... args) { (instance->*function)(args...); };
        }

        void calculateCallbackPointersFrom(const MemberSlot *other) {
            auto offsetInBytes = bytePtr(this) - bytePtr(other);
            instance = reinterpret_cast<T *>(bytePtr(other->instance) + offsetInBytes);
            function = other->function;
        }

        template<typename X>
        unsigned char *bytePtr(X *p) noexcept {
            return reinterpret_cast<unsigned char *>(p);
        }

        template<typename X>
        const unsigned char *bytePtr(const X *p) noexcept {
            return reinterpret_cast<const unsigned char *>(p);
        }

    private:
        T *instance = nullptr;

        void (T::*function)(Args...) = nullptr;
    };

}