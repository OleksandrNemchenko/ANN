// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <numbers>
#include <stdexcept>

#include <avn/neuron/gaussian.h>

void ANeuron::AGaussianCalculator::Reset() noexcept
{
    _mu = 0;
    _sigma = 0;
}

void ANeuron::AGaussianCalculator::Serialize(TSerializer& buffer) const noexcept
{
    ANeuronBase::Serialize(_mu, buffer);
    ANeuronBase::Serialize(_sigma, buffer);
}
ANeuron::AGaussianCalculator::TSerializer::const_iterator ANeuron::AGaussianCalculator::Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end)
{
    TSerializer::const_iterator it = ANeuronBase::Deserialize(_mu, buffer, end);
    it = ANeuronBase::Deserialize(_sigma, it, end);
    return it;
}

ANeuron::AGaussianCalculator::TValue ANeuron::AGaussianCalculator::TransferFunction(TValue sum) const noexcept
{
    TValue res;

    if (_sigma <= 0)
        res = std::numeric_limits<TValue>::infinity();
    else
        res = exp(-pow(sum - _mu, 2) / (2 * _sigma * _sigma)) / sqrt(2 * std::numbers::pi * _sigma);

    return res;
}

ANeuron::ANeuronGaussian::PData ANeuron::ANeuronGaussian::DescribeStructure() const noexcept
{
    PData data = std::make_unique<SData>();
    auto& fields = data->_fields;

    fields["Mu"] = std::to_string(_data._mu);
    fields["Sigma"] = std::to_string(_data._sigma);

    return data;
}

/* static */ ANeuron::ANeuronGaussian::PNeuron ANeuron::ANeuronGaussian::CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure)
{
    auto neuron = std::make_unique<ANeuronGaussian>(name);
    neuron->SetInputs(inputs);

    const auto& fields = neuronStructure->_fields;

    neuron->_data._mu =    std::stold(fields.at("Mu"));
    neuron->_data._sigma = std::stold(fields.at("Sigma"));

    return neuron;
}
