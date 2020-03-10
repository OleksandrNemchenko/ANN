// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <avn/neuron/gaussian.h>
#include <stdexcept>

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
