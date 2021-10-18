// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdexcept>

#include <avn/neuron/gaussian.h>
#include <avn/neuron/sigmoid.h>
#include <avn/neuron/threshold.h>
#include <avn/neuron/variable.h>

/* static */ void ANeuron::ANeuronBase::Serialize(TValue value, TSerializer& buffer)
{
    TSerializer realValue;
    std::byte *buf = reinterpret_cast<std::byte*>(&value);
    realValue.assign(buf, buf + sizeof(value));
    buffer.insert(buffer.end(), realValue.begin(), realValue.end());
}

/* static */ ANeuron::ANeuronBase::TSerializer::const_iterator ANeuron::ANeuronBase::Deserialize(TValue& value, TSerializer::const_iterator buffer, TSerializer::const_iterator& end)
{
    auto valueEnd = buffer + sizeof(TValue);

    if (valueEnd > end)
        throw std::runtime_error("Buffer is not enough to store all values");

    TValue *deserializedValue = reinterpret_cast<TValue*>(*buffer);
    value = *deserializedValue;

    return valueEnd;

}

ANeuron::ANeuronBase::PData ANeuron::ANeuronBase::DescribeStructureBase(const std::string& type, PData&& neuronStructure) const noexcept
{
    using namespace std::string_literals;

    neuronStructure->_name = _name;

    auto& fields = neuronStructure->_fields;

    fields["Type"] = type;

    fields["InputsAmount"] = std::to_string(_inputs.size());
    for (size_t input = 0; input < _inputs.size(); ++input) {
        std::string name = "Input"s + std::to_string(input + 1) + ".Name"s;
        fields[name] = _inputs[input]->_name;
    }

    return neuronStructure;
}

/* static */ ANeuron::ANeuronBase::PNeuron ANeuron::ANeuronBase::CreateNeuron(const PData& neuronStructure)
{
    const auto& fields = neuronStructure->_fields;
    const std::string& type = fields.at("Type");
    const std::string& name = neuronStructure->_name;
    const auto inputs = std::stoll(fields.at("Inputs"));

    if (type == ANeuronGaussian::Type())                    return ANeuronGaussian::CreateNeuron( inputs, name, neuronStructure);
    else if (type == ANeuron::ANeuronSigmoid::Type())       return ANeuronSigmoid::CreateNeuron(  inputs, name, neuronStructure);
    else if (type == ANeuron::ANeuronThreshold::Type())     return ANeuronThreshold::CreateNeuron(inputs, name, neuronStructure);
    else if (type == ANeuron::ANeuronVariable::TypeStr())   return ANeuronVariable::CreateNeuron( inputs, name, neuronStructure);
    else {
        assert(false);
        throw std::runtime_error("Unsupported neuron type");
    }
}
