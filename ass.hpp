/**
 * This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

template<typename... Args>
class Signal;

template<typename... Args>
class Slot final {

    friend class Signal<Args...>;

public:

    explicit Slot(std::function<void(Args...)> callback)
            : callback(std::move(callback)) {}

    ~Slot() {
        disconnectFromAll();
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

    void connectTo(Signal<Args...> &signal) {
        signals.push_back(&signal);
    }

    void disconnectFrom(Signal<Args...> &signal) {
        signals.erase(std::remove(signals.begin(), signals.end(), &signal), signals.end());
    }

    void disconnectFromAll() {
        for (auto *signal : signals) {
            signal->disconnectFrom(*this);
        }
        signals.clear();
    }

private:

    std::function<void(Args...)> callback;

    std::vector<Signal<Args...> *> signals;

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
        for (auto *slot : other.slots) {
            this->connect(*slot);
        }
    }

    /**
     * Connects this Signal to the provided Slot.
     *
     * @param slot Slot to connect this Signal to.
     */
    void connect(Slot<Args...> &slot) {
        slots.push_back(&slot);
        slot.connectTo(*this);
    }

    /**
     * Disconnects this Signal from the provided Slot if connected.
     *
     * @param slot Slot to disconnect this Signal from.
     */
    void disconnect(Slot<Args...> &slot) {
        disconnectFrom(slot);
        slot.disconnectFrom(*this);
    }

    /**
     * Disconnects this Signal from all connected Slot.
     */
    void disconnectAll() {
        for (auto *slot : slots) {
            slot->disconnectFrom(*this);
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

    void disconnectFrom(Slot<Args...> &slot) {
        slots.erase(std::remove(slots.begin(), slots.end(), &slot), slots.end());
    }

private:

    std::vector<Slot<Args...> *> slots;

};