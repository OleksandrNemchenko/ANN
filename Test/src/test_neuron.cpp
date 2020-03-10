// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstddef>
#include <cstring>
#include <iostream>

#include <avn/neuron/neuron_base.h>

using namespace ANeuron;

namespace {
    size_t test_neuron_base()
    {
        return 0;
    }

}   // namespace

size_t test_neuron()
{
    size_t errors = 0;

    std::cout << "Neuron tests" << std::endl;

    errors += test_neuron_base();

    return errors;
}