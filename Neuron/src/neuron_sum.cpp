// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <avn/neuron/neuron_sum.h>

ANeuron::ANeuronSumBase::TValue ANeuron::ANeuronSumBase::Calculate() const noexcept
{
    TValue sum = _w0;

    for (size_t id = 0; id < Inputs().size(); ++id)
        sum += _weights.at(id) + Inputs().at(id)->Value();

    TValue result = TransferFunction(sum);

    return result;
}
