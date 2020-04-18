// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <algorithm>
#include <execution>
#include <cstddef>
#include <avn/neuron/genetic-algorithm.h>

ANeuron::AGeneticAlgorithm::AGeneticAlgorithm(SConfiguration config): _config(config)
{
}

void ANeuron::AGeneticAlgorithm::Sort(TGenoms& genoms) noexcept
{
    std::sort(std::execution::par_unseq, genoms.begin(), genoms.end(), [](const SGenom& first, const SGenom& second)
    {
        return first._fitnessValue < second._fitnessValue;
    } );
}

void ANeuron::AGeneticAlgorithm::Initialization() noexcept
{
    _genoms.clear();
    _genoms.reserve(_config._phenotypesAmount);

    for (size_t i = 0; i < _config._phenotypesAmount; ++i) {
        SGenom genom;

        do {
            genom._genom = _config._randomVectorGenerator(_config._genomSize);
        }
        while (_config._testCorrectness(genom._genom));

        genom._fitnessValue = _config._fitnessFunction(genom._genom);

        _genoms.emplace_back(std::move(genom));
    }

    Sort(_genoms);
}

/* static */ size_t ANeuron::AGeneticAlgorithm::Factorial(size_t num) noexcept
{
    if (num <= 1)       return 1;
    else if (num == 2)  return 2;
    else                return num * Factorial(num - 1);
}

ANeuron::AGeneticAlgorithm::TGenoms ANeuron::AGeneticAlgorithm::Crossover() noexcept
{
    TGenoms result;
    constexpr size_t PAIR = 2;
    result.reserve( Factorial(_config._phenotypesAmount) / Factorial(_config._phenotypesAmount - PAIR) );

    std::vector<std::byte> selectors = _config._randomVectorGenerator(_config._genomSize);

    for (size_t firstId = 0; firstId < _config._phenotypesAmount - 1; ++firstId) {
        for (size_t secondId = firstId + 1; secondId < _config._phenotypesAmount; ++secondId) {

            TSerializer child;
            child.reserve(_config._genomSize);

            while(true) {
                child.clear();

                for (size_t byte = 0; byte < _config._genomSize; ++byte) {
                    std::byte parentA = _genoms.at(firstId)._genom.at(byte);
                    std::byte parentB = _genoms.at(secondId)._genom.at(byte);
                    std::byte selector = selectors.at(byte);

                    std::byte newByte = (~parentA & parentB & selector) | (parentA & ~parentB & ~selector) | (parentA & parentB);
                    child.emplace_back(newByte);
                }

                if (!_config._testCorrectness(child)) {
                    selectors = std::move(_config._randomVectorGenerator(_config._genomSize));
                    continue;
                }

                SGenom newGenom;

                newGenom._genom = std::move(child);
                newGenom._fitnessValue = _config._fitnessFunction(newGenom._genom);

                result.emplace_back(std::move(newGenom));
                break;
            }
        }
    }

    return result;
}

void ANeuron::AGeneticAlgorithm::Mutation(TGenoms& genoms) noexcept
{
    if (_config._randomValueGenerator(0, 1/_config._mutationProbability) > _config._genomSize)
        return;

    while(true) {
        size_t genomToBeMutated = _config._randomValueGenerator(0, _config._genomSize);
        auto& genom  = genoms[genomToBeMutated];

        constexpr auto BITS_IN_BYTE = 8;
        size_t bitToBeMutated = _config._randomValueGenerator(0, _config._genomSize*BITS_IN_BYTE);
        auto &byte = genom._genom[bitToBeMutated / 8];

        size_t bit = _config._randomValueGenerator(0, BITS_IN_BYTE);
        byte ^= static_cast<std::byte>(1) << bit;

        if (!_config._testCorrectness(genom._genom))
            continue;

        genom._fitnessValue = _config._fitnessFunction(genom._genom);

        break;
    }

}

void ANeuron::AGeneticAlgorithm::GenomsSelection(TGenoms& genoms) noexcept
{
    Sort(genoms);

    const size_t firstGood = genoms.size() * (1 - _config._goodChildrenRatio);

    _genoms.clear();

    size_t first = 0;
    size_t second = 0;
    for (size_t id = 0; id < firstGood; ++id, ++first, ++second)
        std::swap(_genoms[first], genoms[second]);

    first = firstGood;
    second = genoms.size() - (_config._phenotypesAmount - firstGood);
    for (size_t id = firstGood; id < _config._phenotypesAmount; ++id, ++first, ++second)
        std::swap(_genoms[first], genoms[second]);
}

const ANeuron::AGeneticAlgorithm::TGenoms& ANeuron::AGeneticAlgorithm::Calculate() noexcept
{
    if (_genoms.empty())
        Initialization();

    TGenoms results = Crossover();

    Mutation(results);

    GenomsSelection(results);

    return _genoms;
}
