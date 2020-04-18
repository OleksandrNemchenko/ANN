// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file gaussian.h
 * \brief Gaussian neuron.
 *
 */

#ifndef _AVN_ANN_NEURON_GAUSSIAN_H_
#define _AVN_ANN_NEURON_GAUSSIAN_H_

#include <math.h>
#include <avn/neuron/neuron_sum.h>

namespace ANeuron {

    struct AGaussianCalculator{
        using TValue = ANeuronSumBase::TValue;
        using TSerializer = ANeuronSumBase::TSerializer;

        TValue _mu;
        TValue _sigma;

        void Reset() noexcept;

        bool ToBeCalculated() const noexcept                { return _sigma > 0; }
        static constexpr size_t SerializeSize() noexcept    { return sizeof(_mu) + sizeof(_sigma); }
        void Serialize(TSerializer& buffer) const noexcept;
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end);
        TValue TransferFunction(TValue sum) const noexcept;

    };

    class ANeuronGaussian : public ANeuronSumBase {
    public:
        using TValue = ANeuronSumBase::TValue;

        ANeuronGaussian(size_t inputs, const std::string& name = "") : ANeuronSumBase(inputs, name) { _data.Reset(); }

        void SetMu(TValue mu) noexcept          { _data._mu = mu; }
        void SetSigma(TValue sigma) noexcept    { _data._sigma = sigma; }

        TValue Mu() const noexcept              { return _data._mu; }
        TValue Sigma() const noexcept           { return _data._sigma; }

        bool ToBeCalculated() const noexcept override               { return _data.ToBeCalculated(); }
        size_t SerializeSize() const noexcept override              { return AGaussianCalculator::SerializeSize(); }
        void Serialize(TSerializer& buffer) const override          { return _data.Serialize(buffer); }
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) override  { return _data.Deserialize(buffer, end); }

        static const std::string& Type() noexcept                   { static const std::string type{"Gaussian"}; return type; }

        PData DescribeStructure() const noexcept override;
        static PNeuron CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure);

    private:
        AGaussianCalculator _data;
        TValue TransferFunction(TValue sum) const noexcept { return _data.TransferFunction(sum); }
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_GAUSSIAN_H_
