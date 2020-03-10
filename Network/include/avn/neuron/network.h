// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file network.h
 * \brief Neural network.
 *
 */

#ifndef _AVN_ANN_NEURON_NETWORK_H_
#define _AVN_ANN_NEURON_NETWORK_H_

#include <vector>
#include <memory>

#include <avn/neuron/neuron_base.h>
#include <avn/neuron/neuron_input.h>

namespace ANeuron {

    class ANetwork{
    public:
        using PNeuron = std::unique_ptr<ANeuronBase>;
        using TNeurons = std::vector<PNeuron>;
        using TInputNeurons = std::vector<ANeuronInput>;
        using TOutputNeurons = std::vector<ANeuronBase*>;
        using TValue = ANeuronBase::TValue;
        using TInputValues = std::vector<TValue>;
        using TSerializer = ANeuronBase::TSerializer;

        ANetwork(size_t inputs, size_t outputs = 1);
        bool AddNeuron(PNeuron&& neuron, size_t pos);
        bool AddNeuron(PNeuron&& neuron) noexcept;
        void SetNeurons(TNeurons&& neurons) noexcept                        { _neurons = std::move(neurons); }
        void SwapNeuron(size_t fromPos, size_t toPos) noexcept;

        void SetInput(size_t id, TValue value) noexcept                     { assert(id < _inputs.size()); _inputs.at(id).SetInputValue(value); }
        void SetInputNeuronName(size_t id, const std::string& name)         { assert(id < _inputs.size()); _inputs.at(id).SetName(name); }
        void SetOutputNeuron(size_t id, const std::string& name);

        void ConnectNeurons(const std::string& outNeuron, const std::string& inNeuron, size_t input);

        const TNeurons& Neurons() const noexcept                            { return _neurons; }
        const ANeuronBase& Neuron(size_t id) const                          { assert(id < _neurons.size()); return *_neurons.at(id).get(); }
        const ANeuronBase& Neuron(const std::string& name) const            { return NeuronImpl<const TNeurons>(_neurons, name); }

        ANeuronBase& Neuron(size_t id)                                      { assert(id < _neurons.size()); return *_neurons.at(id).get(); }
        ANeuronBase& Neuron(const std::string& name)                        { return NeuronImpl<TNeurons>(_neurons, name); }

        TValue Input(size_t id) const noexcept                              { assert(id < _neurons.size()); return _inputs.at(id).Value(); }
        TValue Output(size_t id) const noexcept                             { assert(id < _outputs.size()); return _outputs.at(id)->Value(); }

        void Calculate() const noexcept;
        void Calculate(const TInputValues& inputs) const noexcept;

        TSerializer Serialize() const;
        void Deserialize(const TSerializer& buf);

        TValue operator()(size_t id) const noexcept                         { return Output(id); }

    private:
        TNeurons _neurons;
        TInputNeurons _inputs;
        TOutputNeurons _outputs;

        TNeurons::const_iterator NeuronIt(const std::string& name) const    { return NeuronItImpl<TNeurons::const_iterator, const TNeurons>(_neurons, name); }
        TNeurons::iterator NeuronIt(const std::string& name)                { return NeuronItImpl<TNeurons::iterator, TNeurons>(_neurons, name); }

        template<typename TIt, typename TArray>
        static TIt NeuronItImpl(TArray& array, const std::string& name);

        template<typename TArray>
        static typename TArray::value_type::element_type& NeuronImpl(TArray& array, const std::string& name);

    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_NETWORK_H_
