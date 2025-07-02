/*
 * Main program implementing a hybrid optimization approach combining
 * genetic algorithms and hill climbing for the coin selection problem
 */

// Header files inclusion for standard I/O, string manipulation, memory allocation, and custom modules
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "algoritmo.h"
#include "funcao.h"
#include "hillclimbing.h"

int main()
{
    // Program initialization
    // - Variable declarations
    // - File input handling
    // - Parameter setting

    // Variable declarations for file handling and problem parameters
    char fname[40];                  // File name buffer
    dados grafico;                   // Structure to hold problem data
    int runs = 25, i;               // Number of algorithm runs and loop counter
    float mbf = 0.0;                // Mean best fitness across all runs
    struct info EA_param;            // Structure for evolutionary algorithm parameters
    pchrom pop = NULL, parents = NULL; // Population and parents arrays
    chrom best_run, best_ever;      // Best solutions for current run and overall
    int gen_actual, r, inv;         // Generation counter, run counter, and invalid solutions counter

    // Get input file name from user
    printf("\nNome Ficheiro: ");
    scanf("%s", fname);

    // Initialize random number generator and read problem data
    init_rand();
    init_dados(fname, &grafico);

    // Set evolutionary algorithm parameters
    EA_param.popsize = 50;         // Population size
    EA_param.pm = 0.5;              // Mutation probability
    EA_param.pr = 0.5;              // Recombination probability
    EA_param.tsize = 2;             // Tournament size
    EA_param.numGenerations = 5500; // Maximum number of generations
    EA_param.numGenes = grafico.numMoedas; // Number of genes (coins)
    EA_param.capacity = grafico.valorPagar; // Target sum to achieve

    // Initialize best ever solution with invalid values
    best_ever.fitness = INFINITY;
    best_ever.valido = 0;
    best_ever.p = NULL;

    // Main algorithm loop - multiple independent runs
    for (r = 0; r < runs; r++)
    {
        // Initialize population and hill climbing
        printf("Repeticao %d\n", r + 1);
        pop = init_pop(EA_param, grafico);
        
        // Initialize hill climbing solution array
        int *hill_solution = malloc(sizeof(int) * EA_param.numGenes);
        if (hill_solution == NULL) {
            printf("Erro na alocação de memória para solução hill climbing\n");
            exit(1);
        }
        
        // Copy initial population's first solution for hill climbing
        for (i = 0; i < EA_param.numGenes; i++) {
            hill_solution[i] = pop[0].p[i];
        }
        
        // Run hill climbing before genetic algorithm
        //printf("Aplicando Hill Climbing (PRE-GA)...\n");
        
        trepaColinas(hill_solution, grafico, 50);  // 50 iterations
        
        // Update first solution in population with hill climbing result
        for (i = 0; i < EA_param.numGenes; i++) {
            pop[0].p[i] = hill_solution[i];
        }
        
        // Evaluate initial population
        evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

        // Initialize generation counter and best solution for this run
        gen_actual = 1;
        best_run = pop[0];
        best_run = get_best(pop, EA_param, best_run);

        // Allocate memory for parent population
        parents = malloc(sizeof(chrom) * EA_param.popsize);
        if (parents == NULL)
        {
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }

        // Main generational loop
        while (gen_actual <= EA_param.numGenerations)
        {
            roulette_selection(pop, EA_param, parents);
            //tournament_geral(pop, EA_param, parents);
            genetic_operators(parents, EA_param, pop, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            // apply hillclimnbign to the best solution
            
            /*
            for (i = 0; i < EA_param.numGenes; i++) {
                hill_solution[i] = pop[0].p[i];
            }
            trepaColinas(hill_solution, grafico, 50);  // Apply hill climbing with 50 iterations
*/
            best_run = get_best(pop, EA_param, best_run);
            gen_actual++;
        }

        // Post-optimization with hill climbing
        for (i = 0; i < EA_param.numGenes; i++) {
            hill_solution[i] = best_run.p[i];
        }
        
/*
        printf("Aplicando Hill Climbing (POST-GA)...\n");
        trepaColinas(hill_solution, grafico, 50);  // Apply hill climbing with 50 iterations
        
        // Update best run with hill climbing results
        for (i = 0; i < EA_param.numGenes; i++) {
            best_run.p[i] = hill_solution[i];
        }*/
        
        // Re-evaluate the solution after hill climbing
        int valido;
        best_run.fitness = eval_individual(best_run.p, EA_param, grafico.moedas, grafico.valorPagar, &valido);
        best_run.valido = valido;

        

        // Count invalid solutions in final population
        for (inv = 0, i = 0; i < EA_param.popsize; i++)
            if (pop[i].valido == 0)
                inv++;

        // Output results for this run
        printf("\nRepeticao %d:", r + 1);
        write_best(best_run, EA_param, grafico);
        mbf += best_run.fitness;

        // Update best ever solution if current run is better
        if (!best_ever.valido ||
            (best_run.valido && best_run.fitness < best_ever.fitness))
        {
            // Memory management for best solution
            if (best_ever.p != NULL)
            {
                free(best_ever.p);
            }

            // Deep copy of the new best solution
            best_ever.fitness = best_run.fitness;
            best_ever.valido = best_run.valido;
            best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
            
            if (best_ever.p == NULL)
            {
                printf("Erro na alocação de memória para a melhor solução\n");
                exit(1);
            }

            for (i = 0; i < EA_param.numGenes; i++)
            {
                best_ever.p[i] = best_run.p[i];
            }
        }

        // Clean up memory for this run
        free(parents);
        free_population(pop, EA_param.popsize);
        free(pop);
        free(hill_solution);
        pop = NULL;
    }

    // Output final results and cleanup
    printf("\n\nMBF: %f\n", mbf / runs);
    printf("\nMelhor solucao encontrada");
    write_best(best_ever, EA_param, grafico);

    // Final memory cleanup
    if (best_ever.p != NULL)
    {
        free(best_ever.p);
    }

    return 0;
}