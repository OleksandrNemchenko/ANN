// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file threshold.h
 * \brief Threshold neuron.
 *
 */

#ifndef _AVN_ANN_NEURON_THRESHOLD_H_
#define _AVN_ANN_NEURON_THRESHOLD_H_

#include <math.h>
#include <avn/neuron/neuron_sum.h>

namespace ANeuron {

    struct AThresholdCalculator{
        using TValue = ANeuronBase::TValue;
        using TSerializer = ANeuronSumBase::TSerializer;

        static bool ToBeCalculated() noexcept               { return true; }
        static constexpr size_t SerializeSize() noexcept    { return 0; }
        void Serialize(TSerializer& buffer) const           { (void) buffer; }
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end)  { return buffer; }

        TValue TransferFunction(TValue sum) const noexcept
        {
            return sum > 0 ? 1 : 0;
        }
    };

    class ANeuronThreshold : public ANeuronSumBase  {
    public:
        using TValue = ANeuronSumBase::TValue;

        ANeuronThreshold(const std::string& name = "") : ANeuronSumBase(name)   {}

        size_t SerializeSize() const noexcept override              { return AThresholdCalculator::SerializeSize(); }
        void Serialize(TSerializer& buffer) const override          { return _data.Serialize(buffer); }
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) override  { return _data.Deserialize(buffer, end); }

        static const std::string& Type() noexcept                   { static const std::string type{"Threshold"}; return type; }
        static PNeuron CreateNeuron(const PData& neuronStructure);

        PData DescribeStructure() const noexcept override           { return std::make_unique<SData>(); }
        static PNeuron CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure) { auto neuron = std::make_unique<ANeuronThreshold>(name); neuron->SetInputs(inputs); return neuron; }

    private:
        AThresholdCalculator _data;
        TValue TransferFunction(TValue sum) const noexcept { return _data.TransferFunction(sum); }
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_THRESHOLD_H_
