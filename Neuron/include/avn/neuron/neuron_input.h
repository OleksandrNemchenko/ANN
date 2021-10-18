// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _AVN_ANN_NEURON_INPUT_H_
#define _AVN_ANN_NEURON_INPUT_H_

#include <string>
#include <vector>

#include <avn/neuron/neuron_base.h>

namespace ANeuron {

    class ANeuronInput : public ANeuronBase {
    public:
        using TValue = ANeuronBase::TValue;
        ANeuronInput(const std::string& name = "") noexcept : ANeuronBase(name) {}

        void SetInputValue(TValue value) const      { _inValue = value; }
        TValue Calculate() const noexcept override  { return _inValue; }

        virtual size_t SerializeSize() const noexcept override      { return 0;}
        virtual void Serialize(TSerializer& buffer) const override  { (void) buffer; }
        virtual TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) override   { (void) end; return buffer; }

        static const std::string& Type() noexcept                   { static const std::string type{"Input"}; return type; }

    private:
        mutable TValue _inValue;
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_INPUT_H_
