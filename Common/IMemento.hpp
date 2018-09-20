#pragma once

namespace Common {

    class IMemento
    {
    public: // Data
        enum Type {Json, };

    public: // Methods
        virtual ~IMemento() = default;

        virtual Type type() const = 0;
    };

} // namespace Common
