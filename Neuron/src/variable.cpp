// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <avn/neuron/variable.h>
#include <stdexcept>

size_t ANeuron::ANeuronVariable::SerializeSize() const noexcept
{
    return std::max(AGaussianCalculator::SerializeSize(),
            std::max(ASigmoidCalculator::SerializeSize(), AThresholdCalculator::SerializeSize()));
}

void ANeuron::ANeuronVariable::Serialize(TSerializer& buffer) const
{
    buffer.push_back(static_cast<std::byte>(_type));
    size_t bytes = 0;

    switch (_type) {
        case EType::GAUSSIAN :
            bytes = AGaussianCalculator::SerializeSize();
            _data._gaussian.SerializeSize();
            break;

        case EType::SIGMOID :
            bytes = ASigmoidCalculator::SerializeSize();
            _data._sigmoid.Serialize(buffer);
            break;

        case EType::THRESHOLD :
            bytes = AThresholdCalculator::SerializeSize();
            _data._threshold.Serialize(buffer);
            break;

        case EType::UNKNOWN :
            break;

        default:
            assert(false);
            break;
    }

    TSerializer filler(SerializeSize() - bytes);
    buffer.insert(buffer.end(), filler.begin(), filler.end());

}

ANeuron::ANeuronBase::TSerializer::const_iterator ANeuron::ANeuronVariable::Deserialize(TSerializer::const_iterator buffer, TSerializer::const_iterator end)
{
    _type = static_cast<EType>(*buffer++);
    size_t bytes = 0;

    switch (_type) {
        case EType::GAUSSIAN :
            bytes = AGaussianCalculator::SerializeSize();
            buffer = _data._gaussian.Deserialize(buffer, end);
            break;

        case EType::SIGMOID :
            bytes = ASigmoidCalculator::SerializeSize();
            buffer = _data._sigmoid.Deserialize(buffer, end);
            break;

        case EType::THRESHOLD :
            bytes = AThresholdCalculator::SerializeSize();
            buffer = _data._threshold.Deserialize(buffer, end);
            break;

        default:
            _type = EType::UNKNOWN;
            break;
    }

    buffer += SerializeSize() - bytes;

    return buffer;
}

bool ANeuron::ANeuronVariable::ToBeCalculated() const noexcept
{
    switch (_type) {
        case EType::GAUSSIAN :  return _data._gaussian.ToBeCalculated();
        case EType::SIGMOID :   return _data._sigmoid.ToBeCalculated();
        case EType::THRESHOLD : return _data._threshold.ToBeCalculated();
        case EType::UNKNOWN :   return false;
        default:
            assert(false);
            return false;
    }

}


ANeuron::ANeuronVariable::PData ANeuron::ANeuronVariable::DescribeStructure() const noexcept
{
    PData data = std::make_unique<SData>();
    auto& fields = data->_fields;

    switch (_type)
    {
    case EType::GAUSSIAN:
        fields["Type"] = "Gaussian";
        fields["Mu"] = std::to_string(_data._gaussian._mu);
        fields["Sigma"] = std::to_string(_data._gaussian._sigma);
        break;
    case EType::SIGMOID:
        fields["Type"] = "Sigmoid";
        fields["Beta"] = std::to_string(_data._sigmoid._beta);
        break;
    case EType::THRESHOLD:
        fields["Type"] = "Threshold";
        break;
    default:
        assert(false);
    }

    return data;
}

/* static */ ANeuron::ANeuronVariable::PNeuron ANeuron::ANeuronVariable::CreateNeuron(size_t inputs, const std::string& name, const PData& neuronStructure)
{
    auto neuron = std::make_unique<ANeuronVariable>(name);
    neuron->SetInputs(inputs);

    const auto& fields = neuronStructure->_fields;

    const std::string& type = fields.at("Type");

    if (type == "Gaussian")
    {
        neuron->_type = EType::GAUSSIAN;
        neuron->_data._gaussian._mu = std::stold(fields.at("Mu"));
        neuron->_data._gaussian._sigma = std::stold(fields.at("Sigma"));
    }
    else if (type == "Sigmoid")
    {
        neuron->_type = EType::SIGMOID;
        neuron->_data._sigmoid._beta = std::stold(fields.at("Beta"));
    }
    else if (type == "Threshold")
    {
        neuron->_type = EType::THRESHOLD;
    }
    else
    {
        assert(false);
    }

    return neuron;
}
