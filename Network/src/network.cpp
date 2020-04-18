// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <algorithm>
#include <avn/neuron/network.h>

bool ANeuron::ANetwork::AddNeuron(PNeuron&& neuron) noexcept
{
    return AddNeuron(std::move(neuron), _neurons.size());
}

void ANeuron::ANetwork::SwapNeuron(size_t fromPos, size_t toPos) noexcept
{
    assert(fromPos < _neurons.size());
    assert(toPos < _neurons.size());

    std::swap(_neurons.at(fromPos), _neurons.at(toPos));
}

bool ANeuron::ANetwork::AddNeuron(PNeuron&& neuron, size_t pos)
{
    if (pos > _neurons.size()) {
        assert(false);
        throw std::out_of_range("Position is bigger than array size");
    }

    if (NeuronIt(neuron->Name()) != _neurons.cend())
        return false;

    _neurons.emplace(_neurons.begin() + pos, std::move(neuron));

    return true;
}


template<typename TArray>
static typename TArray::value_type::element_type& NeuronImpl(TArray& array, const std::string& name)
{
    auto it = NeuronItImpl(array, name);

    if (it == array.cend()) {
        assert(false);
        throw std::out_of_range("There is no neuron with such name");
    }

    return *it->get();
}

template<typename TIt, typename TArray>
TIt ANeuron::ANetwork::NeuronItImpl(TArray& array, const std::string& name)
{
    auto it = std::find_if(array.begin(), array.end(), [name](const ANeuronBase& savedNeuron)
    {
        return savedNeuron.Name() == name;
    } );

    return it;
}

void ANeuron::ANetwork::ConnectNeurons(const std::string& outNeuron, const std::string& inNeuron, size_t input)
{
    const ANeuronBase& from = Neuron(outNeuron);
    ANeuronBase& to = Neuron(inNeuron);

    to.SetInput(input, &from);
}

void ANeuron::ANetwork::SetOutputNeuron(size_t id, const std::string& name)
{
    assert(id < _outputs.size());

    auto neuron = NeuronIt(name);
    assert(neuron != _inputs.end());

    _outputs[id] = neuron->get();
}

void ANeuron::ANetwork::Calculate() const noexcept
{
    for (size_t id = 0; id < _neurons.size(); ++id)
        _neurons.at(id)->Calculate();
}

void ANeuron::ANetwork::Calculate(const ANeuron::ANetwork::TInputValues& inputs) const noexcept
{
    assert(inputs.size() == _inputs.size());

    for (size_t id = 0; id < inputs.size(); ++id)
        _inputs.at(id).SetInputValue(inputs.at(id));

    Calculate();
}

ANeuron::ANetwork::TSerializer ANeuron::ANetwork::Serialize() const
{
    size_t size = 0;

    for (const auto &neuron : _neurons) {
        size += neuron->SerializeSize();
    }

    TSerializer buf;
    buf.reserve(size);

    for (const auto &neuron : _neurons) {
        neuron->Serialize(buf);
    }

    return buf;
}

void ANeuron::ANetwork::Deserialize(const ANeuron::ANetwork::TSerializer& buf)
{
    size_t size = 0;

    for (const auto &neuron : _neurons) {
        size += neuron->SerializeSize();
    }

    if (size != buf.size()) {
        assert(false);
        throw std::out_of_range("Buffer size is not convenient for current network");
    }

    auto it = buf.cbegin();

    for (const auto &neuron : _neurons) {
        it = neuron->Deserialize(it, buf.cend());
    }
}

bool ANeuron::ANetwork::ToBeCalculated() const noexcept
{
    for (const auto &neuron : _neurons) {
        if(!neuron->ToBeCalculated())
            return false;
    }

    return true;
}

size_t ANeuron::ANetwork::SerializeSize() const noexcept
{
    size_t size = 0;

    for (const auto &neuron : _neurons) {
        size += neuron->SerializeSize();
    }

    return size;
}
