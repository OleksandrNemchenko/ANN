// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file neuron_base.h
 * \brief Base class for artificial neurons.
 *
 */

#ifndef _AVN_ANN_NEURON_BASE_H_
#define _AVN_ANN_NEURON_BASE_H_

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ANeuron {

    struct SData {
        std::string _name;

        std::unordered_map<std::string, std::string> _fields;
        std::unique_ptr<SData> _subStructure;
    };

    class ANeuronBase {
    public:
        using TInputs = std::vector<const ANeuronBase*>;
        using TSerializer = std::vector<std::byte>;
        using TValue = long double;
        using PData = std::unique_ptr<SData>;
        using PNeuron = std::unique_ptr<ANeuronBase>;

        ANeuronBase(const std::string& name = "") noexcept : _name(name)
        {
            _value = 0;
        }

        virtual ~ANeuronBase() noexcept = default;

        virtual void SetInputs(size_t inputs) noexcept
        {
//            _inputs.resize(inputs);
//            std::fill(_inputs.begin(), _inputs.end(), nullptr);
        }

        void SetName(const std::string& name) noexcept  { _name = name; }
        void SetInput(size_t id, const ANeuronBase* input) noexcept
        {
            assert(id < _inputs.size());
            assert(input);
            _inputs[id] = input;
        }

        virtual TValue Calculate() const noexcept = 0;
        virtual bool ToBeCalculated() const noexcept    { return true; }

        const TInputs& Inputs() const noexcept          { return _inputs; }
        TValue Value() const noexcept                   { return _value; }
        const std::string& Name() const noexcept        { return _name; }

        virtual size_t SerializeSize() const noexcept = 0;
        virtual void Serialize(TSerializer& buffer) const = 0;
        virtual TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) = 0;

        static void Serialize(TValue value, TSerializer& buffer);
        static TSerializer::const_iterator Deserialize(TValue& value, TSerializer::const_iterator buffer, TSerializer::const_iterator& end);

        virtual PData DescribeStructure() const noexcept = 0;
        static PNeuron CreateNeuron(const PData& neuronStructure);

    protected:
        void SetValue(TValue value) const noexcept  { _value = value; }
        PData DescribeStructureBase(const std::string& type, PData&& neuronStructure) const noexcept;

    private:
        std::string _name;
        TInputs _inputs;
        mutable TValue _value;
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_BASE_H_
