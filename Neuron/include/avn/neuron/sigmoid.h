// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file sigmoid.h
 * \brief Sigmoid neuron.
 *
 */

#ifndef _AVN_ANN_NEURON_SIGMOID_H_
#define _AVN_ANN_NEURON_SIGMOID_H_

#include <math.h>
#include <avn/neuron/neuron_sum.h>

namespace ANeuron {

    struct ASigmoidCalculator{
        using TValue = ANeuronBase::TValue;
        using TSerializer = ANeuronSumBase::TSerializer;

        TValue _beta;

        void Reset() noexcept                               { _beta = 0; }

        static bool ToBeCalculated() noexcept               { return true; }
        static constexpr size_t SerializeSize() noexcept    { return sizeof(_beta); }
        void Serialize(TSerializer& buffer) const           { ANeuronBase::Serialize(_beta, buffer); }

        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end);

        TValue TransferFunction(TValue sum) const noexcept
        {
            TValue res = static_cast<TValue>(1) / (1 + exp(-_beta * sum));
            return res;
        }
    };

    class ANeuronSigmoid : public ANeuronSumBase {
    public:
        using TValue = ANeuronSumBase::TValue;

        ANeuronSigmoid(size_t inputs, const std::string& name = "") : ANeuronSumBase(inputs, name)   { _data.Reset(); }

        void SetBeta(TValue beta) noexcept  { _data._beta = beta; }

        TValue Beta() const noexcept        { return _data._beta; }

        size_t SerializeSize() const noexcept override              { return ASigmoidCalculator::SerializeSize(); }
        void Serialize(TSerializer& buffer) const override          { return _data.Serialize(buffer); }
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) override  { return _data.Deserialize(buffer, end); }

        static const std::string& Type() noexcept                   { static const std::string type{"Sigmoid"}; return type; }

        PData DescribeStructure() const noexcept override;
        static PNeuron CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure);

    private:
        ASigmoidCalculator _data;
        TValue TransferFunction(TValue sum) const noexcept { return _data.TransferFunction(sum); }
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_SIGMOID_H_
