// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file sigmoid.h
 * \brief Sigmoid neuron.
 *
 */

#ifndef _AVN_ANN_NEURON_VARIABLE_H_
#define _AVN_ANN_NEURON_VARIABLE_H_

#include <math.h>
#include <avn/neuron/neuron_sum.h>
#include <avn/neuron/gaussian.h>
#include <avn/neuron/sigmoid.h>
#include <avn/neuron/threshold.h>

namespace ANeuron {

    class ANeuronVariable : public ANeuronSumBase {
    public:
        using TValue = ANeuronSumBase::TValue;
        using TType = unsigned short;

        enum class EType : unsigned short {
            GAUSSIAN = 0,
            SIGMOID,
            THRESHOLD,
            UNKNOWN = 255
        };

        ANeuronVariable(const std::string& name = "") : ANeuronSumBase(name)
        {
            std::byte *buf = reinterpret_cast<std::byte*>(&_data);
            std::fill(buf, buf + sizeof(_data), static_cast<std::byte>(0));
        }

        void SetType(TType type) noexcept
        {
            _type = (type < static_cast<TType>(EType::UNKNOWN)) ? static_cast<EType>(type) : EType::UNKNOWN;
        }

        EType Type() const noexcept                     { return _type; }

        AGaussianCalculator Gaussian() const noexcept   { return _data._gaussian; }
        ASigmoidCalculator Sigmoid() const noexcept     { return _data._sigmoid; }
        AThresholdCalculator Threshold() const noexcept { return _data._threshold; }

        bool ToBeCalculated() const noexcept override;
        size_t SerializeSize() const noexcept override;
        void Serialize(TSerializer& buffer) const override;
        TSerializer::const_iterator Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end) override;

        static const std::string& TypeStr() noexcept    { static const std::string type{"Variable"}; return type; }

        PData DescribeStructure() const noexcept override;
        static PNeuron CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure);

    private:
        EType _type;

        union UData {
            AGaussianCalculator _gaussian;
            ASigmoidCalculator _sigmoid;
            AThresholdCalculator _threshold;
        } _data;

        TValue TransferFunction(TValue sum) const noexcept
        {
            switch (_type) {
                case EType::GAUSSIAN :  return _data._gaussian.TransferFunction(sum);
                case EType::SIGMOID :   return _data._sigmoid.TransferFunction(sum);
                case EType::THRESHOLD : return _data._threshold.TransferFunction(sum);
                case EType::UNKNOWN :   return 0;
                default:                assert(false); return 0;
            }
        }
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_VARIABLE_H_
