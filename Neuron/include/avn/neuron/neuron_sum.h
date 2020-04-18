// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file neuron_sum.h
 * \brief Base class for artificial neurons that sum their weights before calculation.
 *
 */

#ifndef _AVN_ANN_NEURON_SUM_H_
#define _AVN_ANN_NEURON_SUM_H_

#include <avn/neuron/neuron_base.h>

namespace ANeuron {

    class ANeuronSumBase : public ANeuronBase {
    public:
        using TValue = ANeuronBase::TValue;
        using TWeight = TValue;
        using TWeights = std::vector<TValue>;

        ANeuronSumBase(size_t inputs, const std::string& name = "") : ANeuronBase(inputs, name), _weights(inputs)
        {
            _w0 = 0;
            std::fill(_weights.begin(), _weights.end(), 0);
        }

        void SetW0(TWeight w0) noexcept                     { _w0 = w0; }
        void SetWeight(size_t id, TWeight weight) noexcept  { assert(id < _weights.size()); _weights[id] = weight; }

        TValue Calculate() const noexcept override;

        TValue W0() const noexcept                  { return _w0; }
        TValue Weight(size_t id) const noexcept     { assert(id < _weights.size()); return _weights[id]; }
        const TWeights& Weights() const noexcept    { return _weights; }

    protected:
        virtual TValue TransferFunction(TValue sum) const noexcept = 0;

    private:
        TWeight _w0;
        TWeights _weights;

    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_SUM_H_
