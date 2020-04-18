// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*! \file network.h
 * \brief Neural network.
 *
 */

#ifndef _AVN_ANN_NEURON_GENETIC_ALGORITHM_H_
#define _AVN_ANN_NEURON_GENETIC_ALGORITHM_H_

#include <vector>
#include <memory>

#include <avn/neuron/neuron_base.h>

namespace ANeuron {

    class AGeneticAlgorithm {
    public:
        using TSerializer = ANeuronBase::TSerializer;
        using TFitnessValue = long double;
        using TRatio = float;

        /**< Function to test genom's calculation ability
         *
         * \param[in] Genom to be used
         *
         * \return true if such genom can be used for calculation
         */
        using FTestCorrectness = std::function<bool (const TSerializer& genom)>;

        /**< Function to calculate genom's fitness function
         *
         * \param[in] Genom to be used
         *
         * \return Fitness value.
         */
        using FFitnessFunction = std::function<TFitnessValue (const TSerializer& genom)>;

        /**< Function to calculate random values
         *
         * \param[in] bytes Random bytes amount to generate
         *
         * \return Vector with random values.
         */
        using FRNG = std::function<TSerializer (size_t bytes)>;

        /**< Function to calculate random value in the range [ \a minValue, \a maxValue )
         *
         * \param[in] minValue The lowest value of the range
         * \param[in] maxValue Next after the highest value of the range
         *
         * \return Value inside range specified
         */
        using FRNGinRange = std::function<size_t (size_t minValue, size_t maxValue)>;

        struct SGenom {
            TSerializer _genom;             /**< Genom to be used */
            TFitnessValue _fitnessValue;    /**< Fitness value for current genom */
        };
        using TGenoms = std::vector<SGenom>;

        struct SConfiguration {
            size_t _genomSize;                  /**< Genom (vector) size */
            size_t _phenotypesAmount = 10;      /**< Phenotypes amount for each step */
            FTestCorrectness _testCorrectness;  /**< Callback to test genom correctness */
            FFitnessFunction _fitnessFunction;  /**< Calculate fitness function for the genom specified */
            FRNG _randomVectorGenerator;        /**< Calculate random vector */
            FRNGinRange _randomValueGenerator;  /**< Calculate random value */
            TRatio _goodChildrenRatio = 0.8;    /**< Amount of good children to be used for the next step */
            TRatio _mutationProbability = 1e-4; /**< Mutation probability for each bit */
        };
        AGeneticAlgorithm(SConfiguration config);

        const TGenoms& Calculate() noexcept;
        const TGenoms& Genoms() const noexcept  { return _genoms; }

    private:
        SConfiguration _config;
        TGenoms _genoms;

        void Initialization() noexcept;
        TGenoms Crossover() noexcept;
        void Mutation(TGenoms& genoms) noexcept;
        void GenomsSelection(TGenoms& genoms) noexcept;

        static void Sort(TGenoms& genoms) noexcept;
        static size_t Factorial(size_t num) noexcept;
    };

} // namespace ANeuron

#endif  // _AVN_ANN_NEURON_GENETIC_ALGORITHM_H_
