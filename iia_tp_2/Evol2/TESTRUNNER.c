#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "algoritmo.h"
#include "funcao.h"

// Structure to store parameter exploration results
typedef struct {
    char param_name[20];
    float param_value;
    float best_fitness;
    int run_number;
} ParamResult;

// Function to perform parameter exploration
void explore_parameters(char* fname, ParamResult* results, int* result_count) {
    int totalruns = 25;

    // Parameter ranges to explore
    int popsize_values[] = {50, 100, 250, 500};
    float pm_values[] = {0.01, 0.1, 0.5, 0.9};
    float pr_values[] = {0.3, 0.5, 0.7, 0.9};
    int tsize_values[] = {1, 2, 5, 10};
    int numGenerations_values[] = {2500, 5500, 7500, 10000};

    // Base parameters
    int base_popsize = 100;
    float base_pm = 0.5;
    float base_pr = 0.3;
    int base_tsize = 2;
    int base_numGenerations = 5500;

    // Best parameter values
    float best_fitness_popsize = INFINITY;
    float best_fitness_pm = INFINITY;
    float best_fitness_pr = INFINITY;
    float best_fitness_tsize = INFINITY;
    float best_fitness_numGenerations = INFINITY;

    int best_popsize = INFINITY;
    float best_pm = INFINITY;
    float best_pr = INFINITY;
    int best_tsize = INFINITY;
    int best_numGenerations = INFINITY;

    dados grafico;
    struct info EA_param;
    pchrom pop = NULL, parents = NULL;
    chrom best_run, best_ever;
    int gen_actual, r, i, inv, result_index = 0;

    init_rand();
    init_dados(fname, &grafico);

    // Explore recombination probability (pr)
    printf("\nStarting Recombination Probability Exploration...\n");
    for (i = 0; i < sizeof(pr_values)/sizeof(pr_values[0]); i++) {
        printf("  Exploring pr = %f\n", pr_values[i]);
        EA_param.popsize = base_popsize;
        EA_param.pm = base_pm;
        EA_param.pr = pr_values[i];
        EA_param.tsize = base_tsize;
        EA_param.numGenerations = base_numGenerations;
        EA_param.numGenes = grafico.numMoedas;
        EA_param.capacity = grafico.valorPagar;

        float run_fitnesses[25];
        float mbf = 0.0;
        float avg_fitness = 0.0;
        best_ever.fitness = INFINITY;
        best_ever.valido = 0;
        best_ever.p = NULL;

        for (r = 0; r < totalruns; r++) {
            pop = init_pop(EA_param, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            gen_actual = 1;
            best_run = pop[0];
            best_run = get_best(pop, EA_param, best_run);

            parents = malloc(sizeof(chrom) * EA_param.popsize);
            if (parents == NULL) {
                printf("Erro na alocacao de memoria\n");
                exit(1);
            }

            while (gen_actual <= EA_param.numGenerations) {
                tournament_geral(pop, EA_param, parents);
                //roulette_selection(pop, EA_param, parents);
                genetic_operators(parents, EA_param, pop, grafico);
                evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

                best_run = get_best(pop, EA_param, best_run);
                gen_actual++;
            }

            run_fitnesses[r] = best_run.fitness;
            mbf += best_run.fitness;

            if (!best_ever.valido ||
                (best_run.valido && best_run.fitness < best_ever.fitness)) {
                if (best_ever.p != NULL) {
                    free(best_ever.p);
                }

                best_ever.fitness = best_run.fitness;
                best_ever.valido = best_run.valido;

                best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
                if (best_ever.p == NULL) {
                    printf("Erro na alocação de memória para a melhor solução\n");
                    exit(1);
                }

                for (int j = 0; j < EA_param.numGenes; j++) {
                    best_ever.p[j] = best_run.p[j];
                }
            }

            free(parents);
            free_population(pop, EA_param.popsize);
            free(pop);
            pop = NULL;
        }

        // Calculate average fitness
        avg_fitness = mbf / totalruns;
        if(avg_fitness < best_fitness_pr){
            best_fitness_pr = avg_fitness;
            best_pr = pr_values[i];
        }

        // Store result
        strcpy(results[result_index].param_name, "pr");
        results[result_index].param_value = pr_values[i];
        results[result_index].best_fitness = best_ever.fitness;
        results[result_index].run_number = i + 1;
        result_index++;

        // Print detailed results
        printf("  Pr %f Results:\n", pr_values[i]);
        printf("\n    Best Fitness: %f\n", best_ever.fitness);
        printf("    Average Fitness: %f\n\n", avg_fitness);
        /*printf("    Fitness Variance: ");
        
        // Calculate and print variance
        float variance = 0.0;
        for (r = 0; r < totalruns; r++) {
            variance += pow(run_fitnesses[r] - avg_fitness, 2);
        }
        variance /= totalruns;
        printf("%f\n", variance);*/

        printf("  Completed pr = %f exploration\n", pr_values[i]);
    }

    // Explore mutation probability (pm)
    printf("\nStarting Mutation Probability Exploration...\n");
    for (i = 0; i < sizeof(pm_values)/sizeof(pm_values[0]); i++) {
        printf("  Exploring pm = %f\n", pm_values[i]);
        EA_param.popsize = base_popsize;
        EA_param.pm = pm_values[i];
        EA_param.pr = best_pr;
        EA_param.tsize = base_tsize;
        EA_param.numGenerations = base_numGenerations;
        EA_param.numGenes = grafico.numMoedas;
        EA_param.capacity = grafico.valorPagar;

        float run_fitnesses[25];
        float mbf = 0.0;
        float avg_fitness = 0.0;
        best_ever.fitness = INFINITY;
        best_ever.valido = 0;
        best_ever.p = NULL;

        for (r = 0; r < totalruns; r++) {
            pop = init_pop(EA_param, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            gen_actual = 1;
            best_run = pop[0];
            best_run = get_best(pop, EA_param, best_run);

            parents = malloc(sizeof(chrom) * EA_param.popsize);
            if (parents == NULL) {
                printf("Erro na alocacao de memoria\n");
                exit(1);
            }

            while (gen_actual <= EA_param.numGenerations) {
                tournament_geral(pop, EA_param, parents);
                //roulette_selection(pop, EA_param, parents);
                genetic_operators(parents, EA_param, pop, grafico);
                evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

                best_run = get_best(pop, EA_param, best_run);
                gen_actual++;
            }

            run_fitnesses[r] = best_run.fitness;
            mbf += best_run.fitness;

            if (!best_ever.valido ||
                (best_run.valido && best_run.fitness < best_ever.fitness)) {
                if (best_ever.p != NULL) {
                    free(best_ever.p);
                }

                best_ever.fitness = best_run.fitness;
                best_ever.valido = best_run.valido;

                best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
                if (best_ever.p == NULL) {
                    printf("Erro na alocação de memória para a melhor solução\n");
                    exit(1);
                }

                for (int j = 0; j < EA_param.numGenes; j++) {
                    best_ever.p[j] = best_run.p[j];
                }
            }

            free(parents);
            free_population(pop, EA_param.popsize);
            free(pop);
            pop = NULL;
        }

        // Calculate average fitness
        avg_fitness = mbf / totalruns;
        if(avg_fitness < best_fitness_pm){
            best_fitness_pm = avg_fitness;
            best_pm = pm_values[i];
        }

        // Store result
        strcpy(results[result_index].param_name, "pm");
        results[result_index].param_value = pm_values[i];
        results[result_index].best_fitness = best_ever.fitness;
        results[result_index].run_number = i + 1;
        result_index++;

        // Print detailed results
        printf("  Pm %f Results:\n", pm_values[i]);
        printf("\n    Best Fitness: %f\n", best_ever.fitness);
        printf("    Average Fitness: %f\n\n", avg_fitness);
        /*printf("    Fitness Variance: ");
        
        // Calculate and print variance
        float variance = 0.0;
        for (r = 0; r < totalruns; r++) {
            variance += pow(run_fitnesses[r] - avg_fitness, 2);
        }
        variance /= totalruns;
        printf("%f\n", variance);*/

        printf("  Completed pm = %f exploration\n", pm_values[i]);
    }

    // Explore tournament size (tsize)
    printf("\nStarting Tournament Size Exploration...\n");
    for (i = 0; i < sizeof(tsize_values)/sizeof(tsize_values[0]); i++) {
        printf("  Exploring tsize = %d\n", tsize_values[i]);
        EA_param.popsize = base_popsize;
        EA_param.pm = best_pm;
        EA_param.pr = best_pr;
        EA_param.tsize = tsize_values[i];
        EA_param.numGenerations = base_numGenerations;
        EA_param.numGenes = grafico.numMoedas;
        EA_param.capacity = grafico.valorPagar;

        float run_fitnesses[25];
        float mbf = 0.0;
        float avg_fitness = 0.0;
        best_ever.fitness = INFINITY;
        best_ever.valido = 0;
        best_ever.p = NULL;

        for (r = 0; r < totalruns; r++) {
            pop = init_pop(EA_param, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            gen_actual = 1;
            best_run = pop[0];
            best_run = get_best(pop, EA_param, best_run);

            parents = malloc(sizeof(chrom) * EA_param.popsize);
            if (parents == NULL) {
                printf("Erro na alocacao de memoria\n");
                exit(1);
            }

            while (gen_actual <= EA_param.numGenerations) {
                tournament_geral(pop, EA_param, parents);
                //roulette_selection(pop, EA_param, parents);
                genetic_operators(parents, EA_param, pop, grafico);
                evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

                best_run = get_best(pop, EA_param, best_run);
                gen_actual++;
            }

            run_fitnesses[r] = best_run.fitness;
            mbf += best_run.fitness;

            if (!best_ever.valido ||
                (best_run.valido && best_run.fitness < best_ever.fitness)) {
                if (best_ever.p != NULL) {
                    free(best_ever.p);
                }

                best_ever.fitness = best_run.fitness;
                best_ever.valido = best_run.valido;

                best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
                if (best_ever.p == NULL) {
                    printf("Erro na alocação de memória para a melhor solução\n");
                    exit(1);
                }

                for (int j = 0; j < EA_param.numGenes; j++) {
                    best_ever.p[j] = best_run.p[j];
                }
            }

            free(parents);
            free_population(pop, EA_param.popsize);
            free(pop);
            pop = NULL;
        }

        avg_fitness = mbf / totalruns;
        if(avg_fitness < best_fitness_tsize){
            best_fitness_tsize = avg_fitness;
            best_tsize = tsize_values[i];
        }

        // Store result
        strcpy(results[result_index].param_name, "tsize");
        results[result_index].param_value = tsize_values[i];
        results[result_index].best_fitness = best_ever.fitness;
        results[result_index].run_number = i + 1;
        result_index++;

        // Print detailed results
        printf("  Tsize %d Results:\n", tsize_values[i]);
        printf("\n    Best Fitness: %f\n", best_ever.fitness);
        printf("    Average Fitness: %f\n\n", avg_fitness);
        /*printf("    Fitness Variance: ");

        // Calculate and print variance
        float variance = 0.0;
        for (r = 0; r < totalruns; r++) {
            variance += pow(run_fitnesses[r] - avg_fitness, 2);
        }
        variance /= totalruns;
        printf("%f\n", variance);*/

        printf("  Completed tsize = %d exploration\n", tsize_values[i]);
    }

    // Explore population size (popsize)
    printf("\nStarting Population Size Exploration...\n");
    for (i = 0; i < sizeof(popsize_values)/sizeof(popsize_values[0]); i++) {
        printf("  Exploring popsize = %d\n", popsize_values[i]);
        EA_param.popsize = popsize_values[i];
        EA_param.pm = best_pm;
        EA_param.pr = best_pr;
        EA_param.tsize = best_tsize;
        EA_param.numGenerations = base_numGenerations;
        EA_param.numGenes = grafico.numMoedas;
        EA_param.capacity = grafico.valorPagar;

        // Array to store fitness of each run
        float run_fitnesses[25];
        float mbf = 0.0;
        float avg_fitness = 0.0;
        best_ever.fitness = INFINITY;
        best_ever.valido = 0;
        best_ever.p = NULL;

        for (r = 0; r < totalruns; r++) {
            pop = init_pop(EA_param, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            gen_actual = 1;
            best_run = pop[0];
            best_run = get_best(pop, EA_param, best_run);

            parents = malloc(sizeof(chrom) * EA_param.popsize);
            if (parents == NULL) {
                printf("Erro na alocacao de memoria\n");
                exit(1);
            }

            while (gen_actual <= EA_param.numGenerations) {
                tournament_geral(pop, EA_param, parents);
                //roulette_selection(pop, EA_param, parents);
                genetic_operators(parents, EA_param, pop, grafico);
                evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

                best_run = get_best(pop, EA_param, best_run);
                gen_actual++;
            }

            // Store run fitness
            run_fitnesses[r] = best_run.fitness;
            mbf += best_run.fitness;

            if (!best_ever.valido ||
                (best_run.valido && best_run.fitness < best_ever.fitness)) {
                if (best_ever.p != NULL) {
                    free(best_ever.p);
                }

                best_ever.fitness = best_run.fitness;
                best_ever.valido = best_run.valido;

                best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
                if (best_ever.p == NULL) {
                    printf("Erro na alocação de memória para a melhor solução\n");
                    exit(1);
                }

                for (int j = 0; j < EA_param.numGenes; j++) {
                    best_ever.p[j] = best_run.p[j];
                }
            }

            free(parents);
            free_population(pop, EA_param.popsize);
            free(pop);
            pop = NULL;
        }

        // Calculate average fitness
        avg_fitness = mbf / totalruns;
        if(avg_fitness < best_fitness_popsize){
            best_fitness_popsize = avg_fitness;
            best_popsize = popsize_values[i];
        }

        // Store result
        strcpy(results[result_index].param_name, "popsize");
        results[result_index].param_value = popsize_values[i];
        results[result_index].best_fitness = best_ever.fitness;
        results[result_index].run_number = i + 1;
        result_index++;

        // Print detailed results
        printf("  Popsize %d Results:\n", popsize_values[i]);
        printf("\n    Best Fitness: %f\n", best_ever.fitness);
        printf("    Average Fitness: %f\n\n", avg_fitness);
        /*printf("    Fitness Variance: ");
        
        // Calculate and print variance
        float variance = 0.0;
        for (r = 0; r < totalruns; r++) {
            variance += pow(run_fitnesses[r] - avg_fitness, 2);
        }
        variance /= totalruns;
        printf("%f\n", variance);*/

        printf("  Completed popsize = %d exploration\n", numGenerations_values[i]);
    }

    // Explore number of generations
    printf("\nStarting Number of Generations Exploration...\n");
    for (i = 0; i < sizeof(numGenerations_values)/sizeof(numGenerations_values[0]); i++) {
        printf("  Exploring numGenerations = %d\n", numGenerations_values[i]);
        EA_param.popsize = best_popsize;
        EA_param.pm = best_pm;
        EA_param.pr = best_pr;
        EA_param.tsize = best_tsize;
        EA_param.numGenerations = numGenerations_values[i];
        EA_param.numGenes = grafico.numMoedas;
        EA_param.capacity = grafico.valorPagar;

        float run_fitnesses[25];
        float mbf = 0.0;
        float avg_fitness = 0.0;
        best_ever.fitness = INFINITY;
        best_ever.valido = 0;
        best_ever.p = NULL;

        for (r = 0; r < totalruns; r++) {
            pop = init_pop(EA_param, grafico);
            evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

            gen_actual = 1;
            best_run = pop[0];
            best_run = get_best(pop, EA_param, best_run);

            parents = malloc(sizeof(chrom) * EA_param.popsize);
            if (parents == NULL) {
                printf("Erro na alocacao de memoria\n");
                exit(1);
            }

            while (gen_actual <= EA_param.numGenerations) {
                tournament_geral(pop, EA_param, parents);
                //roulette_selection(pop, EA_param, parents);
                genetic_operators(parents, EA_param, pop, grafico);
                evaluate(pop, EA_param, grafico.moedas, grafico.valorPagar);

                best_run = get_best(pop, EA_param, best_run);
                gen_actual++;
            }

            run_fitnesses[r] = best_run.fitness;
            mbf += best_run.fitness;

            if (!best_ever.valido ||
                (best_run.valido && best_run.fitness < best_ever.fitness)) {
                if (best_ever.p != NULL) {
                    free(best_ever.p);
                }

                best_ever.fitness = best_run.fitness;
                best_ever.valido = best_run.valido;

                best_ever.p = malloc(sizeof(int) * EA_param.numGenes);
                if (best_ever.p == NULL) {
                    printf("Erro na alocação de memória para a melhor solução\n");
                    exit(1);
                }

                for (int j = 0; j < EA_param.numGenes; j++) {
                    best_ever.p[j] = best_run.p[j];
                }
            }

            free(parents);
            free_population(pop, EA_param.popsize);
            free(pop);
            pop = NULL;
        }

        // Calculate average fitness
        avg_fitness = mbf / totalruns;
        if(avg_fitness < best_fitness_numGenerations){
            best_fitness_numGenerations = avg_fitness;
            best_numGenerations = numGenerations_values[i];
        }

        // Store result
        strcpy(results[result_index].param_name, "numGenerations");
        results[result_index].param_value = numGenerations_values[i];
        results[result_index].best_fitness = best_ever.fitness;
        results[result_index].run_number = i + 1;
        result_index++;

        // Print detailed results
        printf("  numGenerations %d Results:\n", numGenerations_values[i]);
        printf("\n    Best Fitness: %f\n", best_ever.fitness);
        printf("    Average Fitness: %f\n\n", avg_fitness);
        /*printf("    Fitness Variance: ");
        
        // Calculate and print variance
        float variance = 0.0;
        for (r = 0; r < totalruns; r++) {
            variance += pow(run_fitnesses[r] - avg_fitness, 2);
        }
        variance /= totalruns;
        printf("%f\n", variance);*/

        printf("  Completed numGenerations = %d exploration\n", numGenerations_values[i]);
    }

    printf("Best Parameters:\n");
    printf("  popsize: %d\n", best_popsize);
    printf("  pm: %f\n", best_pm);
    printf("  pr: %f\n", best_pr);
    printf("  tsize: %d\n", best_tsize);
    printf("  numGenerations: %d\n", best_numGenerations);

    *result_count = result_index;
}

int main()
{
    char fname[40];
    ParamResult results[100];  // Adjust size as needed
    int result_count = 0;

    printf("\nNome Ficheiro: ");
    scanf("%s", fname);

    // Perform parameter exploration
    explore_parameters(fname, results, &result_count);

    // Print more detailed results
    printf("\n\nParameter Exploration Detailed Results:\n");
    printf("---------------------------------------\n");
    
    // Temporary variables to track current parameter set
    int current_popsize = 0;
    float current_pm = 0.0;
    float current_pr = 0.0;
    int current_tsize = 0;
    int current_numGenerations = 0;

    for (int i = 0; i < result_count; i++) {
        // Check the parameter type and update current values
        if (strcmp(results[i].param_name, "popsize") == 0) {
            current_popsize = (int)results[i].param_value;
            
            // Reset other parameter tracking
            current_pm = 0.0;
            current_pr = 0.0;
            current_tsize = 0;
            current_numGenerations = 0;
        } else if (strcmp(results[i].param_name, "pm") == 0) {
            current_pm = results[i].param_value;
        } else if (strcmp(results[i].param_name, "pr") == 0) {
            current_pr = results[i].param_value;
        } else if (strcmp(results[i].param_name, "tsize") == 0) {
            current_tsize = (int)results[i].param_value;
        } else if (strcmp(results[i].param_name, "numGenerations") == 0) {
            current_numGenerations = (int)results[i].param_value;
        }

        // Print the full set of parameters and fitness details
        printf("popsize:%d, pm:%.2f, pr:%.2f, tsize:%d, numGenerations:%d - Best Fitness: %.2f, Run %d\n", 
               current_popsize, 
               current_pm, 
               current_pr, 
               current_tsize, 
               current_numGenerations,
               results[i].best_fitness, 
               results[i].run_number);
    }

    return 0;
}