// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdexcept>

#include <avn/neuron/sigmoid.h>

ANeuron::ASigmoidCalculator::TSerializer::const_iterator ANeuron::ASigmoidCalculator::Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end)
{
    TSerializer::const_iterator it = ANeuronBase::Deserialize(_beta, buffer, end);
    return it;
}

ANeuron::ANeuronSigmoid::PData ANeuron::ANeuronSigmoid::DescribeStructure() const noexcept
{
    PData data = std::make_unique<SData>();
    auto& fields = data->_fields;

    fields["Beta"] = std::to_string(_data._beta);

    return data;
}

/* static */ ANeuron::ANeuronSigmoid::PNeuron ANeuron::ANeuronSigmoid::CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure)
{
    auto neuron = std::make_unique<ANeuronSigmoid>(inputs, name);
    const auto& fields = neuronStructure->_fields;

    neuron->_data._beta =    std::stold(fields.at("Beta"));

    return neuron;
}
